/**
 *  \file data_test.h
 *  \brief This file contains prototypes required by the data_test.c script
 */
int test_lecture_capacity(SemesterData *sd, Lecture *lect);

int test_overlap(SemesterData *sd, Lecture *lect);

int test_teacher_availability(SemesterData *sd, Lecture *lect);

int test_doublebooking(SemesterData *sd, Lecture *lect);

int test_weekly_distribution(SemesterData *sd, Lecture *lect);

int test_semester_distribution(SemesterData *sd, Lecture *lect);
int test_semester_distribution_inner(SemesterData *sd, Lecture *lect, Specialization *sp);