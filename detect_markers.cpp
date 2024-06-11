#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include "align_and_crop.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

#include "DataContainer.h"

#include "GlobalData.h"

using namespace std;
using namespace cv;
using namespace std::chrono;

int getIndex(vector<int> v, const int K)
{
  // If element was found
  if (const auto it = find(v.begin(), v.end(), K); it != v.end())
  {
    const int index = it - v.begin();
    // cout << index << endl;
    return index;
  }
  else {
    return -1;
  }
}

void detect_markers()
{
  // Define names of each possible ArUco tag OpenCV supports
  const Ptr<aruco::Dictionary> arucoDict = makePtr<aruco::Dictionary>(aruco::getPredefinedDictionary(aruco::DICT_7X7_100));
  const Ptr<aruco::DetectorParameters> arucoParams = makePtr<aruco::DetectorParameters>();

  // Initialize the video capture with lower resolution for display
  cout << "[INFO] Starting video stream..." << endl;
  int cameraIdx = GlobalData::getInstance()->GlobalData::getFormCameraIdx();
  cout << "[INFO] Using index: " << cameraIdx << " For form camera" << endl;
  VideoCapture cap(cameraIdx, cv::CAP_V4L2); // Use V4L2 backend directly
  if (!cap.isOpened())
  {
    cerr << "[ERROR] Unable to open video capture" << endl;
    return;
  }
  cap.set(CAP_PROP_FRAME_WIDTH, 640);
  cap.set(CAP_PROP_FRAME_HEIGHT, 480);
  std::this_thread::sleep_for(chrono::seconds(1));

  cap.set(CAP_PROP_AUTO_EXPOSURE, 3);

  // Get the current camera settings
  double exposure = cap.get(CAP_PROP_EXPOSURE);
  double brightness = cap.get(CAP_PROP_BRIGHTNESS);
  double gain = cap.get(CAP_PROP_GAMMA);

  cout << "[INFO] Current camera settings: " << endl;
  cout << "Exposure: " << exposure << endl;
  cout << "Brightness: " << brightness << endl;
  cout << "Gamma: " << gain << endl;

  vector<int> foundMarkers;
  auto all_time_start = steady_clock::time_point();

  int frame_skip = 0;

  // Loop over the frames from the video stream
  while (true)
  {
    Mat low_res_frame;
    cap.read(low_res_frame);
    if (low_res_frame.empty())
    {
      cerr << "[ERROR] Failed to capture frame." << endl;
      break;
    }

    if (frame_skip < 2)
    {
      frame_skip++;
      continue;
    }
    frame_skip = 0;

    // Detect ArUco markers in the low-resolution frame
    vector<int> ids;
    vector<vector<Point2f>> corners, rejected;
    aruco::detectMarkers(low_res_frame, arucoDict, corners, ids, arucoParams);

    // Verify *at least* one ArUco marker was detected
    if (!corners.empty())
    {
      foundMarkers.clear();

      for (int i = 0; i < corners.size(); ++i)
      {
        foundMarkers.push_back(ids[i]);

        rectangle(low_res_frame, Point(5+(i*12), 5), Point(15 + (i * 12), 15), Scalar(255, 255, 255), -1);

        // Draw the bounding box of the ArUCo detection
        for (int j = 0; j < 4; j++)
        {
          line(low_res_frame, corners[i][j], corners[i][(j + 1) % 4], Scalar(255, 255, 255), 1);
        }
      }

      // Check if the number of markers found is greater than 3
      if (foundMarkers.size() > 3 && GlobalData::getInstance()->getReadyStatus())
      {
        // Check if the timer has been started
        if (all_time_start.time_since_epoch().count() == 0)
        {
          // Start the timer
          all_time_start = steady_clock::now();
        }
        // Check if the duration is greater than 2 seconds
        else if (duration_cast<seconds>(steady_clock::now() - all_time_start).count() > 2)
        {

          // Switch to high resolution for capturing and apply the same settings
          cap.set(CAP_PROP_AUTO_EXPOSURE, 1);
          cap.set(CAP_PROP_EXPOSURE, 50);
          cap.set(CAP_PROP_FRAME_WIDTH, 2272);
          cap.set(CAP_PROP_FRAME_HEIGHT, 1704);
          std::this_thread::sleep_for(chrono::seconds(1));


	        // Waste some frames so the camera can adjust
	        Mat waste_frame;
	        cap >> waste_frame;
	        cap >> waste_frame;
	        cap >> waste_frame;
	        waste_frame.release();


          // Print the camera settings after manual adjustment
           cout << "[INFO] Settings after manual adjustment:" << endl;
           cout << "Exposure: " << cap.get(CAP_PROP_EXPOSURE) << endl;
           cout << "Brightness: " << cap.get(CAP_PROP_BRIGHTNESS) << endl;
           cout << "Gamma: " << cap.get(CAP_PROP_GAMMA) << endl;

          // Wait a moment for the settings to take effect
          //std::this_thread::sleep_for(chrono::seconds(2));

          // Capture a high-resolution frame
          Mat high_res_frame;
          cap >> high_res_frame;
	        std::this_thread::sleep_for(chrono::seconds(1));
          //cap.set(CAP_PROP_FRAME_WIDTH, 640);
          //cap.set(CAP_PROP_FRAME_HEIGHT, 480);

          cap.set(CAP_PROP_AUTO_EXPOSURE, 3);
	  //cap.set(CAP_PROP_BRIGHTNESS, 0);

          if (!high_res_frame.empty())
          {
            // Send the high-resolution frame for cropping and alignment
            // cout << "[INFO] Marker 1, is at: " << corners[getIndex(ids, 1)] << endl;

	    cv::imwrite("/tmp/pre-align-image.jpg", high_res_frame);

            // Check if marker one is in the top right corner
            vector<Point2f> markerOneLocation = corners[getIndex(ids, 1)];
            bool rotate;
            if(markerOneLocation[0].x > low_res_frame.cols/2. && markerOneLocation[0].y < low_res_frame.rows/2.) {
              rotate = true;
            } else {
              rotate = false;
            }
            cout << "[INFO] Rotate page? " << rotate << endl;
            // Set the ready status to false
            GlobalData::getInstance()->setReadyStatus(false);
            align_and_crop(high_res_frame, arucoDict, arucoParams, rotate);
          }
          else
          {
            cout << "[ERROR] Failed to capture high-resolution frame." << endl;
          }


          cap.set(CAP_PROP_FRAME_WIDTH, 640);
          cap.set(CAP_PROP_FRAME_HEIGHT, 480);

          // Reset the timer
          all_time_start = steady_clock::time_point();
        }
        // If the duration is less than 2 seconds, print the number of markers found in green
        else
        {
          // rectangle(low_res_frame, Point(5,17), Point(51, 20), Scalar(255, 255, 255), -1);
          rectangle(low_res_frame, Point(5,17), Point(std::min(chrono::duration<double>(steady_clock::now() - all_time_start).count() * 25.5, 51.0), 20), Scalar(255, 255, 255), -1);
        }
      }
      else
      {
        all_time_start = steady_clock::time_point();
      }

    } else {
      GlobalData::getInstance()->setReadyStatus(true);
    }

    GlobalData::getInstance()->setFrameData(low_res_frame);
  }
}
