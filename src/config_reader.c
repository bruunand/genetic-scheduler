#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "structs.h"
#include "config_reader.h"
#include "defs.h"

#define BUFFER_SIZE 512

int read_config(char *fileName, SemesterData *sd)
{
    FILE* fp;
    char buffer[BUFFER_SIZE];
    int p;
    char c;

    /* Open input file in read mode */
    fp = fopen(fileName, "r");
    
    /* Check if it opened correctly */
    if (fp == NULL)
        return 0;
    
    do
    {
        /* Reset buffer position */
        p = 0;
        
        /* Handle next line */
        do
        {
            c = fgetc(fp);
            
            /* Replace tab with space */
            if (c == '\t')
                c = ' ';
            
            /* Ignore double spacing */
            if (p > 0 && c == ' ' && buffer[p - 1] == ' ')
                continue;

            /* Add if it's a printable char */
            if (isprint(c))
                buffer[p++] = c;
        } while (c != '\n' && c != EOF);
            
        /* Set last char to null-terminator */
        buffer[p] = '\0';
        
        /* Ignore comments and empty lines */
        if (strlen(buffer) > 0 && buffer[0] != '#')
            handle_line(buffer, sd);
    } while (c != EOF);
    
    fclose(fp);
       
    return 1;
}

void handle_line(char* line, SemesterData *sd)
{
    char command[16], typeName[16];
    int p = 0; /* Keeps track of our current position in the string */
    
    /* Read command */
    if (!sscanf(line + p, "%s", command))
        return;
    p += strlen(command) + 1;
    
    /* Read type */
    if (!sscanf(line + p, "%s", typeName))
        return;
    p += strlen(typeName) + 1;
        
    /* Handle based on command */
    if (!strcmp(command, "set"))
    {
        /* Get value */
        int objectValue;
        if (!read_int(line, &p, &objectValue))
            return;
        
        /* Set value of type */
        if (!strcmp(typeName, "WEEKS"))
            sd->numWeeks = objectValue;
        else
            printf("Error: Unknown type name '%s'\n", typeName);
    }
    else if(!strcmp(command, "add"))
    {
        /* Handle based on type */
        if (!strcmp(typeName, "COURSE"))
        {
            char courseName[32];
            int totLectures, curTeacherId, numTeachers = 0;
            Teacher **teachers = 0;
            
            /* Read name of course */
            if (!read_multiple_words(line, &p, courseName))
                return;

            /* Read num. lectures */
            if (!read_int(line, &p, &totLectures))
                return;
            
            /* Read teachers into an array */
            while (read_int(line, &p, &curTeacherId))
            {
                teachers = realloc(teachers, (numTeachers + 1) * sizeof(Teacher*));
                if (!teachers)
                    exit(ERROR_OUT_OF_MEMORY);
                
                teachers[numTeachers] = sd->teachers[curTeacherId];
                numTeachers++;
            }
            
            add_course(sd, courseName, totLectures, numTeachers, teachers);
        }
        else if(!strcmp(typeName, "TEACHER"))
        {
            char teacherName[32];
            int numOffTimes = 0, curOffDay, i;
            OffTime *offTimes = 0;
            
            /* Read name of teacher */
            if (!read_multiple_words(line, &p, teacherName))
                return;
            
            /* Read offtimes for teacher */
            /* Start by checking if we can read the next day */
            while (read_int(line, &p, &curOffDay))
            {
                offTimes = realloc(offTimes, (numOffTimes + 1) * sizeof(OffTime));
                if (!offTimes)
                    exit(ERROR_OUT_OF_MEMORY);
                
                /* Set day for this offTime */
                offTimes[numOffTimes].day = curOffDay;
                
                /* Add MAX_PERIODS periods */
                for (i = 0; i < MAX_PERIODS; i++)
                    read_int(line, &p, &offTimes[numOffTimes].periods[i]);

                numOffTimes++;
            }
            
            add_teacher(sd, teacherName, numOffTimes, offTimes);
        }
        else if(!strcmp(typeName, "ROOM"))
        {
            char roomName[32];
            int seatsInRoom;
            
            /* Read name of room */
            if (!read_multiple_words(line, &p, roomName))
                return;
            
            /* Get amount of seats in room */
            if (!read_int(line, &p, &seatsInRoom))
                return;
            
            add_room(sd, roomName, seatsInRoom);
        }
        else if(!strcmp(typeName, "SPECIALIZATION"))
        {
            char specName[32];
            int numStudents, curCourseId, numCourses = 0;
            Course **courses = 0;
            
            /* Read name of specialization */
            if (!read_multiple_words(line, &p, specName))
                return;
            
            /* Read number of students */
            if (!read_int(line, &p, &numStudents))
                return;
            
            /* Read all remaining integers */
            while (read_int(line, &p, &curCourseId))
            {
                courses = realloc(courses, (numCourses + 1) * sizeof(Course*));
                if (!courses)
                    exit(ERROR_OUT_OF_MEMORY);
                
                courses[numCourses] = sd->courses[curCourseId];
                numCourses++;
            }
            
            add_specialization(sd, specName, numStudents, numCourses, courses); 
        }
    }
}

