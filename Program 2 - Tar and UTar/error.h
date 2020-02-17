#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int error(char* myError)
{
	char buffer[256];
	sprintf(buffer, "%s", myError);
	write(1, buffer, strlen(buffer));
	exit(1);
}

// argError
// Checks to see if there are enour arguments
// Otherwise exits
// @argCount, amount of arguments
// @argRequirements, amount of arguments needed
void argError(int argCount, int argRequirements)
{
	if (argCount < argRequirements)
		error("Error: Not enough arguments!\n");	
}

// argType
// Check what kind of argument it is
// @argType the argument type to check
// @numArgs the number of arguments passed
void argType(char* argument, int numArgs)
{
	if (strcmp(argument, "-a") == 0)
		argError(numArgs, 3);

	else if (strcmp(argument, "-d") == 0)
		argError(numArgs, 4);

	else
		error("Error: Argument type not defined.\n");
}

// fileExists
// Check if a file exists
// @int fdIn passed in file descriptor
// @returns 1 if exists, otherwise 0
int fileExists (int fdIn)
{
	if (fdIn == -1)
		return 0;

	
	return 1;
}

// writeError
// Check if writing was successful
// @int writeIn passed in how many bytes written
void writeError(int writeIn)
{
	if (writeIn == 0)
		error("Error: End of file.\n");

	else if (writeIn == -1)
		error("Error: Could not write to file.\n");
}

// writeError
// Check if closing was successful
// @int writeIn passed in how many bytes written
void closeError(int closeIn)
{
	if (closeIn == -1)
		error("Error: Could not close the file.\n");
}