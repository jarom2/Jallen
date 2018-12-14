#include "Fifo.hpp"
#include "Transformer.hpp"
#include "TransN.hpp"
#include "TransBW.hpp"
#include "TransED.hpp"
#include <stdint.h>
#include <functional>
#include <thread>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>
#include <sstream>
#include <unistd.h>

std::mutex mtx1;

void callbackN(std::shared_ptr<cv::Mat> frame){
  std::cout << "callback 1" << std::endl;
  mtx1.lock();
  cv::namedWindow("FrameN",CV_WINDOW_AUTOSIZE);
  cv::imshow("FrameN", *frame);
  cv::waitKey(1);
  std::cout << "After imshow" << std::endl;
  mtx1.unlock();
}

void callbackBW(std::shared_ptr<cv::Mat> frame){
  mtx1.lock();
  std::cout << "callback 2" << std::endl;
  cv::namedWindow("FrameBW",CV_WINDOW_AUTOSIZE);
  cv::imshow("FrameBW", *frame);
  cv::waitKey(1);
  mtx1.unlock();
}

void callbackED(std::shared_ptr<cv::Mat> frame){
  mtx1.lock();
  std::cout << "callback 3" << std::endl;
  cv::namedWindow("FrameED",CV_WINDOW_AUTOSIZE);
  cv::imshow("FrameED", *frame);
  cv::waitKey(1);
  mtx1.unlock();
}

int main(int argc, char **argv){
  std::string vName = argv[1];
  int NumberOfFrames = atoi(argv[2]);
  std::function<void(std::shared_ptr<cv::Mat>)> cbN = callbackN;
  std::function<void(std::shared_ptr<cv::Mat>)> cbBW = callbackBW;
  std::function<void(std::shared_ptr<cv::Mat>)> cbED = callbackED;
  Fifo <std::shared_ptr<cv::Mat>>f1;
  Fifo <std::shared_ptr<cv::Mat>>f2;
  Fifo <std::shared_ptr<cv::Mat>>f3;
  TransN t1(f1, f2, cbN, NumberOfFrames);
  TransBW t2(f2, f3, cbBW, NumberOfFrames);
  TransED t3(f3, cbED, NumberOfFrames);
  cv::VideoCapture cap(vName);
  if(!cap.isOpened()) {  // check if we succeeded
	std::cout << "Open unsuccessful." << std::endl;
        return -1;
  }
  for(int i = 0; i < NumberOfFrames; i++){
    std::shared_ptr<cv::Mat> pframe(new cv::Mat());
    std::cout << "Frame Number: " << i << std::endl;
    cap >> *pframe;
    if (pframe->empty()) {
      std::cout << "Empty Frame" << std::endl;
      return -1;
    }
    f1.addItem(pframe);
    }
    t1.wait();
    t2.wait();
    t3.wait();
    std::cout << "Ended program" << std::endl;
    return 0;
}

//g++ -std=c++11 Transformer.cpp TransN.cpp TransBW.cpp TransED.cpp  main.cpp -lopencv_core -lopencv_highgui -lopencv_improc -pthread -o test

