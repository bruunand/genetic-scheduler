/**
 *  \file data_utility.c
 *  \brief Brief
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "data_utility.h"
#include "structs.h"
#include "defs.h"

const char* periodNames[] = {"08:15 - 12:00", "12:30 - 16:15"};
const char* dayNames[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};

/**
 *  \brief Reset flags for all lectures in a specific schedule
 *  
 *  \param [in] schedule Parameter_Description
 *  \return Return_Description
 *  
 *  \details Details
 */
void reset_schedule_flags(Schedule *schedule)
{
    int i;

    /* Reset flags for all lectures in schedule */
    for (i = 0; i < schedule->parentGen->sd->numLectures; i++)
        memset(&schedule->lectures[i].flags, 0, sizeof(Flags));
}

/**
 *  \brief Set members of a lecture struct
 *  
 *  \param [in] lect Parameter_Description
 *  \param [in] day Parameter_Description
 *  \param [in] period Parameter_Description
 *  \param [in] room Parameter_Description
 *  \param [in] course Parameter_Description
 *  \return Return_Description
 *  
 *  \details Details
 */
void set_lecture(Lecture *lect, int day, int period, Room *room, Course *course)
{
    lect->day = day;
    lect->period = period;
    lect->assignedRoom = room;
    lect->assignedCourse = course;
}

/**
 *  \brief Brief
 *  
 *  \param [in] sp Pointer to a specialization
 *  \param [in] lect Parameter_Description
 *  \return Returns whether a specialization is on a specific lecture
 *  
 *  \details Details
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
 *  \brief Brief
 *  
 *  \param [in] sd Parameter_Description
 *  \param [in] teacher Parameter_Description
 *  \param [in] dayId Parameter_Description
 *  \param [in] periodId Parameter_Description
 *  \return Returns 1 if a teacher has an offtime (not available) on a day and period
 *  
 *  \details Details
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
 *  \brief Brief
 *  
 *  \param [in] sd Parameter_Description
 *  \param [in] course Parameter_Description
 *  \return Returns the amount of students on a specific course
 *  
 *  \details Details
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
 *  \brief Brief
 *  
 *  \param [in] sd Parameter_Description
 *  \return Returns the total amount of lectures in the semester
 *  
 *  \details Details
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
 *  \brief Brief
 *  
 *  \param [in] periodId Parameter_Description
 *  \return Returns the name of a period specified by the periodId parameter
 *  
 *  \details Details
 */
const char* get_name_of_period(int periodId)
{
    if (periodId < 0 || periodId > MAX_PERIODS - 1)
        return "UNKNOWN";
    return periodNames[periodId];
}

/**
 *  \brief Brief
 *  
 *  \param [in] dayId Parameter_Description
 *  \return Returns the name of a day specified by the dayId parameter
 *  
 *  \details Details
 */
const char* get_name_of_day(int dayId)
{
    return dayNames[dayId % DAYS_PER_WEEK];
}

/**
 *  \brief Brief
 *  
 *  \param [in] sd Parameter_Description
 *  \param [in] course Parameter_Description
 *  \param [in] specs Parameter_Description
 *  \return Returns amount of specializations
 *  
 *  \details Details
 */
int get_specializations_for_course(SemesterData *sd, Course *course, Specialization ***specs)
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