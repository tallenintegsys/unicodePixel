CXXFLAGS=-g

main: main.cc unicodeFrameBuffer.o

%.o: %.cc %.h
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -f *.o main
