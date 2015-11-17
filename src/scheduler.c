#include <stdio.h>
#include <stdlib.h>
#include "structs.h"

#define NUM_ENTRIES 10
#define NUM_WEEKS 1
#define NUM_PERIODS 2

#define DAYS_PER_WEEK 5

/* Global array of lectures */
lecture* currentGeneration = 0;

/* Allocates memory for our global arrays */
void allocate_memory()
{
	currentGeneration = (lecture*) malloc(sizeof(lecture) * NUM_ENTRIES);
}

/* Generate initial population */
void generate_population()
{
	int i;
	for(i = 0; i < NUM_ENTRIES; i++)
	{
		lecture nextItem;
		
		nextItem.day = rand() % (NUM_WEEKS * DAYS_PER_WEEK);
		nextItem.period = rand() % NUM_PERIODS;
		/* To do: Assign random course and room */
		
		currentGeneration[i] = nextItem;
	}
}

int main(void)
{
	/* To do: configuration file */
	/* Get amount of courses */
	/* Get amount of rooms */
	
	/* Allocate memory for the initial population */
	allocate_memory();
	
	/* Generate initial population */
	generate_population();
	
	free(currentGeneration);
}
