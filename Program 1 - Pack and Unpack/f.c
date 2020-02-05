#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define  F_first        1   /* This is the first call to the function. */
#define  F_last         2   /* This is the last call to the function. Free the memory area. */
#define  F_data_int     3   /* Void * argument points to integer data. */
#define  F_data_char    4   /* Void * argument points to character string. */
#define  F_data_float   5   /* Void * argument points to a float data value. */
#define  F_print        6   /* Print the accumulated values. */

// Corbin Holz

void * f (int code, void * mem, void * data)
{
    long long int z_data = (long long int)data;
    //printf("%d : %p %p %lld\n",code, mem, data, z_data);


    // Create a switch statement to determine the code
    switch (code)
    {
        /*
         * First Call to the Function
         * Most likely allocate memory
         */ 
        case 1:
            if (!mem && z_data != 0) 
            {
                // Allocate z_data bytes to memory
                mem = (char*) malloc(z_data * sizeof(char));

                // Create an integer pointer to allocate the first two bytes
                short defaultLoc = 2;
                short * locPoint;

                // Set the first two bytes of memory to the end of memory (mem + 2)
                locPoint = mem;
                *locPoint = defaultLoc;

                // Return the allocated address
                return mem;
            }

            else if (z_data == 0)
                return 0;

            break;

        /*
         * Last Call to the Function
         * Free the memory
         */
        case 2:
            if (mem)
            {
                free(mem);
            } 
            break;

        /*
         * Void * argument points to integer data.
         */
        case 3:
            if (mem)
            {
                // Get the integer value of the data pointer
                int * iPoint;
                iPoint = data;
                int setter = *iPoint;
                setter = htonl(setter);

                // Find Current location of the string
                short *currentAdd;
                short memLoc;

                currentAdd = mem;
                memLoc = *currentAdd;

                // Set identifier
                char identifier = code;
                char *charLoc = (char *) currentAdd + (char) memLoc;
                *charLoc = identifier;
                charLoc = charLoc + 1;

                // Set the integer
                int * destination = (int *) charLoc;
                *destination = setter;
                *currentAdd = *currentAdd + 5;
            }
            break;

        /*
         * Void * argument points to character string.
         */
        case 4:
            if (mem)
            {
                // Get the string value of the data pointer
                char * cPoint;
                cPoint = data;

                // Find Current location of the string
                short *currentAdd;
                short memLoc;

                currentAdd = mem;
                memLoc = *currentAdd;

                // Set identifier
                char identifier = code;
                char *charLoc = (char *) currentAdd + (char) memLoc;
                *charLoc = identifier;
                *currentAdd = *currentAdd + 1;
                charLoc = charLoc + 1;

                int i = 0;

                // Print each character
                while (cPoint[i] != 0)
                {
                    *charLoc = cPoint[i];
                    charLoc = charLoc + (char) 1;
                    *currentAdd = *currentAdd + 1;
                    i = i + 1;
                }

                // Add a 0 at the end of the string
                *charLoc = (int) 0;
                *currentAdd = *currentAdd + 1;
            }
            break;

        /*
         * Void * argument points to a float data value.
         */
        case 5:
            if (mem)
            {
                // Get the float value of the data pointer
                float * fPoint;

                // Get the Pointer
                fPoint = data;
                float temp = *fPoint;

                float setter;

                // Flip the bits using chars
                char * flipped = (char *) &setter;
                char * given = (char *) fPoint;

                flipped[0] = given[3];
                flipped[1] = given[2];
                flipped[2] = given[1];
                flipped[3] = given[0];


                // Find Current location of the string
                short *currentAdd;
                short memLoc;

                currentAdd = mem;
                memLoc = *currentAdd;

                // Set identifier
                char identifier = code;
                char *charLoc = (char *) currentAdd + (char) memLoc;
                *charLoc = identifier;
                charLoc = charLoc + 1;

                // Set the integer
                float * destination = (float *) charLoc;
                *destination = setter;
                *currentAdd = *currentAdd + 5;
            }
            break;

        /*
         * Print the accumulated values.
         */
        case 6:
            if (mem)
            {
                // Find Current location of the string
                short *currentAdd;
                short memLoc;

                currentAdd = mem;
                memLoc = *currentAdd;
                int i = 2;

                while (i < memLoc)
                {
                    // Find the first character code
                    char * findCode = mem + i;
                    char decoded = *findCode;
                    i = i + 1;

                    // Print statement for each datatype
                    switch (decoded)
                    {
                        // Int Print
                        case 3:
                            ;
                            int * intAddress;
                            intAddress = mem + (char) i;
                            int containedInt = *intAddress;
                            printf("%d", containedInt);
                            i = i + 4;
                            break;

                        // String Print
                        case 4:
                            ;
                            char * charAddress;
                            charAddress = mem + (char) i;
                            char containedChar = *charAddress;
                            while (containedChar != 0)
                            {
                                printf("%c", containedChar);
                                i = i + 1;
                                charAddress = mem + (char) i;
                                containedChar = *charAddress;
                            }
                            i = i + 1;
                            break;

                        // Float Print
                        case 5:
                            ;
                            float * fAddress;
                            fAddress = mem + (char) i;
                            float containedFloat = *fAddress;
                            printf("%f", containedFloat);
                            i = i + 4;
                            break;
                    }
                }
            }
            printf("\n");
            break;
    }

    
    return mem;
}
