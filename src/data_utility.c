/**
 *  \file data_utility.c
 *  \brief A set of utility functions when parts of code can be used more than once
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "data_utility.h"
#include "structs.h"
#include "defs.h"
#include "fitness_calculation.h"

const char* periodNames[] = {"08:15 - 12:00", "12:30 - 16:15"};
const char* dayNames[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};

/* Print the issues with a lecture */
void print_schedule_issues(Schedule *schedule)
{
    int i, capacity = 0, teacher = 0, doublebooking = 0, distWeekly = 0, distSemester = 0;
    
    reset_schedule_flags(schedule);
    
    /* Calculate fitness values */
    for (i = 0; i < schedule->parentGen->sd->numLectures; i++)
    {
        Lecture *curLect = &schedule->lectures[i];
        
        capacity       += calcfit_capacity(schedule->parentGen->sd, curLect);
        teacher        += calcfit_teacher_availability(schedule, curLect);
        doublebooking  += calcfit_doublebooking(schedule, curLect);
        distWeekly     += calcfit_distribution_weekly(schedule, curLect);
        distSemester   += calcfit_distribution_semester(schedule, curLect);
    }
    
    /* Print issues */
    if (capacity)
        printf("Room capacity score: %3d\n", capacity);
    
    if (teacher)
        printf("Teacher availability score: %3d\n", teacher);
    
    if (doublebooking)
        printf("Doublebooking score: %3d\n", doublebooking);
    
    if (distWeekly)
        printf("Weekly distribution score: %3d\n", distWeekly);
    
    if (distSemester)
        printf("Semester distribution score: %3d\n", distSemester);
}

/**
 *  \brief Reset flags for all lectures in a specific schedule
 *  
 *  \param [in] schedule Schedule contains the lectures we want to reset
 *  
 *  \details Iterates through all lectures in a given schedule and resets all the flags
 */
void reset_schedule_flags(Schedule *schedule)
{
    int i;

    /* Reset flags for all lectures in schedule */
    for (i = 0; i < schedule->parentGen->sd->numLectures; i++)
        memset(&schedule->lectures[i].flags, 0, sizeof(Flags));
}

/* Copy a lecture to another destination. */
void copy_lecture(Lecture *dest, Lecture *src)
{
    memcpy(dest, src, sizeof(Lecture));
}

/*
 * Copy a schedule to another destination.
 * Set parent generation if not null.
*/
void copy_schedule(Schedule *dest, Schedule *src, Generation *newGen)
{
    memcpy(dest, src, sizeof(Schedule));
    
    if (newGen)
        dest->parentGen = newGen;
}

/**
 *  \brief Set members of a lecture struct
 *  
 *  \param [in] lect The lecture we want to work with
 *  \param [in] day The new value for day
 *  \param [in] period The new value for period
 *  \param [in] room The new value for assignedRoom
 *  \param [in] course The new value for assignedCourse
 */
void set_lecture(Lecture *lect, int day, int period, Room *room, Course *course)
{
    lect->day = day;
    lect->period = period;
    lect->assignedRoom = room;
    lect->assignedCourse = course;
}

/**
 *  \brief Checks if a specialization has a given lecture
 *  
 *  \param [in] sp Pointer to a specialization we want to check
 *  \param [in] lect Pointer to a lecture we want to check with
 *  \return Returns whether a specialization is on a specific lecture
 *  
 *  \details Check if a course in the specialization matches the assigned course for the lecture
 */
int specialization_has_lecture(Specialization *sp, Lecture *lect)
{
    int i;
    
    /* Iterate through all courses */
    for (i = 0; i < sp->numCourses; i++)
    {
        if (sp->courses[i] == lect->assignedCourse)
            return 1;
    }

    return 0;
}

/**
 *  \brief Check if a teacher has offtime on a given day and period
 *  
 *  \param [in] sd SemesterData contains some information needed by the function
 *  \param [in] teacher Pointer to the teacher we are checking
 *  \param [in] dayId The ID of the day we check for
 *  \param [in] periodId The ID of the period we check for
 *  \return Returns 1 if a teacher has an offtime (not available) on a day and period
 *  
 *  \details First we validate the period on the day. Then we iterate through all offtimes
 */
