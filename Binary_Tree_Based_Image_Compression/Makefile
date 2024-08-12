EXEIMGTREE = pa3
OBJS  = PNG.o RGBAPixel.o lodepng.o pa3.o stats.o imgtree.o imgtree-given.o

CXX = clang++
CXXFLAGS = -std=c++14 -c -g -O0 -Wall -Wextra -pedantic
LD = clang++
LDFLAGS = -std=c++14 -lpthread

all : pa3

$(EXEIMGTREE) : $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $(EXEIMGTREE)

pa3.o : pa3.cpp stats.h imgtree.h imgtree-private.h cs221util/PNG.h cs221util/RGBAPixel.h
	$(CXX) $(CXXFLAGS) pa3.cpp

imgtree.o : imgtree.cpp imgtree.h imgtree-private.h
	$(CXX) $(CXXFLAGS) -Wfloat-conversion imgtree.cpp

imgtree-given.o : imgtree-given.cpp imgtree.h
	$(CXX) $(CXXFLAGS) -Wfloat-conversion imgtree-given.cpp

stats.o : stats.cpp stats.h
	$(CXX) $(CXXFLAGS) -Wfloat-conversion stats.cpp

PNG.o : cs221util/PNG.cpp cs221util/PNG.h cs221util/RGBAPixel.h cs221util/lodepng/lodepng.h
	$(CXX) $(CXXFLAGS) cs221util/PNG.cpp

RGBAPixel.o : cs221util/RGBAPixel.cpp cs221util/RGBAPixel.h
	$(CXX) $(CXXFLAGS) cs221util/RGBAPixel.cpp

lodepng.o : cs221util/lodepng/lodepng.cpp cs221util/lodepng/lodepng.h
	$(CXX) $(CXXFLAGS) cs221util/lodepng/lodepng.cpp

clean :
	-rm -f *.o $(EXEIMGTREE)
