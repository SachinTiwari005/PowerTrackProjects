#include "mp3.h"

FILE *mp3_check(char fname[], int mode)
{
    char tag[4];
    short ver;

    FILE *fp;

    if (strstr(fname, ".mp3") == NULL) // MODE: View
    {
        printf("Not a mp3 file\n");
        return NULL;
    }

    if (mode == 0)
    {
        fp = fopen(fname, "r");
    }
    else
    {
        fp = fopen(fname, "r+");
    }

    if (fp == NULL)
    {
        perror("fopen");
        printf("ERROR: Unable to open file\n");
        return NULL;
    }

    // check ID3 tag
    fread(tag, sizeof(char), 3, fp);
    tag[3] = '\0';

    if (strcmp(tag, "ID3") != 0)
    {
        // tag ID error
        printf("Invalid ID3 tag.\n");
        fclose(fp);
        return NULL;
    }

    // check version
    fread(&ver, sizeof(short), 1, fp);

    if (ver != 3)
    {
        // different version : error
        printf("Unsupported version.\n");
        fclose(fp);
        return NULL;
    }
    return fp;
}

int mp3view(FILE *fptr)
{
    char frame_id[5];
    char data[256];

    unsigned char size[4];
    unsigned char tag_bytes[4];

    unsigned int tag_size;
    long tag_end;

    // Read ID3 tag size
    fseek(fptr, 6, SEEK_SET);

    fread(tag_bytes, 4, 1, fptr);

    printf("%s", VIEW_OUTPUT);

    // synchsafe integer  msb bit
    tag_size = ((tag_bytes[0] & 0x7F) << 21) |
               ((tag_bytes[1] & 0x7F) << 14) |
               ((tag_bytes[2] & 0x7F) << 7) |
               (tag_bytes[3] & 0x7F);

    tag_end = 10 + tag_size; // add the initial 10 byte header

    // first frame
    fseek(fptr, 10, SEEK_SET);

    while (ftell(fptr) < tag_end)
    {

        unsigned int frame_size;

        fread(frame_id, 4, 1, fptr);
        frame_id[4] = '\0';

        // Reached the padding
        if (frame_id[0] == 0)
            break;

        if (strcmp(frame_id, "TIT2") == 0 ||
            strcmp(frame_id, "TPE1") == 0 ||
            strcmp(frame_id, "TALB") == 0 ||
            strcmp(frame_id, "TYER") == 0 ||
            strcmp(frame_id, "TCON") == 0 ||
            strcmp(frame_id, "COMM") == 0)
        {

            fread(size, 4, 1, fptr);

            frame_size = ((unsigned int)size[0] << 24) |
                         ((unsigned int)size[1] << 16) |
                         ((unsigned int)size[2] << 8) |
                         size[3];

            // skip flags
            fseek(fptr, 2, SEEK_CUR);

            // skip encoding byte
            fseek(fptr, 1, SEEK_CUR);

            fread(data, frame_size - 1, 1, fptr);

            data[frame_size - 1] = '\0';

            if (strstr(frame_id, "TIT2") != NULL)
                printf("TITLE\t\t:\t\t%s\n", data);
            else if (strstr(frame_id, "TPE1") != NULL)
                printf("ARTIST\t\t:\t\t%s\n", data);
            else if (strstr(frame_id, "TALB") != NULL)
                printf("ALBUM\t\t:\t\t%s\n", data);
            else if (strstr(frame_id, "TYER") != NULL)
                printf("YEAR\t\t:\t\t%s\n", data);
            else if (strstr(frame_id, "TCON") != NULL)
                printf("MUSIC\t\t:\t\t%s\n", data);
            else if (strstr(frame_id, "COMM") != NULL)
                printf("COMMENT\t\t:\t\t%s\n", data);
        }
        else
        {
            // Skip the unwanted frame
            fseek(fptr, frame_size, SEEK_CUR);
        }
    }

    printf("------------------------------------------------------------------------------------------------------\n");

    return 0;
}




