/**
 *  \file defs.h
 *  \brief This file contains the defines required by the program
 */

/* Used in input_reader.c */
#define BUFFER_SIZE 512
/**
 *  \def BUFFER_SIZE
 *  The size of the string buffer to hold the content from the read file
 */

/* Used in html_output.c */
#define WEEK_WIDTH  10.0f
/**
 *  \def WEEK_WIDTH
 *  The space allocated for the width of the week on the web page
 */
#define TABLE_WIDTH 100.0f
/**
 *  \def TABLE_WIDTH
 *  The The width of the table on the web page
 */

#define MAX_PERIODS   2
/**
 *  \def MAX_PERIODS
 *  The maximum amount of periods on one day
 */
#define DAYS_PER_WEEK 5
/**
 *  \def DAYS_PER_WEEK
 *  The amount of days on one week
 */

#define GENERATION_SIZE 250
/**
 *  \def GENERATION_SIZE
 *  The amount of Schedules in one Generation
 */
#define MAX_GENERATIONS 500
/**
 *  \def MAX_GENERATIONS
 *  The maximum amount of generations till the program stops trying
 */
#define MUTATION_CHANCE 30
/**
 *  \def MUTATION_CHANCE
 *  The chance of a mutation to happen
 */

/* Error codes */
#define ERROR_OUT_OF_MEMORY 1
/**
 *  \def ERROR_OUT_OF_MEMORY
 *  Sent in the event that we cannot allocate required memory
 */
#define ERROR_ARRAY_BOUNDS_EXCEEDED 2
/**
 *  \def ERROR_ARRAY_BOUNDS_EXCEEDED
 *  Sent in the event that we exceed the bounds of an array
 */
#define ERROR_FILE_NULL_PTR 3
/**
 *  \def ERROR_FILE_NULL_PTR
 *  Sent in the event that we cannot find a requested file
 */
 #define ERROR_INVALID_INPUT 4
/**
 *  \def ERROR_INVALID_INPUT
 *  Error caused by invalid user input
 */

#define MAX_OVER_CAPACITY 1.05
/**
 *  \def MAX_OVER_CAPACITY
 *  The amount of students a room is allowed to be over capacity. For example 5%
 */
#define MAX_LECTURES_PER_WEEK 7
/**
 *  \def MAX_LECTURES_PER_WEEK
 *  The maximum amount of lectures per week
 */

/* Penalties */
#define PENALTY_ROOM_TOO_BIG      5
/**
 *  \def PENALTY_ROOM_TOO_BIG
 *  Given if the room is twice the amount of students
 */
#define PENALTY_ROOM_TOO_SMALL    50
/**
 *  \def PENALTY_ROOM_TOO_SMALL
 *  Given if the room is smaller than the amount of students
 */
#define PENALTY_DOUBLEBOOKING     200
/**
 *  \def PENALTY_DOUBLEBOOKING
 *  Given in case of room or specialization being doublebooked
 */
#define PENALTY_TEACHER_BOOKED    200
/**
 *  \def PENALTY_TEACHER_BOOKED
 *  Given in case of a teacher being doublebooked
 */
#define PENALTY_TEACHER_OFFTIME   200
/**
 *  \def PENALTY_TEACHER_OFFTIME
 *  Given in case of a teacher being off work
 */
#define PENALTY_DAILY_LIMIT       25
/**
 *  \def PENALTY_DAILY_LIMIT
 *  Given if a specific lecture appeares more than once per day
 */
#define PENALTY_WEEKLY_LIMIT      5
/**
 *  \def PENALTY_WEEKLY_LIMIT
 *  Given if there are more lectures in one week than a defined number
 */
#define PENALTY_SEMESTER_DISTRIB  50
/**
 *  \def PENALTY_SEMESTER_DISTRIB
 *  Given if there are many lectures by the end of a semester
 */

#define MIN(a,b) (((a)<(b))?(a):(b))
/**
 *  \def MIN(a,b)
 *  Computes the minumum of a and b
 */
#define MAX(a,b) (((a)>(b))?(a):(b))
/**
 *  \def MAX(a,b)
 *  Computes the maximum of a and b
 */