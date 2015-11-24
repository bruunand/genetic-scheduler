#pragma once

#define MAX_PERIODS 2

/* Room struct */
typedef struct
{
    char name[32];
    int seats;
} room;

/* OffTime struct */
/* A day and time period (0 or 1) where the teacher isn't available */
typedef struct
{
    int day;
    int periods[MAX_PERIODS];
} offTime;

/* Teacher struct */
typedef struct
{
    char name[32];
    int numOffTimes;
    offTime *offTimes;
} teacher;

/* Course struct */
typedef struct
{
    char name[32];
    /* int totStudents; */
    int totLectures;
    int numTeachers;
    int *teachers; /* Array of associated teachers */
} course;

/* Specialization (major) struct */
typedef struct
{
    char name[32];
    int numStudents;
    int numCourses;
    int *courses;
} specialization;

/* Lecture struct */
typedef struct
{
    int day;
    int period;
    int numLectures;
    int assignedRoom;
    int assignedCourse;
} lecture;

/* SemesterData struct */
typedef struct
{
    int numWeeks;
    
    int numRooms;
    room *rooms;
    
    int numTeachers;
    teacher *teachers;
    
    int numCourses;
    course *courses;
    
    int numSpecializations;
    specialization *specializations;
    
    int numLectures;
    lecture *lectures;
} semesterData;

typedef struct
{
    semesterData *currentGeneration;
    semesterData *newGeneration;
} generations;