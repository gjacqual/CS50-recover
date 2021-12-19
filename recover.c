#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{

    char *filename = NULL;
    char filenameout[8];
    BYTE buf[512];

    // Check quantity of arguments
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return (1);
    }

    filename = argv[1];

    // Open memory card
    FILE *cardptr = fopen(filename, "r");
    if (cardptr == NULL)
    {
        printf("Usage: ./recover image\n");
        return (1);
    }
    FILE *img = fopen("000.jpg", "a");
    if (img == NULL)
    {
        fclose(cardptr);
        printf("Could not create jpg.\n");
        return (1);
    }

    // Repeat reading until end of card
    int i = 0;
    while (fread(&buf, sizeof(BYTE), 512, cardptr))
    {
        // Look for beginning of JPEG
        if ((buf[0] == 0xff && buf[1] == 0xd8 && buf[2] == 0xff) && ((buf[3] & 0xf0) == 0xe0))
        {
            // if first jpeg
            if (i == 0)
            {
                fwrite(&buf, sizeof(BYTE), 512, img);
            }
            else
            {
                fclose(img);
                sprintf(filenameout, "%03i.jpg", i);
                img = fopen(filenameout, "a");
                fwrite(&buf, sizeof(BYTE), 512, img);
            }
            i++;
        }
        // If allready  found jpeg, continue writing to file
        else
        {
            if (i == 0)
            {
                continue;
            }
            else
            {
                fwrite(&buf, sizeof(BYTE), 512, img);
            }
        }
    }

    // Close card and img file
    fclose(cardptr);
    fclose(img);
    return (0);
}