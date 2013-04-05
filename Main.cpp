#include "CHeaders.h"
#include "Headers.h"

bool isStringValidFile(char* s, FILE** f);
bool isStringValidMax(char* s, int* i);
bool isStringValidAlgorithm(char* s, Algorithm* a);
bool isValidArgs(int argc, char** argv, FILE** dataFile,
	int* maxProcesses, int* maxCPUBursts, Algorithm* algorithm,
	int* quantum);
void printUsage();

int main(int argc, char** argv)
{
	if(argc < 5)
	{
		printUsage();
		return 1;
	}	

	FILE* dataFile = 0;
	int maxProcesses = 0;
	int maxCPUBursts = 0;
	Algorithm algorithm = FCFS;
	int quantum = 0;
	

	if(!isValidArgs(argc, argv, &dataFile, &maxProcesses, &maxCPUBursts, &algorithm, &quantum))
	{
		printUsage();
		return 1;
	}

	if(argc < 6)
	{
		quantum = 1;
	}
	
	if(DEBUG_INPUT)
	{
		printf("dataFile pointer = %p\n", dataFile);
		printf("maxProcesses = %d\n", maxProcesses);
		printf("maxCPUBursts = %d\n", maxCPUBursts);
		printf("algorithm = %d\n", algorithm);
		printf("quantum = %d\n", quantum);
	}

	SchedSim schedSim(maxProcesses, maxCPUBursts, algorithm, 1, dataFile);
	schedSim.DES();

	return 0;
}

bool isValidArgs(int argc, char** argv, FILE** dataFile,
	int* maxProcesses, int* maxCPUBursts, Algorithm* algorithm,
	int* quantum)
{
	if(!isStringValidFile(argv[1], dataFile))
	{
		printf("Invalid filename\n");
		return false;
	}
	if(!isStringValidMax(argv[2], maxProcesses))
	{
		printf("Invalid maxProcesses\n");
		return false;
	}
	if(!isStringValidMax(argv[3], maxCPUBursts))
	{
		printf("Invalid maxCPUbursts\n");
		return false;
	}
	if(!isStringValidAlgorithm(argv[4], algorithm))
	{
		printf("Invalid algorithm\n");
		return false;
	}
	if(argc > 5)
	{
		if(!isStringValidMax(argv[5], quantum))
		{
			printf("Invalid quantum\n");
			return false;
		}
	}

	return true;
}

bool isStringValidFile(char* s, FILE** f)
{
	*f = fopen(s, "r");
	return *f;
}

bool isStringValidMax(char* s, int* i)
{
	return ((*i = atoi(s)) > 0);
}

bool isStringValidAlgorithm(char* s, Algorithm* a)
{
	int i = atoi(s);

	switch(i)
	{
		case 1:
			*a = FCFS;
			return true;
		case 2:
			*a = SJF;
			return true;
		case 3:
			*a = SRTF;
			return true;
		default:
			return false;
	}
}

void printUsage()
{
	printf("usage: ./SchedSim <filename> <maxProcesses> <maxCPUBursts> <algorithm> [quantum] \n");
}
