int test_gen(semesterData **curGen)
{
    int i, j, combinedFitness;
    
    for(i = 0; i < GENERATION_SIZE ; i++)
    {
        /* Test all parameters for all lectures in single generation member*/
        for (j = 0; j < sd->numLectures; j++)
        {
            Lecture *curLect = curGen[i].lectures[j];
            curLect->fitness = 0;
            
            /* Test capacity for lecture room */
            curLect->fitness += test_lecture_capacity(sd, curLect);
            curLect->fitness += test_doublebooking(sd, curLect);
            curLect->fitness += test_teacher_availability(sd, curLect);
            
            /* Add to combined schedule fitness */
            curGen[i].fitness = curLect->fitness;
            
            /* Add to combined generation fitness */
            combinedFitness += curLect->fitness;
        }
    }
    return combinedFitness;
}