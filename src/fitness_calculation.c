/**
 *  \file fitness_calculation.c
 *  \brief This script contains the functions responsible for calculating fitness values for the population
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "structs.h"
#include "data_utility.h"
#include "defs.h"
#include "fitness_calculation.h"

/**
 *  \brief Calculates how well the lecture fits into the assigned room
 *  
 *  \param [in] sd SemesterData contains all the information about the structs
 *  needed for this function
 *  \param [in] lect Pointer to lecture to calculate for
 *  \return Returns the fitness of the calculation
 *  
 *  \details This function checks the capacity of the room and the amount of
 *  students on the lecture and determins the penalty in fitness by comparing the two
 */
int calcfit_capacity(SemesterData *sd, Lecture *lect)
{
    int roomCap, numStudents, fitness = 0, roomSeats;
    
    /* Functions, calculations and pointers used multiple times are assigned to local variables. */
    roomSeats = lect->assignedRoom->seats;
    roomCap = MAX_OVER_CAPACITY * roomSeats;
    numStudents = get_students_on_course(sd, lect->assignedCourse);

    /*
     * Fitness is increased every time by PENALTY_ROOM_TOO_SMALL per 5% room capacity exceeded.
     * Set to PENALTY_ROOM_TOO_BIG if half or less of the seats are used.
    */
    if (numStudents > roomCap)
        fitness = PENALTY_ROOM_TOO_SMALL * ((numStudents - roomCap) / (roomCap - roomSeats));
    else if ((roomSeats / numStudents) >= 2)
        fitness = PENALTY_ROOM_TOO_BIG;

    return fitness;
}

/**
 *  \brief Calculates fitness based on the teacher's availability at the time of the lecture
 *  
 *  \param [in] schedule Pointer to a schedule
 *  \param [in] lect Pointer to lecture to calculate
 *  \return Returns the fitness of the calculation
 *  
 *  \details Also test whether the teacher is already assigned to a lecture on the same date
 */
int calcfit_teacher_availability(Schedule *schedule, Lecture *lect)
{
    int fitness = 0, i, j, k;
    
    /*
     * Iterate through all teachers assigned to this course.
     * If they have an offtime on this day, increase the fitness.
     * If they have another lecture, increase the fitness.
    */
    for (i = 0; i < lect->assignedCourse->numTeachers; i++)
    {
        Teacher *curTeacher = lect->assignedCourse->teachers[i];
        
        /* Test if they are off on this day */
        if (teacher_has_offtime(schedule->parentGen->sd, curTeacher, lect->day, lect->period))
            fitness += PENALTY_TEACHER_OFFTIME;
        
        /* Test if they have another lecture at this time */
        for (j = 0; j < schedule->parentGen->sd->numLectures; j++)
        {
            Lecture *curLect = &schedule->lectures[i];
            
            /* Skip lecture being tested */
            if (curLect == lect)
                continue;
            
            /* Skip lectures that are at different times */
            if (curLect->day != lect->day || curLect->period != lect->period)
                continue;
            
            /* Increase fitness if this teacher is on this course */
            for (k = 0; k < curLect->assignedCourse->numTeachers; k++)
            {
                if (curLect->assignedCourse->teachers[k] == curTeacher)
                    fitness += PENALTY_TEACHER_BOOKED;
            }
        }
    }
    
    return fitness;
}

/**
 *  \brief Calculates fitness based on whether the room or period is doublebooked
 *  
 *  \param [in] schedule Pointer to a schedule
 *  \param [in] lect Pointer to lecture to calculate
 *  \return Returns the fitness of the calculation
 *  
 *  \details Performs calculations for both room and lecture doublebooking
 */
