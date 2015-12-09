#include "structs.h"

void add_lecture(semesterData *sd, int scheduleId, int lectIndex, int day, int period, int assignedRoom, int assignedCourse)

int teacher_has_offtime(SemesterData *sd, Teacher *teacher, int dayId, int periodId);
int specialization_has_lecture(Specialization *sp, Lecture *lect);

int get_students_on_course(SemesterData *sd, Course* course);
int get_amount_of_lectures(SemesterData *sd);

const char* get_name_of_period(int periodId);
const char* get_name_of_day(int dayId);