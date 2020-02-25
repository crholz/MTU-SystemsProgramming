#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "hdr.h"
#include "error.h"

int main(int argc, char **argv)
{
    // Check for sufficient number of arguments
	argError(argc, 2);

    int fd;
    hdr myHeader;

    // Attempt to Open the Archive File
	fd = open(argv[1], O_RDONLY, 0644);

    if (!fileExists(fd))
			error("Error: Archive doesn't exist!\n");

    archiveCheck(argv[1]);

    lseek(fd, 0, SEEK_SET);
    init(&myHeader);

    readError(read(fd, &myHeader, sizeof(myHeader)));

    int last = 0;


    int i = 0;
    while (i <= myHeader.block_count)
    {
        if (myHeader.deleted[i] != 0 && myHeader.file_name[i] != 0)
        {
            int file = myHeader.file_name[i];
            int size = myHeader.file_size[i];

            // Go to where the file name is located
            lseek(fd, file, SEEK_SET);

            char readBuffer[2];
			int bytes = read(fd, readBuffer, sizeof(short));

            // Size of the name
			short mySize = (short) *(readBuffer);


			char nameBuffer[100] = "";	
			read(fd, nameBuffer, mySize);

            // See if the file exists first
            int inArch = open(nameBuffer, O_RDONLY, 0644);

            if(fileExists(inArch))
                error("Error: File exists in path. Please move out of directory\n");

            inArch = open(nameBuffer,  O_RDWR| O_CREAT, 0644);

            char fileBuffer[size];

            readError(read(fd, fileBuffer, size));
            writeError(write(inArch, fileBuffer, size));
            closeError(close(inArch));
                
            if (i == 7 && myHeader.next != 0)
            {
                i = -1;
                last = myHeader.next;

                init(&myHeader);
                lseek(fd, last, SEEK_SET);
                readError(read(fd, &myHeader, sizeof(myHeader)));
            }
            }
        i++;
    }

    
}