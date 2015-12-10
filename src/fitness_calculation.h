/**
 *  \file fitness_calculation.h
 *  \brief This file contains prototypes required by the fitness_calculation.c script
 */
int calcfit_capacity(SemesterData *sd, Lecture *lect);

int calcfit_teacher_availability(Schedule *schedule, Lecture *lect);

int calcfit_doublebooking(Schedule *schedule, Lecture *lect);

int calcfit_distribution_weekly(Schedule *schedule, Lecture *lect);

int calcfit_distribution_semester(Schedule *schedule, Lecture *lect);
int calcfit_distribution_semester_inner(Schedule *schedule, Lecture *lect, Specialization *sp);

int calcfit_lecture(Schedule *schedule, Lecture *lect);

int calcfit_schedule(Schedule *schedule);

void calcfit_generation(Generation *gp);