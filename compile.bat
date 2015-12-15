mkdir bin
mkdir obj

:: Compile data utility object
gcc -g -c -ansi -pedantic -Wall src/data_utility.c -o obj/data_utility.o

:: Compile data test object
gcc -g -c -ansi -pedantic -Wall src/fitness_calculation.c -o obj/fitness_calculation.o

:: Compile input reader object
gcc -g -c -ansi -pedantic -Wall src/input_reader.c -o obj/input_reader.o

:: Compile HTML-output object
gcc -g -c -ansi -pedantic -Wall src/html_output.c -o obj/html_output.o

:: Compile genetic algorithm object
gcc -g -c -ansi -pedantic -Wall src/genetic_algorithm.c -o obj/genetic_algorithm.o

:: Compile main executable
gcc -g -ansi -pedantic -Wall src/scheduler.c obj/html_output.o obj/fitness_calculation.o obj/input_reader.o obj/data_utility.o obj/genetic_algorithm.o -o bin/Scheduler.exe

:: Compile test executable
gcc -g -ansi -pedantic -Wall src/test.c obj/html_output.o obj/fitness_calculation.o obj/input_reader.o obj/data_utility.o -o bin/Test.exe

pause