#include "error.h"
#include "functions.h"

int main(int argc, char **argv) {

    // Make sure there are enough arguments
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

    // Try to create write file
    //char prefix[500];
    //strcpy(prefix, "old.");
    //char * writeName = strcat( prefix, argv[1]);
    //wfError(writeName);

    //int writeFile = open(writeName, O_RDWR | O_CREAT, 0644);
    //if (writeFile < 0)
    //    error("Error: Error creating new file.\n");

    int file = 0;
    int writeFile = 1;

    unsigned char readBuf = 0;
    // Read in the dictionary
    for (int i = 1; i < 16; i++) {
        read(file, &readBuf, 1);
        dict[i] = readBuf;
    }

    // Start the decode/encode process
    int eof = 0;
    while (eof != 192) {
        int fetched = read_bit(file);
        if (fetched == 1) {
            // Get the next bit
            fetched = read_bit(file);
            unsigned char insertChar = 0;


            // Zero Case
            if (fetched == 0) {
                for (int i = 0; i < 8; i++)
                    write_bit(0, writeFile, 0);
            }
            

            // Multiplicity Case
            else if (fetched == 1) {
                
                int multBit1 = read_bit(file);
                int multBit2 = read_bit(file);

                // At least one multiplicity
                char multiplicity = 1;

                if (multBit1 == 1)
                    multiplicity = multiplicity + 2;

                if (multBit2 == 1)
                    multiplicity = multiplicity + 1;

                unsigned char lookup = 0;

                // Create the lookup number by adding bits
                for (int i = 0; i < 4; i++) {
                    switch(i) {
                        case 0:
                            fetched = read_bit(file);
                            
                            if (fetched == 1)
                                lookup = lookup + (2*2*2);
                            break;
                        case 1:
                            fetched = read_bit(file);
                            
                            if (fetched == 1)
                                lookup = lookup + (2*2);
                            break;
                        case 2:
                            fetched = read_bit(file);
                            
                            if (fetched == 1)
                                lookup = lookup + (2);
                            break;

                        case 3:
                            fetched = read_bit(file);
                            
                            if (fetched == 1)
                                lookup = lookup + (1);
                            break;
                    }
                }

                // In the case of a single multiplicity zero, end.
                if (lookup == 0 && multiplicity == 1) {
                    eof == 192;
                    break;
                }
                    

                else {
                    char dictChar = dict[lookup];

                    for (int i = 0; i < multiplicity; i++) {
                        for (int i = 7; i >= 0; i--) {
                            int toWrite = 0;
                            toWrite = (dictChar << 7 - i) >> 7;
                            write_bit(toWrite, writeFile, 0);
            }           }
                }

            }
        }

        // If the bit fetched equals zero, write the next 8 bytes as a character
        else if (fetched == 0) {
            for (int i = 0; i < 8; i++) {
                int next_bit = read_bit(file);
                write_bit(next_bit, writeFile, 0);
            }
        }
    }

    close(writeFile);
    close(file);

}