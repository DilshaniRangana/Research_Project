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

int main()
{
	int keyboard = 0;
	VideoCapture cap("D:\\4th Year Project\\people.mp4");
	
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
			cv::imshow("Segmented Image", foreground);

			// extract the features

			Mat crop = foreground(r);
			imshow("Cropped image", crop);

	/*			// adding alpha channel
			Mat src = imread("C:/Users/Dilshani Rangana/Desktop/a.jpg");
			Mat dst(src.rows,src.cols,CV_8UC4);
			Mat tmp, alpha;

			cvtColor(src, tmp, CV_BGR2GRAY);
			threshold(tmp, alpha, 100,10 , CV_THRESH_TOZERO);

			Mat rgb[3];
			split(src, rgb);

			Mat rgba[4] = { rgb[0],rgb[1],rgb[2],alpha };
			merge(rgba, 4, dst);
			imwrite("dst1.png", dst);
			imshow("DST ", dst);



				//end of adding alpha channel

*/




			int height = r.height;
			int width = r.width;
			Mat hsv;

			//BGR values
			
			

			
			for (int i = 0; i < crop.rows; i++)
			{
				int left = 0, right = 0, lvalidate = 0, rvalidate = 0;vector<int> v;
				for (int j = 0, k = crop.cols - 1; j < crop.cols / 2; j++, k--) 
				{
					
					int B = crop.at<cv::Vec3b>(i, j)[0];
					int G = crop.at<cv::Vec3b>(i, j)[1];
					int R = crop.at<cv::Vec3b>(i, j)[2];
					

					if ((B!=0 || G!=0 || R==0) && lvalidate==0)
					{
						left = j;
						lvalidate = 1;
						addto_map(B, G, R);


					}else if(lvalidate==1)
					{
						addto_map(B, G, R);

					}

					int Br = crop.at<cv::Vec3b>(i, k)[0];
					int Gr= crop.at<cv::Vec3b>(i, k)[1];
					int Rr = crop.at<cv::Vec3b>(i, k)[2];

					if ((Br != 0 || Gr != 0 || Rr == 0) && rvalidate == 0)
					{
						right = k;
						rvalidate = 1;
						addto_map(Br, Gr, Rr);
					}
					else if (rvalidate == 1)
					{
						addto_map(Br, Gr, Rr);

					}

				}


			}

			for (auto elem : BGRValues)
			{
				cout << elem.first << " " << elem.second << endl;
			}

			//end of BGR values


		
			

			//finding the max values
					//do the coding here
			//end of finding max values

	
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


			
			waitKey(0);
			keyboard = waitKey(1);
		
		}
	//	imshow("HOG ", b);
		
	}
	return 0;
}