#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "structs.h"
#include "config_reader.h"
#include "scheduler.h"

#define BUFFER_SIZE 256

int read_config(char *fileName, semesterData *data)
{
    FILE* fp;
    char buffer[BUFFER_SIZE];
    int p;
    char c;

    /* Open config file in read mode */
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
            handle_line(buffer, data);
    } while(c != EOF);
    
    fclose(fp);
       
    return data;
}

void handle_line(char* line, semesterData *data)
{
    char command[16], typeName[16];
    int p = 0; /* p keeps track of our current position in the string */
    
    /* Get command */
    if (!sscanf(line + p, "%s", command))
        return;
    p += strlen(command) + 1;
    
    /* Read object name*/
    if (!sscanf(line + p, "%s", typeName))
        return;
    p += strlen(typeName) + 1;
        
    /* Handle based on command */
    if (!strcmp(command, "set"))
    {
        /* Read object value */
        char objectValue[16];
        if (!sscanf(line + p, "%s", objectValue))
            return;
        p += strlen(objectValue) + 1;
        
        /* Convert string to int */
        int objectValueInt = atoi(objectValue);
        
        /* Return if atoi returns 0 and objectValue isn't 0 */
        if (!objectValueInt && strcmp(objectValue, "0"))
            return;

        /* Set value to matching object name */
        if (!strcmp(typeName, "WEEKS"))
            data->numWeeks = objectValueInt;
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
            
            data->numCourses++;
            /* data->courses = realloc()... */
            
            printf("add course '%s'\n", courseName);
        }
        else if(!strcmp(typeName, "TEACHER"))
        {
            /* Read teachers name */
            char teacherName[32];
            if (!read_multiple_words(line, &p, teacherName))
                return;
            
            data->numTeachers++;
            
            printf("teacher '%s'\n", teacherName);
        }
        else if(!strcmp(typeName, "ROOM"))
        {
            /* Read room name */
            char roomName[32];
            if (!read_multiple_words(line, &p, roomName))
                return;
            
            data->numRooms++;
            
            printf("room '%s'\n", roomName);
        }
    }
}

int read_multiple_words(char* line, int* position, char* out)
{
    if (!sscanf(line + *position, "'%[^']", out))
        return 0;
    *position += strlen(out);
    return strlen(out) ? 1 : 0;
}