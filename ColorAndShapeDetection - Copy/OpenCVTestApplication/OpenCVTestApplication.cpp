// OpenCVTestApplication.cpp : This file contains the 'main' function. Program execution begins and ends there.

//
//
//THIS IS THE JUNIOR PROJECT CODE
//
//


//color detection
//RGB was tried for thresholding instead of HSV, however it pings black/dark gray as well as the wanted color, so thresholding was switched to HSV
    //thresholding was later scrapped but some parts of it still exist in commented out code
    //this commented out code was left in to show some of the build process

#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    //video input from webcam (0) or external camera (1)
    VideoCapture cap(0);

    //if camera can't be opened, close program and return error
    if (!cap.isOpened())
    {
        cout << "Cannot open the linked camera" << endl;
            return -1;
    }

    //radius value window with sliders
    namedWindow("Radius Size Adjusters", WINDOW_AUTOSIZE); //creates a window titled radius slide adjusters

    int iOuterCircleLowRadius;
    int iOuterCircleHighRadius;

    int iInnerCircleRadius;

    createTrackbar("Lower Outer", "Radius Size Adjusters", &iOuterCircleLowRadius, 200);
    createTrackbar("Upper Outer", "Radius Size Adjusters", &iOuterCircleHighRadius, 200);

    createTrackbar("Inner Radius", "Radius Size Adjusters", &iInnerCircleRadius, 200);

    //boundary checking cause you can't have negative pixel radius
    if ((iInnerCircleRadius <= 0) || (iOuterCircleLowRadius <= 0) || (iOuterCircleHighRadius <= 0)) {
        iInnerCircleRadius = 1;
        iOuterCircleLowRadius = 1;
        iOuterCircleHighRadius = 1;
    }

 

    //this originally created the sliders for the thresholding, however once thresholding got scrapped they were commented out
    // they were left in both as references for when I made the other sliders, and to show the build process
    //slider parameters
   /* int iLowHue = 0; //0
    int iHighHue = 179; //179

    int iLowSaturation = 0; //0
    int iHighSaturation = 255; //255
    
    int iLowValue = 0; //0
    int iHighValue = 255; //255

    //trackbars within control window
    createTrackbar("Low Hue", "Control", &iLowHue, 179); //Hue (0 - 179)
    createTrackbar("High Hue", "Control", &iHighHue, 179);

    createTrackbar("Low Saturation", "Control", &iLowSaturation, 255); //Saturation (0 - 255)
    createTrackbar("High Saturation", "Control", &iHighSaturation, 255);

    createTrackbar("Low Value", "Control", &iLowValue, 255); //Value (0 - 255)
    createTrackbar("High Value", "Control", &iHighValue, 255);

    */

    //intake frames from webcam stream and analyze them
    while (true)
    {

        Mat imgOriginal;

        // grab frame from video
        bool bSuccess = cap.read(imgOriginal);
        
        //break loop if unsuccessful
        if (!bSuccess)
        {
            cout << "Cannot read a frame from video stream" << endl;
            break;
        }

        Mat imgGray;

        cvtColor(imgOriginal, imgGray, COLOR_BGR2GRAY);

        //removes noise from the image
        medianBlur(imgGray, imgGray, 5);

        //apply Hough Circle transform
        //3 element float array (vector), for circle coordinates
        vector <Vec3f> circles;
        HoughCircles(imgGray, circles, HOUGH_GRADIENT, 1,
            //change this value to detect circles with different distances to each other
            imgGray.rows / 1,
            //change the last two parameters
            100, 20, iOuterCircleLowRadius, iOuterCircleHighRadius
            //(min_radius & max_radius) to detect larger circles
        );

        //drawing the detected circles
        for (size_t i = 0; i < circles.size(); i++)
        {
            Vec3i c = circles[i];
            Point centerPoint = Point(c[0], c[1]);
            //center point circle
            //superimpose the circle on both images
            circle(imgOriginal, centerPoint,iInnerCircleRadius, Scalar(0, 255, 0), 3, LINE_AA);
            circle(imgGray, centerPoint, iInnerCircleRadius, Scalar(255, 0, 255), 3, LINE_AA);

            //outline of circle
            int radius = c[2];
            circle(imgOriginal, centerPoint, radius, Scalar(0, 255, 0), 3, LINE_AA);
            circle(imgGray, centerPoint, radius, Scalar(255, 0, 255), 3, LINE_AA);

        }
       
       
        //show the thresholded image
        imshow("Computer Sees", imgGray);

        //show the original image
        imshow("Original", imgOriginal); 
        
        
        //wait for 'esc' key press for 30ms. Break loop if 'esc' key is pressed
        if (waitKey(30) == 27) 
        {
            cout << "esc key is pressed by user" << endl;
            break;
        }
    }

    return 0;
}