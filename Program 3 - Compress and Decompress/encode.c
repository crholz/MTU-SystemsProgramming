// Infrequen symbol = 9 Bit quntity with leading 0 00000000 (8 bits)

// A byte of 0 is represented using binary 10 (2 bits)

// A bit pattern of 11 indicates the folowwing bits encode a repeating pattern such that the first 2 bits give a repeat count (0 means no repeat)
// and the following 4 bits represent a frequent symbol code

// Bit patternn 11000000 indicates the end of the file and any remaining bits on the input are ignored. Frequent symbol value zero = EOF

/*
 *
 * 1. Count the frequency of characters and sort them from the highest frequency down to lowest. Top 15 are frequent characters.
 *        - 0 is not a frequent character, always 10
 *        - In a tie in frequency take the unsigned magnitudes of each, smaller in magnitude first
 *        - Unused dictionary entries = 0
 * 
 * 2. Write 15 byte dictionary to output file first
 * 
 * 3. Seek the file to the beginning (if not seekable error)
 * 
 * 4. Read the next character
 * 
 * 5. If current character is frequent, calculate the run length for a maximum of 4. Output encoded
 * 
 * 6. If the character is infrequent write a zero bit then write the character
 * 
 * 7. No padding or filler between entries.
 * 
 * 8. Output the end-of-file symbol by padding it with as many zero bits as necessary
 * 
 */

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
        
    argError(argc, 2);

    // Argv[1] will be the file
    // Check for foError
    foError(argv[1], 1);

    // Create Dictionary
    unsigned char dict[16];
    dict[0] = 192;
    for (int i = 1; i < 16; i++) {
        dict[i] = 0;
    }

    // Open the file
    int file = open(argv[1], O_RDONLY, 0644);

    // Try to create write file
    char prefix[500];
    strcpy(prefix, "new.");
    char * writeName = strcat( prefix, argv[1]);
    wfError(writeName);

    int writeFile = open(writeName, O_RDWR | O_CREAT, 0644);
    if (writeFile < 0)
        error("Error: Error creating new file.\n");

    frequencyFiller(file, dict);

    // Set cursor to beggining
    lseek(file, 0, SEEK_SET);

    // Start the encode
    unsigned char read_char;
    unsigned char byte_read = read(file, &read_char, 1);


    unsigned char dictBuff;
    for (int i = 0; i < 16; i++) {
        write(writeFile, &dict[i], 1);
    }
        
    while (byte_read != 0) {
        int indexOf = findInDict(byte_read, dict);
        // Find Repeats And Write
        if (indexOf > 0) {
            // Count repeats up to 4
            int multiplicity = 0;
            unsigned char match = read_char;
            byte_read = read(file, &read_char, 1);
            while (read_char = match && multiplicity < 3) {
                multiplicity++;
                byte_read = read(file, &read_char, 1);
            }

            // Move cursor back
            lseek(file, (multiplicity + 1), SEEK_CUR);

            write_bit(1, writeFile);
            write_bit(1, writeFile);

            for (int i = 1; i >= 0; i++) {
                int toWrite = 0;
                toWrite = (multiplicity << 7 - i) >> 7;
                write_bit(toWrite, writeFile);
            }

            for (int i = 3; i >= 0; i++) {
                int toWrite = 0;
                toWrite = (indexOf << 7 - i) >> 7;
                write_bit(toWrite, writeFile);
            }
        }

        // Write 10
        else if (read_char == 0) {
            write_bit(1, writeFile);
            write_bit(0, writeFile);
        }

        // Write 0 Number
        else {
            write_bit(0, file);
            for (int i = 7; i >= 0; i--) {
                int toWrite = 0;
                toWrite = (read_char << 7 - i) >> 7;
                write_bit(toWrite, writeFile);
            }
        }

        byte_read = read(file, &read_char, 1);
    }

    write_bit(256, writeFile);
    write(writeFile, &dict[0], 1);
    close(file);
    close(writeFile);

}