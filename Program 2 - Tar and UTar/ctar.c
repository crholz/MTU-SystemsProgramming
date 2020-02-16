#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "hdr.h"
#include "error.h"

// Errors to check for
// Check to make sure it is an archive file if existing
// Check to make sure input files are regular
// Check if the file is already in the archive
// Not an archive file


int main(int argc, char **argv)
{
	// Check for sufficient number of arguments
	argError(argc, 2);
	
	// Parse argument type
	argType(argv[1], argc);


	// Define needed variables
	int i, fd, archFile;
	hdr myHeader;

	// Open the archive file
	fd = open(argv[2], O_RDWR, 0644);
	read(fd, &myHeader, sizeof(myHeader));

	// Determine Operation
	// Create File
	if (strcmp(argv[1], "-a") == 0 && argc == 3)
	{
		// Throw error if the file exists
		if (fileExists(fd))
			error("File exists!\n")

		// Create the file
		else
		{
			// Create the header
			char instBuff[256];	

		}
		
	}


	// Append File
	else if (strcmp(argv[1], "-a") == 0 && argc > 3)
	{
		exit(1);
	}

	// Delete File
	else if (strcmp(argv[1], "-d"))
	{
		exit(1);
	}

	
}
