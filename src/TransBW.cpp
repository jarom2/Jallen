#include "Transformer.hpp"
#include "TransBW.hpp"
#include "Fifo.hpp"
#include <iostream>
#include <stdint.h>
#include <functional>
#include <thread>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

  //single sided
  TransBW::TransBW(Fifo<std::shared_ptr<cv::Mat> > &inFifo,
         std::function<void(std::shared_ptr<cv::Mat>)> displayFunc,
         uint32_t frameCount) : Transformer(inFifo, displayFunc, frameCount){}
         
  //double sided
  TransBW::TransBW(Fifo<std::shared_ptr<cv::Mat> > &inFifo,
         Fifo<std::shared_ptr<cv::Mat> > &outFifo,
         std::function<void(std::shared_ptr<cv::Mat>)> displayFunc,
         uint32_t frameCount) : Transformer(inFifo, outFifo, displayFunc, frameCount){
	   std::cout << "BW constructor" << std::endl;
	}
         
  
  //Personally implemented BLACK AND WHITE
  std::shared_ptr<cv::Mat> TransBW::transform(std::shared_ptr<const cv::Mat> pInFrame){
    std::cout << "Declaring temp mat BW" << std::endl;
    std::shared_ptr<cv::Mat> temp(new cv::Mat(pInFrame->rows, pInFrame->cols, CV_8UC1));
    std::cout << "Transform BW" << std::endl;
    uint8_t total = 0;
    
    for(int x = 0; x < temp->rows; ++x){
      for(int y = 0; y < temp->cols; ++y){
	cv::Vec3b color = pInFrame->at<cv::Vec3b>(x,y);
	//std::cout << color[0] << color[1] << color [2] << std::endl;
	total = ((int)color[0] + (int)color[1] + (int)color[2]) / 3;
	//std::cout << total << std:: endl;
	temp->at<uint8_t>(x,y) = total;
      }
    }
    
    //cv::cvtColor(*pInFrame, *temp, CV_BGR2GRAY);
    std::cout << "After Transform BW" << std::endl;
    return temp;
    //return pInFrame;
  }
        

  //original version
/*  std::shared_ptr<cv::Mat> TransBW::transform(std::shared_ptr<const cv::Mat> pInFrame){
    std::cout << "Declaring temp mat BW" << std::endl;
    std::shared_ptr<cv::Mat> temp(new cv::Mat());
    std::cout << "Transform BW" << std::endl;
    pInFrame->copyTo(*temp);
    cv::cvtColor(*pInFrame, *temp, CV_BGR2GRAY);
    std::cout << "After Transform BW" << std::endl;
    return temp;
  
  }*/
  
