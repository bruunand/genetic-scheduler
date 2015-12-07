int mutate(semesterData *sd)
{
    int i, mutations = 0;
    
    /* Show the 10 % worst */
    for (i = 0; i < sd->numLectures / 10; i++)
    {
        /* Skip perfect lectures */
        if (lecturePtrs[i]->fitness == 0)
            continue;
        
        /* Mutate room */
        lecturePtrs[i]->assignedRoom = &sd->rooms[rand() % sd->numRooms];
    }
    
    
    /* A random amount of lectures are mutated, moving them to a random day, period and room. 
    for(i = 0;i < numLectures;i += rand() % numLectures) 
    {
        sd->lectures[i].day = rand() % (sd->numWeeks * DAYS_PER_WEEK)
        sd->lectures[i].period = rand() % 2;
        sd->lectures[i].assignedRoom = rand() % sd->numRooms;
        mutations++;
    }
    */
    
    return mutations;
}
