CXXFLAGS=-g

main: main.cc unicodeFrameBuffer.o
unicodeFrameBuffer.o: unicodeFrameBuffer.cc unicodeFrameBuffer.h
%.o: %.cc %.h

.PHONY: clean

clean:
	rm -f *.o main
