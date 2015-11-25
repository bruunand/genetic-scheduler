#include "structs.h"

const char* periodNames[] = {"08:15 - 12:00", "12:30 - 16:15"};
const char* dayNames[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};

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
            /* Check if they have the same course index */
            if (spec->courses[j] == courseIndex)
                totStudents += spec->numStudents;
        }
    }
    
    return totStudents;
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