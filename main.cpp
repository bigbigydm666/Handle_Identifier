/*
 * @Author: your name
 * @Date: 2021-03-14 22:13:42
 * @LastEditTime: 2021-03-15 14:50:15
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /Hand-Identify/main.c
 */
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(void)
{
   Mat src = imread("/home/ydm/Codes/yaotongv1.0/build/1020.jpg");
   if(src.empty())
   {
      cerr << "could not load image";
      exit(1);
   }

   //imshow("111", src);
   namedWindow("control", 1);

   int hMax = 150, hMin = 80, sMax = 110, sMin = 40, vMax = 100, vMin = 0;

   cv::createTrackbar("hMax", "control", &hMax, 255);
   cv::createTrackbar("hMin", "control", &hMin, 255);
   cv::createTrackbar("sMax", "control", &sMax, 255);
   cv::createTrackbar("sMin", "control", &sMin, 255);
   cv::createTrackbar("vMax", "control", &vMax, 255);
   cv::createTrackbar("vMin", "control", &vMin, 255);

   
   Mat temp = imread("xuxieban.png", IMREAD_GRAYSCALE);
   if(temp.empty())
   {
      cerr << "could not load template image";
      exit(1);
   }
   vector<Mat> temp_contours;
   temp_contours.push_back(temp);

   vector<Rect> minRec;
   double pro = 1;
   //double min_pro = 999;
   double min_thresh = 3;
   vector<int> min_pro_loc;
   Mat maskr;
   Mat maskb;
   vector<Mat>src_split;
   Mat temp1 = src.clone();
   Mat temp2 = src.clone();
   cvtColor(src, temp1, COLOR_BGR2HSV);
   split(src, src_split);
   //imshow("b_channel", src_split[0]);
   //imshow("g_channel", src_split[1]);
   //imshow("r_channel", src_split[2]);
   threshold(src_split[2], maskr, 127, 1, THRESH_BINARY_INV);
   threshold(src_split[0], maskb, 127, 1, THRESH_BINARY_INV);

   cvtColor(maskr, maskr, COLOR_GRAY2BGR);
   cvtColor(maskb, maskb, COLOR_GRAY2BGR);
   //merge(masks, mask);
   multiply(temp2, maskr, temp2);
   multiply(temp2, maskb, temp2);
   //bitwise_and(temp2, mask, temp2);
   //temp2.copyTo(temp2, mask);
   imshow("temp2", temp2);
   cvtColor(src, temp2, COLOR_BGR2HSV);
   inRange(temp2, Scalar(hMin, sMin, vMin), Scalar(hMax, sMax, vMax), temp2);
   Mat kernel = getStructuringElement(MORPH_RECT, Size(9, 9));
   erode(temp2, temp2, kernel, Point(-1, -1), 3);
   dilate(temp2, temp2, kernel, Point(-1, -1), 3);
  // morphologyEx(temp2, temp2, MORPH_CLOSE, kernel);
   
   vector<vector<Point>> contours;
   findContours(temp2, contours, 0, 1);
   if(!contours.empty())
   {
      drawContours(temp1, contours, -1, Scalar(128, 128, 240), 2);
   }
   for(int i = 0; i < contours.size(); i++)
   {
      if(contours[i].size() < 500)
      {
         continue;
      }
      pro = matchShapes(contours[i], temp_contours[0], CONTOURS_MATCH_I1, 0);
      if(pro < min_thresh)
      {
         //min_pro = pro;
         min_pro_loc.push_back(i);
      }
   }
   //cout << min_pro;
   for(int i = 0; i < min_pro_loc.size(); i ++)
      {  
         minRec.push_back(boundingRect(contours[min_pro_loc[i]]));
         

         rectangle(src, minRec[i], Scalar(0, 255, 0), 2);
      }

   //putText(src, to_string(min_pro), Point(40, 40), 2, 2, Scalar(0, 0, 0));
   //imshow("222", temp1);
   imshow("111", src);
   imshow("333", temp2);
   waitKey(0);

   
   return 0;
}