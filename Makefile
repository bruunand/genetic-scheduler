IDIR = src
CC=gcc
CFLAGS=-I$(IDIR)

ODIR = obj

_DEPS = data_utility.h defs.h fitness_calculation.h genetic_algorithm.h html_output.h input_reader.h scheduler.h structs.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = data_utility.o fitness_calculation.o genetic_algorithm.o html_output.o input_reader.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

hellomake: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
