#define MAX_OVER_CAPACITY 1.05

int test_capacity(semesterData *sd)
{
    int i, numStudents, roomCap, infractions = 0;
    
    for(i = 0; i < numLectures;i++)
    {
        lecture *lect = get_lecture(sd, i);
        roomCap = MAX_OVER_CAPACITY * room[lect->assignedRoom].seats
        numStudents = get_students_on_course(sd, lect->assignedCourse)
        if(roomCap < numStudents)
            infractions += 1 + ((numStudents - roomCap) / (roomCap - room[lect->assignedRoom].seats))
    }
    
    return infractions;
    
}