mkdir bin
mkdir obj

:: Compile data utility object
gcc -g -c -ansi -pedantic -Wall src/data_utility.c -o obj/data_utility.o

:: Compile data test object
gcc -g -c -ansi -pedantic -Wall src/fitness_calculation.c -o obj/fitness_calculation.o

:: Compile config reader object
gcc -g -c -ansi -pedantic -Wall src/config_reader.c -o obj/config_reader.o

:: Compile HTML-output object
gcc -g -c -ansi -pedantic -Wall src/html_output.c -o obj/html_output.o

:: Compile executable
gcc -g -ansi -pedantic -Wall src/scheduler.c obj/html_output.o obj/fitness_calculation.o obj/config_reader.o obj/data_utility.o -o bin/Scheduler.exe

pause