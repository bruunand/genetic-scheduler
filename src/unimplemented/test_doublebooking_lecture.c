/**
 *  \brief Tests doublebooking on an array of lectures
 *  
 *  \param [in] sd Semester data
 *  \param [in] lecture the lectures we use
 *  \param [in] numLectures the amount of lectures in the array
 *  \return returns the fitness of the test
 *  
 *  \details This function goes through an array of lectures and determins the fitness of the lectures within.
 */
int test_doublebooking_lectures(semesterData *sd, lecture *lecture, int numLectures)
{
    int fitness = 0, a, b, c, d = 0, e, f, g;
    specialization *specWithCurrentCourse;
    
    if(!specWithCurrentCourse = (specialization*) malloc(16*sizeof(specialization)))
    {
        exit(ERROR_OUT_OF_MEMORY);
    }
    
    /* This part checks for overlap on rooms */
    for(a = 0; a < numLectures-1; a++)
    {
        for(b = a+1; b < numLectures-1; b++)
        {
            if(lecture[a].assignedRoom == lecture[b].assignedRoom)
            {
                printf("Lecture %d is conflicting with lecture %d. Reason: Both lectures are in the same room!\n", a, b);
                fitness++;
                /* return ERROR_OVERLAP_ROOM */
            }
        }
    }
    
    /* Here we check for overlaps on specializations */
    for(a = 0; a < numLectures-1; a++)
    {
        get_specializations_for_course(sd, lecture[a].assignedCourse, specWithCurrentCourse);
        for(b = 0; b < numLectures-1; b++)
        {
            for(c = 0; c < specWithCurrentCourse[f].numCourses; c++)
            {
                if(lecture[a].assignedCourse == specWithCurrentCourse[b].courses[c])
                {
                    printf("Lecture %d is conflicting with lecture %d. Reason: Specialization %d contains both lectures!\n", a, c, b);
                    fitness++;
                    /* return ERROR_OVERLAP_SPECIALIZATION */
                }
            }
        }
    }
    
    /* Here we check for overlaps on teachers */
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
                        /* return ERROR_OVERLAP_TEACHER */
                    }
                }
            }
        }
    }
    return fitness;
}