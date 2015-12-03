#include "structs.h"
#include "data_utility.h"
#include "defs.h"


int test_lecture_capacity(SemesterData *sd, int lectureId)
{
    int roomCap, numStudents, severity = 0, roomSeats;
    Lecture *lect = &sd->lectures[lectureId];
    
    /* Functions, calculations and pointers used multiple times are assigned to local variables. */
    roomSeats = lect->assignedRoom->seats;
    roomCap = MAX_OVER_CAPACITY * roomSeats;
    numStudents = get_students_on_course(sd, lect->assignedCourse);

    /*
	 * Severity is increased by 1 per 5% room capacity is exceeded.
	 * Increased by 1 if half or less of the seats are used.
	*/
    if (roomCap < numStudents)
        severity += 1 + (numStudents - roomCap) / ((roomCap > roomSeats) ? (roomCap - roomSeats) : 1);
    else if ((roomSeats / numStudents) >= 2)
        severity++;
    
    return severity;
}