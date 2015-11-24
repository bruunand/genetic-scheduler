#pragma once

typedef unsigned int uint;

/* Room struct */
typedef struct
{
    char name[32];
    uint seats;
} room;

/* OffTime struct */
typedef struct
{
    uint day;
    uint periods[2];
} offTime;

/* Teacher struct */
typedef struct
{
    char name[32];
    
    uint numOffTimes;
    uint *offDays;
} teacher;

/* Course struct */
typedef struct
{
    char name[32];
    /* uint totalStudents; */
    uint totalLectures;
    uint numTeachers;
    uint *teachers; /* Teacher index */
} course;

/* Specialization (major) struct */
typedef struct
{
    char name[32];
    uint numStudents;
    uint numCourses;
    uint *courses;
} specialization;

/* Lecture struct */
typedef struct
{
    uint day;
    uint period;
    uint numLectures;
    uint assignedRoom;
    uint assignedCourse;
} lecture;

/* SemesterData struct */
typedef struct
{
    uint numWeeks;
    
    uint numRooms;
    room *rooms;
    
    uint numTeachers;
    teacher *teachers;
    
    uint numCourses;
    course *courses;
    
    uint numSpecializations;
    specialization *specializations;
    
    uint numLectures;
    lecture *currentGeneration;
    lecture *nextGeneration;
} semesterData;