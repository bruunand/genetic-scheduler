/**
 *  \file data_utility.h
 *  \brief This file contains prototypes required by the data_utility.c script
 */
#include "structs.h"

void reset_schedule_flags(Schedule *schedule);

void set_lecture(Lecture *lect, int day, int period, Room *room, Course *course);

int teacher_has_offtime(SemesterData *sd, Teacher *teacher, int dayId, int periodId);
int specialization_has_lecture(Specialization *sp, Lecture *lect);

int get_students_on_course(SemesterData *sd, Course* course);
int get_amount_of_lectures(SemesterData *sd);

int get_specializations_for_course(SemesterData *sd, Course *course, Specialization ***specs);

const char* get_name_of_period(int periodId);
const char* get_name_of_day(int dayId);