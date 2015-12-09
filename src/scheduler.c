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

int compare_fitness(const void *a, const void *b)
{
    Lecture *firstLecture  = *(Lecture**) a;
    Lecture *secondLecture = *(Lecture**) b;
   
    return secondLecture->fitness - firstLecture->fitness;
}

/* Work in progress */
int generate_next(SemesterData **curGen, SemesterData **newGen)
{
    Lecture **lecturePtrs;
    int i, combinedFitness = 0;
 
    for(i = 0; i < GENERATION_SIZE ; i++)
    {
        /* Sort array of  by highest fitness */
        qsort(curGen, GENERATION_SIZE, sizeof(SemesterData), compare_generation_fitness);
    }
    
    mutate(curGen, newGen);
    crossbreed(curGen, newGen);
    carry_on(curGen, newGen)
    
    return 1;
}

int main(void)
{   
    int i;
    
    SemesterData sd;
    Generations generation;
    memset(&sd, 0, sizeof(SemesterData))
    memset(&generation, 0, sizeof(Generations));
 
    srand(time(NULL));
    
    /* Read configuration file */
    if (!read_config("scheduler.input", &sd))
    {
        printf("Error: Could not read configuration file.\n");
        exit(1);
    }
    
    /* Generate schedules */
    generate_initial_schedule(&sd, &generation);
    
    /* WIP: Run max 200 generations */
    for (i = 0; i < 200; i++)
    {
        int combinedFitness = test_gen(curGen);
        generate_next(curGen, newGen);
        
        printf("Generation %d has a combined fitness of %d\n", i + 1, combinedFitness);
        
        if (combinedFitness == 0)
            break;
    }
    
    /* Print schedules to files */
    print_schedule_to_file(&sd, &sd.specializations[0], "swdat.html");
    print_schedule_to_file(&sd, &sd.specializations[1], "robotics.html");
    
    free_all(&sd);
    
    return 0;
}

/*
 * Generate a 'dumb' schedule (array of lectures)
 * The only fulfilled requirement is the amount of lectures per course
*/
void generate_initial_schedule(SemesterData *sd)
{
    int i, j, k = 0, l;

    /* Get total amount of lectures */
    sd->generation->numLectures = get_amount_of_lectures(sd);
    
    /* Allocate memory for the lectures */
    for(i = 0; i < GENERATION_SIZE ; i++)
    {
        sd->generation[i] = malloc(sd->generation->numLectures * sizeof(Lecture));
        if (!sd->generation[i])
            exit(ERROR_OUT_OF_MEMORY);
    }

    /* Go through all courses */
    for (i = 0, k = 0; i < sd->numCourses; i++)
    {
        Course *crs = &sd->courses[i];
        
        /* Randomly generate all lectures for this course */
        for (j = 0; j < crs->totLectures; j++)
        {
            for(l = 0; l < GENERATION_SIZE ; l++)
            {
                add_lecture(sd, l, k++, rand() % (DAYS_PER_WEEK * sd->numWeeks), rand() % MAX_PERIODS, rand() % sd->numRooms, i)
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