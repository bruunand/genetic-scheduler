/**
 *  \file structs.h
 *  \brief The header file containing all the structs required by the program
 */
#pragma once

#include "defs.h"

/* Room struct */
struct Room
{
    char name[32];
    
    int seats;
};

/* Teacher struct */
struct Teacher
{
    char name[32];
    
    int numOffTimes;
    struct OffTime *offTimes;
};

/* OffTime struct */
/* A day and time period (0 or 1) where the teacher isn't available */
struct OffTime
{
    int day;
    int periods[MAX_PERIODS];
};

/* Course struct */
struct Course
{
    char name[64];
    int totLectures;
    
    int numTeachers;     /* Number of teachers associated */
    struct Teacher **teachers;  /* Array of associated teachers */
};

/* Specialization (major) struct */
struct Specialization
{
    char name[32];
    
    int numStudents;
    
    int numCourses;
    struct Course **courses;
};

/* Flags struct: Used to prevent double fitness. */
struct Flags
{
    int doubleBookingRoom;
    int doubleBookingLecture;
    int lectureOverflow;
    int semesterOverflow;
};

/* Lecture struct */
struct Lecture
{
    int day;
    int period;

    struct Room   *assignedRoom;
    struct Course *assignedCourse;

    struct Flags flags;
};

/* SemesterData struct */
struct SemesterData
{
	/* Total amount of weeks in the semester */
    int numWeeks;
    
	/* Size and array of rooms */
    int numRooms;
    struct Room *rooms;
    
	/* Size and array of teachers */
    int numTeachers;
    struct Teacher *teachers;
    
	/* Size and array of courses */
    int numCourses;
    struct Course *courses;
    
	/* Size and array of specializations */
    int numSpecializations;
    struct Specialization *specializations;
    
    /* The amount of lectures in each schedule. */
    int numLectures;
};

/* Schedule struct */
struct Schedule
{
	/* Pointer to the generation that this schedule belongs to */
	struct Generation *parentGen;
	
	/* Array of lectures */
	struct Lecture *lectures;
	
	/* Last calculated fitness value */
	int fitness;
};

/* Generation struct */
struct Generation
{    
    /* Array of schedules */
    struct Schedule schedules[GENERATION_SIZE];
    
    /* Pointer to SemesterData, which contains relevant information */
    struct SemesterData *sd;
};

typedef struct Room Room;
typedef struct OffTime OffTime;
typedef struct Teacher Teacher;
typedef struct Course Course;
typedef struct Specialization Specialization;
typedef struct Flags Flags;
typedef struct Lecture Lecture;
typedef struct SemesterData SemesterData;
typedef struct Schedule Schedule;
typedef struct Generation Generation;