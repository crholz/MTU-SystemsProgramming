#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int error(char* myError)
{
	char buffer[512];
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

// readError
// Check if reading was successful
// @int readIn passed in how many bytes written
void readError(int readIn)
{
	if (readIn == -1)
		error("Error: Could not read from file.\n");
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

// closeError
// Check if closing was successful
// @int writeIn passed in how many bytes written
void closeError(int closeIn)
{
	if (closeIn == -1)
		error("Error: Could not close the file.\n");
}

// archiveCheck
// checks to make sure the file is an archive file
// @fileName the name of the file
void archiveCheck(char* fileName) 
{
	hdr newHeader;
	int fd = open(fileName, O_RDONLY);
	
	int rdError = read(fd, &newHeader, sizeof(newHeader));
	readError(rdError);

	if (newHeader.magic != 0x63746172 && newHeader.eop != lseek(fd, 0, SEEK_END))
		error("Error: Not an archive file.\n");

	closeError(close(fd));
}

// inArchiveError
// Throws and error if a file with the same name exists in the archive
// @fileName the name that is being searched for
// @fileCheck a pointer to the header being searched
// @archFD the archive file descriptor
// @deletionMark 1 if looking for a deletion
int inArchiveError(char* fileName, hdr* fileCheck, int archFD, int start, int deletionMark) 
{
	
	int i = 0;
	while (i < 8)
	{
		if (fileCheck->file_name[i] != 0)
		{
			lseek(archFD, fileCheck->file_name[i], SEEK_SET);


			char readBuffer[2];
			int bytes = read(archFD, readBuffer, sizeof(short));
			short mySize = (short) *(readBuffer);


			char nameBuffer[512] = "";	
			read(archFD, nameBuffer, mySize);


			if (strcmp(fileName, nameBuffer) == 0 && deletionMark == 0 && fileCheck->deleted[i] == 0)
				error("Error: File already exists in the archive.\n");

			else if (strcmp(fileName, nameBuffer) == 0 && deletionMark == 1 && fileCheck->deleted[i] != 1)
			{
				if (start != 0)
				{
					lseek(archFD, start, SEEK_SET);

					hdr myDelete;
					init(&myDelete);

					readError(read(archFD, &myDelete, sizeof(myDelete)));
					lseek(archFD, start, SEEK_SET);
					myDelete.deleted[i] = 1;
					writeError(write(archFD, &myDelete, sizeof(myDelete)));
					return 1;
				}
				else
				{
					lseek(archFD, 0, SEEK_SET);

					hdr myDelete;
					init(&myDelete);

					readError(read(archFD, &myDelete, sizeof(myDelete)));
					lseek(archFD, 0, SEEK_SET);
					myDelete.deleted[i] = 1;
					writeError(write(archFD, &myDelete, sizeof(myDelete)));

					return 1;
				}
			}
			
		}

		i++;
	}

	if (fileCheck->next > 0) 
	{
		lseek(archFD, fileCheck->next, SEEK_SET);

		hdr newHead;
		init(&newHead);
		// Read the next header
		read(archFD, &newHead, sizeof(newHead));

		inArchiveError(fileName, &newHead, archFD, fileCheck->next, deletionMark);
	}

}