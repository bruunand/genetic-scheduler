void genetic_optimization(Generation *curGen, Generation *nextGen)
{
    int i, j, genFitness;
    Generation *tempGen;
    
    /* Run up to 1000 generations */
    calcfit_generation(curGen);
    
    for (i = 0; i < 1000; i++)
    {
        genFitness = 0;
        generate_next_generation(curGen, nextGen);
        tempGen = curGen;
        curGen = nextGen;
        nextGen = tempGen;
        for(j = 0; j < GENERATION_SIZE ; j++)
        {
            genFitness += curGen->schedules[j].fitness
        }
        
        printf("\nGen %3d->Fitness: Lowest = %5d, Average = %5d", j, curGen->schedules[0].fitness, genFitness / 1000);
        
        if(curGen->schedules[0].fitness == 0)
            break;
        
        /* TODO: Exit if low fitness is found? */
    }        
}

/**
 *  \brief Generate next generation based on existing generation
 *  
 *  \param [in] oldGen Parameter_Description
 *  \param [in] newGen Parameter_Description
 *  \return Return_Description
 *  
 *  \details Details
 */
void generate_next_generation(Generation *oldGen, Generation *newGen)
{
    int i, x, y, carryover, newGenMembers = 0;

    /* DEBUG: Print best half % */
    for (i = 0; i < GENERATION_SIZE / 2; i++)
        printf("DEBUG: Genome %02d has a fitness of %04d\n", i + 1, oldGen->schedules[i].fitness);
    
    /* 
     * Let half the population of oldGen survive. Randomly chosen, but weighted 
     * towards better fitness.
     */
    for (i = 0; i < GENERATION_SIZE / 2; i++)
    {
        x = rand() % GENERATION_SIZE;
        y = rand() % GENERATION_SIZE;
        
        copy_schedule(&newGen->schedules[i], &oldGen->schedules[(x > y) ? y : x], newGen);
    }

    /* Crossbreed to get a total of 100 genomes */
    carryover = newGenMembers;    
    while (newGenMembers < GENERATION_SIZE)
    {
        crossbreed(newGen, newGenMembers++, carryover);
    }
    
    /* Mutate randomly */
    do
    {
        x = rand() % GENERATION_SIZE;
        if (x < MUTATION_CHANCE)
            mutate(newGen, rand() % GENERATION_SIZE);
    } while (x < MUTATION_CHANCE);
    
    /* Calculate fitness for new generation */
    calcfit_generation(newGen);
    
    /* Sort genomes by fitness */
    qsort(oldGen->schedules, GENERATION_SIZE, sizeof(Schedule), compare_schedule_fitness);
    
    
}