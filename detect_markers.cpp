#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include "align_and_crop.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

#include "SharedData.h"
#include "DataContainer.h"

#include "GlobalData.h"

using namespace std;
using namespace cv;
using namespace std::chrono;

int getIndex(vector<int> v, int K)
{
  auto it = find(v.begin(), v.end(), K);

  // If element was found
  if (it != v.end())
  {

    // calculating the index
    // of K
    int index = it - v.begin();
    // cout << index << endl;
    return index;
  }
  else {
    // If the element is not
    // present in the vector
    // cout << "-1" << endl;
    return -1;
  }
}

void detect_markers()
{
  // SharedData sharedData;
  // auto container = sharedData.getDataContainer();

  // Define names of each possible ArUco tag OpenCV supports
  const Ptr<aruco::Dictionary> arucoDict = makePtr<aruco::Dictionary>(aruco::getPredefinedDictionary(aruco::DICT_7X7_100));
  const Ptr<aruco::DetectorParameters> arucoParams = makePtr<aruco::DetectorParameters>();

  // Initialize the video capture with lower resolution for display
  cout << "[INFO] Starting video stream..." << endl;
  VideoCapture cap(0, cv::CAP_V4L2); // Use V4L2 backend directly
  if (!cap.isOpened())
  {
    cerr << "[ERROR] Unable to open video capture" << endl;
    return;
  }
  cap.set(CAP_PROP_FRAME_WIDTH, 640);
  cap.set(CAP_PROP_FRAME_HEIGHT, 480);
  std::this_thread::sleep_for(chrono::seconds(2));

  // Get the current camera settings
  double exposure = cap.get(CAP_PROP_EXPOSURE);
  double brightness = cap.get(CAP_PROP_BRIGHTNESS);
  double gain = cap.get(CAP_PROP_GAIN);

  cout << "[INFO] Current camera settings: " << endl;
  cout << "Exposure: " << exposure << endl;
  cout << "Brightness: " << brightness << endl;
  cout << "Gain: " << gain << endl;

  vector<int> foundMarkers;
  auto all_time_start = steady_clock::time_point();

  int frame_skip = 0;
  const int skip_rate = 2; // Adjust this value to skip frames

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

    if (frame_skip < skip_rate)
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

        // Point start(5,5);
        // int size = 10;
        // int gap = 2;
        rectangle(low_res_frame, Point(5+(i*12), 5), Point(15 + (i * 12), 15), Scalar(255, 255, 255), -1);
        // rectangle(low_res_frame, Point(start.x+(i*(size+gap)), start.y), Point((start.x+size)+(i*(size+gap)), start.y+size), Scalar(255, 255, 255), -1);

        // Draw the bounding box of the ArUCo detection
        for (int j = 0; j < 4; j++)
        {
          line(low_res_frame, corners[i][j], corners[i][(j + 1) % 4], Scalar(255, 255, 255), 1);
        }

        // Compute and draw the center (x, y)-coordinates of the ArUco marker
        // Point2f center(0, 0);
        // for (const auto &pt : corners[i])
        // {
        //   center += pt;
        // }
        // center *= (1.0 / 4);
        // circle(low_res_frame, center, 4, Scalar(0, 0, 255), -1);

        // Draw the ArUco marker ID on the frame
        // putText(low_res_frame, to_string(ids[i]), corners[i][0] - Point2f(0, 15),
        //         FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 2);
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
          // Print the number of markers found in blue
          // putText(low_res_frame, "Found Markers: " + to_string(foundMarkers.size()), Point(10, 30),
          //         FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 255, 0), 2);

          // Switch to high resolution for capturing and apply the same settings
          cap.set(CAP_PROP_FRAME_WIDTH, 3264);
          cap.set(CAP_PROP_FRAME_HEIGHT, 2448);

          // Apply the copied camera settings to the high-resolution frame
          cap.set(CAP_PROP_EXPOSURE, exposure);
          cap.set(CAP_PROP_BRIGHTNESS, brightness);
          cap.set(CAP_PROP_GAIN, gain);

          // Print the camera settings after applying them
          // cout << "[INFO] Settings after resolution change:" << endl;
          // cout << "Exposure: " << cap.get(CAP_PROP_EXPOSURE) << endl;
          // cout << "Brightness: " << cap.get(CAP_PROP_BRIGHTNESS) << endl;
          // cout << "Gain: " << cap.get(CAP_PROP_GAIN) << endl;

          // Manually adjust the exposure to a lower value to prevent overexposure
          cap.set(CAP_PROP_EXPOSURE, -6); // Adjust this value as needed

          // Print the camera settings after manual adjustment
          // cout << "[INFO] Settings after manual adjustment:" << endl;
          // cout << "Exposure: " << cap.get(CAP_PROP_EXPOSURE) << endl;
          // cout << "Brightness: " << cap.get(CAP_PROP_BRIGHTNESS) << endl;
          // cout << "Gain: " << cap.get(CAP_PROP_GAIN) << endl;

          // Wait a moment for the settings to take effect
          std::this_thread::sleep_for(chrono::milliseconds(500));

          // Capture a high-resolution frame
          Mat high_res_frame;
          cap.read(high_res_frame);
          cap.set(CAP_PROP_FRAME_WIDTH, 640);
          cap.set(CAP_PROP_FRAME_HEIGHT, 480);

          if (!high_res_frame.empty())
          {
            // Send the high-resolution frame for cropping and alignment
            // cout << "[INFO] Marker 1, is at: " << corners[getIndex(ids, 1)] << endl;

            // Check if marker one is in the top right corner
            vector<Point2f> markerOneLocation = corners[getIndex(ids, 1)];
            bool rotate;
            if(markerOneLocation[0].x > low_res_frame.cols/2 && markerOneLocation[0].y < low_res_frame.rows/2) {
              rotate = true;
            } else {
              rotate = false;
            }
            // cout << "[INFO] Rotate page? " << rotate << endl;
            // Set the ready status to false
            GlobalData::getInstance()->setReadyStatus(false);
            align_and_crop(high_res_frame, arucoDict, arucoParams, rotate);
          }
          else
          {
            cerr << "[ERROR] Failed to capture high-resolution frame." << endl;
          }

          // Reset the timer
          all_time_start = steady_clock::time_point();
        }
        // If the duration is less than 2 seconds, print the number of markers found in green
        else
        {
          // rectangle(low_res_frame, Point(5,17), Point(51, 20), Scalar(255, 255, 255), -1);
          rectangle(low_res_frame, Point(5,17), Point(std::min(chrono::duration<double>(steady_clock::now() - all_time_start).count() * 25.5, 51.0), 20), Scalar(255, 255, 255), -1);
          // putText(low_res_frame, "Found Markers: " + to_string(foundMarkers.size()), Point(10, 30),
          //         FONT_HERSHEY_SIMPLEX, 0.6, Scalar(255, 0, 0), 2);
        }
      }
      // If the number of markers found is less than 3, reset the timer and print the number of markers found in red
      else
      {
        all_time_start = steady_clock::time_point();
        // putText(low_res_frame, "Found Markers: " + to_string(foundMarkers.size()), Point(10, 30),
        //         FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 0, 255), 2);
      }

      // putText(low_res_frame, to_string(duration_cast<seconds>(steady_clock::now() - all_time_start).count()),
      //         Point(10, 60), FONT_HERSHEY_SIMPLEX, 0.6, Scalar(255, 0, 255), 2);
    } else {
      GlobalData::getInstance()->setReadyStatus(true);
    }

    // container->setFrameData(low_res_frame);
    // bool* currentResults = container->getResults();
    // cout << "Results: ";
    // for (int i = 0; i < 40; i++)
    // {
    //   cout << currentResults[i] << " ";
    // }
    // cout << endl;

    GlobalData::getInstance()->setFrameData(low_res_frame);

    // cout << "Frame data set" << low_res_frame.size() << endl;
    // Show the output frame
    // imshow("Frame", low_res_frame);
    // char key = static_cast<char>(waitKey(1));
    // if (key == 'q')
    // {
      // break;
    // }
  }

  // Do a bit of cleanup
  destroyAllWindows();
  cap.release();

  // return 0;
}
