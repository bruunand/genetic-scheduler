/**
 *  \file genetic_algorithm.h
 *  \brief This file contains prototypes required by the genetic_algorithm.c script
 */
void run_ga(Generation **curGen, SemesterData *sd);

int ga_select(Generation *curGen, Generation *newGen);

void ga_crossbreed(Generation *gp, int carryOver);

void ga_mutate(Generation *gp);

int compare_schedule_fitness(const void *a, const void *b);