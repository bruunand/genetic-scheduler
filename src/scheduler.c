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
    Generation gen;
    SemesterData sd;

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
    gen = generate_initial_generation(&sd);
    
    /* Run up to 200 generations */
    for (i = 0; i < MAX_GENERATIONS; i++)
    {
        gen = generate_next_generation(&gen);
        
        /* TODO: Exit if low fitness is found? */
    }

    /* Print schedules to files */
    print_schedule_to_file(&gen, 0, &sd.specializations[0], "swdat.html");
    print_schedule_to_file(&gen, 0, &sd.specializations[1], "robotics.html");
    
    free_generation(&gen);
    free_semesterdata(&sd);
    
    return 0;
}

/* Generate next generation based on existing generation */
Generation generate_next_generation(Generation *curGen)
{
    int i, schedules[GENERATION_SIZE];
    Generation nextGen;
    
    /* Sort genomes by fitness */
    /* Keep best 5 of the bunch */
    /* Crossbreed 5 to get a total of 100 genomes */
    /* Mutate randomly, even on the best ones */
    
    return nextGen;
}

/* Generate initial generation by generating n schedules */
Generation generate_initial_generation(SemesterData *sd)
{
    int i, j, k, l;
    Generation initialGen;

    initialGen.sd = sd;
    
    /* Set total amount of lectures */
    sd->numLectures = get_amount_of_lectures(sd);
    
    /* Allocate memory for the lectures in each schedule */
    for (i = 0; i < GENERATION_SIZE; i++)
    {
        initialGen.schedules[i] = malloc(sd->numLectures * sizeof(Lecture));
        if (!initialGen.schedules[i])
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
                add_lecture(&initialGen, i, k++, rand() % (DAYS_PER_WEEK * sd->numWeeks), rand() % MAX_PERIODS, rand() % sd->numRooms, j);
            }
        }
    }
    
    /* Debug: Test fitness for all generations */
    for (i = 0; i < GENERATION_SIZE; i++)
    {
        printf("%d has a fitness of %d\n", i, test_schedule_fitness(&initialGen, i));
    }
    
    return initialGen;
}

void free_generation(Generation *gp)
{
    printf("Warning: Free_generation is not implemented yet!\n");
}

/*
 * Free all memory associated with the SemesterData struct.
 * Dynamically allocated arrays inside the structs are also freed.
*/
void free_semesterdata(SemesterData *sd)
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