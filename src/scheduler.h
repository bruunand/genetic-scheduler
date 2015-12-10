/**
 *  \file scheduler.h
 *  \brief This file contains prototypes required by the scheduler.c script
 */
 
void free_generation(Generation *gp);
void free_semesterdata(SemesterData *sd);

void initialize_schedule(Generation *parentGen, int scheduleIndex);
void generate_next_generation(Generation *oldGen, Generation *newGen);
void generate_initial_generation(Generation *initialGen, SemesterData *sd);