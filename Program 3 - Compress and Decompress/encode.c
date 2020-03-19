#include "functions.h"
#include "error.h"
#include "string.h"

/*
 * frequencyFiller
 * Find the frequency of bytes
 * @fd file descriptor
 * @dictFill the dictionary to fill
 */
void frequencyFiller(int fd, unsigned char *dictFill) {
    unsigned char buf;
    int long long freq[256];
    char byte_read;
    int max;
    int maxLoc;

    // Seek to the beggining
    lseek(fd, 0, SEEK_SET);

    for (int i = 0; i < 256; i++)
        freq[i] = 0;

    byte_read = read(fd, &buf, 1);
    while (byte_read != 0) {
        freq[buf] = freq[buf] + 1;
        byte_read = read(fd, &buf, 1);
    }

    for (int i = 1; i < 16; i++) {
        max = 0;
        for (int j = 1; j < 256; j++) {
            if (freq[j] > max) {
                max = freq[j];
                maxLoc = j;
                dictFill[i] = j;
            }
        }
        freq[maxLoc] = 0;
    }
}

int main(int argc, char **argv) {
        
    //argError(argc, 2);

    // Argv[1] will be the file
    // Check for foError
    //foError(argv[1], 1);

    // Create Dictionary
    unsigned char dict[16];
    dict[0] = 192;
    for (int i = 1; i < 16; i++) {
        dict[i] = 0;
    }

    // Open the file
    //int file = open(argv[1], O_RDONLY, 0644);
    int file = 0;

    // Try to create write file
    //char prefix[500];
    //strcpy(prefix, "new.");
    //char * writeName = strcat( prefix, argv[1]);
    //wfError(writeName);

    //int writeFile = open(writeName, O_RDWR | O_CREAT, 0644);
    //if (writeFile < 0)
    //    error("Error: Error creating new file.\n");

    int writeFile = 1;

    frequencyFiller(file, dict);

    // Set cursor to beggining
    lseek(file, 0, SEEK_SET);

    // Start the encode
    unsigned char read_char;
    unsigned char byte_read = read(file, &read_char, 1);


    unsigned char dictBuff;
    for (int i = 1; i < 16; i++) {
        write(writeFile, &dict[i], 1);
    }
        
    while (byte_read != 0) {
        int indexOf = findInDict(read_char, dict);
        // Find Repeats And Write
        if (indexOf > 0) {
            // Count repeats up to 3
            unsigned char multiplicity = 0;
            unsigned char match = read_char;
            byte_read = read(file, &read_char, 1);
            while (read_char == match && multiplicity < 3 && byte_read != 0) {
                multiplicity++;
                byte_read = read(file, &read_char, 1);
            }

            
            if (byte_read == 1)
                lseek(file, -1, SEEK_CUR);

            write_bit(1, writeFile, 1);
            write_bit(1, writeFile, 1);


            if (multiplicity == 2 || multiplicity == 3)
                write_bit(1, writeFile, 1);

            else
                write_bit(0, writeFile, 1);

            if (multiplicity == 1 || multiplicity == 3)
                write_bit(1, writeFile, 1);

            else
                write_bit(0, writeFile, 1);
            
            

            for (int i = 3; i >= 0; i--) {
                unsigned char toWrite = 0;
                toWrite = (indexOf << 7 - i) >> 7;
                write_bit(toWrite, writeFile, 1);
            }
        }

        // Write 10
        else if (read_char == 0) {
            write_bit(1, writeFile, 1);
            write_bit(0, writeFile, 1);
        }

        // Write 0 Number
        else {
            write_bit(0, writeFile, 1);
            for (int i = 7; i >= 0; i--) {
                unsigned char toWrite = 0;
                toWrite = (read_char << 7 - i) >> 7;
                write_bit(toWrite, writeFile, 1);
            }
        }

        byte_read = read(file, &read_char, 1);
    }

    write_bit(256, writeFile, 1);
    close(file);
    close(writeFile);

}