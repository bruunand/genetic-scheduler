#include <unistd.h>

void initialGen(void);
{
    /* access function requires unistd.h */
    /* Checks if scheduler.data exists */
    if (access("scheduler.data", F_OK) != -1)
    {
        load_data_file("scheduler.data");
    }
    else
    {
       calculate_amount_of_lectures(); 
    }
}

