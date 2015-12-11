/**
 *  \file defs.h
 *  \brief This file contains the defines required by the program
 */
#define MAX_PERIODS   2
#define DAYS_PER_WEEK 5
#define GENERATION_SIZE 1000
#define MAX_GENERATIONS 250
#define MUTATION_CHANCE 10

/* Error codes */
#define ERROR_OUT_OF_MEMORY 1
#define ERROR_ARRAY_BOUNDS_EXCEEDED 2
#define ERROR_FILE_NULL_PTR 3

/* MAX_OVER_CAPACITY is the amount a room is allowed to be over capacity. For example 5 % */
#define MAX_OVER_CAPACITY 1.05
#define MAX_LECTURES_PER_WEEK 7

#define PENALTY_DOUBLEBOOKING     200
#define PENALTY_TEACHER_BOOKED    200
#define PENALTY_TEACHER_OFFTIME   200
#define PENALTY_DAILY_LIMIT       50
#define PENALTY_WEEKLY_LIMIT      10
#define PENALTY_SEMESTER_DISTRIB  50