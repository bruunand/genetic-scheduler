#include "structs.h"
#include "data_utility.h"

#define MAX_OVER_CAPACITY 1.05

int test_lecture_capacity(semesterData *sd, int lectureId)
{
    int roomCap, numStudents, severity = 0, roomSeats;
    lecture *lect = get_lecture(sd, lectureId);
    
    roomSeats = sd->rooms[lect->assignedRoom].seats;
    roomCap = MAX_OVER_CAPACITY * roomSeats;
    numStudents = get_students_on_course(sd, lect->assignedCourse);
    
    if (roomCap < numStudents)
        severity += 1 + ((numStudents - roomCap) / ((roomCap > roomSeats)?(roomCap - roomSeats):1));
    else if ((roomCap / numStudents) >= 2)
        severity++;
    
    return severity;
}