#include "structs.h"
/**  **/
/* This function returns the room specified by the index parameter */
room get_room(semesterData *sd, int index)
{
    return sd->rooms[index];
}

/* This function returns the teacher specified by the index parameter */
teacher get_teacher(semesterData *sd, int index)
{
    return sd->teachers[index];
}

/* This function returns the course specified by the index parameter */
course get_course(semesterData *sd, int index)
{
    return sd->courses[index];
}

/* This function returns the specialization specified by the index parameter */
specialization get_specialization(semesterData *sd, int index)
{
    return sd->specializations[index];
}

/* This function returns the lecture specified by the index parameter */
lecture get_lecture(semesterData *sd, int index)
{
    return sd->lectures[index];
}