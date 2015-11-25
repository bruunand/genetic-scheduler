#include <stdlib.h>
#include <stdio.h>

#include "structs.h"
#include "defs.h"

const char* periodNames[] = {"08:15 - 12:00", "12:30 - 16:15"};
const char* dayNames[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};

/* Adds lecture to the semesterData struct */
void add_lecture(semesterData *sd, int lectIndex, int day, int period, int roomId, int courseId)
{
    /* Check if the index goes beyond the bounds of the lecture array */
    if (lectIndex >= sd->numLectures)
    {
        printf("Error: Lecture index %d exceeds the bounds of the lecture array.\n");
        exit(1);
    }
    
    /* Set lecture values */
    sd->lectures[lectIndex].day = day;
    sd->lectures[lectIndex].period = period;
    sd->lectures[lectIndex].assignedRoom = roomId;
    sd->lectures[lectIndex].assignedCourse = courseId;
}

/* Returns the room pointer specified by the index parameter */
/*!
    Name: get_room
    Input: semesterData *sd, int index
    Output: room struct from index
 */
room* get_room(semesterData *sd, int index)
{
    if (index < 0 || index >= sd->numTeachers)
        return 0;
    return &(sd->rooms[index]);
}

/* Returns the teacher pointer specified by the index parameter */
teacher* get_teacher(semesterData *sd, int index)
{
    if (index < 0 || index >= sd->numTeachers)
        return 0;
    return &(sd->teachers[index]);
}

/* Returns the course pointer specified by the index parameter */
course* get_course(semesterData *sd, int index)
{
    if (index < 0 || index >= sd->numCourses)
        return 0;
    return &(sd->courses[index]);
}

/* Returns the specialization pointer specified by the index parameter */
specialization* get_specialization(semesterData *sd, int index)
{
    if (index < 0 || index >= sd->numSpecializations)
        return 0;
    return &(sd->specializations[index]);
}

/* Returns the lecture pointer specified by the index parameter */
lecture* get_lecture(semesterData *sd, int index)
{
    if (index < 0 || index >= sd->numLectures)
        return 0;
    else
        return &(sd->lectures[index]);
}

/* Returns the offtime pointer specified by the index parameter */
offTime* get_offTime(teacher *t, int index)
{
    if (index < 0 || index >= t->numOffTimes)
        return 0;
    else
        return &(t->offTimes[index]);
}

/* Returns the amount of students on a specific course */
int get_students_on_course(semesterData *sd, int courseIndex)
{
    int i, j, totStudents = 0;
    
    /* Go through all specializations */
    for (i = 0; i < sd->numSpecializations; i++)
    {
        specialization *spec = get_specialization(sd, i);
        
        /* Go through all the courses for this specialization */
        for (j = 0; j < spec->numCourses; j++)
        {
            /* Increase totStudents if this course matches courseIndex */
            if (spec->courses[j] == courseIndex)
                totStudents += spec->numStudents;
        }
    }
    
    return totStudents;
}

/* Returns the total amount of lectures in the semester */
int get_amount_of_lectures(semesterData *sd)
{
    int totLectureAmount = 0, i;
    
    /* Add amount of lectures from each course */
    for (i = 0; i < sd->numCourses; i++)
        totLectureAmount += get_course(sd, i)->totLectures;
    
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

void get_specializations_for_course(semesterData *sd, int courseIndex, specialization *spec)
{
    int i, j, h;
    
    for(i = 0; i < sd->numSpecializations; i++)
    {
        for(j = 0; j < sd->specializations[i].numCourses; j++)
        {
            if(courceIndex == sd->specializations[i].courses[j])
            {
                spec[h++] = sd->specializations[i];
            }
        }
    }
}