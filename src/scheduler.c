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
int generate_next(SemesterData *sd)
{
    Lecture **lecturePtrs;
    int i, combinedFitness = 0;
    
    /* Initialize array of pointers to lectures */
    lecturePtrs = malloc(sd->numLectures * sizeof(Lecture*));
    if (!lecturePtrs)
        exit(ERROR_OUT_OF_MEMORY);
    
    /* Test all parameters for all lectures */
    for (i = 0; i < sd->numLectures; i++)
    {
        Lecture *curLect = &sd->lectures[i];
        curLect->fitness = 0;
        
        /* Test capacity for lecture room */
        curLect->fitness += test_lecture_capacity(sd, curLect);
        curLect->fitness += test_doublebooking(sd, curLect);
        curLect->fitness += test_teacher_availability(sd, curLect);
        
        /* Add to combined fitness */
        combinedFitness += curLect->fitness;
        
        /* Insert pointer to this lecture in array */
        lecturePtrs[i] = curLect;
    }
    
    /* Sort array of pointers by highest fitness */
    /*qsort(lecturePtrs, sd->numLectures, sizeof(Lecture*), compare_fitness);*/
    
    /* Mutate random elements */
    for (i = 0; i < 25; i++)
    {
        int rnd = rand() % sd->numLectures;
        
        /* Skip perfect lectures */
        if (lecturePtrs[rnd]->fitness == 0)
            continue;

        /* Mutate */
        lecturePtrs[rnd]->day = rand() % (sd->numWeeks * DAYS_PER_WEEK);
        lecturePtrs[rnd]->period = rand() % 2;
        lecturePtrs[rnd]->assignedRoom = &sd->rooms[rand() % sd->numRooms];
    }
    
    return combinedFitness;
}

int main(void)
{   
    int i;
    
    SemesterData sd;
    memset(&sd, 0, sizeof(SemesterData));
 
    srand(time(NULL));
    
    /* Read configuration file */
    if (!read_config("scheduler.input", &sd))
    {
        printf("Error: Could not read configuration file.\n");
        exit(1);
    }
    
    /* Generate schedule */
    generate_initial_schedule(&sd);
    
    /* WIP: Run max 500 generations */
    for (i = 0; i < 500; i++)
    {
        int combinedFitness = generate_next(&sd);
        printf("Generation %d has a combined fitness of %d\n", i + 1, combinedFitness);

        if (combinedFitness < 50)
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
        Course *crs = &sd->courses[i];
        
        /* Randomly generate all lectures for this course */
        for (j = 0; j < crs->totLectures; j++)
            add_lecture(sd, k++, rand() % (DAYS_PER_WEEK * sd->numWeeks), rand() % MAX_PERIODS, rand() % sd->numRooms, i);
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