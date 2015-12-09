#include <stdio.h>
#include <structs.h>

void add_lecture(semesterData *sd, int scheduleId, int index, int day, int period, int assignedRoom, int assignedCourse)
{
    /* Set values */
    sd->generation[scheduleId][index].day = day;
    sd->generation[scheduleId][index].period = period;
    sd->generation[scheduleId][index].assignedRoom = &sd->rooms[assignedRoom];
    sd->generation[scheduleId][index].assignedCourse = &sd->courses[assignedCourse];
}