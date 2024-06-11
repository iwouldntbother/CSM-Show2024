//
// Created by williamwestwood on 20/05/24.
//

#include "align_and_crop.h"
#include <iostream>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include "ocr.h"
#include "get_results.h"
#include "GlobalData.h"

using namespace std;
using namespace cv;

vector<Point2f> get_corners(const vector<pair<vector<Point2f>, int>> &markers)
{
  map<int, Point2f> points;
  for (const auto &[fst, snd] : markers)
  {
    const int markerID = snd;
    const auto &corners = fst;
    switch (markerID)
    {
    case 1:
      points[1] = corners[0]; // top-left of marker 1
      break;
    case 2:
      points[2] = corners[1]; // top-right of marker 2
      break;
    case 3:
      points[3] = corners[3]; // bottom-right of marker 3
      break;
    case 4:
      points[4] = corners[2]; // bottom-left of marker 4
      break;
    default:
      break;
    }
  }
  if (points.size() != 4)
  {
    cerr << "[ERROR] Detected markers: ";
    for (const auto &p : points)
    {
      cerr << p.first << " ";
    }
    cerr << endl;
    throw runtime_error("All 4 markers (1, 2, 3, 4) must be provided");
  }
  return {points[1], points[2], points[3], points[4]};
}

vector<Point2f> order_points(const vector<Point2f> &pts)
{
  vector<Point2f> rect(4);
  vector<float> s(pts.size()), diff(pts.size());
  for (size_t i = 0; i < pts.size(); ++i)
  {
    s[i] = pts[i].x + pts[i].y;
    diff[i] = pts[i].x - pts[i].y;
  }
  rect[0] = pts[min_element(s.begin(), s.end()) - s.begin()];          // top-left
  rect[2] = pts[max_element(s.begin(), s.end()) - s.begin()];          // bottom-right
  rect[1] = pts[min_element(diff.begin(), diff.end()) - diff.begin()]; // top-right
  rect[3] = pts[max_element(diff.begin(), diff.end()) - diff.begin()]; // bottom-left
  return rect;
}

Mat rotate_image(const Mat &image, double angle)
{
  const Point2f center(image.cols / 2., image.rows / 2.);
  const Mat rot = getRotationMatrix2D(center, angle, 1.0);
  Mat rotated;
  warpAffine(image, rotated, rot, image.size());
  return rotated;
}

void align_and_crop(Mat &image, const Ptr<aruco::Dictionary>& arucoDict, const Ptr<aruco::DetectorParameters>& arucoParams, bool rotateImage)
{
  // Re-detect the markers in the high-resolution frame
  vector<int> ids;
  vector<vector<Point2f>> corners, rejected;
  aruco::detectMarkers(image, arucoDict, corners, ids, arucoParams);

  if (corners.empty())
  {
    cout << "[ERROR] No markers detected in high-resolution frame." << endl;
    imwrite("/tmp/img-no-detect.jpg", image);
    return;
  }

  GlobalData::getInstance()->setProgressText("Collecting form data...");
  GlobalData::getInstance()->setShowProgress(true);

  // Debug: Print detected marker IDs
  cout << "[INFO] Detected marker IDs: ";
  for (const int id : ids)
  {
    cout << id << " ";
  }
  cout << endl;

  vector<pair<vector<Point2f>, int>> combined;
  for (size_t i = 0; i < corners.size(); ++i)
  {
    combined.emplace_back(corners[i], ids[i]);
  }

  try
  {
    vector<Point2f> src_pts = get_corners(combined);
    vector<Point2f> ordered_pts = order_points(src_pts);

    double aspect_ratio = 210.0 / 297.0;
    int orig_height = image.rows, orig_width = image.cols;
    int new_width, new_height;
    if (orig_width / orig_height < aspect_ratio)
    {
      new_width = orig_width;
      new_height = static_cast<int>(orig_width / aspect_ratio);
    }
    else
    {
      new_height = orig_height;
      new_width = static_cast<int>(orig_height * aspect_ratio);
    }

    vector<Point2f> dst_pts = {Point2f(0, 0), Point2f(new_width, 0), Point2f(new_width, new_height), Point2f(0, new_height)};
    Mat M = getPerspectiveTransform(ordered_pts, dst_pts);
    Mat output_image;
    warpPerspective(image, output_image, M, Size(new_width, new_height));
    cv::Mat output_image_flipped;
    cv::flip(output_image, output_image_flipped, 1);
    cv::Mat final_image;
    if (rotateImage) {
      final_image = rotate_image(output_image_flipped, 180);
    } else {
      final_image = output_image_flipped;
    }
    get_circle_results(final_image);
  }
  catch (const runtime_error &e)
  {
    cerr << e.what() << endl;
  }
}
