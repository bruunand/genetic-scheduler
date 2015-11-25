#include "structs.h"

/* Returns the room specified by the index parameter */
/*!
    Name: get_room
    Input: semesterData *sd, int index
    Output: room struct from index
 */
room get_room(semesterData *sd, int index)
{
    if (index < 0 || index >= sd->numTeachers)
        return NULL;
    else
        return sd->teachers[index];
}

/* Returns the teacher specified by the index parameter */
teacher get_teacher(semesterData *sd, int index)
{
    if (index < 0 || index >= sd->numTeachers)
        return NULL;
    else
        return sd->teachers[index];
}

/* Returns the course specified by the index parameter */
course get_course(semesterData *sd, int index)
{
    if (index < 0 || index >= sd->numCourses)
        return NULL;
    else
        return sd->courses[index];
}

/* Returns the specialization specified by the index parameter */
specialization get_specialization(semesterData *sd, int index)
{
    if (index < 0 || index >= sd->numSpecializations)
        return NULL;
    else
        return sd->specializations[index];
}

/* Returns the lecture specified by the index parameter */
lecture get_lecture(semesterData *sd, int index)
{
    if (index < 0 || index >= sd->numLectures)
        return NULL;
    else
        return sd->lectures[index];
}