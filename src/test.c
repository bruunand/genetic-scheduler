#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "structs.h"
#include "config_reader.h"

int main(void)
{
    Generation *testGen;
    SemesterData sd;
    OffTime offTimes[2];
    srand(1234);
    
    memset(&sd, 0, sizeof(SemesterData));
        
    /* Create test SemesterData */
    offTimes[0] = {2, 0, 0};
    offTimes[1] = {2, 0, 0};
    
    add_teacher(&sd, "Dummy Teacher", offTimes);
    
    return 0;
}