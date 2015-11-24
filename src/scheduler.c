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
        free(sd->teachers);
    /* TODO: Free offtimes for all teachers */
    
    if (sd->rooms)
        free(sd->rooms);
    
    if (sd->courses)
        free(sd->courses);
    /* TODO: Free teacher arrays for all courses */
    
    if (sd->specializations)
        free(sd->specializations);
    /* TODO: Free course arrays for all specializations */
    
    /*if (sd->currentGeneration)
        free(sd->currentGeneration);
    
    if (sd->nextGeneration)
        free(sd->nextGeneration);*/
}

int main(void)
{
    semesterData sd = {0, 0, NULL, 0, NULL, 0, NULL, 0, NULL, 0, NULL};
    
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
    
    int i, j, k;
    for (i = 0; i < sd.numSpecializations; i++)
    {
        printf("Specialization: '%s'\n", sd.specializations[i].name);
        
        for (j = 0; j < sd.specializations[i].numCourses; j++)
        {
            printf("\t Course: %s\n", sd.courses[sd.specializations[i].courses[j]].name);
            
            for (k = 0; k < sd.courses[sd.specializations[i].courses[j]].numTeachers; k++)
            {
                printf("\t\tTeacher: %s\n", sd.teachers[sd.courses[sd.specializations[i].courses[j]].teachers[k]].name);
            }
        }
        printf("\n");
    }
    
    free_all(&sd);
}
