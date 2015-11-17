/* Lecture struct */
typedef struct
{
	unsigned int day;
	unsigned int period;
	unsigned int numLecture;
	course* assignedCourse;
	room* assignedRoom;
} lecture;

/* Room struct */
typedef struct
{
	char[32] name;
	unsigned int size;
} room;

/* Teacher struct */
typedef struct
{
	char[32] name;
	unsigned int* offDays;
} teacher;

/* Course struct */
typedef struct
{
	char[32] name;
	unsigned int totalStudents;
	unsigned int totalLectures;
	teacher* assignedTeacher;
} course;