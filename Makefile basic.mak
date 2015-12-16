CC = gcc
CFLAGS = -I
EXE = Scheduler
OBJ = data_utility.o fitness_calculation.o genetic_algorithm.o html_output.o input_reader.o

shceduler: $(OBJ)
    $(CC) -o $(EXE) $(CFLAGS)

data_utility.o: data_utility.c
    $(CC) -c -o data_utility.o $(CFLAGS)
    
fitness_calculation.o: fitness_calculation.c
    $(CC) -c -o fitness_calculation.o $(CFLAGS)

genetic_algorithm.o: genetic_algorithm.c
    $(CC) -c -o genetic_algorithm.o $(CFLAGS)

html_output.o: html_output.c
    $(CC) -c -o html_output.o $(CFLAGS)
    
input_reader.o: input_reader.c
    $(CC) -c -o input_reader.o $(CFLAGS)