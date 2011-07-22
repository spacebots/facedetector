// $Id: FaceDetector.cpp,v 1.6 2011/07/22 14:46:58 david Exp $
//
// Copyright (C) 2008-2011 INESC ID Lisboa.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Log: FaceDetector.cpp,v $
// Revision 1.6  2011/07/22 14:46:58  david
// Minor cleanup.
//
// Revision 1.5  2011/07/22 14:30:26  ferreira
// Added Copyright comment
//

#include <sstream>
#include "FaceDetector.h"
#include "Database.h"

//-------------------------------------------------------------

FaceDetector::~FaceDetector() {
  cv::destroyWindow(_windowName);
}

//-------------------------------------------------------------

void FaceDetector::StartFaceDetection(const efj::Database *efjdb) {
  cv::VideoCapture capture(_camId);

  if (!capture.isOpened()) {
    std::cerr << "Failed to capture from device " << _camId << std::endl;
    return;
  }

  while (true) {
    cv::Mat frame;
    capture >> frame;
    DetectAndDraw(frame, m_dScale, efjdb);
    char key = (char)cv::waitKey(10);
    switch (key) {
    case -1:
      break;
    case 'B':
      m_criteria |= CV_HAAR_FIND_BIGGEST_OBJECT;
      std::cout << "FIND BIGGEST: ON\n";
      break;
    case 'b':
      m_criteria = CV_HAAR_FIND_BIGGEST_OBJECT;
      break;
    case 'P':
      m_criteria |= CV_HAAR_DO_CANNY_PRUNING;
      break;
    case 'p':
      m_criteria = CV_HAAR_DO_CANNY_PRUNING;
      break;
    case 'R':
      m_criteria |= CV_HAAR_DO_ROUGH_SEARCH;
      break;
    case 'r':
      m_criteria = CV_HAAR_DO_ROUGH_SEARCH;
      break;
    case 'S':
      m_criteria |= CV_HAAR_SCALE_IMAGE;
      break;
    case 's':
      m_criteria = CV_HAAR_SCALE_IMAGE;
      break;
    case '0':
      m_criteria = 0;
      break;
    case 'f':
      m_drawFrame = !m_drawFrame;
      break;
    case 'n':
      m_drawName = !m_drawName;
      break;
    case 'c':
      m_writeFrames = !m_writeFrames;
      break;
    case 27:
      return; // escape
    default:
      ;/* ignore */
    }
  }
}

