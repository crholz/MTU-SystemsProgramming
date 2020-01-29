#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define  F_first        1   /* This is the first call to the function. */
#define  F_last         2   /* This is the last call to the function. Free the memory area. */
#define  F_data_int     3   /* Void * argument points to integer data. */
#define  F_data_char    4   /* Void * argument points to character string. */
#define  F_data_float   5   /* Void * argument points to a float data value. */
#define  F_print        6   /* Print the accumulated values. */

void * f (int code, void * mem, void * data)
{
    long long int z_data = (long long int)data;
    printf("%d : %p %p %lld\n",code, mem, data, z_data);


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
                free(mem);
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
                printf("Wrote %d to memory.\n", setter);
                *currentAdd = *currentAdd + 4;
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
                while (cPoint[i] != 0)
                {
                    *charLoc = cPoint[i];
                    charLoc = charLoc + (char) 1;
                    *currentAdd = *currentAdd + 1;
                    printf("%c", cPoint[i]);
                    i = i + 1;
                }

                // Add a 0 at the end of the string
                *charLoc = (int) 0;
                *currentAdd = *currentAdd + 1;
                printf(" wrote to memory.\n");
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
                fPoint = data;
                float setter = *fPoint;

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
                printf("Wrote %f to memory.\n", setter);
                *currentAdd = *currentAdd + 4;
            }
            break;

        /*
         * Print the accumulated values.
         */
        case 6:
            if (mem)
            {
                
            }
            break;
    }

    
    return mem;
}
