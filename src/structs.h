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
    OffTime **offTimes;
} Teacher;

/* Course struct */
typedef struct
{
    char name[64];
    int totLectures;
    
    int numTeachers;     /* Number of teachers associated */
    Teacher ***teachers; /* Array of associated teachers */
} Course;

/* Specialization (major) struct */
typedef struct
{
    char name[32];
    
    int numStudents;
    
    int numCourses;
    Course **courses;
} Specialization;

/* Lecture struct */
typedef struct
{
    int day;
    int period;
    
    /* int numLectures; */
    
    Room   *assignedRoom;
    Course *ssignedCourse;
} Lecture;

/* SemesterData struct */
typedef struct
{
    int numWeeks;
    
    int numRooms;
    Room **rooms;
    
    int numTeachers;
    Teacher **teachers;
    
    int numCourses;
    Course **courses;
    
    int numSpecializations;
    Specialization **specializations;
    
    int numLectures;
    Lecture **lectures;
} SemesterData;

typedef struct
{
    SemesterData *currentGeneration;
    SemesterData *newGeneration;
} Generations;