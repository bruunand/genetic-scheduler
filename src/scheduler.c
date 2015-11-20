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
    semesterData data = {0, 0, NULL, 0, NULL, 0, NULL};
    
    /* Read configuration file */
    if (!read_config("scheduler.input", &data))
    {
        printf("Error: Could not read configuration file.\n");
        exit(1);
    }
    
    printf("Teachers %d, Courses %d, Rooms %d, Weeks %d\n", data.numTeachers, data.numCourses, data.numRooms, data.numWeeks);
}
