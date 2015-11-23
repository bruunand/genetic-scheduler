#include <stdio.h>
#include <stdlib.h>

#include "structs.h"
#include "scheduler.h"
#include "config_reader.h"

#define DAYS_PER_WEEK 5
#define NUM_ENTRIES   100
#define NUM_PERIODS   2

void free_all(semesterData *sd)
{
    if (sd->teachers)
    {
        free(sd->teachers->offDays);
        free(sd->teachers);
    }
    
    if (sd->rooms)
        free(sd->rooms);
    
    if (sd->courses)
        free(sd->courses);
    
    if (sd->lectures)
        free(sd->lectures);
}

int main(void)
{
    semesterData sd = {0, 0, NULL, 0, NULL, 0, NULL, 0, NULL};
    
    /* Read configuration file */
    if (!read_config("scheduler.input", &sd))
    {
        printf("Error: Could not read configuration file.\n");
        exit(1);
    }
    
    /* Debug */
    printf("%d teachers\n", sd.numTeachers);
    printf("%d rooms\n", sd.numRooms);
    printf("%d courses\n", sd.numCourses);
    printf("%d specializations\n", sd.numSpecializations);
    printf("%d weeks\n", sd.numWeeks);
    
    int i;
    for (i = 0; i < sd.numTeachers; i++)
    {
        printf("'%s'\n", sd.teachers[i].name);
    }
    
    for (i = 0; i < sd.numRooms; i++)
    {
        printf("Room '%s' has %d seats\n", sd.rooms[i].name, sd.rooms[i].seats);
    }
    
    free_all(&sd);
}
