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