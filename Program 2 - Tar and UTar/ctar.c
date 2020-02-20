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

	// Attempt to Open the Archive File
	fd = open(argv[2], O_RDONLY, 0644);
	read(fd, &myHeader, sizeof(myHeader));

	// Determine Operation
	// Create File
	if (strcmp(argv[1], "-a") == 0 && argc == 3)
	{
		// Throw error if the file exists
		if (fileExists(fd))
			error("Error: Archive File exists!\n");

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

		char isFile = 0;
		int startLine = 0;

		while (i < argc)
		{
			while (myHeader.next != 0) 
			{
				startLine = myHeader.next;
				lseek(fd, myHeader.next, SEEK_SET);
				init(&myHeader);

				readError(read(fd, &myHeader, sizeof(myHeader)));
			}

			// Less than 8 files
			if (myHeader.block_count < 8)
			{
				// Check if file is already in archive
				hdr begginingHdr;
				init(&begginingHdr);
				lseek(fd, 0, SEEK_SET);
				read(fd, &begginingHdr, sizeof(begginingHdr));


				inArchiveError(argv[i], &begginingHdr, fd, 0, 0);

				// Go to end of file to append
				lseek(fd, 0, SEEK_END);

				// Attempt to open the file
				int newFD = open(argv[i], O_RDONLY);
				readError(newFD);
				
				myHeader.eop = lseek(fd, 0, SEEK_END);

				myHeader.file_name[myHeader.block_count] = myHeader.eop;
				myHeader.file_size[myHeader.block_count] = lseek(newFD, 0, SEEK_END);


				//File Name (predecessed by a short)
				short nameSize = strlen(argv[i]);

				// write the namesize
				writeError(write(fd, &nameSize, sizeof(nameSize)));

				lseek(fd, 0, SEEK_END);

				writeError(write(fd, argv[i], nameSize));


				char* readBuffer[1024];

				// Start Reading the file from beggining
				lseek(newFD, 0, SEEK_SET);
				int bytes_read;
				do {
					bytes_read = read(newFD, readBuffer, 1024);
					write(fd, readBuffer, bytes_read);
				}	while(bytes_read != 0);

				closeError(close(newFD));

				// Overwrite the beggining
				myHeader.block_count = myHeader.block_count + 1;
				myHeader.eop = lseek(fd, 0, SEEK_END);
				lseek(fd, startLine, SEEK_SET);
				write(fd, &myHeader, sizeof(myHeader));

				isFile = 1;

				i++;
			}

			// More than 8 files
			// Create new header
			else
			{
				// Update the next
				myHeader.next = lseek(fd, 0, SEEK_END);
				startLine = myHeader.next;

				lseek(fd, 0, SEEK_SET);
				write(fd, &myHeader, sizeof(myHeader));

				lseek(fd, 0, SEEK_END);

				// Reset and write header
				init(&myHeader);
				writeError(write(fd,  &myHeader, sizeof(myHeader)));
			}

		}

		// Update all EOP
		if (isFile)
		{
			// Read the first header
			lseek(fd, 0, SEEK_SET);
			readError(read(fd, &myHeader, sizeof(myHeader)));

			// Update EOP
			myHeader.eop = lseek(fd, 0, SEEK_END);
			lseek(fd, 0, SEEK_SET);
			writeError(write(fd, &myHeader, sizeof(myHeader)));

			while(myHeader.next > 0)
			{
				int temp = myHeader.next;
				init(&myHeader);

				lseek(fd, temp, SEEK_SET);
				readError(read(fd, &myHeader, sizeof(myHeader)));

				myHeader.eop = lseek(fd, 0, SEEK_END);
				lseek(fd, temp, SEEK_SET);

				writeError(write(fd, &myHeader, sizeof(myHeader)));
			}
		}



	}

	// Delete File
	else if (strcmp(argv[1], "-d") == 0)
	{
		if (!fileExists(fd))
			error("Error: Archive does not exist!\n");

		
		archiveCheck(argv[2]);

		// Close current file
		closeError(close(fd));
		fd = open(argv[2],  O_RDWR, 0644);


		init(&myHeader);
		lseek(fd, 0, SEEK_SET);
		readError(read(fd, &myHeader, sizeof(myHeader)));

		int isDeleted = inArchiveError(argv[3], &myHeader, fd, 0, 1);

		if (isDeleted == 0)
			error("Error: File not in archive!\n");


		closeError(close(fd));

	}

	
}
