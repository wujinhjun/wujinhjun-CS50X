#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    FILE *filePtr = fopen(argv[1], "r");

    if (filePtr == NULL)
    {
        printf("IMAGE: Can't open %s.\n", argv[1]);
        return 1;
    }

    BYTE buffer[512];
    int count = 0;
    char *title = malloc(8 * sizeof(char));
    FILE *imagePtr = NULL;

    while (fread(buffer, sizeof(char), 512, filePtr))
    {
        if ((buffer[0] == 0xff) && (buffer[1] == 0xd8) && (buffer[2] == 0xff) && ((buffer[3] & 0xf0) == 0xe0))
        {
            sprintf(title, "%03i.jpg", count++);
            imagePtr = fopen(title, "w");
            // fwrite(buffer, 512, sizeof(char), imagePtr);
            // count++;
        }

        if (imagePtr != NULL)
        {
            fwrite(buffer, sizeof(char), 512, imagePtr);
        }
    }

    free(title);
    fclose(imagePtr);
    fclose(filePtr);

    return 0;
}