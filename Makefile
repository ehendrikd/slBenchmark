ifeq ($(OS),Windows_NT)
	CV_INCLUDE = /c/opencv/install/include
	CV_LIB = /c/opencv/bin
	CV_VERSION = 300
else
	CV_INCLUDE = /usr/local/include
	CV_LIB = /usr/local/lib
	CV_VERSION = 
endif

CV_MODULES = core imgcodecs imgproc videoio highgui
CV_LIBRARIES = $(patsubst %,-lopencv_%$(CV_VERSION),$(CV_MODULES))

CPPFLAGS = -g -I$(CV_INCLUDE) -DDEBUG_BUILD
LFLAGS = -L$(CV_LIB) $(CV_LIBRARIES) -I$(CV_INCLUDE) -DDEBUG_BUILD

LIBSRC := $(wildcard *Implementation.cpp) $(wildcard *Infrastructure.cpp) slBenchmark.cpp
LIBOBJS = $(patsubst %.cpp, %.o, $(LIBSRC))

all: main


main: $(LIBOBJS) main.cpp
	echo $(LIBOBJS)
	g++ $(LFLAGS) $(LIBOBJS) main.cpp -o main

$(LIBOBJS):%.o: %.cpp
	$(CXX) $(CPPFLAGS) -c $< -o $@

clean:
	-$(RM) $(LIBOBJS) main
