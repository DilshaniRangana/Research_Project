// Research_1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "opencv2\core.hpp"
#include "opencv\highgui.h"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\objdetect\objdetect.hpp"
#include <iostream>
#include < opencv2/video/background_segm.hpp>
#include <unordered_map>


using namespace std;
using namespace cv;


int main1()
{
	int select = 3;
	int source = 0;
	VideoCapture cap("D:\\4th Year Project\\people.mp4");

//	VideoCapture cap(2);
	if (!cap.isOpened())
	{
		return -1;
	}
	
	Mat a,b;
	
	for (int x = 1;;x++)
	{
		cap >> a;
		b = a;
		Mat copy = Mat(a.rows, a.cols, CV_8UC3);
		if (source == 1)
		{
			int length = 14;
			std::stringstream path1;
			path1 << "D:\\4th Year Project\\People\\" << x << ".jpg";
			std::string path = path1.str();
			std::cout << "path name " << path << endl;
			a = imread(path, CV_LOAD_IMAGE_GRAYSCALE);

		}
		if (select == 3)
		{
			////start of people detection
			
			HOGDescriptor hog;
			hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
			cvtColor(a, a, CV_RGB2GRAY);
			//	GaussianBlur(a, a,Size(5,5),0);
			//	medianBlur(a, a, 5);
			//	normalize(a, a, 0, 255, NORM_MINMAX, -1, Mat());
			//equalizeHist(a, a);

			if (a.data) {

				vector<Rect> found, found_filtered;
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

				vector<Mat> people;
				int blue = 0, green = 0, red = 0;
				for (i = 0; i < found_filtered.size(); i++)
				{
					Rect r = found_filtered[i];
					people.push_back(b(r));
				//	imshow("people size", b(r));
					Mat frame = b(r);

			/*		//////////////////////////////////////////////////////////
					//////////////get color levels //////////////////////////
					
					blue = 0, green = 0, red = 0;
					unordered_map <string, int> color_levels;
					unordered_map<string, int> ::iterator itr;
					
					string color, delimiter = ",";
					for (int y = 0; y <frame.rows; y++)
					{
						for (int x = 0; x <frame.cols; x++)
						{
							color = "";
							Vec3b intensity = frame.at<Vec3b>(y, x);
							color = color +to_string( (int)intensity.val[0]);
							color = color + delimiter + to_string((int)intensity.val[1]);
							color = color + delimiter + to_string((int)intensity.val[2]);
							cout << "color is " << color << endl;
							
							itr = color_levels.find(color);
							if (y==0 && x==0)
							{
								color_levels[color] = 1;

							}
							if (itr != color_levels.end())
							{
								itr->second = itr->second + 1.0;
							}
							else
							{
								color_levels[color] = 1;
							}
							cout << "size " << color_levels.size() << endl;
						}
					}
					
					
					int max1 = 0, max2 = 0;
					string color1, color2;
					for (auto kv : color_levels) {
						cout << "key " << kv.first << "value " << kv.second << endl;
						if (max1 < kv.second)
						{
							max1 = kv.second;
							color1 = kv.first;
						}
						else if (max2 < kv.second)
						{
							max2 = kv.second;
							color2 = kv.first;

						}

					}
					 blue = stoi(color1.substr(0, color1.find(delimiter)));
					 green = stoi(color1.substr(0, color1.find(delimiter)));
					 red = stoi(color1.substr(0, color1.find(delimiter)));
					 cout << "B " << blue << "G " << green << "R " << red << " value " << max1 << endl;
					Mat Frequent_color = Mat(250, 250, CV_8UC3);
					for (size_t x = 0; x <250; x++)
					{
						for (size_t y = 0; y < 250; y++)
						{
							Frequent_color.at<Vec3b>(x, y)[0] = blue;
							Frequent_color.at<Vec3b>(x, y)[1] = green;
							Frequent_color.at<Vec3b>(x, y)[2] = red;

						}

					}
					imshow("Average Color ", Frequent_color);
					waitKey(0);
					*/

					//////////////////////////////////////////////////////////
					////otsu binarization ///////////////////////////////////
					
		/*			cvtColor(frame, frame, CV_RGB2GRAY);

					threshold(frame, frame, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
					imshow("Binary", frame);

					////////////////////////////////////////////
					///////////contour detection ///////////////
					vector<vector<Point> > contours;
					vector<Vec4i> hierarchy;
					
					int thresh = 50;
					Canny(frame, frame, thresh, thresh * 2, 3);
					/// Find contours
					findContours(frame, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
					
					Mat drawing = Mat::zeros(frame.size(), CV_8UC3);
					vector<Rect> boundRect(contours.size());
					vector<vector<Point> > contours_poly(contours.size());
					float area = 0.0;
						int index=0;
					for (int i = 0; i< contours.size(); i++)
					{
						approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
						boundRect[i] = boundingRect(Mat(contours_poly[i]));
						if (area < boundRect[i].height*boundRect[i].width)
						{
							area = boundRect[i].height*boundRect[i].width;
							index = i;
						}
						
					}
					Scalar color = Scalar(255, 0, 0);
					drawContours(drawing, contours, index, color, 2, 8, hierarchy, 0, Point());

					
					imshow("Drawing", drawing);
					waitKey(0);*/

					/////////////////////////////////////////////////////////////
					/////////////////////// use harris corner ///////////////////

					Mat src, gray;
					// Load source image and convert it to gray
					src = frame;
					cvtColor(src, gray, CV_BGR2GRAY);
					Mat dst, dst_norm, dst_norm_scaled;
					dst = Mat::zeros(src.size(), CV_32FC1);

					// Detecting corners
					cornerHarris(gray, dst, 7, 5, 0.05, BORDER_DEFAULT);

					// Normalizing
					normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
					convertScaleAbs(dst_norm, dst_norm_scaled);

					// Drawing a circle around corners
					for (int j = 0; j < dst_norm.rows; j++)
					{
						for (int i = 0; i < dst_norm.cols; i++)
						{
							if ((int)dst_norm.at<float>(j, i) > 200)
							{
								circle(dst_norm_scaled, Point(i, j), 5, Scalar(0), 2, 8, 0);
							}
						}
}

				//	imshow("Harris Corner ", dst_norm_scaled);
				//	waitKey(0);


					////////////////////////////////////////////////////////////



					r.x += cvRound(r.width*0.1);
					r.width = cvRound(r.width*0.8);
					r.y += cvRound(r.height*0.06);
					r.height = cvRound(r.height*0.9);
					rectangle(a, r.tl(), r.br(), cv::Scalar(0, 255, 0), 2);
					
					
				}
				
				cout << x << " Number of filtered detection" << found_filtered.size() << endl;
				imshow("Copy", a);
				if (waitKey(10) >= 0)
					break;

		}
			
			
			

			
			/*if (EVENT_RBUTTONDOWN) {
				cout << "mouse click detected " << endl;
				destroyWindow("Test");
				continue;
			}*/

		}
	

		if (select == 4)
		{
			cvtColor(a, a, CV_RGB2GRAY);

			Mat grad_x, grad_y, abs_grad_x, abs_grad_y;
			Sobel(a, grad_x, CV_16S, 1, 0, 3, 1, 0, BORDER_DEFAULT);
			convertScaleAbs(grad_x, abs_grad_x);

			/// Gradient Y

			Sobel(a, grad_y, CV_16S, 0, 1, 3, 1, 0, BORDER_DEFAULT);
			convertScaleAbs(grad_y, abs_grad_y);
			addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, a);

			imshow("DOG", a);
			if (waitKey(10) >= 0)
				break;

		}
		if (select == 5)
		{
			CascadeClassifier face_cascade;
			face_cascade.load("D:/Visual_Studio_Workspace/opencv/New folder/sources/data/haarcascades/haarcascade_fullbody.xml");
			std::vector<Rect> faces;
			face_cascade.detectMultiScale(a, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));

			// Draw circles on the detected faces
			for (int i = 0; i < faces.size(); i++)
			{
				Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
				ellipse(a, center, Size(faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);
			}
			imshow("Haar ", a);
			if (waitKey(20) > 0)
				break;

		}
		if (select == 6)
		{
			// MOG, MOG2, GMG Background substraction
			Mat frame = b;
			Mat fgMaskMOG; //fg mask generated by MOG method
			Mat fgMaskMOG2; //fg mask fg mask generated by MOG2 method
			Mat fgMaskGMG; //fg mask fg mask generated by MOG2 method


			Ptr< BackgroundSubtractor> pMOG; //MOG Background subtractor
			Ptr< BackgroundSubtractor> pMOG2; //MOG2 Background subtractor
			Ptr< BackgroundSubtractorGMG> pGMG; //MOG2 Background subtractor



			pMOG = new BackgroundSubtractorMOG();
			pMOG2 = new BackgroundSubtractorMOG2();
			pGMG = new BackgroundSubtractorGMG();

			pMOG->operator()(frame, fgMaskMOG);
			pMOG2->operator()(frame, fgMaskMOG2);
			pGMG->operator()(frame, fgMaskGMG);

			imshow("Frame", frame);
			imshow("MOG", fgMaskMOG);
			imshow("MOG2", fgMaskMOG2);
			imshow("GMG", fgMaskGMG);
			waitKey(0);

		}
	}
	
    return 0;
}