//-------------------------------------------------------------
// Some code in this function was partially/originally written
// in C at Heriot-Watt University/MACS (www.lirec.eu) by
// Amol Deshmukh (17/03/2009)
//-----------------------------------------------
void FaceDetector::DetectAndDraw(cv::Mat &frame, double m_dScale, const efj::Database *efjdb) {
  cv::Mat gray(frame.cols, frame.rows, 8, 1);
  cv::cvtColor(frame, gray, CV_BGR2GRAY);

  cv::Mat small_img;
  cv::resize(gray, small_img,
             cv::Size(cvRound(frame.cols / m_dScale), cvRound(frame.rows / m_dScale)));
  cv::equalizeHist(small_img, small_img);

  double t = (double)cvGetTickCount();
  std::vector<cv::Rect> faces;
  m_cascade.detectMultiScale(small_img, faces, m_dScale, 3, m_criteria, cv::Size(20, 20));
  t = (double)cvGetTickCount() - t;
  //fprintf(stderr, "detection time = %gms\n", t/((double)cvGetTickFrequency()*1000.) );

  for (int fx = 0; fx < faces.size(); fx++) {
    cv::Rect &r = faces[fx];
    cv::Point2d center((r.x + r.width * 0.5) * m_dScale, (r.y + r.height * 0.5) * m_dScale);
    double radius = (r.width + r.height) * 0.25 * m_dScale;

    //cutout the portion which is detected as face
    cv::Mat &detected = frame.adjustROI(cvRound(r.x * m_dScale), cvRound(r.y * m_dScale),
                                        cvRound(r.width * m_dScale), cvRound(r.height * m_dScale));

    cv::Mat recorte(
                    frame,
                    cv::Rect(cv::Point(cvRound(r.x * m_dScale), cvRound(r.y * m_dScale)),
                             cv::Size(cvRound(r.width * m_dScale), cvRound(r.height * m_dScale))));
    //DEBUG cv::imshow("detected"+(char)fx, recorte);

    if (m_writeFrames) {
      // SLOW WRITE
      std::ostringstream oss;
      oss << "img" << (int)t << ".png";
      cv::imwrite(oss.str(), recorte);
    }

    // redetect face for more accuracy
    if (1 || DetectSubFace(detected)) {
      m_dMidX = (frame.cols / 2) - center.x;
      m_dMidY = (frame.rows / 2) - center.y;

      //face detect flag set to true
      m_bFaceDetected = true;

      //calculate angles from detected face
      m_dAngleX = m_dMidX * (180.0 / frame.cols);
      m_dAngleY = m_dMidY * (180.0 / frame.rows);
      //std::cout << "Face Angle X " << m_dAngleX  << std::endl;
      //std::cout << "Face Angle Y " << m_dAngleY << std::endl;

      //Create a point to represent the face locations
      cv::Point2d pt1(r.x * m_dScale, r.y * m_dScale);
      cv::Point2d pt1e(r.x * m_dScale, r.y * m_dScale - 4);
      cv::Point2d pt2((r.x + r.width) * m_dScale, (r.y + r.height) * m_dScale);

      if (m_drawName && efjdb) {
        std::vector<std::string> people = {"david", "joana", "miguel", "jaime"};
        int result;

        Eigen::VectorXd image;
        image.resize(72 * 72);
        cv::Mat scaled;
        cv::resize(recorte, scaled, cv::Size(72, 72), 72.0 / frame.cols, 72.0 / frame.rows,
                   cv::INTER_CUBIC);
        cv::cvtColor(scaled, scaled, CV_BGR2GRAY);
        //DEBUG cv::imshow("scaled", scaled);
        for (int rx = 0; rx < scaled.rows; rx++) {
          for (int cx = 0; cx < scaled.cols; cx++) {
            image(72 * rx + cx) = (double)scaled.data[72 * rx + cx];
            //DEBUG std::cout << (int)image(72 * rx + cx) << "\n";
          }
        }
        std::string name = "";  //unknown
        if (efjdb->id(image, 0.6, result)) {
          //namess << "R" << result;
          name = people[result];
        }
        cv::putText(frame, name, pt1e, cv::FONT_HERSHEY_SIMPLEX, 0.5,
                    cv::Scalar(0, 0, 255, 0), 2);
      }
      if (m_drawFrame)
        cv::rectangle(frame, pt1, pt2, CV_RGB(255, 0, 0), 1, 8, 0);

      // Calculate the area of the face bounding box
      //calculate difference in area of face bounding box and image size
      int areaFace = r.height * r.width;
      int areaImage = frame.cols * frame.rows;
      int areaDiff = areaImage - areaFace;

      //std::cout << "Area difference "<< areaDiff << std::endl;

      // set flag if face  detected at threshold distance 80cm-100 cm (camera and resolution dependent)
      // resolution required 640 * 480 (can be modified acc to requirement)
      if (areaDiff < 297000) {
        m_bUserProximicFlag = true;
        std::cout << "[" << _camId << "][" << fx << "]: " << ((double)areaFace) / areaImage << " ("
            << t / ((double)cvGetTickFrequency() * 1000) << "ms)" << std::endl;
        //std::cout << "[" << m_camId << "]: user proximity 80cm to 100 cm " << std::endl;
      } else {
        m_bUserProximicFlag = false;
      }
    }

    m_iNumFaces = faces.size();
  }

  if (m_bflagShowResult)
    cv::imshow(_windowName, frame);
}
