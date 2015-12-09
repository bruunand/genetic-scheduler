/**
 *  \file scheduler.c
 *  \brief The main script of the program, the magic starts here
 */
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
    
    /* Generate initial generation */
    generate_initial_generation(&sd, &gen);
    
    /* Print schedules to files */
    print_schedule_to_file(&gen, 0, &sd.specializations[0], "swdat.html");
    print_schedule_to_file(&gen, 0, &sd.specializations[1], "robotics.html");
    
    free_all(&sd);
    
    return 0;
}

/* Generate initial generation by generating n schedules */
void generate_initial_generation(SemesterData *sd, Generation *gp)
{
    int i, j, k, l;

    /* Set total amount of lectures */
    sd->numLectures = get_amount_of_lectures(sd);
    
    /* Allocate memory for the lectures in each schedule */
    for (i = 0; i < GENERATION_SIZE; i++)
    {
        gp->schedules[i] = malloc(sd->numLectures * sizeof(Lecture));
        if (!gp->schedules[i])
            exit(ERROR_OUT_OF_MEMORY);
    }

    /* Iterate through all generations */
    for (i = 0; i < GENERATION_SIZE; i++)
    {
        k = 0;
        
        /* Iterate through all courses */
        for (j = 0; j < sd->numCourses; j++)
        {
            Course *course = &sd->courses[j];
            
            /* Iterate through all lectures for this course */
            for (l = 0; l < course->totLectures; l++)
            {
                add_lecture(gp, i, k++, rand() % (DAYS_PER_WEEK * sd->numWeeks), rand() % MAX_PERIODS, rand() % sd->numRooms, j);
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