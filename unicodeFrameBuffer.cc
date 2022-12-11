#include "unicodeFrameBuffer.h"
#include <cstdio>


unicodeFrameBuffer::unicodeFrameBuffer(uint8_t x, uint8_t y) {
       frameBuffer = new uint8_t*[y/2];
       for (int i = 0; i < y/2; i++)
               frameBuffer[i] = new uint8_t[x/2];
}

unicodeFrameBuffer::~unicodeFrameBuffer(void) {
        delete frameBuffer; //XXX wrong
}

void unicodeFrameBuffer::plot(uint8_t x, uint8_t y) {

        const char *c[16] = {"\u0020", "\u2598", "\u259d", "\u2580", "\u2596", "\u258c", "\u259e", "\u259b", \
                             "\u2597", "\u259a", "\u2590", "\u259c", "\u2584", "\u2599", "\u259f", "\u2588"};
        const uint8_t b[2][2] = {{1, 2}, {4, 8}};
        uint8_t cx = x/2;
        uint8_t cy = y/2;
        uint8_t sx = x%2;
        uint8_t sy = y%2;

        frameBuffer[cx][cy] |= b[sx][sy]; 

        printf("\e[%d;%dH%s", cy+1, cx+1, c[frameBuffer[cx][cy]]);
        fflush(stdout);
}
