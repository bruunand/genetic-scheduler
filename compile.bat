mkdir bin
mkdir obj

:: Compile data utility object
gcc -g -c -ansi -pedantic -Wall src/data_utility.c -o obj/data_utility.o

:: Compile data test object
gcc -g -c -ansi -pedantic -Wall src/data_test.c -o obj/data_test.o

:: Compile config reader object
gcc -g -c -ansi -pedantic -Wall src/config_reader.c -o obj/config_reader.o

:: Compile executable
gcc -g -ansi -pedantic -Wall src/scheduler.c obj/data_test.o obj/config_reader.o obj/data_utility.o -o bin/Scheduler.exe

pause