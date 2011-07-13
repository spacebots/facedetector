EIGEN=/afs/l2f.inesc-id.pt/home/ferreira/face-recognition/eigen
EFJ=../efj
QTDIR=/usr/lib64/qt4
INC=-I. -I/usr/include/opencv -I$(EFJ) -I$(EIGEN) -I$(QTDIR)/include/QtCore -I$(QTDIR)/include/QtGui
CXXFLAGS = -fPIC -fpic -m64 -pipe -std=c++0x

all: libfacedetector.so

libfacedetector.so: FaceDetector.o
	g++ -shared -o $@ $^

%.o: %.cpp
	g++ $(INC) $(CXXFLAGS) -c $< -o $@

clean:
	-rm FaceDetector.o libfacedetector.so
