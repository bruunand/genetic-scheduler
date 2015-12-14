/**
 *  \file structs.h
 *  \brief The header file containing all the structs required by the program
 */
#pragma once

#include "defs.h"

/**
 *  \struct Room
 *  \brief The Room struct contains the name of a specific room
 *  
 *  \var Room::name
 *  The name of the room
 *  
 *  \var Room::seats
 *  The number of seats in the room
 */
struct Room
{
    char name[32];
    
    int seats;
};

/**
 *  \struct Teacher
 *  \brief The Teacher struct contains information about a specific teacher
 *  
 *  \var Teacher::name
 *  The name of the teacher
 *  
 *  \var Teacher::numOffTimes
 *  The number of OffTimes the teacher has_denorm
 *  
 *  \var Teacher::offTimes
 *  Array of OffTimes
 */
struct Teacher
{
    char name[32];
    
    int numOffTimes;
    struct OffTime *offTimes;
};

/**
 *  \struct OffTime
 *  \brief The OffTime struct contains a day and time period (0 or 1) where
 *  the teacher isn't available for a teacher
 *  
 *  \var OffTime::day
 *  A specific day the OffTime effects
 *  
 *  \var OffTime::periods
 *  Array of effected periods
 */
struct OffTime
{
    int day;
    int periods[MAX_PERIODS];
};

/**
 *  \struct Course
 *  \brief The Course struct contains information about a specific course
 *  
 *  \var Course::name
 *  The name of the teacher
 *  
 *  \var Course::totLectures
 *  The total amount of lectures on the course
 *  
 *  \var Course::numTeachers
 *  Number of teachers associated
 *  
 *  \var Course::teachers
 *  Array of associated teachers
 */
struct Course
{
    char name[64];
    int totLectures;
    
    int numTeachers;
    struct Teacher **teachers;
};

/**
 *  \struct Specialization
 *  \brief The Specialization struct contains information about a specific specialization
 *  
 *  \var Specialization::name
 *  The name of the specialization
 *  
 *  \var Specialization::numStudents
 *  The number of students on the specialization
 *  
 *  \var Specialization::numCourses
 *  The number of courses on the specialization
 *  
 *  \var Specialization::courses
 *  Array of associated courses
 */
struct Specialization
{
    char name[32];
    
    int numStudents;
    
    int numCourses;
    struct Course **courses;
};

/**
 *  \struct Flags
 *  \brief The Flags struct contains a list of flags used to prevent double calculation of fitness
 *  
 *  \var Flags::doubleBookingRoom
 *  This flag is set when a lecture has been calculated for room doublebooking
 *  
 *  \var Flags::doubleBookingLecture
 *  This flag is set when a lecture has been calculated for overlapping with
 *  another lecture from the same specialization
 *  
 *  \var Flags::lectureCounted
 *  This flag is set when the lecture has been counted
 *  
 *  \var Flags::semesterCounted
 *  This flag is set when the semester that the lecure is part of is counted
 */
struct Flags
{
    int doubleBookingRoom;
    int doubleBookingLecture;
    int lectureCounted;
    int semesterCounted;
};

/**
 *  \struct Lecture
 *  \brief The Lecture struct contains information about a specific lecture
 *  
 *  \var Lecture::day
 *  The day the lecture is on
 *  
 *  \var Lecture::period
 *  The period of the day the lecture is on
 *  
 *  \var Lecture::assignedRoom
 *  Pointer to the room assigned to the lecture
 *  
 *  \var Lecture::assignedCourse
 *  Pointer to the course the lecture is part of
 *  
 *  \var Lecture::flags
 *  Link to lecture flags
 *  
 *  \var Lecture::fitness
 *  Last calculated fitness value
 */
struct Lecture
{
    int day;
    int period;

    struct Room   *assignedRoom;
    struct Course *assignedCourse;

    struct Flags flags;
    
    int fitness;
};

/**
 *  \struct SemesterData
 *  \brief The SemesterData struct contains all available information about a specific semester
 *  \details A generation would be built with an amount of SemesterData structs
 *  
 *  \var SemesterData::numWeeks
 *  Total amount of weeks in the semester
 *  
 *  \var SemesterData::numRooms
 *  The number of rooms assigned to the semester 
 *  
 *  \var SemesterData::rooms
 *  Array of assigned rooms
 *  
 *  \var SemesterData::numTeachers
 *  The number of teachers assigned to the semester
 *  
 *  \var SemesterData::teachers
 *  Array of assiged teachers
 *  
 *  \var SemesterData::numCourses
 *  The number of courses assigned to the semester
 *  
 *  \var SemesterData::courses
 *  Array of assigned courses
 *  
 *  \var SemesterData::numSpecializations
 *  The number of specializations assigned to the semester
 *  
 *  \var SemesterData::specializations
 *  Array of assigned specializations
 *  
 *  \var SemesterData::numLectures
 *  The number of lectures in each schedule
 */
struct SemesterData
{
    int numWeeks;
    
    int numRooms;
    struct Room *rooms;
    
    int numTeachers;
    struct Teacher *teachers;
    
    int numCourses;
    struct Course *courses;
    
    int numSpecializations;
    struct Specialization *specializations;
    
    int numLectures;
};

/**
 *  \struct Schedule
 *  \brief The Schedule struct contains all lectures for a given time spand
 *  
 *  \var Schedule::parentGen
 *  Pointer to the generation that this schedule belongs to
 *  
 *  \var Schedule::lectures
 *  Array of lectures
 *  
 *  \var Schedule::fitness
 *  Last calculated fitness value
 */
struct Schedule
{
	struct Generation *parentGen;
	struct Lecture *lectures;
    
	int fitness;
};
 
/**
 *  \struct Generation
 *  \brief The Generation struct contains an array of schedules in the generation
 *  and a pointer to SemesterData which contains relevant information
 *  
 *  \var Generation::fitness
 *  Combined fitness of generation
 *  
 *  \var Generation::schedules
 *  Array of schedules
 *  
 *  \var Generation::sd
 *  Pointer to SemesterData, which contains relevant information
 */
struct Generation
{
    int fitness;
    struct Schedule schedules[GENERATION_SIZE];
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