#include <stdio.h>
#include <stdlib.h>

#include "structs.h"
#include "scheduler.h"
#include "config_reader.h"

#define DAYS_PER_WEEK 5
#define NUM_ENTRIES   100
#define NUM_PERIODS   2

int main(void)
{
    semesterData sd = {0, 0, NULL, 0, NULL, 0, NULL, 0, NULL};
    
    /* Read configuration file */
    if (!read_config("scheduler.input", &sd))
    {
        printf("Error: Could not read configuration file.\n");
        exit(1);
    }
    
    printf("Teachers %d, Courses %d, Rooms %d, Weeks %d\n", sd.numTeachers, sd.numCourses, sd.numRooms, sd.numWeeks);
    
    int i;
    for (i = 0; i < sd.numTeachers; i++)
    {
        printf("'%s'\n", sd.teachers[i].name);
    }
    
    for (i = 0; i < sd.numRooms; i++)
    {
        printf("Room '%s' has %d seats\n", sd.rooms[i].name, sd.rooms[i].seats);
    }
    
    while(1)
    {
        
    }
    
}