int calcfit_doublebooking(Schedule *schedule, Lecture *lect)
{
    Specialization **specs;
    int fitness = 0, numSpecs, i, j;
        
    /* Test room doublebooking */
    for(i = 0; i < schedule->parentGen->sd->numLectures; i++)
    {
        Lecture *curLect = &schedule->lectures[i];

        if (curLect == lect)
            continue;
        
        if (curLect->day != lect->day || curLect->period != lect->period)
            continue;
        
        if (curLect->flags.doubleBookingRoom)
            continue;

        if (curLect->assignedRoom == lect->assignedRoom)
        {
            curLect->flags.doubleBookingRoom = 1;
            
            fitness += PENALTY_DOUBLEBOOKING;
        }
    }
    
    /* Find out who has this lecture */
    numSpecs = get_specializations_on_course(schedule->parentGen->sd, lect->assignedCourse, &specs);
    for (i = 0; i < numSpecs; i++)
    {
        /* Find lectures on the same day as the test lecture */
        for (j = 0; j < schedule->parentGen->sd->numLectures; j++)
        {
            Lecture *curLect = &schedule->lectures[j];
            
            if (curLect == lect)
                continue;
            
            if (curLect->day != lect->day || curLect->period != lect->period)
                continue;

            if (curLect->flags.doubleBookingLecture)
                continue;
            
            if (specialization_has_lecture(specs[i], curLect))
            {
                curLect->flags.doubleBookingLecture = 1;

                fitness += PENALTY_DOUBLEBOOKING;
            }
        }
    }
    
    free(specs);
    
    return fitness;
}

/**
 *  \brief Calculates how well the lecture fits into its week
 *  
 *  \param [in] schedule Pointer to a schedule
 *  \param [in] lect Pointer to lecture to calculate
 *  \return Returns the fitness of the lecture distribution
 *  
 *  \details Goes through each lecture and comparing it to another
 *  lecture in a schedule. When a lecture has been compared, it is
 *  flagged as such and will not be compared again
 */
int calcfit_distribution_weekly(Schedule *schedule, Lecture *lect)
{
    int fitness = 0, i, weekNum;
    int totCoursePerWeek = 0, totCoursePerDay = 0;

    weekNum = lect->day / DAYS_PER_WEEK;

    /* Test distribution for this course */
    for (i = 0; i < schedule->parentGen->sd->numLectures; i++)
    {
        Lecture *curLect = &schedule->lectures[i];
        
        /* Skip other courses */
        if (curLect->assignedCourse != lect->assignedCourse)
            continue;
        
        /* Skip flagged lectures */
        if (curLect->flags.lectureCounted)
            continue;

        if (curLect->day == lect->day)
        {
            curLect->flags.lectureCounted = 1;
            totCoursePerDay++;
        }
        if (curLect->day / DAYS_PER_WEEK == weekNum)
        {
            curLect->flags.lectureCounted = 1;
            totCoursePerWeek++;
        }
    }
    
    /* Compute fitness */
    if (totCoursePerDay > 1)
        fitness += MIN(500, pow(PENALTY_DAILY_LIMIT, totCoursePerDay - 1));
    if (totCoursePerWeek > 3)
        fitness += MIN(500, pow(PENALTY_WEEKLY_LIMIT, totCoursePerWeek - 3));
    
    return fitness;
}

 /**
 *  \brief Calculates the semester distribution
 *  
 *  \param [in] schedule Pointer to a schedule
 *  \param [in] lect Pointer to lecture to calculate
 *  \return Returns the fitness of the calculation
 *  
 *  \details Goes through each specialization on the
 *  course and calculates their fitness
 */
int calcfit_distribution_semester(Schedule *schedule, Lecture *lect)
{
    int fitness = 0, numSpecs, i;
    Specialization **specs = 0;
    
    numSpecs = get_specializations_on_course(schedule->parentGen->sd, lect->assignedCourse, &specs);
    
    /* Get fitness for all specializations */
    for (i = 0; i < numSpecs; i++)
        fitness += calcfit_distribution_semester_inner(schedule, lect, specs[i]);
    
    free(specs);
    
    return fitness;
}

