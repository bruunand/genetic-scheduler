/**
 *  \file input_reader.h
 *  \brief This file contains prototypes required by the input_reader.c script
 */
int read_config(char *fileName, SemesterData *data);
void handle_line(char *line, SemesterData *data);

int read_int(char *line, int *position, int *out);
int read_multiple_words(char *line, int *position, char *out);

void add_teacher(SemesterData *sd, char *name, int numOffTimes, OffTime *offTimes);
void add_room(SemesterData *sd, char *name, int seats);
void add_course(SemesterData *sd, char *name, int totLectures, int numTeachers, Teacher **teachers);
void add_specialization(SemesterData *sd, char *name, int numStudents, int numCourses, Course **courses);

void validate_input(SemesterData *sd);