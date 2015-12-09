int crossbreed(Generation *old, Generation *new)
{
    int i, j, genesSwitched = 0;
    
    /*  */
    for (i = (GENERATION_SIZE / 10) - 1; i < GENERATION_SIZE - 10; i++)
    {    
        newGen[i] = curGen[i];
        
        for(j = 0; j < newGen[i].numLectures ; j++)
        {
            /* Skip perfect lectures */
            if (newGen[i].lectures[j].fitness == 0)
                continue;
            if(newGen[i].lectures[j].fitness < curGen[i + 1 - (2 * (i % 2))].lectures[j].fitness){
                newGen[i].lectures[j] = curGen[].lectures[j];
                genesSwitched++;
            }
            
    }
    
    return genesSwitched;
}
