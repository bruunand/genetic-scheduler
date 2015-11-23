#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "structs.h"
#include "config_reader.h"
#include "scheduler.h"

#define BUFFER_SIZE 512

void add_teacher(teacher *newTeacher, char* name)
{
    strcpy(newTeacher->name, name);
}

void add_room(room *newRoom, char* name, int seats)
{
    strcpy(newRoom->name, name);
    newRoom->seats = seats;
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
            int numLectures;
            if (!read_int(line, &p, &numLectures))
                return;
            
            /* Add course to array */
            sd->numCourses++;
            data->courses = (course*) realloc(sd->courses, sd->numCourses * sizeof(course));
            add_course(&sd->courses[sd->numCourses - 1], courseName, numLectures);
        }
        else if(!strcmp(typeName, "TEACHER"))
        {
            /* Read teachers name */
            char teacherName[32];
            if (!read_multiple_words(line, &p, teacherName))
                return;
            
            /* Add teacher to array */
            sd->numTeachers++;
            sd->teachers = (teacher*) realloc(sd->teachers, sd->numTeachers * sizeof(teacher));
            add_teacher(&sd->teachers[sd->numTeachers - 1], teacherName);
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
            
            /* Add room to array */
            sd->numRooms++;
            sd->rooms = (room*) realloc(sd->rooms, sd->numRooms * sizeof(room));
            add_room(&sd->rooms[sd->numRooms - 1], roomName, seatsInRoom);
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