#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX 50

typedef struct 
{
    int idx;
    char label[6];
    int distance;
    int previous;
    int visited;
}
Vertex;

void openFile(FILE ** fp, char *filename)
{
	*fp = fopen(filename, "r");
	if(*fp == NULL)
	{
		printf("File \"%s\" failed to open...exiting\n", filename);
		exit(-1);
	}
}

void printArray(int AdjMatrix[MAX][MAX], int VertexCount)
{
	int i, j;
	printf("\n");
	for(i = 0; i < VertexCount; i++)
	{
		for(j = 0; j < VertexCount; j++)
			printf("%5d\t", AdjMatrix[i][j]);
		printf("\n");
	}
}

int populateAdjMatrix(int AdjacencyMatrix[MAX][MAX], char * argv[], Vertex VertexArray[])
{
	FILE * fp;
	openFile(&fp, argv[1]);

	int vertexCount = 0;
	char buffer[100];

	while (fgets(buffer,99,fp) != NULL)
	{
		vertexCount++;
	}

	if(vertexCount > MAX)
	{
		printf("Too many vertexes, please try a different file.\n. . .Exiting\n");
		exit(-1);
	}
	
	fseek(fp, 0.0, SEEK_SET);
	
	int j;
	for(j = 0; j < vertexCount; j++)
		memset(AdjacencyMatrix[j], -1, sizeof(int)*(vertexCount));

	#ifdef PRINTIT
	printArray(AdjacencyMatrix, vertexCount);
	#endif

	int i = 0;

	char *Token = NULL;
	int vert;
	int weight;

	while(fgets(buffer, 99, fp) != NULL)
	{
		if(buffer[strlen(buffer)-1] == '\n')
			buffer[strlen(buffer)-1] = '\0';
		
		Token = strtok(buffer, ",");
		strcpy(VertexArray[i].label, Token);
		VertexArray[i].idx = i;
		VertexArray[i].visited = 0;
		VertexArray[i].distance = INT_MAX;
		VertexArray[i].previous = -1;

		Token = strtok(NULL, ",");

		while(Token != NULL)
		{
			vert = atoi(Token);
			Token = strtok(NULL, ",");
			weight = atoi(Token);
			AdjacencyMatrix[i][vert] = weight;
			Token = strtok(NULL, ",");
		}
		i++;
	}

	return vertexCount;
}

int dijkstra(int AdjacencyMatrix[MAX][MAX], Vertex VertexArray[], int VertexCount, int start, int dest)
{
	int Current_idx = start;
	VertexArray[start].distance = 0;
	VertexArray[start].previous = -1;
	VertexArray[start].visited = 1;

	int x,y,z, dofu, cofuv, dofv, SmallestVertexIndex;
	for(x = 0; x < VertexCount-1; x++)
	{
		for(y = 0; y < VertexCount; y++)
		{
			if(AdjacencyMatrix[Current_idx][y] != -1 && !(VertexArray[y].visited))
			{
				dofu = VertexArray[Current_idx].distance;
				cofuv = AdjacencyMatrix[Current_idx][y];
				dofv = VertexArray[y].distance;

				if(dofu + cofuv < dofv)
				{
					dofv = dofu + cofuv;
					VertexArray[y].distance = dofv;
					VertexArray[y].previous = Current_idx;
				}

			}
		}
		SmallestVertexIndex = -1;
		int SmallestVertex = INT_MAX;

		for(z = 0; z < VertexCount; z++)
		{
			if(!VertexArray[z].visited)
			{
				if(VertexArray[z].distance < SmallestVertex)
				{
					SmallestVertex = VertexArray[z].distance;
					SmallestVertexIndex = z;
				}
			}
		}
		Current_idx = SmallestVertexIndex;
		VertexArray[Current_idx].visited = 1;
	}
	return VertexArray[dest].distance;
}

void printPath( Vertex VertexArray[], int start, int end, int vertexCount)
{
	int path[vertexCount];
	int i=0 ,current_idx = end;
	while(VertexArray[current_idx].previous != -1)
	{
		path[i] = VertexArray[current_idx].previous;
		i++;
	}
	path[i] = VertexArray[current_idx].previous;
	i++;
	for(i; i>= 0; i--)
	{
		printf("%s->", VertexArray[i].label);
	}
}

int main(int argc, char * argv[])
{
    if(argc < 2)
	{
		printf("File must be provided on command line...exiting\n");
		exit(-1);
	}

	int AdjacencyMatrix[MAX][MAX] = {};
	Vertex VertexArray[MAX] = {};
	int vertexCount = populateAdjMatrix(AdjacencyMatrix, argv, VertexArray);
	
	#ifdef PRINTIT
	printArray(AdjacencyMatrix,vertexCount);
	#endif

	char starting_label[6];
    char dest_label[6];
	printf("What is the starting vertex? ");
	scanf(" %s", starting_label);

	printf("What is the destination vertex? ");
	scanf(" %s", dest_label);

	int i, start_idx = 0, dest_idx = 0;
	for(i = 0; i < vertexCount; i++)
	{
		if(strcmp(starting_label, VertexArray[i].label) == 0)
			start_idx = VertexArray[i].idx;
		else if (strcmp(dest_label, VertexArray[i].label) == 0)
			dest_idx = VertexArray[i].idx;
	}

	int dijkstra_len = dijkstra(AdjacencyMatrix, VertexArray, vertexCount, start_idx, dest_idx);
	
	printf("The path from %s to %s is ", VertexArray[start_idx].label, VertexArray[dest_idx].label);
	printPath(VertexArray, start_idx, dest_idx, vertexCount);
	printf(" and\nhas a length of %d\n", dijkstra_len);
    return 0;
}