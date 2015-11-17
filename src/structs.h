typedef struct
{
	unsigned int day;
	unsigned int period;
	unsigned int numLecture;
	course* assignedCourse;
	room* assignedRoom;
} lecture;

typedef struct
{
	char[32] name;
	unsigned int size;
} room;

typedef struct
{
	char[32] name;
	unsigned int* offDays;
} teacher;

typedef struct
{
	char[32] name;
	unsigned int totalStudents;
	unsigned int totalLectures;
	teacher* assignedTeacher;
} course;