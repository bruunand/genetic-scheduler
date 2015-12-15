#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "structs.h"
#include "input_reader.h"
#include "data_utility.h"
#include "fitness_calculation.h"
#include "html_output.h"

#define TEST_LECTURES_IN_COURSE 15
#define TEST_NUM_WEEKS 4
#define TEST_NUM_STUDENTS 160

void test_calc_lecture_amount(SemesterData *sd, int expected)
{
    calc_amount_of_lectures(sd);
    printf("Lecture test: %d\n", sd->numLectures);
    assert(sd->numLectures == expected);
}

void test_calcfit_capacity(SemesterData *sd, Lecture *lect)
{
    int res = calcfit_capacity(sd, lect);
    printf("Capacity test: %d\n", res);
    assert(res > 0);
}

int main(void)
{
    Generation *testGen;
    Schedule *testSchedule;
    SemesterData sd;
    OffTime offTimes[2];
    
    /* Set seed to a constant value so we'll always use the same schedule.
     * The test file will need to be updated if the generation
     * initializer is changed.
    */
    srand(0);
    
    /* Setup test */
    memset(&sd, 0, sizeof(SemesterData));
    testGen->sd = &sd;
    sd.numWeeks = TEST_NUM_WEEKS;

    /* Teacher is unavailable on day 2, period 0 */
    offTimes[0].day = 2;
    offTimes[0].periods[0] = 1;
    offTimes[0].periods[1] = 0;
    
    /* Teacher is unavailable on day 3, period 1 */
    offTimes[1].day = 3;
    offTimes[1].periods[0] = 0;
    offTimes[1].periods[1] = 1;

    /* Add rooms */
    add_room(&sd, "Dummy Room 1", 150);
    add_room(&sd, "Dummy Room 2", 100);
    
    /* Add a teacher with two offtimes */
    add_teacher(&sd, "Dummy Teacher", 2, offTimes);
    
    /* Add a course that has the Dummy Teacher */
    add_course(&sd, "Dummy Course", TEST_LECTURES_IN_COURSE, sd.numTeachers, &sd.teachers);
    
    /* Add a specialization that has the Dummy Course */
    add_specialization(&sd, "Dummy Specialization", TEST_NUM_STUDENTS, sd.numCourses, &sd.courses);

    /* Initialize test generation */
    initialize_generation(&testGen, &sd);
    testSchedule = &testGen->schedules[0];
    
    /* Write to file */
    print_schedule_to_file(testSchedule, &sd.specializations[0], "testschedule.html");
    
    return 0;
}