#include "stdafx.h"
#include "opencv2/opencv.hpp"
#include <iostream>
#include "ctime"
#include <map>

using namespace cv;
using namespace std;
void addto_map(int B, int G, int R);
static map<string, int> BGRValues;


void addto_map(int B, int G, int R)
{
	
	string key = to_string(B) + ":" + to_string(G) + ":" + to_string(R);
	if (!BGRValues.empty())
	{
		
		if (BGRValues.find(key)== BGRValues.end())
		{
			BGRValues.insert(pair<string, int>(key, 1));
		}else
		{
			int value = BGRValues.find(key)->second;
			value++;
			BGRValues.find(key)->second = value;
		}
	}
	else
	{
		BGRValues.insert(pair<string,int>(key,1));

	}

}

int findMax(int color) 
{
	if (color+3 <= 255)
	{
		color += 3;

	}
	else {
		color = 255;
	}
	//cout << " max color " << color << endl;
	return color;

}

int findMin(int color)
{
	if (color-3 >=0)
	{
		color -= 3;

	}
	else {
		color = 0;
	}

	//cout << " min color " << color<< endl;
	return color;
}

void colorReduce(cv::Mat& image, int div = 64)
{
	int nl = image.rows;                    // number of lines
	int nc = image.cols * image.channels(); // number of elements per line

	for (int j = 0; j < nl; j++)
	{
		// get the address of row j
		uchar* data = image.ptr<uchar>(j);

		for (int i = 0; i < nc; i++)
		{
			// process each pixel
			data[i] = data[i] / div * div + div / 2;
		}
	}
}

