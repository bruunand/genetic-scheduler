/**
 *  \file data_test.c
 *  \brief This script contains the functions responsible for performing tests on the generations
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "structs.h"
#include "data_utility.h"
#include "defs.h"
#include "data_test.h"

/**
 *  \brief Test how well the lecture fits into the assigned room
 *  
 *  \param [in] sd SemesterData contains all the information about the structs needed for this function
 *  \param [in] lect Pointer to lecture to test
 *  \return Returns the severity of the test. Aka. Fitness
 *  
 *  \details This function checks the capacity of the room and the amount of students on the lecture and determins the penalty in fitness by comparing the two.
 */
int test_lecture_capacity(SemesterData *sd, Lecture *lect)
{
    int roomCap, numStudents, severity = 0, roomSeats;
    
    /* Functions, calculations and pointers used multiple times are assigned to local variables. */
    roomSeats = lect->assignedRoom->seats;
    roomCap = MAX_OVER_CAPACITY * roomSeats;
    numStudents = get_students_on_course(sd, lect->assignedCourse);

    /*
     * Severity is increased by 1 per 5% room capacity exceeded.
     * Set to 1 if half or less of the seats are used.
    */
    if (roomCap < numStudents)
        severity = 1 + (numStudents - roomCap) / ((roomCap > roomSeats) ? (roomCap - roomSeats) : 1);
    else if ((roomSeats / numStudents) >= 2)
        severity = 1;
    
    return severity;
}

/**
 *  \brief Test whether the teacher has an offtime on a given date
 *  
 *  \param [in] sd SemesterData contains all the information about the structs needed for this function
 *  \param [in] lect Pointer to lecture to test
 *  \return Returns the severity of the test. Aka. Fitness
 *  
 *  \details Also test whether the teacher is already assigned to a lecture on the same date
 */
int test_teacher_availability(Generation *gp, int scheduleId, Lecture *lect)
{
    int severity = 0, i, j, k;
    
    /*
     * Iterate through all teachers assigned to this course.
     * If they have an offtime on this day, increase the severity.
     * If they have another lecture, increase the severity.
    */
    for (i = 0; i < lect->assignedCourse->numTeachers; i++)
    {
        Teacher *curTeacher = lect->assignedCourse->teachers[i];
        
        /* Test if they are off on this day */
        if (teacher_has_offtime(gp->sd, curTeacher, lect->day, lect->period))
            severity += 100;
        
        /* Test if they have another lecture at this time */
        for (j = 0; j < gp->sd->numLectures; j++)
        {
            Lecture *curLect = &gp->schedules[scheduleId][j];
            
            /* Skip lecture being tested */
            if (curLect == lect)
                continue;
            
            /* Skip lecture that are at different times */
            if (curLect->day != lect->day || curLect->period != lect->period)
                continue;
            
            /* Increase severity if this teacher is on this course */
            for (k = 0; k < curLect->assignedCourse->numTeachers; k++)
            {
                if (curLect->assignedCourse->teachers[k] == curTeacher)
                    severity += 100;
            }
        }
    }
    
    return severity;
}

/**
 *  \brief Test for doublebooking
 *  
 *  \param [in] sd SemesterData contains all the information about the structs needed for this function
 *  \param [in] lect Pointer to lecture to test
 *  \return Returns the severity of the test. Aka. Fitness
 *  
 *  \details Performs tests for both room and lecture doublebooking
 */
int test_doublebooking(Generation *gp, int scheduleId, Lecture *lect)
{
    Specialization **specs;
    int severity = 0, numSpecs, i, j;
    
    /* Test room doublebooking */
    for(i = 0; i < gp->sd->numLectures; i++)
    {
        Lecture *curLect = &gp->schedules[scheduleId][i];

        if (curLect == lect)
            continue;
        
        if (curLect->day != lect->day || curLect->period != lect->period)
            continue;
        
        if (curLect->flags.doubleBookingRoom)
            continue;

        if (curLect->assignedRoom == lect->assignedRoom)
        {
            curLect->flags.doubleBookingRoom = 1;
            
            severity += 100;
        }
    }
    
    /* Find out who has this lecture */
    numSpecs = get_specializations_for_course(gp->sd, lect->assignedCourse, &specs);
    for (i = 0; i < numSpecs; i++)
    {
        /* Find lectures on the same day as the test lecture */
        for (j = 0; j < gp->sd->numLectures; j++)
        {
            Lecture *curLect = &gp->schedules[scheduleId][j];
            
            if (curLect == lect)
                continue;
            
            if (curLect->day != lect->day || curLect->period != lect->period)
                continue;

            if (curLect->flags.doubleBookingLecture)
                continue;
            
            if (specialization_has_lecture(specs[i], curLect))
            {
                curLect->flags.doubleBookingLecture = 1;

                severity += 100;
            }
        }
    }
    
    free(specs);
    
    return severity;
}

