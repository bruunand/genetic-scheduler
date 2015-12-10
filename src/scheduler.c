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
#include "fitness_calculation.h"
#include "defs.h"
#include "html_output.h"

/**
 *  \brief Brief
 *  
 *  \return Returns weather the program has exited with an error or success
 *  
 *  \details Details
 */
int main(void)
{
    int i, seed;
    Generation curGen;
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
    generate_initial_generation(&curGen, &sd);
    
    /* Run up to 200 generations */
    for (i = 0; i < 1; i++)
    {
        Generation nextGen;
        generate_next_generation(&curGen, &nextGen);
        
        /*
         * free_generation(&curGen);
         * curGen = nextGen;
        */
        
        /* TODO: Exit if low fitness is found? */
    }

    /* Print schedules to files */
    print_schedule_to_file(&curGen.schedules[0], &sd.specializations[0], "swdat.html");
    print_schedule_to_file(&curGen.schedules[0], &sd.specializations[1], "robotics.html");
    
    free_generation(&curGen);
    free_semesterdata(&sd);
    
    return 0;
}

void initialize_schedule(Generation *parentGen, int scheduleIndex)
{
    Schedule *newSchedule = &parentGen->schedules[scheduleIndex];
    
    /* Allocate memory for the amount of lectures */
    newSchedule->lectures = malloc(parentGen->sd->numLectures * sizeof(Lecture));
    if (!newSchedule->lectures)
        exit(ERROR_OUT_OF_MEMORY);
    
    /* Set parent generation */
    newSchedule->parentGen = parentGen;
}

/* Generate initial generation by generating n schedules */
void generate_initial_generation(Generation *initialGen, SemesterData *sd)
{
    int i, j, k, l;
    
    /* Set parent semesterdata */
    initialGen->sd = sd;
    
    /* Set total amount of lectures */
    sd->numLectures = get_amount_of_lectures(sd);

    /* Iterate through all generations */
    for (i = 0; i < GENERATION_SIZE; i++)
    {
        k = 0;
        
        /* Initialize new schedule/genome */
        initialize_schedule(initialGen, i);
        
        /* Iterate through all courses */
        for (j = 0; j < sd->numCourses; j++)
        {
            Course *course = &sd->courses[j];
            
            /* Iterate through all lectures for this course and add */
            for (l = 0; l < course->totLectures; l++)
            {
				set_lecture(&initialGen->schedules[i].lectures[k++],
                    rand() % (DAYS_PER_WEEK * sd->numWeeks),
                    rand() % MAX_PERIODS, 
                    &sd->rooms[rand() % sd->numRooms],
                    &sd->courses[j]);
            }
        }
    }
}

/**
 *  \brief Brief
 *  
 *  \param [in] a Parameter_Description
 *  \param [in] b Parameter_Description
 *  \return Return_Description
 *  
 *  \details Details
 */
int compare_schedule_fitness(const void *a, const void *b)
{
    Schedule *scheduleA = (Schedule*)a;
    Schedule *scheduleB = (Schedule*)b;
    
    return scheduleA->fitness - scheduleB->fitness;
}

/**
 *  \brief Generate next generation based on existing generation
 *  
 *  \param [in] oldGen Parameter_Description
 *  \param [in] newGen Parameter_Description
 *  \return Return_Description
 *  
 *  \details Details
 */
void generate_next_generation(Generation *oldGen, Generation *newGen)
{
    int i;

    /* Calculate fitness for old generation */
    calcfit_generation(oldGen);
    
    /* Sort genomes by fitness */
    qsort(oldGen->schedules, oldGen->sd->numLectures, sizeof(Schedule), compare_schedule_fitness);
    
    /* Print best half % */
    for (i = 0; i < GENERATION_SIZE / 2; i++)
        printf("DEBUG: Genome %02d has a fitness of %04d\n", i + 1, oldGen->schedules[i].fitness);
    
    /* Crossbreed best to get a total of 100 genomes */
    /* Mutate randomly, even on the best ones */
}

/**
 *  \brief Brief
 *  
 *  \param [in] gp Parameter_Description
 *  \return Return_Description
 *  
 *  \details Details
 */
void free_generation(Generation *gp)
{
    printf("Warning: Free_generation is not implemented yet!\n");
    /* free gp->schedules->lectures for every schedule */
}

/**
 *  \brief Free all memory associated with the SemesterData struct. Dynamically allocated arrays inside the structs are also freed.
 *  
 *  \param [in] sd Parameter_Description
 *  \return Return_Description
 *  
 *  \details Details
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