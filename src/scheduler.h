/**
 *  \file scheduler.h
 *  \brief This file contains prototypes required by the scheduler.c script
 */
void free_generation(Generation *gp);
void free_semesterdata(SemesterData *sd);

int compare_schedule_fitness(const void *a, const void *b);

void initialize_schedule(Generation *parentGen, int scheduleIndex);

void free_generation(Generation *gp);