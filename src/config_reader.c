#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "scheduler.h"
#include "config_reader.h"
#include "structs.h"

#define BUFFER_SIZE 256

int read_config()
{
    FILE* fp;
    char* buffer = (char *) malloc(BUFFER_SIZE * sizeof(char));
    int p;
    char c;
    
    /* Open config file in read mode */
    fp = fopen("scheduler.cfg", "r");
    
    /* Check if it opened correctly */
    if (fp == NULL)
        return 0;
    
    if (fp)
    {
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
                handle_line(buffer);
        } while(c != EOF);
        
        fclose(fp);
    }
    else
    {
        printf("Could not read configuration file!\n");
    }
    
    free(buffer);   
       
    return 1;
}

void handle_line(char* line)
{
    /* p keeps track of our current position in the string */
    int p = 0;
    
    /* Get command */
    char command[16];
    if (!sscanf(line + p, "%s", command))
        return;
    p += strlen(command) + 1;
    
    /* Read name token */
    char objectName[16];
    if (!sscanf(line + p, "%s", objectName))
        return;
    p += strlen(objectName) + 1;
        
    /* Handle based on command */
    if (!strcmp(command, "set"))
    {   
        /* Read value */
        char objectValue[16];
        if (!sscanf(line + p, "%s", objectValue))
            return;
        p += strlen(objectValue) + 1;
        
        /* Verify value */
        int objectValueInt = atoi(objectValue);
        if (!objectValueInt)
            return;

        /* Print */
        printf("%s=%d\n", objectName, objectValueInt);
    }
    else if(!strcmp(command, "add"))
    {
        /* Handle based on type */
        if (!strcmp(objectName, "COURSE"))
        {
            /* Read course name */
            char courseName[32];
            if (!read_multiple_words(line, &p, courseName))
                return;
            
            printf("course '%s'\n", courseName);
        }
        else if(!strcmp(objectName, "TEACHER"))
        {
            /* Read teachers name */
            char teacherName[32];
            if (!read_multiple_words(line, &p, teacherName))
                return;
            
            printf("teacher '%s'\n", teacherName);
            
            /* Allocate memory for this teacher */
            
        }
        else if(!strcmp(objectName, "ROOM"))
        {
            /* Read room name */
            char roomName[32];
            if (!read_multiple_words(line, &p, roomName))
                return;
            
            printf("room '%s'\n", roomName);
        }
    }
}

int read_multiple_words(char* line, int* position, char* out)
{
    if (!sscanf(line + *position, "'%[^']", out))
        return 0;
    (*position) += strlen(out);
    return strlen(out) ? 1 : 0;
}