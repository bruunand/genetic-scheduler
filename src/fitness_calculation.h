/**
 *  \file fitness_calculation.h
 *  \brief This file contains prototypes required by the fitness_calculation.c script
 */
int calcfit_capcity(SemesterData *sd, Lecture *lect);

int calcfit_teacher_availability(Generation *gp, int scheduleId, Lecture *lect);

int calcfit_doublebooking(Generation *gp, int scheduleId, Lecture *lect);

int calcfit_distribution_weekly(Generation *gp, int scheduleId, Lecture *lect);

int calcfit_distribution_semester(Generation *gp, int scheduleId, Lecture *lect);
int calcfit_distribution_semester_inner(Generation *gp, int scheduleId, Lecture *lect, Specialization *sp);

int calcfit_schedule(Generation *gp, int scheduleId);

int calcfit_lecture(Generation *gp, int scheduleId, Lecture *lect);