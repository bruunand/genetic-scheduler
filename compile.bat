mkdir bin
gcc -c -ansi -pedantic -Wall src/config_reader.c -o bin/config_reader.o -g
gcc src/scheduler.c bin/config_reader.o -o bin/Scheduler.exe -g
pause