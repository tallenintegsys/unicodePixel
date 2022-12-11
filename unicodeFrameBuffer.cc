#include "unicodeFrameBuffer.h"
#include <cstdio>


unicodeFrameBuffer::unicodeFrameBuffer(uint8_t x, uint8_t y) : _x(x), _y(y) {
       _frameBuffer = new uint8_t[(_x/2)*(_y/2)];
}

unicodeFrameBuffer::~unicodeFrameBuffer(void) {
        delete _frameBuffer;
}

void unicodeFrameBuffer::plot(uint8_t x, uint8_t y) {

        const char *c[16] = {"\u0020", "\u2598", "\u259d", "\u2580", "\u2596", "\u258c", "\u259e", "\u259b", \
                             "\u2597", "\u259a", "\u2590", "\u259c", "\u2584", "\u2599", "\u259f", "\u2588"};
        const uint8_t b[2][2] = {{1, 2}, {4, 8}};
        uint8_t cx = x/2;
        uint8_t cy = y/2;
        uint8_t sx = x%2;
        uint8_t sy = y%2;

        _frameBuffer[cx+cy*_x] |= b[sx][sy];

        printf("\e[%d;%dH%s", cy+1, cx+1, c[_frameBuffer[cx+cy*_x]]);
        fflush(stdout);
}
