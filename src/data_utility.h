/**
 *  \file data_utility.h
 *  \brief This file contains prototypes required by the data_utility.c script
 */
#include "structs.h"

void initialize_generation(Generation **gen, SemesterData *sd);

void initialize_schedule(Generation *parentGen, int scheduleIndex);

void copy_generation(Generation *dest, Generation *src);

void copy_schedule(Schedule *dest, Schedule *src);

void print_schedule_issues(Schedule *schedule);

void reset_schedule_flags(Schedule *schedule);

void set_lecture(Lecture *lect, int day, int period, Room *room, Course *course);

int teacher_has_offtime(SemesterData *sd, Teacher *teacher, int dayId, int periodId);

int specialization_has_lecture(Specialization *sp, Lecture *lect);

int get_students_on_course(SemesterData *sd, Course* course);

void calc_amount_of_lectures(SemesterData *sd);

int get_specializations_on_course(SemesterData *sd, Course *course, Specialization ***specs);

void free_generation(Generation *gp);

void free_semesterdata(SemesterData *sd);

const char* get_name_of_period(int periodId);
const char* get_name_of_day(int dayId);