#include <stdio.h>
#include <stdlib.h>

#include "structs.h"
#include "scheduler.h"
#include "config_reader.h"
#include "data_utility.h"

#define DAYS_PER_WEEK 5
#define NUM_ENTRIES   100
#define NUM_PERIODS   2

void free_all(semesterData *sd)
{
    int i;
    
    if (sd->teachers)
    {
        /* Free offtimes arrays inside teachers */
        for (i = 0; i < sd->numTeachers; i++)
            free(get_teacher(sd, i).offTimes);
        
        free(sd->teachers);
    }
    
    if (sd->rooms)
        free(sd->rooms);
    
    if (sd->courses)
    {
        /* Free teacher arrays inside courses */
        for (i = 0; i < sd->numCourses; i++)
            free(get_course(sd, i).teachers);
        
        free(sd->courses);
    }

    if (sd->specializations)
    {
        /* Free course arrays inside specializations */
        for (i = 0; i < sd->numSpecializations; i++)
            free(get_specialization(sd, i).courses);
        
        free(sd->specializations);
    }
}

int main(void)
{
    int i, j, k;
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
    
    return 0;
}
