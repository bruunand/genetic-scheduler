#include <stdlib.h>
#include <stdio.h>

#include "structs.h"
#include "defs.h"

const char* periodNames[] = {"08:15 - 12:00", "12:30 - 16:15"};
const char* dayNames[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};

/* Adds lecture to the SemesterData struct */
void add_lecture(SemesterData *sd, int lectIndex, int day, int period, int roomId, int courseId)
{
    /* Check if the index goes beyond the bounds of the lecture array */
    if (lectIndex >= sd->numLectures)
        exit(ERROR_ARRAY_BOUNDS_EXCEEDED);
	
    /* Set lecture values */
    sd->lectures[lectIndex].day = day;
    sd->lectures[lectIndex].period = period;
    sd->lectures[lectIndex].assignedRoom = &sd->rooms[roomId];
    sd->lectures[lectIndex].assignedCourse = &sd->courses[courseId];
}

/*
 * Returns whether a specialization is on a specific lecture.
 *
 * sp: Pointer to a specialization.
*/
int specialization_has_lecture(Specialization *sp, Lecture *lect)
{
    int i;
    
    /* Iterate through all courses for this */
    for (i = 0; i < sp->numCourses; i++)
    {
        if (sp->courses[i] == lect->assignedCourse)
            return 1;
    }

    return 0;
}

/* Returns 1 if a teacher has an offtime (not available) on a day and period. */
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

/* Returns the amount of students on a specific course */
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

/* Returns the total amount of lectures in the semester */
int get_amount_of_lectures(SemesterData *sd)
{
    int totLectureAmount = 0, i;
    
    /* Add amount of lectures from each course */
    for (i = 0; i < sd->numCourses; i++)
        totLectureAmount += sd->courses[i].totLectures;
    
    return totLectureAmount;
}

/* Returns the name of a period specified by the periodId parameter */
const char* get_name_of_period(int periodId)
{
    if (periodId < 0 || periodId > MAX_PERIODS - 1)
        return "UNKNOWN";
    return periodNames[periodId];
}

/* Returns the name of a day specified by the dayId parameter */
const char* get_name_of_day(int dayId)
{
    return dayNames[dayId % DAYS_PER_WEEK];
}

/* Returns amount of specializations */
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