void detectContours(Mat contur)
{
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	Mat gray;
	int thresh = 100;
	int max_thresh = 255;

	cvtColor(contur, gray, COLOR_BGR2GRAY);
	Canny(gray, gray, thresh, thresh * 2, 3);
	findContours(gray, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	//imshow("Contours ", gray);
}

int main()
{
	int keyboard = 0;
	VideoCapture cap("D:\\4th Year Project\\peopele_shopping.mp4");
	
	Mat image,a,b,c;
	
	while ((char)keyboard != 'q' && (char)keyboard != 27) {
		if (!cap.isOpened())
		{
			exit(EXIT_FAILURE);
		}
		cap >> c;
		if (c.empty())
		{
			exit(EXIT_FAILURE);

		}
		resize(c, a, Size(), 0.5, 0.5, INTER_CUBIC);
		b = a.clone();
		/////////use hog descriptor /////////////
		const clock_t beginTime_1 = clock();
		HOGDescriptor hog;
		hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
		vector<Rect> found, found_filtered;
		hog.detectMultiScale(a, found, 0, Size(4, 4), Size(16, 16), 1.05, 2);

		cout << "time for HOG " << float(clock() - beginTime_1) / CLOCKS_PER_SEC << endl;



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
		
		
		for (i = 0; i < found_filtered.size(); i++)
		{
			Rect r = found_filtered[i];
			
			
			///////draw the rectangle//////////////////////////////
			r.x += cvRound(r.width*0.1);
			r.width = cvRound(r.width*0.8);
			r.y += cvRound(r.height*0.06);
			r.height = cvRound(r.height*0.9);
			if (r.y > 0 && r.x > 0)
			{
			//	people.push_back(a(r));
			//	image = a(r).clone();
				rectangle(b, r.tl(), r.br(), cv::Scalar(0, 255, 0), 2);
			}
			
			
			cv::Mat result; // segmentation result (4 possible values)
			cv::Mat bgModel, fgModel;
			//initiate time
			const clock_t beginTime = clock();

			cv::grabCut(a,result,r,bgModel, fgModel,1,cv::GC_INIT_WITH_RECT);

			cv::compare(result, cv::GC_PR_FGD, result, cv::CMP_EQ);
			// Generate output image
			cv::Mat foreground(a.size(), CV_8UC3, cv::Scalar(255, 255, 255));
			a.copyTo(foreground, result); // bg pixels not copied
			//cv::rectangle(image rectangle, cv::Scalar(255, 255, 255), 1);
			//cv::imshow("Image",a);
			//calculate time

			cout << "time for grabcut " << float(clock() - beginTime) / CLOCKS_PER_SEC << endl;
			
			// extract the features

			Mat crop = foreground(r);
	//		imshow("Cropped image", crop);



			//BGR values
			
			medianBlur(crop, crop, 5);

			//clear the static map
			if (!BGRValues.empty())
			{
				BGRValues.clear();

			}


			colorReduce(crop);

			cv::imshow("Color Reduction", crop);
		//	cv::imwrite("output.jpg", crop);
			
			//clear background

			//create a 2d array
			int **boundry = new int*[crop.rows];
			for (int i = 0; i < crop.rows; i++) 
			{
				boundry[i] = new int[2];

			}
				

			// get color values
			for (int i = 0; i < crop.rows; i++)
			{
				int left = 0, right = 0, lvalidate = 0, rvalidate = 0;vector<int> v;
				for (int j = 0, k = crop.cols - 1; j < crop.cols / 2; j++, k--) 
				{
					
					int B = crop.at<cv::Vec3b>(i, j)[0];
					int G = crop.at<cv::Vec3b>(i, j)[1];
					int R = crop.at<cv::Vec3b>(i, j)[2];
					int checkColor = 224;

					if ((B!= checkColor && G!= checkColor && R!= checkColor) && lvalidate==0)
					{
						left = j;
						lvalidate = 1;
						addto_map(B, G, R);
						boundry[i][0] = j;


					}else if(lvalidate==1)
					{
						addto_map(B, G, R);

					}

					int Br = crop.at<cv::Vec3b>(i, k)[0];
					int Gr= crop.at<cv::Vec3b>(i, k)[1];
					int Rr = crop.at<cv::Vec3b>(i, k)[2];

					if ((Br != checkColor && Gr != checkColor && Rr != checkColor) && rvalidate == 0)
					{
						right = k;
						rvalidate = 1;
						addto_map(Br, Gr, Rr);
						boundry[i][1] = k;
					}
					else if (rvalidate == 1)
					{
						addto_map(Br, Gr, Rr);

					}

				}


			}


			//find maximum colors
			int max1 = 0, max2 = 1, max3 = 0;
			string element[3];

			for (auto elem : BGRValues)
			{
				int value = elem.second;
				string index = elem.first;
				if (max1 < value)
				{
					max3 = max2;
					max2 = max1;
					max1 = value;

					element[2] = element[1];
					element[1] = element[0];
					element[0]  = index;

				}
				else if (max2 < value)
				{
					max3 = max2;
					max2 = value;

					element[2] = element[1];
					element[1] = index;

				}
				else if (max3< value)
				{
					max3 = value;

					element[2] = index;

				}


			}

			cout << " max 1 " << element[0] << " " << max1 << " max 2 " << element[1] << " " << max2 << " max 3 " << element[2] << " " << max3 << endl;
			
			

			//detect contours

			
			int maxColors [3][3];
			

			//convert colors to rgb values
			for (int i = 0; i < 3; i++)
			{
				istringstream iss(element[i]);
				string s;
				int j = 0;
				while (getline(iss, s, ':')) {

					maxColors[i][j]= atoi(s.c_str());
					j++;

				}
			}
			
			int colorRange[3][3][2];
			
			
			
			Mat bin1[3];
			Rect rectangles[3];

			//process single image
			for (int i = 0; i < size(bin1); i++)
			{
				
				//find the range for one color
				for (int j = 0; j <3 ; j++)
				{
					int color = maxColors[i][j];
					colorRange[i][j][0] = findMin(color);
					colorRange[i][j][1] = findMax(color);

				}


				//extract the color binary
				inRange(crop, Scalar(colorRange[i][0][0], colorRange[i][1][0], colorRange[i][2][0]), Scalar(colorRange[i][0][1], colorRange[i][1][1], colorRange[i][2][1]),bin1[i]);


				imshow("one color ", bin1[i]);

				vector<vector<Point> > contours;
				vector<Vec4i> hierarchy;

				//
				findContours(bin1[i], contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

				/// Approximate contours to polygons + get bounding rects and circles
				vector<vector<Point> > contours_poly(contours.size());
				vector<Rect> boundRect(contours.size());
				vector<Point2f>center(contours.size());
				vector<float>radius(contours.size());

				Rect maximum_rect;
				int size = 0;
				int index;
				Mat drawing = Mat::zeros(bin1[i].size(), CV_8UC3);
				for (int k = 0; k < contours.size(); k++)
				{
					approxPolyDP(Mat(contours[k]), contours_poly[k], 3, true);
					boundRect[k] = boundingRect(Mat(contours_poly[k]));
					if (size< boundRect[k].area())
					{
						size = boundRect[k].area();
						index = k;
					}

					cout << boundRect[k].area() << endl;
					
					
					
				}
				//draw the rectangle
				rectangles[i] = boundRect[index];
				rectangle(drawing, boundRect[index].tl(), boundRect[index].br(), Scalar(0, 255, 0), 2, 8, 0);
				imshow("rect ",drawing);
				waitKey(0);
				
			


				

				cout << maxColors[i][2] << "," << maxColors[i][1] << "," << maxColors[i][0] << endl;

				
				/*imshow("test", bin);
				waitKey(0);
				Mat crop1;*/

				/*for (int i = 0; i < crop.rows; i++)
				{
					int l = boundry[i][0];
					int r = boundry[i][1];
					if (l<0 || l>crop.cols)
					{
						l = 0;
					}
					if (r<0 || r> crop.cols)
					{
						r = crop.cols - 1;
					}
					

				
				}

				imshow("crop segment", crop);*/

			}
			
			int xCordinates[3];
			int yCordinates[3];
			int maxX = 0 , xind;
			int maxY = 0 , yind;

			for (int i = 0; i < size(rectangles); i++)
			{
				xCordinates[i] = rectangles[i].tl().x;
				if (maxX < rectangles[i].tl().x)
				{
					maxX = rectangles[i].tl().x;
					xind = i;
				}

				if (maxY < rectangles[i].tl().y)
				{
					maxY = rectangles[i].tl().y;
					yind = i;
				}


				yCordinates[i] = rectangles[i].tl().y;
			}

			

			//upperbody color
			int Lb = maxColors[yind][0];
			int Lg = maxColors[yind][1];
			int Lr = maxColors[yind][2];

			cout << "lower body color " << Lb << "," << Lg << "," << Lr << endl;
			

			Mat img(500,500,CV_8UC3);
			img = cv::Scalar(Lr, Lg, Lb);
			
			imshow("Lowerbody ", img);
			//end of detecting contours

			//end of BGR values


		
	
		/*
			//try to extract body parts with head size
			CascadeClassifier head_detection;
			head_detection.load("D:/Visual_Studio_Workspace/opencv/New folder/sources/data/haarcascades/haarcascade_upperbody.xml");
			vector<Rect> upperBody;
			head_detection.detectMultiScale(crop, upperBody, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
			for (int i = 0; i < upperBody.size(); i++)
			{
				Point center(upperBody[i].x + upperBody[i].width*0.5, upperBody[i].y + upperBody[i].height*0.5);
				ellipse(crop, center, Size(upperBody[i].width*0.5, upperBody[i].height*0.5), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);
			}
			*/



			//end of extracting


			keyboard = waitKey(1);
		
		}
	//	imshow("HOG ", b);
		
	}
	return 0;
}