/**
 *  \file scheduler.h
 *  \brief This file contains prototypes required by the scheduler.c script
 */
 
void free_generation(Generation *gp);
void free_semesterdata(SemesterData *sd);

Generation generate_next_generation(Generation *curGen);
Generation generate_initial_generation(SemesterData *sd);