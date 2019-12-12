#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include "common.h"

/* Read specified number of bytes in active blkdev at active offset.
 */

int main(int argc, char **argv)
{
    if (argc != 3) {
        fprintf(stderr, "Usage: ./memdump device bytecount\n");
        return 1;
    }
    unsigned int bytecount = strtol(argv[2], NULL, 16);
    if (!bytecount) {
        // nothing to spit
        return 0;
    }

    int fd = open(argv[1], O_RDWR|O_NOCTTY);
    char s[0x30];
    sendcmdp(fd, "i=0");
    sprintf(s, "while i<0x%04x getb:puth a:i=i+1", bytecount);
    sendcmd(fd, s);

    for (int i=0; i<bytecount; i++) {
        read(fd, s, 2); // read hex pair
        s[2] = 0; // null terminate
        unsigned char c = strtol(s, NULL, 16);
        putchar(c);
    }
    read(fd, s, 2); // read prompt
    return 0;
}

