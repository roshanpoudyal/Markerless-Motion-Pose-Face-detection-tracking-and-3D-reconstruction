/*  
 * TODO : Read video using opencv offline or online
 * Test application to get OpenCV build information and version.
 * ref : https://docs.opencv.org/4.1.0/d5/dc4/tutorial_video_input_psnr_ssim.html -> section : How to read a video stream (online-camera or offline-file)? - has important informations
 * ref : https://docs.opencv.org/3.0-beta/modules/videoio/doc/reading_and_writing_video.html -> has just a simple implementation from older version but can be used for newer version also
 */

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

int main()
{
    VideoCapture live_camera(0); // use this format to read live (online) camera device - usually the device id start from 0, if you have more camera it can +1 on the id
    // VideoCapture offline_video("some_video_filename.some_video_format"); // use this format if you want to read from a video file

    if(!live_camera.isOpened()) // check if we succeded in opening camera of video
    {
        std::cout << "error opening live camera stream or file" << std::endl;
        return -1;
    }

    namedWindow("live_camera",1); // create a named window to display live camera frame or video frames

    // run a loop continiously until last frame or until a key is pressed
    while(1){
        Mat live_camera_frame; // Create new mat object to store an image
        live_camera >> live_camera_frame; // get a new frame from camera
        imshow("live_camera", live_camera_frame); // show each new live camera frame in the named window we created before

        // if an escape key is pressed or video stream online (live) or offline is finished we break from the loop
        if(waitKey(30) >=0 || live_camera_frame.empty()) break;
        // if(waitKey(30) >=0 || offline_video_stream.empty()) break;
    }
    
    return 0;
}