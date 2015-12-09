int mutate(SemesterData **curGen, SemesterData **newGen)
{
    int i, j, mutations = 0;
    
    /* Go through the 10 % worst */
    for (i = 0; i < GENERATION_SIZE / 10; i++)
    {            
        /* Sort array of pointers by highest fitness */
        qsort(curGen[i].lectures, curGen[i].numLectures, sizeof(Lecture*), compare_fitness);
        
        newGen[i] = curGen[i];
        for(j = 0; j < (newGen[i].numLectures / 10) ; j++)
        {
            /* Skip perfect lectures */
            if (newGen[i].lectures[j].fitness == 0)
                continue;
            
            /* printf("%d has a fitness of %d\n", i, lecturePtrs[i]->fitness); */
            
            /* Mutate */
            newGen[i].lectures[j].day = rand() % (sd->numWeeks * DAYS_PER_WEEK);
            newGen[i].lectures[j].period = rand() % 2;
            newGen[i].lectures[j].assignedRoom = &sd->rooms[rand() % sd->numRooms];
            mutations++;
        }
    }
    
    return mutations;
}
