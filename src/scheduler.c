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

int compare_time(const void *a, const void *b)
{
    Lecture *firstLecture  = *(Lecture**) a;
    Lecture *secondLecture = *(Lecture**) b;
   
    if (firstLecture->day != secondLecture->day)
        return firstLecture->day - secondLecture->day;
    else
        return firstLecture->period - secondLecture->period;
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
    
    reset_lecture_flags(sd);
    
    /* Test all parameters for all lectures */
    for (i = 0; i < sd->numLectures; i++)
    {
        Lecture *curLect = &sd->lectures[i];
        curLect->fitness = 0;
        
        /* Test capacity for lecture room */
        curLect->fitness += test_lecture_capacity(sd, curLect);
        curLect->fitness += test_doublebooking(sd, curLect);
        curLect->fitness += test_teacher_availability(sd, curLect);
        curLect->fitness += test_weekly_distribution(sd, curLect);
        
        /* Add to combined fitness */
        combinedFitness += curLect->fitness;
        
        /* Insert pointer to this lecture in array */
        lecturePtrs[i] = curLect;
    }
    
    if (combinedFitness < 100)
        return combinedFitness;
    
    /* Mutate random elements */
    for (i = 0; i < 20; i++)
    {
        int rnd = rand() % sd->numLectures;
        
        /* Skip perfect lectures */
        if (lecturePtrs[rnd]->fitness <= 1)
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
    int i, seed = time(NULL);
    
    SemesterData sd;
    memset(&sd, 0, sizeof(SemesterData));
 
    /*scanf("%d", &seed);*/
    srand(seed);
    
    /* Read configuration file */
    if (!read_config("scheduler.input", &sd))
    {
        printf("Error: Could not read configuration file.\n");
        exit(1);
    }
    
    /* Generate schedule */
    generate_initial_schedule(&sd);
    
    /* WIP: Run max 500 generations */
    int lowestFitness = -1;
    for (i = 0; i < 1000000; i++)
    {
        int combinedFitness = generate_next(&sd);
        if (combinedFitness < lowestFitness || lowestFitness == -1)
            lowestFitness = combinedFitness;
        printf("%d = %d\n", i + 1, combinedFitness);
        if (combinedFitness < 100)
        {
            break;
        }
    }
    
    printf("lowest %d\n", lowestFitness);
    
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