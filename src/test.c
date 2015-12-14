#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "structs.h"
#include "input_reader.h"
#include "data_utility.h"

int main(void)
{
    Generation testGen;
    Schedule testSchedule;
    SemesterData sd;
    OffTime offTimes[2];
    srand(1234);
    
    memset(&sd, 0, sizeof(SemesterData));
    testGen.sd = &sd;
    
    /* Create OffTimes */
    offTimes[0].day = 2;
    offTimes[0].periods[0] = 1;
    offTimes[0].periods[1] = 1;
    
    offTimes[1].day = 3;
    offTimes[1].periods[0] = 1;
    offTimes[1].periods[1] = 1;

    /* Add a room with 150 seats */
    add_room(&sd, "Dummy Room", 150);
    
    /* Add a teacher with two offtimes */
    add_teacher(&sd, "Dummy Teacher", 2, offTimes);
    
    /* Add a course that has the Dummy Teacher */
    add_course(&sd, "Dummy Course", 15, sd.numTeachers, &sd.teachers);
    
    /* Add a specialization that has the Dummy Course */
    add_specialization(&sd, "Dummy Specialization", 150, sd.numCourses, &sd.courses);
    
    
    return 0;
}