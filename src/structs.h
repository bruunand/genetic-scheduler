/**
 *  \file structs.h
 *  \brief The header file containing all the structs required by the program
 */
#pragma once

#include "defs.h"

/* Room struct */
typedef struct
{
    char name[32];
    
    int seats;
} Room;

/* OffTime struct */
/* A day and time period (0 or 1) where the teacher isn't available */
typedef struct
{
    int day;
    int periods[MAX_PERIODS];
} OffTime;

/* Teacher struct */
typedef struct
{
    char name[32];
    
    int numOffTimes;
    OffTime *offTimes;
} Teacher;

/* Course struct */
typedef struct
{
    char name[64];
    int totLectures;
    
    int numTeachers;     /* Number of teachers associated */
    Teacher **teachers;  /* Array of associated teachers */
} Course;

/* Specialization (major) struct */
typedef struct
{
    char name[32];
    
    int numStudents;
    
    int numCourses;
    Course **courses;
} Specialization;

/* Flags struct: Used to prevent double fitness. */
typedef struct
{
    int doubleBookingRoom;
    int doubleBookingLecture;
    int lectureOverflow;
    int semesterOverflow;
} Flags;

/* Lecture struct */
typedef struct
{
    int day;
    int period;

    Room   *assignedRoom;
    Course *assignedCourse;

    Flags flags;
} Lecture;

/* SemesterData struct */
typedef struct
{
    int numWeeks;
    
    int numRooms;
    Room *rooms;
    
    int numTeachers;
    Teacher *teachers;
    
    int numCourses;
    Course *courses;
    
    int numSpecializations;
    Specialization *specializations;
    
    /* The amount of lectures in each schedule. */
    int numLectures;
} SemesterData;

/* Generation struct */
typedef struct
{
    /* Contains a pointer to SemesterData, which contains
     * information about teachers, rooms etc. */
    SemesterData *sd;
    
    /* Array of schedules. */
    Lecture *schedules[GENERATION_SIZE];
} Generation;