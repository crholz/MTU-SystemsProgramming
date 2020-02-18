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
	int i;
	passedHDR->magic = 0x63746172;
	passedHDR->eop = sizeof(hdr);
	passedHDR->block_count = 0;
	for (i = 0; i < 8; i++) 
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
		i = 3;
		while (i < argc)
		{
			// Less than 8 files
			if (myHeader.block_count < 8)
			{

				// Check if file is already in archive
				inArchiveError(argv[i], &myHeader, fd);

				// Go to end of file to append
				lseek(fd, 0, SEEK_END);

				// Attempt to open the file
				int newFD = open(argv[i], O_RDONLY);
				readError(newFD);
				
				myHeader.file_name[myHeader.block_count] = myHeader.eop + 1;
				myHeader.file_size[myHeader.block_count] = lseek(newFD, 0, SEEK_END);


				//File Name (predecessed by a short)
				short nameSize = strlen(argv[i]);

				printf("Name Size: %d,  Block Count: %d\n", nameSize, myHeader.block_count);

				// write the namesize
				writeError(write(fd, &nameSize, sizeof(nameSize)));

				lseek(fd, 0, SEEK_END);

				writeError(write(fd, argv[i], nameSize));

				// Write the file name
				//write(fd, &nameSize, sizeof(nameSize));

				char* readBuffer[1024];
				// Start Reading the file from beggining
				lseek(newFD, 0, SEEK_SET);
				int bytes_read;
				do {
					bytes_read = read(newFD, readBuffer, 1024);
					write(fd, readBuffer, bytes_read);
				}	while(bytes_read != 0);

				closeError(close(newFD));

				myHeader.block_count = myHeader.block_count + 1;
				myHeader.eop = lseek(fd, 0, SEEK_END);
				lseek(fd, 0, SEEK_SET);
				write(fd, &myHeader, sizeof(myHeader));
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
