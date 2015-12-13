/**
 *  \file scheduler.c
 *  \brief The main script of the program, the magic starts here
 */
#include <stdio.h>
#include <stdlib.h>

#include "structs.h"
#include "genetic_algorithm.h"
#include "defs.h"
#include "data_utility.h"
#include "fitness_calculation.h"

/* Run genetic algorithm */
void run_ga(Generation **curGen, SemesterData *sd)
{
    int i, carriedOver, lowestFit;
    FILE *output;
    Generation *nextGen = 0;
    
    /* Open output file */
    output = fopen("output.txt", "w+");
    if (!output)
        exit(ERROR_FILE_NULL_PTR);

    /* Initialize generations */
    initialize_generation(curGen,   sd);
    initialize_generation(&nextGen, sd);
            
    for (i = 0; i < MAX_GENERATIONS; i++)
    {
        /* Calculate fitness of current generation */
        calcfit_generation(*curGen);
        
        /* Sort current generation by fitness */
        qsort((*curGen)->schedules, GENERATION_SIZE, sizeof(Schedule), compare_schedule_fitness);

        /* Print best schedule */
        lowestFit = (*curGen)->schedules[0].fitness;
        printf("%3d: %5d (%5d)\n", i + 1, lowestFit, (*curGen)->fitness / GENERATION_SIZE);
        fprintf(output, "%d\t%d\t%d\n", i + 1, lowestFit, (*curGen)->fitness / GENERATION_SIZE);
        
        if (lowestFit == 0)
            break;
        
        /* Selection */
        carriedOver = ga_select(*curGen, nextGen);

        /* Crossbreeding */
        ga_crossbreed(nextGen, carriedOver);

        /* Mutation */
        ga_mutate(nextGen);

        /* Deep copy nextGen to curGen */
        copy_generation(*curGen, nextGen);
    }
    
    fclose(output);
}

/*
 * Select Schedules using Tournament selection.
 * Schedules (genomes) should be sorted by fitness at this point.
 * Returns amount of genomes carried over.
*/
int ga_select(Generation *curGen, Generation *newGen)
{
    int carriedOver = 0, i, x, y;
    
    for (i = 0; i < GENERATION_SIZE / 2; i++)
    {
        x = rand() % GENERATION_SIZE;
        y = rand() % GENERATION_SIZE;
        
        /* Copy best Schedule of the two randomly selected */
        copy_schedule(&newGen->schedules[carriedOver], &curGen->schedules[MIN(x, y)]);
        carriedOver++;
    }
    
    return carriedOver;
}

/* Breed population from Schedules up to carryOver */
void ga_crossbreed(Generation *gp, int carryOver)
{
    int i, j, parentA, parentB, selectedParent;
    
    /* Breed the remaining part of the Generation */
    for (i = carryOver; i < GENERATION_SIZE; i++)
    {
        /* Find two different parents to be mated */
        do
        {
            parentA = rand() % carryOver;
            parentB = rand() % carryOver;
        } while(parentA == parentB);
        
        /*
         * Iterate through all lectures and compare their fitness.
         * Note: This assumes that the lectures fall in the same order
         * for both parents!
        */
        for (j = 0; j < gp->sd->numLectures; j++)
        {
            /* Select best fit */
            if (gp->schedules[parentA].lectures[j].fitness <= gp->schedules[parentB].lectures[j].fitness)
                selectedParent = parentA;
            else
                selectedParent = parentB;

            /* Copy lecture */
            gp->schedules[i].lectures[j] = gp->schedules[selectedParent].lectures[j];
        }
    }
}

/* Randomly mutate Schedules in a Generation */
void ga_mutate(Generation *gp)
{
    int i, j;

    /* Iterate through all Schedules */
    for (i = 0; i < GENERATION_SIZE; i++)
    {
        if (rand() % 100 > MUTATION_CHANCE)
            continue;
        
        /* Iterate through all Lectures in Schedule */
        for (j = 0; j < gp->sd->numLectures; j++)
        {
            if (rand() % 100 > MUTATION_CHANCE)
                continue;
            
            if (rand() % 2)
                gp->schedules[i].lectures[j].day = rand() % (gp->sd->numWeeks * DAYS_PER_WEEK);
            
            if (rand() % 2)
                gp->schedules[i].lectures[j].period = rand() % MAX_PERIODS;
            
            if (rand() % 2)
                gp->schedules[i].lectures[j].assignedRoom = &gp->sd->rooms[rand() % gp->sd->numRooms];
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