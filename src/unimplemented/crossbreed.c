int crossbreed(Generation *new, int numGenomes, int carryover)
{
    int i, genesSwitched = 0, dad, mom;
    
    do
    {
        dad = rand() % carryover;
        mom = rand() % carryover;
    } while(dad == mom);
    
    for(i = 0; i < new->sd->numLectures ; i++)
    {
        if(new->schedules[dad].lectures[i].fitness >= new->schedules[mom].lectures[i].fitness)
            new->schedules[numGenomes].lectures[i] = new->schedules[dad].lectures[i];
        else{
            new->schedules[i][j] = old->schedules[mate][j];
            genesSwitched++;
        } 
    }
    
    return genesSwitched;
}
