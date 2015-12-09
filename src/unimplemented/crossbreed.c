int crossbreed(Generation *old, Generation *new, int mutations)
{
    int i, j, mate, genesSwitched = 0;
    
    /*  */
    for (i = mutations + 5; i < GENERATION_SIZE; i++)
    {    
        for(j = 0; j < old->sd.numLectures ; j++)
        {
            mate = GENERATION_SIZE - (GENERATION_SIZE / 5) + (rand() % (GENERATION_SIZE / 5));
            
            if(old->schedules[i][j].fitness >= old->schedules[mate][j].fitness)
                new->schedules[i][j] = old->schedules[i][j];
            else{
                new->schedules[i][j] = old->schedules[mate][j];
                genesSwitched++;
            }
        }
    }
    
    return genesSwitched;
}
