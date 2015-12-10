/**
 *  \file structs.h
 *  \brief The header file containing all the structs required by the program
 */
#pragma once

#include "defs.h"

/**
 *  \struct struct Room
 *  \brief The Room struct contains the name of a specific room
 */
struct Room
{
    char name[32];
    
    int seats;
};

/**
 *  \struct struct Teacher
 *  \brief The Teacher struct contains information about a specific teacher
 */
struct Teacher
{
    char name[32];
    
    int numOffTimes;
    struct OffTime *offTimes;
};

/**
 *  \struct struct OffTime
 *  \brief The OffTime struct contains a day and time period (0 or 1) where the teacher isn't available for a teacher
 */
struct OffTime
{
    int day;
    int periods[MAX_PERIODS];
};

/**
 *  \struct struct Course
 *  \brief The Course struct contains information about a specific course
 */
struct Course
{
    char name[64];
    int totLectures;
    
    int numTeachers;     /* Number of teachers associated */
    struct Teacher **teachers;  /* Array of associated teachers */
};

/**
 *  \struct struct Specialization
 *  \brief The Specialization struct contains information about a specific Specialization
 */
struct Specialization
{
    char name[32];
    
    int numStudents;
    
    int numCourses;
    struct Course **courses;
};

/**
 *  \struct struct Flags
 *  \brief The Flags struct contains a list of flags used to prevent double calculation of fitness
 */
struct Flags
{
    int doubleBookingRoom;
    int doubleBookingLecture;
    int lectureCounted;
    int semesterCounted;
};

/**
 *  \struct struct Lecture
 *  \brief The Lecture struct contains information about a specific Lecture
 */
struct Lecture
{
    int day;
    int period;

    struct Room   *assignedRoom;
    struct Course *assignedCourse;

    struct Flags flags;
};

/**
 *  \struct struct SemesterData
 *  \brief The SemesterData struct contains all available information about a specific semester
 *  \details A generation would be built with an amount of SemesterData structs
 */
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

/**
 *  \struct struct Schedule
 *  \brief ????????????????
 */
struct Schedule
{
	/* Pointer to the generation that this schedule belongs to */
	struct Generation *parentGen;
	
	/* Array of lectures */
	struct Lecture *lectures;
	
	/* Last calculated fitness value */
	int fitness;
};

/**
 *  \struct struct Generation
 *  \brief The Generation struct contains an array of schedules in the generation and a pointer to SemesterData which contains relevant information
 */
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