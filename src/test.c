#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "defs.h"
#include "structs.h"
#include "input_reader.h"
#include "data_utility.h"
#include "fitness_calculation.h"
#include "html_output.h"

#define TEST_LECTURES_IN_COURSE 15
#define TEST_NUM_WEEKS 4
#define TEST_NUM_STUDENTS 160
#define TEST_SEED 300

int main(void)
{
    Generation *testGen;
    Schedule *testSchedule;
    SemesterData sd;
    
    /* Set seed to a constant value so we'll always use the same schedule.
     * The test file will need to be updated if the generation
     * initializer is changed.
    */
    srand(TEST_SEED);
    
    /* Setup test */
    memset(&sd, 0, sizeof(SemesterData));
    
    /* Read configuration file */
    if (!read_config("test.cfg", &sd))
    {
        printf("Error: Could not read configuration file.\n");
        exit(1);
    }
    
    calc_amount_of_lectures(&sd);
    
    /* Initialize test generation */
    initialize_generation(&testGen, &sd);
    testSchedule = &testGen->schedules[0];
    
    /* Write to file */
    print_schedule_to_file(testSchedule, &sd.specializations[0], "test-schedule.html");
    
    return 0;
}