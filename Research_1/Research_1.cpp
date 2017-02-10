// Research_1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "opencv2\core.hpp"
#include "opencv\highgui.h"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\objdetect\objdetect.hpp"
#include <iostream>


using namespace std;
using namespace cv;

int main()
{
	VideoCapture cap("D:\\4th Year Project\\people.mp4");
//	VideoCapture cap(2);
	if (!cap.isOpened())
	{
		return -1;
	}

	HOGDescriptor hog;
	hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
	int length = 15;
	Mat a,b;
//	for (int i = 1; i < length; i++)
	for (int x = 1;;x++)
	{
		/*std::stringstream path1;
		path1 << "D:\\4th Year Project\\People\\" << i << ".jpg";
		std::string path = path1.str();
		std::cout << "path name " << path << endl;
		a = imread(path,CV_LOAD_IMAGE_GRAYSCALE);*/

		////start of people detection
		cap >> a;
	//	cvtColor(b, a, CV_RGB2GRAY);
	//	GaussianBlur(a, a,Size(5,5),0);
	//	medianBlur(a, a, 5);

		if (a.data) {


		/*	vector<Rect> found, found_filtered;
			hog.detectMultiScale(a, found, 0, Size(4, 4), Size(16, 16), 1.05, 2);

			size_t i, j;
			for (i = 0; i < found.size(); i++)
			{
				Rect r = found[i];
				for (j = 0; j < found.size(); j++)
					if (j != i && (r & found[j]) == r)
						break;
				if (j == found.size())
					found_filtered.push_back(r);
			}
			for (i = 0; i < found_filtered.size(); i++)
			{
				Rect r = found_filtered[i];
				r.x += cvRound(r.width*0.1);
				r.width = cvRound(r.width*0.8);
				r.y += cvRound(r.height*0.06);
				r.height = cvRound(r.height*0.9);
				rectangle(a, r.tl(), r.br(), cv::Scalar(0, 255, 0), 2);
			}
			*/
			CascadeClassifier face_detection;
			face_detection.load("D:\\Visual_Studio_Workspace\\opencv\\New folder\\sources\\data\\haarcascades\\haarcascade_fullbody.xml");
			vector<Rect> faces;
			face_detection.detectMultiScale(a, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
			for (int i = 0; i < faces.size(); i++)
			{
				Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
				ellipse(a, center, Size(faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);
			}
			cout << x << " Number of filtered detection" << faces.size() << endl;
			imshow("Test", a);
			//waitKey(0);
			if (waitKey(10) >= 0)
				break;

			
			/*if (EVENT_RBUTTONDOWN) {
				cout << "mouse click detected " << endl;
				destroyWindow("Test");
				continue;
			}*/

		}
	
		
	}
	
    return 0;
}

