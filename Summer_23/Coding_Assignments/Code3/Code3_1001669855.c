#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void openFile(FILE ** fp, char *filename)
{
	*fp = fopen(filename, "r");
	if(fp == NULL)
	{
		printf("\nFile \"%s\" failed to open...exiting\n", filename);
		exit(-1);
	}
}

void printArray(int arr[], int sizeArr)
{
    int i;
    for(i = 0; i < sizeArr; i++)
        printf("%d\n", arr[i]);
    
    printf("\n");
}

int ReadFileIntoArray(int argc, char *argv[], int **AP)
{
    if(argc != 2)
	{
		printf("File must be provided on command line...exiting\n");
		exit(-1);
	}

	FILE * fp;
	openFile(&fp, argv[1]);

    int counter = 0;
    char buffer[100];

    while (fgets(buffer,99,fp) != NULL)
    {
        counter++;
    }

    *AP = malloc(sizeof(int) * counter);
    fseek(fp, 0.0, SEEK_SET);
    int i;
    for(i=0; i<counter; i++)
    {
        fgets(buffer,99,fp);
        (*AP)[i] = atoi(buffer);
    }
    fclose(fp);
    #ifdef PRINTARRAY
    printArray(*AP, counter);
    #endif

    return counter;
}

int main(int argc, char *argv[])
{
    clock_t start, end;
    int *AP;

    int n = ReadFileIntoArray(argc, argv, &AP);

    return 0;
}