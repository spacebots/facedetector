#include <thread>
#include <FaceDetector.h>

  FaceDetector detector0(0, "/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml");
void f0() {
  detector0.StartFaceDetection();
}

  FaceDetector detector1(1, "/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml");
void f1() {
  detector1.StartFaceDetection();
}

int main(int argc, char** argv) {
  //FaceDetector detector = "/usr/share/opencv/haarcascades/haarcascade_mcs_lefteye.xml";
  //FaceDetector detector = "/usr/share/opencv/haarcascades/haarcascade_mcs_eyepair_big.xml";
  //FaceDetector detector = "/usr/share/opencv/haarcascades/haarcascade_mcs_mouth.xml";
  //"/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml";

//  FaceDetector detector0 = "/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml";
//  detector0.StartFaceDetection(0);
//  FaceDetector detector1 = "/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml";
//  detector1.StartFaceDetection(1);

  f0();

  printf("program working\n");
  return 0;
}
