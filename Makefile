INC=-I. -I/usr/include/opencv
CXXFLAGS = -fPIC -fpic -std=c++0x

all: libfacedetector.so

libfacedetector.so: FaceDetector.o
	g++ -shared -o $@ $^

%.o: %.cpp
	g++ $(INC) $(CXXFLAGS) -c $< -o $@

clean:
	-rm FaceDetector.o libfacedetector.so
