typedef unsigned int uint;

/* Room struct */
typedef struct
{
    char name[32];
    uint seats;
} room;

/* Teacher struct */
typedef struct
{
    char name[32];
    
    uint numOffDays;
    uint* offDays;
} teacher;

/* Course struct */
typedef struct
{
    char name[32];
    uint totalStudents;
    uint totalLectures;
    teacher* assignedTeacher;
} course;

/* Lecture struct */
typedef struct
{
    uint day;
    uint period;
    uint numLectures;
    course* assignedCourse;
    room* assignedRoom;
} lecture;

/* ?? data struct */
typedef struct
{
    uint numWeeks;
    
    uint numRooms;
    room *rooms;
    
    uint numTeachers;
    teacher *teachers;
    
    uint numCourses;
    course *courses;
    
    uint numLectures;
    lecture *lectures;
} semesterData;