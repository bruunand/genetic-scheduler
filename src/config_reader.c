#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "structs.h"
#include "config_reader.h"
#include "scheduler.h"

#define BUFFER_SIZE 512

void add_teacher(semesterData *sd, char *name)
{
    /* Reallocate memory for the new count */
    int teacherIndex = sd->numTeachers++;
    sd->teachers = (teacher*) realloc(sd->teachers, sd->numTeachers * sizeof(teacher));
    
    /* Set values */
    strcpy(sd->teachers[teacherIndex].name, name);
}

void add_room(semesterData *sd, char *name, int seats)
{
    /* Reallocate memory for the new count */
    int roomIndex = sd->numRooms++;
    sd->rooms = (room*) realloc(sd->rooms, sd->numRooms * sizeof(room));
    
    /* Set values */
    sd->rooms[roomIndex].seats = seats;
    strcpy(sd->rooms[roomIndex].name, name);
}

void add_course(semesterData *sd, char *name, int totalLectures)
{
    /* Reallocate memory for the new count */
    int courseIndex = sd->numCourses++;
    sd->courses = (course*) realloc(sd->courses, sd->numCourses * sizeof(course));
    
    /* Set values */
    strcpy(sd->courses[courseIndex].name, name);
    sd->courses[courseIndex].totalLectures = totalLectures;
}

int read_config(char *fileName, semesterData *sd)
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
            if(p > 0 && c == ' ' && buffer[p - 1] == ' ')
                continue;

            /* Add if it's a printable char */
            if(isprint(c))
                buffer[p++] = c;
        } while(c != '\n' && c != EOF);
            
        /* Set last char to null-terminator */
        buffer[p] = '\0';
        
        /* Ignore comments and empty lines */
        if (strlen(buffer) > 0 && buffer[0] != '#')
            handle_line(buffer, sd);
    } while(c != EOF);
    
    fclose(fp);
       
    return 1;
}

void handle_line(char* line, semesterData *sd)
{
    char command[16], typeName[16];
    int p = 0; /* p keeps track of our current position in the string */
    
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
            /* Read course name */
            char courseName[32];
            if (!read_multiple_words(line, &p, courseName))
                return;
            
            /* Read num. lectures */
            int totalLectures;
            if (!read_int(line, &p, &totalLectures))
                return;
            
            add_course(sd, courseName, totalLectures);
        }
        else if(!strcmp(typeName, "TEACHER"))
        {
            /* Read teachers name */
            char teacherName[32];
            if (!read_multiple_words(line, &p, teacherName))
                return;
            
            add_teacher(sd, teacherName);
        }
        else if(!strcmp(typeName, "ROOM"))
        {
            /* Read room name */
            char roomName[32];
            if (!read_multiple_words(line, &p, roomName))
                return;
            
            /* Get amount of seats in room */
            int seatsInRoom;
            if (!read_int(line, &p, &seatsInRoom))
                return;
            
            add_room(sd, roomName, seatsInRoom);
        }
    }
}

/* Reads an int and adds the amount of digits to position */
int read_int(char* line, int* position, int* out)
{
    char outStr[16];
    if (!sscanf(line + *position, "%s", outStr))
        return 0;
    *position += strlen(outStr) + 1;
    
    /* Convert string to int */
    *out = atoi(outStr);
    
    /* Return if atoi returns 0 and objectValue isn't 0 */
    if (!*out && strcmp(outStr, "0"))
        return 0;
    else
        return 1;
}

/* Reads an entire string between two apostrophes */
int read_multiple_words(char* line, int* position, char* out)
{
    if (!sscanf(line + *position, "'%[^']", out))
        return 0;
    *position += strlen(out) + 2;
    return strlen(out) ? 1 : 0;
}