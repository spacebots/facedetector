LIB=-L/usr/lib64 -lopencv_core -lopencv_highgui -lopencv_objdetect
INC=-I. -I/usr/include/opencv -std=c++0x

all: facedetector-standalone facedetector

facedetector: FaceDetector.o facedetector.o
	g++ -o $@ $^ $(LIB)

facedetector-standalone: FaceDetector.o facedetector-standalone.o
	g++ -o $@ $^ $(LIB)

%.o: %.cpp
	g++ -c $< $(INC)

clean:
	-rm FaceDetector.o facedetector.o facedetector facedetector-standalone
