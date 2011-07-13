//----------------------------------------------
// Heriot-Watt University
// MACS 
// www.lirec.eu
// author: Amol Deshmukh
// Date: 17/03/2009
//-----------------------------------------------

#ifndef FACEDETECT_INCLUDEDEF_H
#define FACEDETECT_INCLUDEDEF_H

#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

#include <cstdlib>
#include <cstring>
#include <assert.h>
#include <cmath>
#include <float.h>
#include <limits.h>
#include <time.h>
#include <cctype>
#include <iostream>

//-----------------------------------------------

namespace efj { class Database; }

//-----------------------------------------------

/**
 * This class provides face detection capabilties
 */
class FaceDetector {
public:

  inline FaceDetector(int camId, const char* cascade_name) :
    m_dScale(1.20), m_bflagShowResult(true), m_dMidX(0), m_iNumFaces(0), m_bFaceDetected(false),
        m_bUserProximicFlag(false), m_cascade(cascade_name), _camId(camId), m_criteria(0),
     m_drawName(true), m_drawFrame(true), m_writeFrames(false) {

    if (m_cascade.empty()) {
      fprintf(stderr, "ERROR: Could not load classifier cascade\n");
      fprintf(stderr, "Usage: facedetect --cascade=\"<cascade_path>\" [filename|camera_index]\n");
    }

    std::ostringstream oss;
    oss << "result " << _camId;
    _windowName = oss.str().c_str();
//    if (m_bflagShowResult)
//      cvNamedWindow(oss.str().c_str(), 1);
  }

  ~FaceDetector();

  //Set scale for the image
  inline void SetScale(double dScale) {
    m_dScale = dScale;
  }

  inline double GetScale() {
    return m_dScale;
  }

  // show output on screen
  inline void ShowResult(bool bflag) {
    m_bflagShowResult = bflag;
  }

  // output screen flag
  inline bool GetShowResultFlag() {
    return m_bflagShowResult;
  }

  void StartFaceDetection(const efj::Database *efjdb = NULL);

  inline double GetFaceMidPointX() {
    return m_dMidX;
  }

  inline double GetFaceMidPointY() {
    return m_dMidY;
  }

  inline double GetFaceAngleX() {
    return m_dAngleX;
  }

  inline double GetFaceAngleY() {
    return m_dAngleY;
  }

private: // attributes

  // number of faces detected
  int m_iNumFaces;

  // boolean value if face is detected
  bool m_bFaceDetected;

  // flag to detect face at threshold distance (camera and resolution dependent)
  // resolution required 640 * 480 (can be modified acc to requirement)
  bool m_bUserProximicFlag;

  // scale factor for image
  double m_dScale;

  //face midpoint(+ve:Face is left side, -ve:right side of camera)
  double m_dMidX, m_dMidY;

  //angle from face midpoint
  //(if value is +ve:Face is at left side, -ve:right side of camera)
  double m_dAngleX, m_dAngleY;

  //show result in a window
  bool m_bflagShowResult;

  cv::CascadeClassifier m_cascade;

  int _camId;

  std::string _windowName;

  int m_criteria;  // detection criteria

  bool m_drawName, m_drawFrame, m_writeFrames;

private: // methods

  //detects a face and draws rectangle around the face
  void DetectAndDraw(cv::Mat &frame, double dScale, const efj::Database *efjdb);

  //detect face in a region of an image
  int DetectSubFace(cv::Mat &cvTempimage) {
    std::vector<cv::Rect> subfaces;
    m_cascade.detectMultiScale(cvTempimage, subfaces, m_dScale, 3, 0 | CV_HAAR_DO_CANNY_PRUNING, cv::Size(0, 0));
    //if face found
    return (subfaces.size() == 1);
  }

};
#endif
