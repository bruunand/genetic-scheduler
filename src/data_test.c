#include "structs.h"
#include "data_utility.h"
#include "defs.h"


int test_lecture_capacity(SemesterData *sd, Lecture *lect)
{
    int roomCap, numStudents, severity = 0, roomSeats;
    
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

int test_doublebooking(SemesterData *sd, Lecture *lect)
{
    int severity = 0, i, j, k;
    
    /* Test room doublebooking */
    
    return severity;
}

int test_teacher_availability(SemesterData *sd, Lecture *lect)
{
    int severity = 0, i, j, k;
    
    /*
     * Iterate through all teachers assigned to this course.
     * If they have an offtime on this day, increase the severity.
     * If they have another lecture, increase the severity.
    */
    for (i = 0; i < lect->assignedCourse->numTeachers; i++)
    {
        Teacher *curTeacher = lect->assignedCourse->teachers[i];
        
        /* Test if they are off on this day */
        if (teacher_has_offtime(sd, curTeacher, lect->day, lect->period))
            severity += 100;
        
        /* Test if they have another lecture at this time */
        for (j = 0; j < sd->numLectures; j++)
        {
            Lecture *curLect = &sd->lectures[j];
            
            /* Skip lecture being tested */
            if (curLect == lect)
                continue;
            
            /* Skip lecture that are at different times */
            if (curLect->day != lect->day || curLect->period != lect->period)
                continue;
            
            /* Increase severity if this teacher is on this course */
            for (k = 0; k < curLect->assignedCourse->numTeachers; k++)
            {
                if (curLect->assignedCourse->teachers[k] == curTeacher)
                    severity += 100;
            }
        }
    }
    
    return severity;
}