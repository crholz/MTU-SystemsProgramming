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


// init
// Initialize the values of hdr
// @passedHDR, the header
void init(hdr* passedHDR)
{
	// Initialize passedHDR
	passedHDR->magic = 0x63746172;
	passedHDR->eop = sizeof(passedHDR);
	passedHDR->block_count = 0;
	for (int i = 0; i < 8; i++) 
	{
		passedHDR->file_size[i] = 0;
		passedHDR->deleted[i] = 0;
		passedHDR->file_name[i] = 0;
	}

	passedHDR->next = 0;
}

int main(int argc, char **argv)
{
	// Check for sufficient number of arguments
	argError(argc, 2);
	
	// Parse argument type
	argType(argv[1], argc);


	// Define needed variables
	int i, fd, archFile;
	hdr myHeader;

	// Attempt to Open the Archive File
	fd = open(argv[2], O_RDONLY, 0644);
	read(fd, &myHeader, sizeof(myHeader));

	// Determine Operation
	// Create File
	if (strcmp(argv[1], "-a") == 0 && argc == 3)
	{
		// Throw error if the file exists
		if (fileExists(fd))
			error("Error: File exists!\n");

		// Create the file
		else
		{
			// Close current file
			closeError(close(fd));
			
			// Create the file
			fd = open(argv[2],  O_RDWR| O_CREAT, 0644);

			if (fd == -1)
				error("Error: Could not create new file!\n");

			init(&myHeader);

			// Write the header
			writeError(write(fd,  &myHeader, sizeof(myHeader)));

			// Close the file
			closeError(close(fd));
		}
		
	}


	// Append File
	// File might exist
	else if (strcmp(argv[1], "-a") == 0 && argc > 3)
	{
		// If the file doesn't exist, create it
		if (fd == -1)
		{
			// Close current file
			closeError(close(fd));
			fd = open(argv[2],  O_RDWR| O_CREAT, 0644);

			if (fd == -1)
				error("Error: Could not access file!\n");

			init(&myHeader);

			// Write the header
			writeError(write(fd,  &myHeader, sizeof(myHeader)));
		}

		else
		{
			// Close current file
			closeError(close(fd));
			fd = open(argv[2],  O_RDWR, 0644);
		}
		

		archiveCheck(argv[2]);

		// Start appending files
		// Append while there are still files left

		while (i < argc)
		{
			// Less than 8 files
			if (myHeader.block_count < 8)
			{
				
			}

			// More than 8 files
			else
			{
				/* code */
			}

			
			i++;
		}
	}

	// Delete File
	else if (strcmp(argv[1], "-d"))
	{
		exit(1);
	}

	
}
