#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "structs.h"
#include "scheduler.h"
#include "config_reader.h"
#include "data_utility.h"
#include "data_test.h"
#include "defs.h"
#include "html_output.h"

int main(void)
{   
    int i, seed;
    int lowestFitness = -1, start;
    Generation gen;
    SemesterData sd;
    
    /* Generation points to this SemesterData struct */
    gen.sd = &sd;
    
    /* Null all SemesterData values */
    memset(&sd, 0, sizeof(SemesterData));
 
    /*if (!scanf("%d", &seed))*/
		seed = time(NULL);
    srand(seed);
    
    /* Read configuration file */
    if (!read_config("scheduler.input", &sd))
    {
        printf("Error: Could not read configuration file.\n");
        exit(1);
    }
    
    /* Generate schedule */
    generate_initial_schedules(&sd, &gen);
    
    /* Print schedules to files */
    /*print_schedule_to_file(&sd, &sd.specializations[0], "swdat.html");
    print_schedule_to_file(&sd, &sd.specializations[1], "robotics.html");*/
    
    free_all(&sd);
    
    return 0;
}

void generate_initial_schedules(SemesterData *sd, Generation *gp)
{
    int i, j, k = 0, l;

    /* Set total amount of lectures */
    gp->numLectures = get_amount_of_lectures(sd);
    
    /* Allocate memory for the amount of generations */
    gp->schedules = malloc(GENERATION_SIZE * sizeof(Lecture*));
    if (!gp->schedules)
        exit(ERROR_OUT_OF_MEMORY);
    
    /* Allocate memory for the lectures in each schedule */
    for (i = 0; i < GENERATION_SIZE; i++)
    {
        gp->schedules[i] = malloc(gp->numLectures * sizeof(Lecture));
        if (!gp->schedules[i])
            exit(ERROR_OUT_OF_MEMORY);
    }

    /* Iterate through all courses */
    for (i = 0; i < sd->numCourses; i++)
    {
        Course *course = &sd->courses[i];
        
        /* Randomly generate all lectures for this course */
        for (j = 0; j < course->totLectures; j++)
        {
            k = 0;
            
            /* Generate for all schedules in generation */
            for (l = 0; l < GENERATION_SIZE; l++)
            {
                add_lecture(gp, l, k++, rand() % (DAYS_PER_WEEK * sd->numWeeks), rand() % MAX_PERIODS, rand() % sd->numRooms, i);
            }
        }
    }
}

/*
 * Free all memory associated with the SemesterData struct.
 * Dynamically allocated arrays inside the structs are also freed.
*/
void free_all(SemesterData *sd)
{
    int i;
    
    /* Free teachers */
    if (sd->teachers)
    {
        /* Free offtimes arrays inside teachers */
        for (i = 0; i < sd->numTeachers; i++)
            free(sd->teachers[i].offTimes);
        
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
            free(sd->courses[i].teachers);
        
        free(sd->courses);
    }

    /* Free specializations */
    if (sd->specializations)
    {
        /* Free course arrays inside specializations */
        for (i = 0; i < sd->numSpecializations; i++)
            free(sd->specializations[i].courses);
        
        free(sd->specializations);
    }
}