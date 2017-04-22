#include "stdafx.h"
#include <iostream>
#include "opencv2\core.hpp"
#include "opencv\highgui.h"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\objdetect\objdetect.hpp"
#include "opencv2\video.hpp"
#include <opencv2/video/background_segm.hpp>

using namespace std;
using namespace cv;

void main2() {

	Mat frame , pMoGFrame, pMoG2Frame, pGMG2Frame;
	int keyboard =0;
	VideoCapture cap("D:\\4th Year Project\\\People\\people.mp4");
	/*if (!cap.isOpened)
	{
		cout << "Cannot open the camera " << endl;
		exit(EXIT_FAILURE);
	}*/
	while ((char)keyboard != 'q' && (char)keyboard != 27) {
		if (!cap.read(frame))
		{
			cout << "cannot read from the frame " << endl;
			exit(EXIT_FAILURE);

		}

		Ptr<BackgroundSubtractor> pMOG = new BackgroundSubtractorMOG();
		Ptr<BackgroundSubtractor> pMOG2 = new BackgroundSubtractorMOG2();
		Ptr<BackgroundSubtractor> pGMG = new BackgroundSubtractorGMG();
		
		pMOG -> operator()(frame, pMoGFrame);
		pMOG2 -> operator()(frame, pMoG2Frame);
		pGMG -> operator()(frame, pGMG2Frame);

		stringstream ss;
		rectangle(frame, cv::Point(10, 2), cv::Point(100, 20),cv::Scalar(255, 255, 255), -1);
		ss << cap.get(CV_CAP_PROP_POS_FRAMES);
		string frameNumberString = ss.str();
		cout << "frame name is " << frameNumberString << endl;
		putText(frame, frameNumberString.c_str(), cv::Point(15, 15),FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));
		imshow("Frame", frame);
		imshow("FG Mask MOG", pMoGFrame);
		imshow("FG Mask MOG 2", pMoG2Frame);
		imshow("GMG Mask ", pGMG2Frame);
		keyboard = waitKey(2);

	
	
	}
}
