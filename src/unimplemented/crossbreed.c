int crossbreed(Generation *new, int genomeId, int carryover)
{
    int i, genesSwitched = 0, parentA, parentB;
    
    /* Find two differents parents to be mated */
    do
    {
        parentA = rand() % carryover;
        parentB = rand() % carryover;
    } while(parentA == parentB);
    
    /*
     * Iterate through all lectures and compare their fitness.
     * Note: This assumes that the lectures fall in the same order
     * for both parents!
    */
    for(i = 0; i < new->sd->numLectures; i++)
    {
        if(new->schedules[parentA].lectures[i].fitness <= new->schedules[parentB].lectures[i].fitness)
            copy_lecture(&new->schedules[genomeId].lectures[i], &new->schedules[parentA].lectures[i]);
        else
            copy_lecture(&new->schedules[genomeId].lectures[i], &new->schedules[parentB].lectures[i]);
        
        genesSwitched++;
    }
    
    return genesSwitched;
}
