int test_doublebooking_lectures(semesterData *sd, lecture *lecture, int numLectures)
{
    int fitness = 0, a, b, c, d = 0, e, f, g;
    specialization *specWithCurrentCourse;
    
    specWithCurrentCourse = (specialization*) malloc(16*sizeof(specialization));
    
    for(a = 0; a < numLectures-1; a++)
    {
        for(b = a+1; b < numLectures-1; b++)
        {
            if(lecture[a].assignedRoom == lecture[b].assignedRoom)
            {
                printf("Lecture %d is conflicting with lecture %d. Reason: Both lectures are in the same room!\n", a, b);
                fitness++;
            }
        }
    }
    
    for(a = 0; a < numLectures-1; a++)
    {
        for(b = a+1; b < numLectures-1; b++)
        {
            for(c = 0; c < sd->specializations[b].numCourses; c++)
            {
                if(lecture[a].assignedCourse == sd->specializations[b].courses[c])
                {
                    specWithCurrentCourse[d++] = sd->specializations[b];
                    
                    for(e = 0; e < c-1; e++)
                    {
                        for(f = 0; f < c-1; f++)
                        {
                            for(g = 0; g < specWithCurrentCourse[f].numCourses; g++)
                            {
                                if(lecture[e].assignedCourse == specWithCurrentCourse[f].courses[g])
                                {
                                    printf("Lecture %d is conflicting with lecture %d. Reason: Specialization %d contains both lectures!\n", e, g, f);
                                    fitness++;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    for(a = 0; a < numLectures-1; a++)
    {
        for(b = 0; b < numLectures-1; b++)
        {
            for(c = 0; c < sd->numTeachers; c++)
            {
                for(e = 0; e < sd->courses[lecture[c].assignedCourse].numTeachers; e++)
                {
                    if(sd->courses[lecture[a].assignedCourse].teachers[b] == sd->courses[lecture[c].assignedCourse].teachers[e])
                    {
                        printf("Lecture %d is conflicting with lecture %d. Reason: Teacher %d has both lectures!\n", a, c, b);
                        fitness++;
                    }
                }
            }
        }
    }
    return fitness;
}