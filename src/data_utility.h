#include "structs.h"

void add_lecture(semesterData *sd, int day, int period, int roomId, int courseId)

room* get_room(semesterData *sd, int index);
teacher* get_teacher(semesterData *sd, int index);
course* get_course(semesterData *sd, int index);
specialization* get_specialization(semesterData *sd, int index);
lecture* get_lecture(semesterData *sd, int index);
offTime* get_offTime(teacher *tchr, int index);

int get_students_on_course(semesterData *sd, int courseIndex);

const char* get_name_of_period(int periodId);
const char* get_name_of_day(int dayId);