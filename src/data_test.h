int test_lecture_capacity(SemesterData *sd, Lecture *lect);

int test_overlap(SemesterData *sd, Lecture *lect);

int test_teacher_availability(Generation *gp, int scheduleId, Lecture *lect);

int test_doublebooking(Generation *gp, int scheduleId, Lecture *lect);

int test_weekly_distribution(Generation *gp, int scheduleId, Lecture *lect);

int test_semester_distribution(Generation *gp, int scheduleId, Lecture *lect);
int test_semester_distribution_inner(Generation *gp, int scheduleId, Lecture *lect, Specialization *sp);