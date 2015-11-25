#include "structs.h"

room get_room(semesterData *sd, int index)
{
    return sd->rooms[index];
}

teacher get_teacher(semesterData *sd, int index)
{
    return sd->teachers[index];
}

course get_course(semesterData *sd, int index)
{
    return sd->courses[index];
}

specialization get_specialization(semesterData *sd, int index)
{
    return sd->specializations[index];
}

lecture get_lecture(semesterData *sd, int index)
{
    return sd->lectures[index];
}