void initialGen(void);
{
    /* Requires unistd.h */
    /* Checks if scheduler.data exists */
    if (access("scheduler.data", F_OK) != -1)
    {
        inp_bin_file();
    }
    else
    {
       calculate_amount_of_lectures(); 
    }
}

