#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h> // this header file is to define BYTE as a datatype

typedef uint8_t  BYTE; // 8 bit integer store a hexadecimal value

int main(void)
{
    FILE *inptr = fopen("card.raw", "r");
    if (inptr == NULL)
    {
        printf("Cannot read Card\n");
        exit(1);
    }
    bool check  = true;
    bool check2 = true;
    BYTE temp[512];
    char fname[200]; // this holds the names of all the recovered image files
    int counter = 0;
    int byte_val = 0; // stores the return value of fread, if it returns less than 512, it is EOF
    int byte_val2 = 0;
    int loc = 0; // this holds the location of the cursor while reading the input file
    do
    {
        byte_val = fread(temp, 1, 512, inptr);
        if (temp[0] == 0xff && temp[1] == 0xd8 && temp[2] == 0xff)
        {
            sprintf(fname, "%03i.jpg", counter); // this writes the mentioned format to fname, a string.
            counter++;
            check2 = true;
            FILE *outptr = fopen(fname, "w");
            fwrite(temp, 1, 512, outptr);
            while (check2)
            {
                loc = ftell(inptr);
                byte_val2 = fread(temp, 1, 512, inptr);
                if ((temp[0] == 0xff && temp[1] == 0xd8 && temp[2] == 0xff) || byte_val2 < 512)
                {
                    check2 = false; // the above condition checks for start of new jpg or EOF
                    fseek(inptr, loc, SEEK_SET);
                    continue; //skips the write statement if new jpg is detected
                }
                fwrite(temp, 1, 512, outptr);
            }
            fclose(outptr);
        }
    }
    while (byte_val >= 512);
    fclose(inptr);
    return 0;

}