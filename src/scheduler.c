#include <stdio.h>
#include <stdlib.h>

#include "structs.h"
#include "scheduler.h"
#include "config_reader.h"
#include "data_utility.h"
#include "data_test.h"

int main(void)
{
    int i, j, k, l;
    semesterData sd = {0, 0, NULL, 0, NULL, 0, NULL, 0, NULL, 0, NULL};
    
    /* Read configuration file */
    if (!read_config("scheduler.input", &sd))
    {
        printf("Error: Could not read configuration file.\n");
        exit(1);
    }

    /* TEST LECTURE */
    add_lecture(&sd, 0, 0, 0, 0);
    printf("severity %d\n", test_lecture_capacity(&sd, 0));
    
    /* Debug */
    printf("%d teachers\n", sd.numTeachers);
    printf("%d rooms\n", sd.numRooms);
    printf("%d courses\n", sd.numCourses);
    printf("%d specializations\n", sd.numSpecializations);
    printf("%d weeks\n", sd.numWeeks);
    printf("\n");
    
    /* DEBUG: Go through all specializations, their courses, their teachers and their offtimes */
    for (i = 0; i < sd.numSpecializations; i++)
    {
        specialization* spec = get_specialization(&sd, i);
        printf("Specialization: %s\n", spec->name);
        
        for (j = 0; j < spec->numCourses; j++)
        {
            course* cour = get_course(&sd, spec->courses[j]);
            printf("\t Course: %s\n", cour->name);
            
            for (k = 0; k < cour->numTeachers; k++)
            {
                teacher* teach = get_teacher(&sd, cour->teachers[k]);
                printf("\t\tTeacher: %s\n", teach->name);

                for (l = 0; l < teach->numOffTimes; l++)
                {
                    offTime* offt = get_offTime(teach, l);
                    printf("\t\t\tOfftime on day %d (%s): %d, %d\n", offt->day, get_name_of_day(offt->day), offt->periods[0], offt->periods[1]);
                }
            }
        }
        
        printf("\n");
    }
    
    free_all(&sd);
    
    return 0;
}

/* Free all memory associated with the semesterData struct */
void free_all(semesterData *sd)
{
    int i;
    
    /* Free teachers */
    if (sd->teachers)
    {
        /* Free offtimes arrays inside teachers */
        for (i = 0; i < sd->numTeachers; i++)
            free(get_teacher(sd, i)->offTimes);
        
        free(sd->teachers);
    }
    
    /* Free rooms */
    if (sd->rooms)
        free(sd->rooms);
    
    /* Free courses */
    if (sd->courses)
    {
        /* Free teacher arrays inside courses */
        for (i = 0; i < sd->numCourses; i++)
            free(get_course(sd, i)->teachers);
        
        free(sd->courses);
    }

    /* Free specializations */
    if (sd->specializations)
    {
        /* Free course arrays inside specializations */
        for (i = 0; i < sd->numSpecializations; i++)
            free(get_specialization(sd, i)->courses);
        
        free(sd->specializations);
    }
}
