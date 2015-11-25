
void test_distribution(semesterData *sd)
{
    int i, courseNum, weekNum, dayNum, totCoursePerWeek, totCoursePerDay, totDays, severity = 0;
  
    totDays = numWeeks * DAYS_PER_WEEK;
    
    for (courseNum = 0; courseNum < numCourses, courseNum++)
        for (weekNum = 0, dayNum = 0; dayNum < totDays; dayNum++)
        {
            if (dayNum / DAYS_PER_WEEK > weekNum)
                weekNum++;
            
            for (i = 0, totCoursePerWeek = 0, totCoursePerDay = 0; i < sd->numLectures); i++)
            {
                if (sd->lectures[i].day / DAYS_PER_WEEK == weekNum && sd->lectures[i].assignedCourse == courseID)
                   totCoursePerWeek++; 
                if (sd->lectures[i].day == dayNum && sd->lectures[i].assignedCourse == courseID)
                    totCoursePerDay++;
            }
            
            if (totCoursePerWeek > 3)
                severity += totCoursePerWeek - 3;
            if (totCoursePerDay > 1)
                severity++;
        }
    
    return severity;
}