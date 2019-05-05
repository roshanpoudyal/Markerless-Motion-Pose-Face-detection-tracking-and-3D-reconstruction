/*
    * TODO : camera calibration
    * ref 1: https://docs.opencv.org/4.1.0/dc/dbb/tutorial_py_calibration.html
    * ref 2: https://www.cds.caltech.edu/~murray/wiki/images/6/68/Assignment_1_MECS132A.pdf
    * ref 3: https://stackoverflow.com/a/10346021
    * ref 4: https://docs.opencv.org/4.1.0/d9/d0c/group__calib3d.html#ga93efa9b0aa890de240ca32b11253dd4a : sample usage of detecting and drawing chessboard corners section in this link
    * ref 5: https://docs.opencv.org/4.1.0/dd/d1a/group__imgproc__feature.html#ga354e0d7c86d0d9da75de9b9701a9a87e : for cornerSubPix() function definition
    * ref 6: https://docs.opencv.org/3.4/da/d54/group__imgproc__transform.html#ga47a974309e9102f5f08231edc7e7529d : resize() api for image in opencv
    * ref 7: https://docs.opencv.org/4.1.0/d9/d0c/group__calib3d.html#ga3207604e4b1a1758aa66acb6ed5aa65d : calibrateCamera() api and its parameters description
*/

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>

using namespace cv; // namespace used for opencv

int main()
{
    VideoCapture live_camera(0); // let us take live feed from camera

    if(!live_camera.isOpened()) // check if we succeded in opening camera
    {
        std::cout << "error opening live camera" << std::endl;
        return -1;
    }

    namedWindow("livecamera", WINDOW_NORMAL); // initialize a named window to display live camera feed
    resizeWindow("livecamera", 426, 240); // resize the window to smaller size
    namedWindow("drawcorners", WINDOW_NORMAL); // initialze a named window to display chessboard with corners drawn on it
    resizeWindow("drawcorners", 426, 240); // resize the window to smaller size

    // create cv::Point3f vector to store 3D object points from all images
    // to learn what are objectPoints, see calibrateCamera() api documentation and its parameter description in ref 7 above
    std::vector<std::vector<Point3f>> objectPoints;

    // create cv::Point2f vector to store 2D image points from all images
    // to learn what are imagePoints, see calibrateCamera() api documentation and its parameter description in ref 7 above
    std::vector<std::vector<Point2f>> imagePoints;

    // create Size object instance that will store the pattern size of our input chessboard image
    // pattern size is the number of corners in a row by number of corners in a column
    Size chessboard_pattern_size(7,7);

    // size of the image we use for camera calibration and for initialization of intrinsic camera parameters
    // we shall use the dimension WIDTH=426 && HEIGHT=240
    Size imageSize(426,240);

    // display new live camera frame on every iteration of the never ending loop such that we see a video
    while(1)
    {
        Mat original_live_camera_frame; // create a Mat object to store single frame from live camera feed
        Mat live_camera_frame; // resized live camera frame
        Mat gray_live_camera_frame; // Mat object to store the live camera frame converted to grayscale

        live_camera >> original_live_camera_frame; // get new frame from our initialized videocapture live_camera for this loop iteration
        
        // resize the live feed to 426 width and 240 height image (which we have already initialized as imageSize above) , 
        // we do not need other optional parameters of this resize api ,
        // resizing the image makes the processing faster
        resize(original_live_camera_frame, live_camera_frame, imageSize); 

        cvtColor(live_camera_frame, gray_live_camera_frame, COLOR_BGR2GRAY); // convert the color of live camera frames to grayscale

        imshow("livecamera", gray_live_camera_frame); // show this live camera frame in our previously initialized named window

        // create vector of opencv type Point2f to store detected corners it also will store refined corners from cornerSubPix() api
        std::vector<Point2f> chessboard_corners;

        // now let us find chess board corners and return the status if found as well
        bool chessboard_pattern_found = findChessboardCorners(gray_live_camera_frame, 
                                                    chessboard_pattern_size,
                                                    chessboard_corners,
                                                    CALIB_CB_ADAPTIVE_THRESH
                                                    + CALIB_CB_NORMALIZE_IMAGE
                                                    + CALIB_CB_FAST_CHECK);

        // create a condition to break this never ending loop : if ESC (ASCII code is 27) key is pressed
        if(waitKey(30) == 27 ) 
        {
            destroyAllWindows();   
            break; // break the while loop
        }
        else if(chessboard_pattern_found) // if chessboard pattern is found do these computations in this scope 
        {
            std::cout << "chessboard pattern found" << std::endl;

            // the detected corners with findChessboardCorners() function is approximate
            // cornerSubPix() function iterates to find the sub-pixel accurate location of corners
            // and refines corner locations for more accuracy
            cornerSubPix(gray_live_camera_frame, chessboard_corners, Size(11,11), Size(-1,-1),
                    TermCriteria(TermCriteria::EPS + TermCriteria::MAX_ITER, 30, 0.1));
            
            // now draw chess board corners on every frame a pattern is found but on not grayscale but colorful video frame
            // so that the drawing is seen colorful
            drawChessboardCorners(live_camera_frame, chessboard_pattern_size, chessboard_corners, chessboard_pattern_found);

            imshow("drawcorners", live_camera_frame);
            
        }        
    }
    return 0;
}