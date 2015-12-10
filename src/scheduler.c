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
    Generation nextGen;
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
    printf("init\n");
    generate_initial_generation(&curGen, &sd);

    printf("opti\n");
    FILE *output = fopen("output.txt", "w+");
    genetic_optimization(&curGen, &nextGen, output);
    fclose(output);
    
    /* Print schedules to files */
    printf("best %d\n", nextGen.schedules[0].fitness);
    print_schedule_to_file(&nextGen.schedules[0], &sd.specializations[0], "swdat.html");
    print_schedule_to_file(&nextGen.schedules[0], &sd.specializations[1], "robotics.html");
    
    free_generation(&curGen);
    free_semesterdata(&sd);
    
    return 0;
}

int mutate(Generation *new, int genomeId)
{
    int i = 0, mutations = 0;

    do
    {
        i++;
        
        if(rand() % 2)
        {
            new->schedules[genomeId].lectures[i].day = rand() % (new->sd->numWeeks * DAYS_PER_WEEK);
            mutations++;
        }

        if(rand() % 2)
        {
            new->schedules[genomeId].lectures[i].period = rand() % MAX_PERIODS;
            mutations++;
        }
      
        if(rand() % 2)
        {
            new->schedules[genomeId].lectures[i].assignedRoom = &new->sd->rooms[rand() % new->sd->numRooms];
            mutations++;
        }
        
    } while(i < new->sd->numLectures);

    return mutations;
}

int crossbreed(Generation *new, int genomeId, int carryover)
{
    int i, genesSwitched = 0, parentA, parentB, selectedParent;
    
    /* Initialize schedule */
    initialize_schedule(new, genomeId);
    
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
    for(i = 0; i < new->sd->numLectures; i++)
    {
        /* Select best fit */
        if (new->schedules[parentA].lectures[i].fitness <= new->schedules[parentB].lectures[i].fitness)
            selectedParent = parentA;
        else
            selectedParent = parentB;
        
        /* Copy lecture */
        copy_lecture(&new->schedules[genomeId].lectures[i], &new->schedules[selectedParent].lectures[i]);
        
        genesSwitched++;
    }
    
    return genesSwitched;
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

void genetic_optimization(Generation *curGen, Generation *nextGen, FILE* output)
{
    int i, genFitness, lowestGenomeFitness = -1, startTime;
    Generation *tmpGen;
    
    startTime = time(NULL);
    
    /* Run up to n generations */
    for (i = 0; i < MAX_GENERATIONS; i++)
    {
        /* Calculate fitness of curGen (old) generation */
        calcfit_generation(curGen);
        
        /* Sort genomes by fitness */
        qsort(curGen->schedules, GENERATION_SIZE, sizeof(Schedule), compare_schedule_fitness);
        
        printf("%3d: %5d\n", i + 1, curGen->schedules[0].fitness);
                
        fprintf(output, "%d\t%d\n", i + 1, curGen->schedules[0].fitness);
        
        /* Determine best schedule */
        if (curGen->schedules[0].fitness < lowestGenomeFitness || lowestGenomeFitness == -1)
            lowestGenomeFitness = curGen->schedules[0].fitness;
        
        if (curGen->schedules[0].fitness < 150)
        {
            printf("Schedule issues:\n");
            print_schedule_issues(&curGen->schedules[0]);
        }
        
        if (curGen->schedules[0].fitness < 15)
            break;
        
        /* Put next generation into nextGen */
        generate_next_generation(curGen, nextGen);
        
        /* Set new current generation */
        tmpGen = curGen;
        curGen = nextGen;
        
        /* Free temp (old) generation */
        free_generation(tmpGen);
    }        
    
    printf("GA Done: Took %d seconds.\n Lowest fit %d\n", time(NULL) - startTime, lowestGenomeFitness);
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
    int i, x, y, carryover, newGenMembers = 0;

    /* Set semesterdata */
    newGen->sd = oldGen->sd;

    /* 
     * Let half the population of oldGen survive. Randomly chosen, but weighted 
     * towards better fitness.
    */
    for (i = 0; i < GENERATION_SIZE / 2; i++)
    {
        x = rand() % GENERATION_SIZE;
        y = rand() % GENERATION_SIZE;
        
        /*
         * Copy schedule to the new generation.
         * The lowest value between x and y is picked, prioriziting good genomes.
        */
        copy_schedule(&newGen->schedules[newGenMembers++], &oldGen->schedules[(x < y) ? x : y], newGen);
    }

    /* Crossbreed to get a total of 100 genomes */
    carryover = newGenMembers; 
    while (newGenMembers < GENERATION_SIZE)
    {
        crossbreed(newGen, newGenMembers++, carryover);
    }

    /* Mutate randomly */
    do
    {
        x = rand() % 100;
        
        if (x < MUTATION_CHANCE)
            mutate(newGen, rand() % GENERATION_SIZE);
    } while (x < MUTATION_CHANCE);
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
 *  \brief Brief
 *  
 *  \param [in] gp Parameter_Description
 *  \return Return_Description
 *  
 *  \details Details
 */
void free_generation(Generation *gp)
{
    if (gp->schedules)
        free(gp->schedules);
    
    free(gp);
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