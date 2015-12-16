CC = gcc
EFLAGS = -ansi -pedantic -Wall
CFLAGS = -I
SRCDIR = src
OBJDIR = obj
BINDIR = bin
EXE = Scheduler.exe

_DEPS = data_utility.h defs.h fitness_calculation.h genetic_algorithm.h html_output.h input_reader.h scheduler.h structs.h
DEPS = $(patsubst %,$(SRCDIR)/%,$(_DEPS))

_OBJ = data_utility.o fitness_calculation.o genetic_algorithm.o html_output.o input_reader.o
OBJ = $(patsubst %,$(OBJDIR)/%,$(_OBJ))

#$(EXE): $(OBJ)
#	$(CC) -o $(BINDIR)/$(EXE) $(OBJ) $(EFLAGS)

#$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS)
#	$(CC) -c -o $@ $< $(CFLAGS)

main: obj/data_utility.o obj/fitness_calculation.o obj/genetic_algorithm.o obj/html_output.o obj/input_reader.o
	gcc  -o test.exe obj/data_utility.o obj/fitness_calculation.o obj/genetic_algorithm.o obj/html_output.o obj/input_reader.o -ansi -pedantic -Wall

fitness_calculation.o: src/fitness_calculation.c src/fitness_calculation.h
	gcc -c src/fitness_calculation.c -o obj/fitness_calculation.o -I
	
genetic_algorithm.o: src/genetic_algorithm.c src/genetic_algorithm.h
	gcc -c src/genetic_algorithm.c -o obj/genetic_algorithm.o -I
	
data_utility.o: src/data_utility.c src/data_utility.h
	gcc -c src/data_utility.c -o obj/data_utility.o -I
	
html_output.o: src/html_output.c src/html_output.h
	gcc -c src/html_output.c -o obj/html_output.o -I
	
input_reader.o: src/input_reader.c src/input_reader.h
	gcc -c src/input_reader.c -o obj/input_reader.o -I

.PHONY: clean

clean:
	rm -f $(OBJDIR)/*.o $(BINDIR)/$(EXE)