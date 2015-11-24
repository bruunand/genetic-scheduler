semesterData load_data_file(char *fileName)
{
    semesterData sd;
    int i = 0;
    
    inp = fopen(fileName, "r");
    
    while (inp != EOF)
    {
        sd[i] = fscanf("%[^\n]");
        i++;
    }
    
    fclose(inp);
    
    return sd;
}