int teacher_has_offtime(SemesterData *sd, Teacher *teacher, int dayId, int periodId)
{
    int i;
    
    /* Validate period on day */
    if (periodId > 2 || dayId > (DAYS_PER_WEEK * sd->numWeeks) - 1)
        return 1;
    
    /* Iterate through all offtimes */
    for (i = 0; i < teacher->numOffTimes; i++)
    {
        OffTime *offTime = &teacher->offTimes[i];
        
        if (offTime->day == dayId && offTime->periods[periodId] == 1)
            return 1;
    }
    
    return 0;
}

/**
 *  \brief Gets the total number of students on a given course
 *  
 *  \param [in] sd SemesterData has some required information
 *  \param [in] course Pointer to the couse we check
 *  \return Returns the amount of students on a specific course
 *  
 *  \details First we go through all specializations and add them to a temp. variable. Then we go through all the courses in these specializations and adds the number of students in each specialization that has the course
 */
int get_students_on_course(SemesterData *sd, Course *course)
{
    int i, j, totStudents = 0;
    
    /* Go through all specializations */
    for (i = 0; i < sd->numSpecializations; i++)
    {
        Specialization *spec = &sd->specializations[i];
        
        /* Go through all the courses for this specialization */
        for (j = 0; j < spec->numCourses; j++)
        {
            /* Increase totStudents if this course matches courseIndex */
            if (spec->courses[j] == course)
                totStudents += spec->numStudents;
        }
    }
    
    return totStudents;
}

/**
 *  \brief Gets the total amount of lectures
 *  
 *  \param [in] sd SemesterData contains the information required to get the amount
 *  \return Returns the total amount of lectures in the semester
 *  
 *  \details Goes through all the courses in SemesterData and adds the lectures assigned to each course to a variable which is then returned
 */
int get_amount_of_lectures(SemesterData *sd)
{
    int totLectureAmount = 0, i;
    
    /* Add amount of lectures from each course */
    for (i = 0; i < sd->numCourses; i++)
        totLectureAmount += sd->courses[i].totLectures;
    
    return totLectureAmount;
}

/**
 *  \brief Gets a name of a period (08:15-12:00 or 12:30-16:15)
 *  
 *  \param [in] periodId The ID of the period to check
 *  \return Returns the name of a period specified by the periodId parameter
 *  
 *  \details Checks if the period ID is outside the range. If it is outside the range it returns an UNKNOWN name. Otherwise it returns the name according to it's place on the schedule
 */
const char* get_name_of_period(int periodId)
{
    if (periodId < 0 || periodId > MAX_PERIODS - 1)
        return "UNKNOWN";
    return periodNames[periodId];
}

/**
 *  \brief Gets a name of a day
 *  
 *  \param [in] dayId The ID of the day to check
 *  \return Returns the name of a day specified by the dayId parameter
 */
const char* get_name_of_day(int dayId)
{
    return dayNames[dayId % DAYS_PER_WEEK];
}

/**
 *  \brief Gets all specializations that contains a specific course
 *  
 *  \param [in] sd SemesterData contains information required for the function to work
 *  \param [in] course The course we check for
 *  \param [out] specs The specializations that contain the course
 *  \return Returns amount of specializations
 *  
 *  \details Counts and returns the specializations with the specific cause.
 */
int get_specializations_on_course(SemesterData *sd, Course *course, Specialization ***specs)
{
    int i, j, amount = 0;
    
    *specs = 0;
    
    for(i = 0; i < sd->numSpecializations; i++)
    {
        for(j = 0; j < sd->specializations[i].numCourses; j++)
        {
            if (sd->specializations[i].courses[j] == course)
            {
                /* Allocate memory for pointers to specializations */
                *specs = realloc(*specs, (amount + 1) * sizeof(Specialization*));
                if (!*specs)
                    exit(ERROR_OUT_OF_MEMORY);
                
                (*specs)[amount] = &sd->specializations[i];
                amount++;
            }
        }
    }
    
    return amount;
}