int mp3edit(char *option, char text[], FILE *fptr)
{
    char t_frame[5];
    unsigned char n=0;

    switch(option[1])
    {
        case 't': 
            n = 1;
            strcpy(t_frame, "TIT2");
            break;
        case 'a': 
            n = 2;
            strcpy(t_frame, "TPE1"); 
            break;
        case 'A': 
            n = 3;
            strcpy(t_frame, "TALB"); 
            break;
        case 'y': 
            n = 4;
            strcpy(t_frame, "TYER"); 
            break;
        case 'm': 
            n = 5;
            strcpy(t_frame, "TCON"); 
            break;
        case 'c': 
            n = 6;
            strcpy(t_frame, "COMM"); 
            break;
        default:
            printf("%s\n", ERROR_USAGE);
            return -1;
    }

    unsigned char tag_bytes[4];

    fseek(fptr, 6, SEEK_SET);
    fread(tag_bytes, 1, 4, fptr);

    unsigned int tag_size =  ((tag_bytes[0] & 0x7F) << 21) |
                             ((tag_bytes[1] & 0x7F) << 14) |
                             ((tag_bytes[2] & 0x7F) << 7)  |
                             (tag_bytes[3] & 0x7F);

    long tag_end = 10 + tag_size;

    fseek(fptr, 10, SEEK_SET);

    while(ftell(fptr) < tag_end)
    {
        char frame_id[5];
        unsigned char size[4];
        unsigned int frame_size;

        fread(frame_id, 1, 4, fptr);
        frame_id[4] = '\0';

        if(frame_id[0] == 0)
            break;

        fread(size, 1, 4, fptr);

        frame_size = ((unsigned int)size[0] << 24) |
                     ((unsigned int)size[1] << 16) |
                     ((unsigned int)size[2] << 8)  |
                      size[3];

        /*Skip flags*/
        fseek(fptr, 2, SEEK_CUR);

        if(strcmp(frame_id, t_frame) == 0)
        {
            long text_pos = ftell(fptr);

            /*Skip Encoding*/
            fseek(fptr, 1, SEEK_CUR);

            if(strlen(text) > frame_size - 1)
            {
                printf("Cannot update the frame.\n");
                printf("ERROR: New text is longer than existing frame text.\n");
                return -1;
            }

            /* Overwrite the text */
            fwrite(text, 1, strlen(text), fptr);

            /* Padding the  remaining bytes with '\0' */
            int remain = (frame_size - 1) - strlen(text);

            while(remain--)
            {
                fputc('\0', fptr);
            }

            printf("%s", EDIT_DETAIL);

            if(n == 1)
            {
                printf("-----------------CHANGE THE TITLE-----------------\n\n");
                printf(" TITLE\t\t:\t%s\n\n", text);
                printf("-------------TITLE CHANGED SUCCESSFULLY-----------\n\n");
            }
            else if(n == 2)
            {
                printf("-----------------CHANGE THE ARTIST-----------------\n\n");
                printf(" ARTIST\t\t:\t%s\n\n", text);
                printf("-------------ARTIST CHANGED SUCCESSFULLY-----------\n\n");
            }
            else if(n == 3)
            {
                printf("-----------------CHANGE THE ALBUM-----------------\n\n");
                printf(" ALBUM\t\t:\t%s\n\n", text);
                printf("-------------ALBUM CHANGED SUCCESSFULLY-----------\n\n");
            }
            else if(n == 4)
            {
                printf("-----------------CHANGE THE YEAR-----------------\n\n");
                printf(" YEAR\t\t:\t%s\n\n", text);
                printf("-------------YEAR CHANGED SUCCESSFULLY-----------\n\n");
            }
            else if(n == 5)
            {
                printf("-----------------CHANGE THE CONTENT TYPE-----------------\n\n");
                printf(" MUSIC\t\t:\t%s\n\n", text);
                printf("-------------CONTENT TYPE CHANGED SUCCESSFULLY-----------\n\n");
            }
            else if(n == 6)
            {
                printf("-----------------CHANGE THE COMMENT-----------------\n\n");
                printf(" COMMENT\t\t:\t%s\n\n", text);
                printf("-------------COMMENT CHANGED SUCCESSFULLY-----------\n\n");
            }

            return 0;
        }
        else
        {
            /* Skip frame data */
            fseek(fptr, frame_size, SEEK_CUR);
        }
    }

    printf("FRAME NOT FOUND\n");

    return -1;
}