/**
 *  \brief Tests doublebooking
 *  
 *  \param [in] sd Semester data
 *  \return Returns the fitness of the test
 *  
 *  \details This function splits the shedule in two, and then goes through them one at a time in a new function
 */
int test_doublebooking(semesterData *sd)
{
    int fitness = 0, a, b, c=0, d=0;
    lecture *lecture0, *lecture1;
    
    /* Size may be changed */
    if(!lecture0 = (lecture*) malloc(16*sizeof(lecture)))
    {
        exit(ERROR_OUT_OF_MEMORY);
    }
    if(!lecture1 = (lecture*) malloc(16*sizeof(lecture)))
    {
        exit(ERROR_OUT_OF_MEMORY);
    }
    
    for(a = 0; a < sd->numWeeks*DAYS_PER_WEEK; a++)
    {
        for(b = 0; b < sd->numLectures; b++)
        {
            while(sd->lectures[b].day == a)
            {
                if(sd->lectures[b].period == 0);
                {
                    lecture0[c] = sd->lectures[b];
                    c++;
                }
                if(sd->lectures[b].period == 1);
                {
                    lecture1[d] = sd->lectures[b];
                    d++;
                }
            }
        }
    }
    fitness += test_doublebooking_lectures(sd, lecture0, c);
    fitness += test_doublebooking_lectures(sd, lecture1, d);

    return fitness;
}