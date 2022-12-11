#include <cstdint>
#include <vector>

class unicodeFrameBuffer {
        uint8_t _x, _y;
        uint8_t *_frameBuffer;

        public:
        unicodeFrameBuffer(uint8_t, uint8_t);
        ~unicodeFrameBuffer(void);
        void plot(uint8_t, uint8_t);
};
