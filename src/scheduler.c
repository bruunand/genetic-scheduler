#include <stdio.h>
#include <stdlib.h>

#include "scheduler.h"
#include "config_reader.h"
#include "structs.h"

#define DAYS_PER_WEEK 5
#define NUM_ENTRIES   100
#define NUM_PERIODS   2
#define NUM_WEEKS     3

/* Global array of lectures */
lecture *currentGeneration = 0, *nextGeneration = 0;
unsigned int totalRooms = 0, totalTeachers = 0, totalCourses = 0;

int main(void)
{
    /* Read configuration file */
    if (!read_config())
    {
        printf("Error: Could not read configuration file.\n");
        exit(1);
    }
    
    free(currentGeneration);
}
