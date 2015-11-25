


int calculateLectureAmount(semesterData *sd)
{
    int lectureAmount = 0;
    int i, j;
    
    /* For each specialization and each course the number
     * of lectures is added to lectureAmount 
     */
    for (i = 0; i < sd->numSpecializations; i++)
    {
        for (j = 0; j < sd->specializations[i].numCourses; j++)
        {
            lectureAmount += sd->specializations[i].courses[j].totalLectures;
        }
        
    }
    
    return lectureAmount;
}