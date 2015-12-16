@echo off
set CC=gcc
set SRCDIR=src
set OBJDIR=obj
set BINDIR=bin
set EXENAME=Scheduler
set FLAGS=-g -ansi -pedantic -Wall -O3 -Wextra

mkdir %BINDIR%
mkdir %OBJDIR%

:: Compile data utility object
%CC% %FLAGS% -c %SRCDIR%/data_utility.c -o %OBJDIR%/data_utility.o

:: Compile data test object
%CC% %FLAGS% -c %SRCDIR%/fitness_calculation.c -o %OBJDIR%/fitness_calculation.o

:: Compile input reader object
%CC% %FLAGS% -c %SRCDIR%/input_reader.c -o %OBJDIR%/input_reader.o

:: Compile HTML-output object
%CC% %FLAGS% -c %SRCDIR%/html_output.c -o %OBJDIR%/html_output.o

:: Compile genetic algorithm object
%CC% %FLAGS% -c %SRCDIR%/genetic_algorithm.c -o %OBJDIR%/genetic_algorithm.o

:: Compile main executable
%CC% %FLAGS% %SRCDIR%/scheduler.c %OBJDIR%/html_output.o %OBJDIR%/fitness_calculation.o %OBJDIR%/input_reader.o %OBJDIR%/data_utility.o %OBJDIR%/genetic_algorithm.o -o %BINDIR%/%EXENAME%.exe

:: Compile test executable
%CC% %FLAGS% %SRCDIR%/test.c %OBJDIR%/html_output.o %OBJDIR%/fitness_calculation.o %OBJDIR%/input_reader.o %OBJDIR%/data_utility.o -o %BINDIR%/Test.exe

pause