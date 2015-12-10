int mutate(Generation *new, int genomeId)
{
    int i = 0, mutations;
    
    do
    {
        i += rand() % new->sd->numLectures;
        if(!(rand() % 2))
        {
            new->schedules[genomeId].lectures[i].day = rand() % (sd->numWeeks * DAYS_PER_WEEK);
            mutations++;
        }

        if(!(rand() % 2))
        {
            new->schedules[genomeId].lectures[i].period = rand() % MAX_PERIODS;
            mutations++;
        }
      
        if(!(rand() % 2))
        {
            new->schedules[genomeId].lectures[i].assignedRoom = &sd->rooms[rand() % sd->numRooms];
            mutations++;
        }
    } while(i < new->sd->numLectures);

    return mutations;
}
