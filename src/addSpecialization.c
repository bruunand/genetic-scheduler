#include <stdio.h>
#include <structs.h>

void add_specialization(semesterData *sd, char *name, uint numStudents)
{
    /* Reallocate memory for the new count */
    uint speclzIndex = sd->numSpecializations++;
    sd->specializations = (*specialization) realloc(sd->specializations, sd->numSpecializations * sizeof(specialization));
    
    /* Set values */
    sd->specializations[speclzIndex].numStudents = numStudents;
    sd->specializations[speclzIndex].numCourses = sd->numCourses;
    strcpy(sd->specializations[speclzIndex].name, name);
}