#include <stdio.h>
#include <structs.h>

void add_lecture(semesterData *sd, uint day, uint period, uint assignedRoom, uint assignedCourse, uint assignedSpecialization)
{
    /* Reallocate memory for the new count */
    uint lectIndex = sd->numLectures++;
    sd->lectures = (*lecture) realloc(sd->lectures, sd->numLectures * sizeof(lecture));
    
    /* Set values */
    sd->lectures[lectIndex].numLectures = numLectures;
    sd->lectures[lectIndex].day = day;
    sd->lectures[lectIndex].period = period;
    sd->lectures[lectIndex].assignedRoom = assignedRoom;
    sd->lectures[lectIndex].assignedCourse = assignedCourse;
    sd->lectures[lectIndex].assignedSpecialization = assignedSpecialization; 
}