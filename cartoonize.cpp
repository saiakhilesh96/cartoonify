// 						AUM SRI SAI RAM

//compilation: g++ cartoonize.cpp -o cartoon `pkg-config --cflags --libs opencv`
//execution: ./cartoon

#include <cv.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include<iostream>
using namespace cv;
using namespace std;


void cartoonize(Mat frame,char mode)
{
  //cout<<mode<<" ";
  Mat edges;
  Mat median,laplacian,threshold,b1,input,img_eroded,img_color,img_eroded_blur,cartoon,img_small,img_output;
  Mat bilateral;
  Mat kernel= Mat::ones(3,3,CV_8U); //kernel mask
  double alpha= 0.5,beta,ds_factor= 4.0;
  beta= 1.0 - alpha;
  namedWindow("edges",WINDOW_NORMAL);
  //convert the actual image into grayscale image
  cv::cvtColor(frame,input,CV_BGR2GRAY);

  //apply median filter to remove the noise
  medianBlur(input,median,7); //on gray scale with a 7x7 mask

  //detect edges in the image and threshold it (use laplacian)
  Laplacian(median,laplacian,CV_8UC1,1,1.0,0.0,BORDER_DEFAULT);

  //threshold the Laplacian
  cv::threshold(laplacian,threshold,10,255,1);

  //erode the thresholded image with the kernel mask
  erode(threshold,img_eroded,kernel,Point(-1,-1),1,BORDER_CONSTANT,morphologyDefaultBorderValue());
    
  //apply median blur to the eroded image
  medianBlur(img_eroded,img_eroded_blur,7);
  //medianBlur(threshold,img_eroded_blur,7);

  //aply bilateral filter on the median blur image
  cv::bilateralFilter(img_eroded_blur,bilateral,15,80,80,BORDER_DEFAULT);
  //cv::bilateralFilter(img_eroded_blur,bilateral,15,80,80);

  //convert the bilateral filter image to color
  cv::cvtColor(bilateral,img_color,CV_GRAY2BGR); //this gives us the sketched image

  bitwise_and(frame,img_color,cartoon);

  if(mode == 's')
  {
    /*//erode the thresholded image with the kernel mask
    erode(threshold,img_eroded,kernel,Point(-1,-1),1,BORDER_CONSTANT,morphologyDefaultBorderValue());
    
    
    //apply median blur to the eroded image
    medianBlur(img_eroded,img_eroded_blur,7);
    //medianBlur(threshold,img_eroded_blur,7);

    //aply bilateral filter on the median blur image
    cv::bilateralFilter(img_eroded_blur,bilateral,15,50,50,BORDER_DEFAULT);
    //cv::bilateralFilter(img_eroded_blur,bilateral,15,80,80);

    //convert the bilateral filter image to color
    cv::cvtColor(bilateral,img_color,CV_GRAY2BGR); //this gives us the sketched image*/
    imshow("cartoon",bilateral);
    //add the weights of the actual image and colored sketched image
    //addWeighted( frame, alpha, img_color, beta, 10.0, cartoon);
    //bitwise_and(frame,img_color,cartoon);*/
  }
  else
  {
    if(mode == 'c')
    {
      //aply bilateral filter on the median blur image
      //cv::bilateralFilter(img_eroded_blur,bilateral,15,50,50,BORDER_DEFAULT);
      /*cv::bilateralFilter(threshold,bilateral,15,80,80);

      //convert the bilateral filter image to color
      cv::cvtColor(bilateral,img_color,CV_GRAY2BGR); //this gives us the sketched image

      //add the weights of the actual image and colored sketched image
      //addWeighted( frame, alpha, img_color, beta, 10.0, cartoon);*/
      //bitwise_and(frame,img_color,cartoon);
      imshow("cartoon",cartoon);
    }
  }

  //display output
  //imshow("cartoon", cartoon);
}



int main(int argc,char** argv)
{
  int key = 0, last = 0;
  char display_mode= 's';
  VideoCapture cap(0); // open the default camera
  if(!cap.isOpened()) // check if we camera is opened
  { 
      cout << "Cannot open selected camera" << endl;
      return -1;   
  }
  cout<<"Press 's' for sketch mode and 'c' for cartoon mode"<<endl;
  //namedWindow("Capture",1);
  Mat frame;
  for(;;) //Loop until user hit "esc"
  {
      //Mat frame;
      cap >> frame; // get a new frame from camera      

      switch(last)
      {
         case 's':
         { 
             display_mode= 's';
             break;
         }
        case 'c': //Binarization to generate Black/White image
        {
	  display_mode= 'c';
	  break;
        }
        default: //use to prevent ecxeption at program start or use case '0' to show original image
        {
	  display_mode= 'c';
          break;
        }
      }

      //imshow("Capture", convertedImage); //show converted image
      cartoonize(frame,display_mode);
      key = waitKey(1);
      if (key != -1)             
        last = key;  
      if(key == 27)
        break;            
    }
    return 0;							
}
