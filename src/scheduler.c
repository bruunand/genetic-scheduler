/**
 *  \file scheduler.c
 *  \brief The main script of the program, the magic starts here
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include "structs.h"
#include "scheduler.h"
#include "config_reader.h"
#include "data_utility.h"
#include "fitness_calculation.h"
#include "defs.h"
#include "html_output.h"

void mutate(Generation *new, int genomeId)
{
    int i;

    for (i = 0; i < new->sd->numLectures; i++)
    {
        if (rand() % 100 > MUTATION_CHANCE)
            continue;
        
        if (rand() % 2)
            new->schedules[genomeId].lectures[i].day = rand() % (new->sd->numWeeks * DAYS_PER_WEEK);
        
        if (rand() % 2)
            new->schedules[genomeId].lectures[i].period = rand() % MAX_PERIODS;
        
        if (rand() % 2)
            new->schedules[genomeId].lectures[i].assignedRoom = &new->sd->rooms[rand() % new->sd->numRooms];
    }
}

void initialize_generation(Generation **gen, SemesterData *sd)
{
    int i, j, k, l;
    
    /* Allocate memory for generation */
    *gen = malloc(sizeof(Generation));
    if (!*gen)
        exit(ERROR_OUT_OF_MEMORY);
    
    /* Set parent SemesterData */
    (*gen)->sd = sd;
    
    /* Initiate schedules */
    /* Iterate through all generations */
    for (i = 0; i < GENERATION_SIZE; i++)
    {
        k = 0;
        
        /* Initialize new schedule/genome */
        initialize_schedule(*gen, i);
        
        /* Iterate through all courses */
        for (j = 0; j < sd->numCourses; j++)
        {
            Course *course = &sd->courses[j];
            
            /* Iterate through all lectures for this course and add */
            for (l = 0; l < course->totLectures; l++)
            {
				set_lecture(&(*gen)->schedules[i].lectures[k++],
                    rand() % (DAYS_PER_WEEK * sd->numWeeks),
                    rand() % MAX_PERIODS, 
                    &sd->rooms[rand() % sd->numRooms],
                    &sd->courses[j]);
            }
        }
    }
}

void crossbreed(Generation *new, int genomeId, int carryover)
{
    int i, parentA, parentB, selectedParent;
    
    /* Find two different parents to be mated */
    do
    {
        parentA = rand() % carryover;
        parentB = rand() % carryover;
    } while(parentA == parentB);
    
    /*
     * Iterate through all lectures and compare their fitness.
     * Note: This assumes that the lectures fall in the same order
     * for both parents!
    */
    for (i = 0; i < new->sd->numLectures; i++)
    {
        /* Select best fit */
        if (new->schedules[parentA].lectures[i].fitness <= new->schedules[parentB].lectures[i].fitness)
            selectedParent = parentA;
        else
            selectedParent = parentB;

        new->schedules[genomeId].lectures[i] = new->schedules[selectedParent].lectures[i];
    }
}

void copy_generation(Generation *dest, Generation *src)
{
    int i;
    
    free_generation(dest);
    
    for (i = 0; i < GENERATION_SIZE; i++)
    {
        initialize_schedule(dest, i);
        copy_schedule(&dest->schedules[i], &src->schedules[i]);
        dest->schedules[i].parentGen = dest;
    }
    
}
void run_ga(Generation **curGen)
{
    int i, j, x, y, newGenMembers, carriedOver, lowestFit;
    Generation *nextGen = 0;
    
    for (i = 0; i < MAX_GENERATIONS; i++)
    {
        newGenMembers = 0;
        
        initialize_generation(&nextGen, (*curGen)->sd);
            
        /* Calculate fitness of current generation */
        calcfit_generation(*curGen);
        
        /* Sort current generation by fitness */
        qsort((*curGen)->schedules, GENERATION_SIZE, sizeof(Schedule), compare_schedule_fitness);

        /* Print best schedule */
        lowestFit = (*curGen)->schedules[0].fitness;
        printf("%3d: %5d (%5d)\n", i, lowestFit, (*curGen)->fitness / GENERATION_SIZE);
        
        if (lowestFit == 0)
            break;
        
        /*
         * Tournament selection.
         * Half of the population is kept alive.
        */
        for (j = 0; j < GENERATION_SIZE / 2; j++)
        {
            x = rand() % GENERATION_SIZE;
            y = rand() % GENERATION_SIZE;
            
            /* Copy element with highest fitness of the two */
            copy_schedule(&nextGen->schedules[newGenMembers], &(*curGen)->schedules[MIN(x, y)]);
            newGenMembers++;
        }
        
        /* Save the amount of genomes carried over,
         * since crossbreed will only breed from these. */
        carriedOver = newGenMembers;

        /* Crossbreed rest of the elements */
        for (; newGenMembers < GENERATION_SIZE; newGenMembers++)
            crossbreed(nextGen, newGenMembers, carriedOver);

        /* Mutate randomly */
        for (j = 0; j < GENERATION_SIZE; j++)
        {
            if (rand() % 100 <= MUTATION_CHANCE)
                mutate(nextGen, j);
        }

        /* Copy nextgen to current generation */
        copy_generation(*curGen, nextGen);
        free_generation(nextGen);
    }
}

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
    Generation *gen = 0;
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

    /* Calculate amount of lectures (genes) */
    calc_amount_of_lectures(&sd);
    
    /* Generate initial generation */
    initialize_generation(&gen, &sd);
    
    /* Run genetic algorithm */
    run_ga(&gen);
    
    /* Print schedules to files */
    print_schedule_to_file(&gen->schedules[0], &sd.specializations[0], "swdat.html");
    print_schedule_to_file(&gen->schedules[0], &sd.specializations[1], "robotics.html");

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
 *  \brief Brief
 *  
 *  \param [in] gp Parameter_Description
 *  \return Return_Description
 *  
 *  \details Details
 */
void free_generation(Generation *gp)
{
    int i;
    
    for (i = 0; i < GENERATION_SIZE; i++)
    {
        if (gp->schedules[i].lectures)
            free(gp->schedules[i].lectures);
    }
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