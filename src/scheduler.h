/**
 *  \file scheduler.h
 *  \brief This file contains prototypes required by the scheduler.c script
 */
void free_generation(Generation *gp);
void free_semesterdata(SemesterData *sd);

int compare_schedule_fitness(const void *a, const void *b);

void initialize_schedule(Generation *parentGen, int scheduleIndex);
void genetic_optimization(Generation *curGen, Generation *nextGen, FILE* output);
void generate_next_generation(Generation *oldGen, Generation *newGen);
void generate_initial_generation(Generation *initialGen, SemesterData *sd);