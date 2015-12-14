#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "structs.h"
#include "input_reader.h"
#include "data_utility.h"
#include "fitness_calculation.h"

#define TEST_LECTURES_IN_COURSE 15

void test_calc_lecture_amount(SemesterData *sd)
{
    calc_amount_of_lectures(sd);
    assert(sd->numLectures == TEST_LECTURES_IN_COURSE);
}

void test_calcfit_capacity(SemesterData *sd, Lecture *lect)
{
    assert(calcfit_capacity(sd, lect) > 0);
}

int main(void)
{
    Generation *testGen;
    Schedule *testSchedule;
    SemesterData sd;
    OffTime offTimes[2];
    srand(1234);
    
    memset(&sd, 0, sizeof(SemesterData));
    testGen->sd = &sd;
    
    /* Test for one week */
    sd.numWeeks = 1;

    /* Teacher is unavailable on day 2, period 0 */
    offTimes[0].day = 2;
    offTimes[0].periods[0] = 1;
    offTimes[0].periods[1] = 0;
    
    /* Teacher is unavailable on day 3, period 1 */
    offTimes[1].day = 3;
    offTimes[1].periods[0] = 0;
    offTimes[1].periods[1] = 1;

    /* Add a room with 150 seats */
    add_room(&sd, "Dummy Room", 150);
    
    /* Add a teacher with two offtimes */
    add_teacher(&sd, "Dummy Teacher", 2, offTimes);
    
    /* Add a course that has the Dummy Teacher */
    add_course(&sd, "Dummy Course", TEST_LECTURES_IN_COURSE, sd.numTeachers, &sd.teachers);
    
    /* Add a specialization that has the Dummy Course */
    add_specialization(&sd, "Dummy Specialization", 0, sd.numCourses, &sd.courses);
    
    /* Test calculation of Lecture amount */
    test_calc_lecture_amount(&sd);
    
    /* Initialize test generation */
    initialize_generation(&testGen, &sd);
    testSchedule = &testGen->schedules[0];
    
    /* Add test lectures */
    set_lecture(&testSchedule->lectures[0], 2, 0, &sd.rooms[0], &sd.courses[0]);
    set_lecture(&testSchedule->lectures[1], 3, 1, &sd.rooms[0], &sd.courses[0]);
    
    /* Test capacity function */
    test_calcfit_capacity(&sd, &testSchedule->lectures[0]);
    test_calcfit_capacity(&sd, &testSchedule->lectures[1]);
    
    /* Reset flags before testing */
    reset_schedule_flags(testSchedule);
    
    return 0;
}