/* Reads an int and adds the amount of digits to position */
int read_int(char* line, int* position, int* out)
{
    char outStr[16];

    /* Do not go beyond the bounds of the string */
    if (*position >= strlen(line))
        return 0;
    
    /* Scan next string */
    if (!sscanf(line + *position, "%s", outStr))
        return 0;
    *position += strlen(outStr) + 1;
    
    /* Convert string to int */
    *out = atoi(outStr);
    
    /* The function fails if atoi returns 0 when the string doesn't equal 0 */
    if (!*out && strcmp(outStr, "0"))
        return 0;
    else
        return 1;
}

/* Reads an entire string between two apostrophes */
int read_multiple_words(char* line, int* position, char* out)
{
    /* Do not go beyond the bounds of the string */
    if (*position >= strlen(line))
        return 0;
    
    /* Scan everything until an apostrophe is found */
    if (!sscanf(line + *position, "'%[^']", out))
        return 0;
    *position += strlen(out) + 2;
    
    return strlen(out) ? 1 : 0;
}

void add_teacher(SemesterData *sd, char *name, int numOffTimes, OffTime *offTimes)
{
    /* Reallocate memory for the new count */
    int teacherIndex = sd->numTeachers++;
    sd->teachers = realloc(sd->teachers, sd->numTeachers * sizeof(Teacher));
    if (!sd->teachers)
        exit(ERROR_OUT_OF_MEMORY);
    
    /* Set values */
    strcpy(sd->teachers[teacherIndex].name, name);
    sd->teachers[teacherIndex].offTimes = offTimes;
    sd->teachers[teacherIndex].numOffTimes = numOffTimes;
}

void add_room(SemesterData *sd, char *name, int seats)
{
    /* Reallocate memory for the new count */
    int roomIndex = sd->numRooms++;
    sd->rooms = realloc(sd->rooms, sd->numRooms * sizeof(Room));
    if (!sd->rooms)
        exit(ERROR_OUT_OF_MEMORY);
    
    /* Set values */
	strcpy(sd->rooms[roomIndex]->name, name);
    sd->rooms[roomIndex]->seats = seats;
}

void add_course(SemesterData *sd, char *name, int totLectures, int numTeachers, Teacher **teachers)
{
    /* Reallocate memory for the new count */
    int courseIndex = sd->numCourses++;
    sd->courses = realloc(sd->courses, sd->numCourses * sizeof(Course));
    if (!sd->courses)
        exit(ERROR_OUT_OF_MEMORY);
    
    /* Set values */
    strcpy(sd->courses[courseIndex].name, name);
    sd->courses[courseIndex].totLectures = totLectures;
    sd->courses[courseIndex].numTeachers = numTeachers;
    sd->courses[courseIndex].teachers = teachers;
}

void add_specialization(SemesterData *sd, char *name, int numStudents, int numCourses, Course **courses)
{
    /* Reallocate memory for the new count */
    int specIndex = sd->numSpecializations++;
    sd->specializations = realloc(sd->specializations, sd->numSpecializations * sizeof(Specialization*));
    if (!sd->specializations)
        exit(ERROR_OUT_OF_MEMORY);
    
    /* Set values */
    strcpy(sd->specializations[specIndex]->name, name);
    sd->specializations[specIndex]->numStudents = numStudents;
    sd->specializations[specIndex]->numCourses = numCourses;
    sd->specializations[specIndex]->courses = courses;
}