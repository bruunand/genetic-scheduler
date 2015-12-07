int crossbreed(semesterData *parent1, semesterData *parent2, semesterData *child1, semesterData *child2)
{
    int i, genesSwitched = 0;
    
    child1 = parent1;
    child2 = parent2;   
    
    /* For a random selection of lectures, they are swi */
    for(i = 0;i < parent1->numLectures;i += rand() % (parent1->numLectures/10)) 
    {
        child1->lectures[i] = parent2->lectures[i];
        child2->lectures[i] = parent1->lectures[i];
        genesSwitched++;
    }
    
    return genesSwitched;
}