/**
 *  \brief Test distribution on a weekly basis to ensure an even workload
 *  
 *  \param [in] sd SemesterData contains all the information about the structs needed for this function
 *  \param [in] lect Pointer to lecture to test
 *  \return Returns the severity of the test. Aka. Fitness
 *  
 *  \details Details
 */
int test_weekly_distribution(Generation *gp, int scheduleId, Lecture *lect)
{
    int severity = 0, i, weekNum;
    int totCoursePerWeek = 0, totCoursePerDay = 0;

    weekNum = lect->day / DAYS_PER_WEEK;

    /* Test distribution for this course */
    for (i = 0; i < gp->sd->numLectures; i++)
    {
        Lecture *curLect = &gp->schedules[scheduleId][i];
        
        /* Skip other courses */
        if (curLect->assignedCourse != lect->assignedCourse)
            continue;
        
        /* Skip flagged lectures */
        if (curLect->flags.lectureOverflow)
            continue;

        if (curLect->day == lect->day)
        {
            curLect->flags.lectureOverflow = 1;
            totCoursePerDay++;
        }
        if (curLect->day / DAYS_PER_WEEK == weekNum)
        {
            curLect->flags.lectureOverflow = 1;
            totCoursePerWeek++;
        }
    }
    
    /* Compute severity */
    if (totCoursePerDay > 1)
        severity += pow(25, totCoursePerDay - 1);
    if (totCoursePerWeek > 3)
        severity += pow(10, totCoursePerWeek - 3);
    
    return severity;
}

 /**
 *  \brief Tests the semester distribution
 *  
 *  \param [in] sd SemesterData contains all the information about the structs needed for this function
 *  \param [in] lect Pointer to lecture to test
 *  \return Returns the severity of the test. Aka. Fitness
 *  
 *  \details Makes a call to the inner test function for every specialization on the specified lecture
 */
int test_semester_distribution(Generation *gp, int scheduleId, Lecture *lect)
{
    int severity = 0, numSpecs, i;
    Specialization **specs = 0;
    
    numSpecs = get_specializations_for_course(gp->sd, lect->assignedCourse, &specs);
    
    /* Get severity for all specializations */
    for (i = 0; i < numSpecs; i++)
    {
    int tmp = test_semester_distribution_inner(gp, scheduleId, lect, specs[i]);
        severity += tmp;
    }
    
    free(specs);
    
    return severity;
}

/**
 *  \brief Test how the lecture fits into the semester distribution
 *  
 *  \param [in] sd SemesterData contains all the information about the structs needed for this function
 *  \param [in] lect Pointer to lecture to test
 *  \param [in] sp ???????????????????????????????????
 *  \return Returns the severity of the test. Aka. Fitness
 *  
 *  \details Details
 */
int test_semester_distribution_inner(Generation *gp, int scheduleId, Lecture *lect, Specialization *sp)
{
    int i, weekNum, lecturesCurWeek = 0, maxLecturesCurWeek;
    
    weekNum = lect->day / DAYS_PER_WEEK;
    
    if (lect->flags.semesterOverflow)
        return 0;
    
    /* Iterate through all lectures */
    for (i = 0; i < gp->sd->numLectures; i++)
    {
        Lecture *curLect = &gp->schedules[scheduleId][i];
        
        /* Only check lectures that are in the same week */
        if (curLect->day / DAYS_PER_WEEK != weekNum)
            continue;
        
        /* Only check lectures that this specialization has */
        if (!specialization_has_lecture(sp, curLect))
            continue;
        
        lecturesCurWeek++;
        curLect->flags.semesterOverflow = 1;
    }

    /* Distribute most lectures in first 3/4 of the semester */
    if (weekNum > gp->sd->numWeeks * 3 / 4)
        maxLecturesCurWeek = 7 - weekNum * ((float) 6 / gp->sd->numWeeks);
    else
        maxLecturesCurWeek = 7;

    /* Check if this week exceeds the allowed lectures */
    if (lecturesCurWeek > maxLecturesCurWeek)
        return (lecturesCurWeek - maxLecturesCurWeek) * 50;
    else
        return 0;
}