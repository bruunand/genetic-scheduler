/* Room struct */
typedef struct
{
    char name[32];
    unsigned int size;
} room;

/* Teacher struct */
typedef struct
{
    char name[32];
    unsigned int* offDays;
} teacher;

/* Course struct */
typedef struct
{
    char name[32];
    unsigned int totalStudents;
    unsigned int totalLectures;
    teacher* assignedTeacher;
} course;

/* Lecture struct */
typedef struct
{
    unsigned int day;
    unsigned int period;
    unsigned int numLecture;
    course* assignedCourse;
    room* assignedRoom;
} lecture;