/**
 *  \brief Calculate how well the lecture fits into the semester distribution
 *  
 *  \param [in] schedule Pointer to a schedule
 *  \param [in] lect Pointer to lecture to calculate
 *  \param [in] sp Pointer to a specialization that is needed to obtain some
 *  information within the function
 *  \return Returns the fitness of the calculation
 *  
 *  \details Goes through all lectures in a week within the specialization
 *  and see how well they are distributed. When a lecture is checked, it is
 *  flagged as such and will not be checked again
 */
int calcfit_distribution_semester_inner(Schedule *schedule, Lecture *lect, Specialization *sp)
{
    int i, weekNum, lecturesCurWeek = 0, maxLecturesCurWeek;
    
    weekNum = lect->day / DAYS_PER_WEEK;
    
    if (lect->flags.semesterCounted)
        return 0;
    
    /* Iterate through all lectures */
    for (i = 0; i < schedule->parentGen->sd->numLectures; i++)
    {
        Lecture *curLect = &schedule->lectures[i];
        
        /* Only check lectures that are in the same week */
        if (curLect->day / DAYS_PER_WEEK != weekNum)
            continue;
        
        /* Only check lectures that this specialization has */
        if (!specialization_has_lecture(sp, curLect))
            continue;
        
        lecturesCurWeek++;
        curLect->flags.semesterCounted = 1;
    }

    /* Distribute most lectures in first 3/4 of the semester */
    if (weekNum > schedule->parentGen->sd->numWeeks * 3 / 4)
        maxLecturesCurWeek = (MAX_LECTURES_PER_WEEK - (weekNum + 1) * ((float) (MAX_LECTURES_PER_WEEK - 1) / schedule->parentGen->sd->numWeeks));
    else
        maxLecturesCurWeek = MAX_LECTURES_PER_WEEK;

    /* Check if this week exceeds the allowed lectures */
    if (lecturesCurWeek > maxLecturesCurWeek)
        return (lecturesCurWeek - maxLecturesCurWeek) * PENALTY_SEMESTER_DISTRIB;
    else
        return 0;
}

/**
 *  \brief Calculate fitness for a single lecture (gene)
 *  
 *  \param [in] schedule The schedule the lecture is a part of
 *  \param [in] lect The specific lecture to calculate fitness for
 *  \return Returns the fitness of the lecture
 *  
 *  \details Performs all the fitness calculations on a specific
 *  lecture and returns the total fitness for that lecture
 */
int calcfit_lecture(Schedule *schedule, Lecture *lect)
{
    lect->fitness = 0;
    
    lect->fitness += calcfit_capacity(schedule->parentGen->sd, lect);
    lect->fitness += calcfit_teacher_availability(schedule, lect);
    lect->fitness += calcfit_doublebooking(schedule, lect);
    lect->fitness += calcfit_distribution_weekly(schedule, lect);
    lect->fitness += calcfit_distribution_semester(schedule, lect);
    
    return lect->fitness;
}

/**
 *  \brief Calculate fitness for a schedule/genome
 *  
 *  \param [in] schedule The schedule to calculate fitness for
 *  \return Returns the fitness of the schedule
 *  
 *  \details Iterates through all lectures and add their fitness
 *  to a variables which is then returned
 */
int calcfit_schedule(Schedule *schedule)
{
    int i;

    schedule->fitness = 0;
    
    reset_schedule_flags(schedule);
    
    /* Iterate through all lectures */
    for (i = 0; i < schedule->parentGen->sd->numLectures; i++)
    {
        Lecture *curLect = &schedule->lectures[i];
        
        schedule->fitness += calcfit_lecture(schedule, curLect);
    }

    return schedule->fitness;
}

/**
 *  \brief Calculates the fitness of a generation
 *  
 *  \param [in] gp The generation to calulate fitness for
 */
int calcfit_generation(Generation *gp)
{
    int i;

    gp->fitness = 0;
    
    /* Calculate fitness for old generation */
    for (i = 0; i < GENERATION_SIZE; i++)
        gp->fitness += calcfit_schedule(&gp->schedules[i]);
    
    return gp->fitness;
}