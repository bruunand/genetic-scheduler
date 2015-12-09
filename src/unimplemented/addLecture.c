#include <stdio.h>
#include <structs.h>

void add_lecture(semesterData *sd, Generations *generation, uint schedule, uint day, uint period, uint assignedRoom, uint assignedCourse, uint assignedSpecialization)
{
    /* Reallocate memory for the new count */
    uint lectIndex = generation->numCurGenLectures[schedule]++;
    generation->curGeneration[schedule] = (*lecture) realloc(generation->curGeneration[schedule], generation->numCurGenLectures[schedule] * sizeof(lecture));
    
    /* Set values */
    /* generation->numCurGenLectures[schedule].lectures[lectIndex].numLectures = numLectures; */
    generation->numCurGenLectures[schedule].lectures[lectIndex].day = day;
    generation->numCurGenLectures[schedule].lectures[lectIndex].period = period;
    generation->numCurGenLectures[schedule].lectures[lectIndex].assignedRoom = assignedRoom;
    generation->numCurGenLectures[schedule].lectures[lectIndex].assignedCourse = assignedCourse;
    generation->numCurGenLectures[schedule].lectures[lectIndex].assignedSpecialization = assignedSpecialization; 
}