int crossbreed(Generation *new, int numGenomes, int carryover)
{
    int i, genesSwitched = 0, parentA, parentB;
    
    do
    {
        parentA = rand() % carryover;
        parentB = rand() % carryover;
    } while(parentA == parentB);
    
    for(i = 0; i < new->sd->numLectures ; i++)
    {
        if(new->schedules[parentA].lectures[i].fitness >= new->schedules[parentB].lectures[i].fitness)
            new->schedules[numGenomes].lectures[i] = new->schedules[parentA].lectures[i];
        else{
            new->schedules[numGenomes].lectures[i] = new->schedules[parentA].lectures[i];
            genesSwitched++;
        } 
    }
    
    return genesSwitched;
}
