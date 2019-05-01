/*
* TODO : camera calibration
* ref : https://docs.opencv.org/4.1.0/dc/dbb/tutorial_py_calibration.html
* ref : https://www.cds.caltech.edu/~murray/wiki/images/6/68/Assignment_1_MECS132A.pdf
* ref : https://stackoverflow.com/a/10346021
*/

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;

int main()
{
    VideoCapture live_camera(0); // let us take live feed from camera

    if(!live_camera.isOpened()) // check if we succeded in opening camera
    {
        std::cout << "error opening live camera" << std::endl;
        return -1;
    }

    namedWindow("livecamera",1); // initialize a named window to display live camera feed

    // create cv::Point3f vector to store 3D object points from all images
    std::vector<std::vector<Point3f>> objectPoints;

    // create cv::Point2f vector to store 2D image points from all images
    std::vector<std::vector<Point2f>> imagePoints;

    // display new live camera frame on every iteration of the never ending loop such that we see a video
    while(1)
    {
        Mat live_camera_frame; // create a Mat object to store single frame from live camera feed
        Mat gray_live_camera_frame; // Mat object to store the live camera frame converted to grayscale
        live_camera >> live_camera_frame; // get new frame from camera for this loop iteration
        
        cvtColor(live_camera_frame, gray_live_camera_frame, COLOR_BGR2GRAY);
        imshow("livecamera", gray_live_camera_frame); // show this live camera frame in our previously initialized named window

        // create a condition to break this never ending loop : if ESC (ASCII code is 27) key is pressed
        if(waitKey(30) == 27 )
        {
            destroyWindow("livecamera");    
            break;
        }
        else // if live stream does not break go on with remaining code
        {
            // some code
            null;
        }        
    }
    return 0;
}