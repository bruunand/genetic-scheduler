#define MAX_OVER_CAPACITY 1.05

int test_capacity(semesterData *sd)
{
    int i, numStudents, roomCap, severity = 0;
    
    for(i = 0; i < numLectures;i++)
    {
        severity += test_lecture_capacity(sd, i);
    }
    
    return severity;
    
}
int test_lecture_capacity(semesterData *sd, int lecture)
{
    lecture *lect = get_lecture(sd, lecture);
    int roomCap, numStudents, severity = 0;
    
    roomCap = MAX_OVER_CAPACITY * room[lect->assignedRoom].seats
    numStudents = get_students_on_course(sd, lect->assignedCourse)
    
    if(roomCap < numStudents)
        severity += 1 + ((numStudents - roomCap) / (roomCap - room[lect->assignedRoom].seats))
    else if((roomCap / numStudents) >= 2)
        severity++
    
    return severity;
}
        