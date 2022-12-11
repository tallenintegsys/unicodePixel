#include "unicodeFrameBuffer.h"
#include <cstdio>
#include <unistd.h>


unicodeFrameBuffer fb(160,80);

int main(int argv, char **argc) {
        printf ("\e[2J");
        fflush(stdout);

        for (int i=0; i<=10; i++) {
                fb.plot(i,i);
                fb.plot(i,10-i);
                sleep(1);
        }

        printf ("\e[15;1H");
        return 0;
}

