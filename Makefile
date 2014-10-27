CXX =		g++
CC = 		$(CXX)
CXXFLAGS =	-O3 -std=c++11 -pg -D_DEBUG -g -c -Wall -Wno-unused-variable $(INCLUDES)

INCLUDES = 	-I /home/akos/Programs/OpenCV/opencv-2.4.9/include \
			-I ../Framework/libLogConfigTime/include/ \
			-I ../Framework/libPlatformSpecifics/include/ \
			-I ../Framework/libCommunication/include/ \
			-I include/

LIBS =		-L/home/akos/Programs/OpenCV/opencv-2.4.9/build/lib \
			-lopencv_core \
			-lopencv_highgui \
			-lopencv_imgproc \
			-lopencv_features2d \
			../Framework/libCommunication/libCommunication.a \
			../Framework/libPlatformSpecifics/libPlatformSpecifics.a \
			../Framework/libLogConfigTime/libLogConfigTime.a 
			
			
SRC = 		$(wildcard src/*.cpp)
OBJS =		$(addprefix obj/,$(notdir $(SRC:.cpp=.o))) 		

TARGET = 	SMEyeLDesktop

$(TARGET): framework $(OBJS)
	$(CXX) $(INCLUDES) -o $(TARGET) $(OBJS) $(LIBS)

#.cpp.o:
obj/%.o: src/%.cpp objdir
	$(CC) $(CXXFLAGS) $< -o $@

framework:
	make -C ../Framework/
	
objdir:
	mkdir -p obj
	
all: $(TARGET)
	
clean:
	rm -rf $(OBJS) $(TARGET)
	
cleanall: clean
	make -C ../Framework/ clean


.PHONY: framework clean
			