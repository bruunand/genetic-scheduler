#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

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
    Lecture testLecture;
    int i;
    
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
    reset_schedule_flags(testSchedule);
    
    /* Test capacity */
    
    /* Lecture 0 is assigned to a small room - expected score:
     * PENALTY_ROOM_TOO_SMALL * ((35 - 11) / (11 - 10)) = 1200 */
    assert(calcfit_capacity(&sd, &testSchedule->lectures[0]) == 1200);
    
    /* Lecture 1 is assigned to a perfect room - it should score 0 */
    assert(calcfit_capacity(&sd, &testSchedule->lectures[1]) == 0);
    
    /* Lecture 2 is assigned to a big room - it should score a penalty */
    assert(calcfit_capacity(&sd, &testSchedule->lectures[2]) == PENALTY_ROOM_TOO_BIG);

    /* Teacher availability */

    /* Teacher has offtime on day 2, period 0 and 1 */
    assert(teacher_has_offtime(&sd, &sd.teachers[0], 2, 0) == 1);
    assert(teacher_has_offtime(&sd, &sd.teachers[0], 2, 1) == 1);
    
    /* Teacher does not have offtime on day 0, period 0 */
    assert(teacher_has_offtime(&sd, &sd.teachers[0], 0, 0) == 0);
    
    /* Teacher should be unavailable on lecture 0 */
    /* Teacher is also booked TWICE on lecture 0, should give double penalty */
    assert(calcfit_teacher_availability(testSchedule, &testSchedule->lectures[0]) == PENALTY_TEACHER_BOOKED + PENALTY_TEACHER_OFFTIME);
    
    /* Fitness for doublebooking */
    
    /* Lecture 0 is conflicting with another lesson */
    assert(calcfit_doublebooking(testSchedule, &testSchedule->lectures[0]) == PENALTY_DOUBLEBOOKING);
    
    /* Lecture 1 has no doublebooking */
    assert(calcfit_doublebooking(testSchedule, &testSchedule->lectures[1]) == 0);
    
    /* Lecture 2 has no doublebooking */
    assert(calcfit_doublebooking(testSchedule, &testSchedule->lectures[2]) == 0);
    
    /* Test room doublebooking by creating a fake lecture */
    memcpy(&testLecture, &testSchedule->lectures[3], sizeof(Lecture));
    /* Since it's an exact copy, it should give a double penalty */
    assert(calcfit_doublebooking(testSchedule, &testLecture) == PENALTY_DOUBLEBOOKING * 2);
    
    /* Fitness for weekly distribution */
    
    /* Lecture 0 exceeds daily and weekly limit */
    printf("%d\n", PENALTY_WEEKLY_LIMIT);
    assert(calcfit_distribution_weekly(testSchedule, &testSchedule->lectures[0]) == pow(PENALTY_WEEKLY_LIMIT, 2) + PENALTY_DAILY_LIMIT);
    
    /* Lecture 1 exceeds weekly limit */
    reset_schedule_flags(testSchedule);
    assert(calcfit_distribution_weekly(testSchedule, &testSchedule->lectures[1]) == pow(PENALTY_WEEKLY_LIMIT, 2));
    
    /* Lecture 2 exceeds weekly limit */
    reset_schedule_flags(testSchedule);
    assert(calcfit_distribution_weekly(testSchedule, &testSchedule->lectures[2]) == pow(PENALTY_WEEKLY_LIMIT, 2));
    
    printf("All tests finished with no errors\n");
    
    /* Write to file */
    print_schedule_to_file(testSchedule, &sd.specializations[0], "test-schedule.html");
    
    return 0;
}