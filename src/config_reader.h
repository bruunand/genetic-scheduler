int read_config(char *fileName, semesterData *data);
void handle_line(char *line, semesterData *data);

int read_int(char *line, int *position, int *out);
int read_multiple_words(char *line, int *position, char *out);

void add_teacher(semesterData *sd, char *name, int numOffTimes, offTime *offTimes);
void add_room(semesterData *sd, char *name, int seats);
void add_course(semesterData *sd, char *name, int totLectures, int numTeachers, int *teachers);
void add_specialization(semesterData *sd, char *name, int numStudents, int numCourses, int *courses);