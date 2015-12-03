#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "structs.h"
#include "scheduler.h"
#include "config_reader.h"
#include "data_utility.h"
#include "data_test.h"
#include "defs.h"

int main(void)
{   
    int i, j, k, l;
    SemesterData sd = {0, 0, NULL, 0, NULL, 0, NULL, 0, NULL, 0, NULL};
 
    srand(time(NULL));
    
    /* Read configuration file */
    if (!read_config("scheduler.input", &sd))
    {
        printf("Error: Could not read configuration file.\n");
        exit(1);
    }
    
    /* Generate schedule */
    generate_initial_schedule(&sd);

    /* Debug */
    printf("%d teachers\n", sd.numTeachers);
    printf("%d rooms\n", sd.numRooms);
    printf("%d courses\n", sd.numCourses);
    printf("%d specializations\n", sd.numSpecializations);
    printf("%d weeks\n", sd.numWeeks);
    printf("\n");
    
    /* DEBUG: Go through all specializations, their courses, their teachers and their offtimes */
    /*for (i = 0; i < sd.numSpecializations; i++)
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
                    printf("\t\t\tOfftime on day %d (%s): %d, %d\n",
						offt->day,
						get_name_of_day(offt->day),
						offt->periods[0],
						offt->periods[1]);
                }
            }
        }
        
        printf("\n");
    }*/
    
    /* DEBUG: Go through all lectures, test their capacity */
    /*for (i = 0; i < sd.numLectures; i++)
    {
        Lecture *lect = get_lecture(&sd, i);
        Course *crs = get_course(&sd, lect->assignedCourse);
        Room *rm = get_room(&sd, lect->assignedRoom);
        
        printf("Lecture %d: %s in %s (severity %d)\n",
			i + 1,
			crs->name,
			rm->name,
			test_lecture_capacity(&sd, i));
    }*/
    
    free_all(&sd);
    
    return 0;
}

/*
 * Generate a 'dumb' schedule (array of lectures)
 * The only fulfilled requirement is the amount of lectures per course
*/
void generate_initial_schedule(SemesterData *sd)
{
    int i, j, k = 0;

    /* Get total amount of lectures */
    sd->numLectures = get_amount_of_lectures(sd);
    
    /* Allocate memory for the lectures */
    sd->lectures = malloc(sd->numLectures * sizeof(Lecture));
    if (!sd->lectures)
        exit(ERROR_OUT_OF_MEMORY);
    
    /* Go through all courses */
    for (i = 0; i < sd->numCourses; i++)
    {
        Course *crs = sd->courses[i];
        
        /* Go through the amount of lectures in this course */
        for (j = 0; j < crs->totLectures; j++)
        {
            /* Pass sd pointer, lectureIndex, day, period, roomId, courseId */
            add_lecture(sd, k++, rand() % (DAYS_PER_WEEK * sd->numWeeks), rand() % MAX_PERIODS, rand() % sd->numRooms, i);
        }
    }
}

/*
 * Free all memory associated with the SemesterData struct
 * Dynamically allocated arrays inside the structs are also freed
*/
void free_all(SemesterData *sd)
{
    int i;
    
    /* Free teachers */
    if (sd->teachers)
    {
        /* Free offtimes arrays inside teachers */
        for (i = 0; i < sd->numTeachers; i++)
            free(sd->teachers[i]->offTimes);
        
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
            free(sd->courses[i]->teachers);
        
        free(sd->courses);
    }

    /* Free specializations */
    if (sd->specializations)
    {
        /* Free course arrays inside specializations */
        for (i = 0; i < sd->numSpecializations; i++)
            free(sd->specializations[i]->courses);
        
        free(sd->specializations);
    }
}