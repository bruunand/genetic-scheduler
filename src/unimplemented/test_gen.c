int test_gen()
{
    int fitness = 0;
    
    /* Requirements are tested and  */
    fitness += test_doublebooking(sd);
    fitness += test_room_capacity(sd);
    fitness += test_coverage(sd);
    fitness += test_teacher_availability(sd);
    fitness += test_distribution(sd);
    
    return fitness;
}