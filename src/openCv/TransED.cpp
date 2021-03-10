#include "Transformer.hpp"
#include "TransED.hpp"
#include "Fifo.hpp"
#include <iostream>
#include <stdint.h>
#include <functional>
#include <thread>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


  //single sided
  TransED::TransED(Fifo<std::shared_ptr<cv::Mat> > &inFifo,
         std::function<void(std::shared_ptr<cv::Mat>)> displayFunc,
         uint32_t frameCount) : Transformer(inFifo, displayFunc, frameCount){
	   std::cout << "ED constructor" << std::endl;
	}
  
  //double sided
  TransED::TransED(Fifo<std::shared_ptr<cv::Mat> > &inFifo,
         Fifo<std::shared_ptr<cv::Mat> > &outFifo,
         std::function<void(std::shared_ptr<cv::Mat>)> displayFunc,
         uint32_t frameCount) : Transformer(inFifo, outFifo, displayFunc, frameCount){}
    
  //transform
   std::shared_ptr<cv::Mat> TransED::transform(std::shared_ptr<const cv::Mat> pInFrame){
    std::cout << "creating  temp ED mat" << std::endl;
    std::shared_ptr<cv::Mat> temp(new cv::Mat());
    std::cout << "Before ED" << std::endl;
    pInFrame->copyTo(*temp);
    cv::Canny(*temp, *temp, 50, 150, 3);
    std::cout << "After Transform ED" << std::endl;
    //pInFrame->convertTo(*temp, CV_8U);
    return temp;
  }
    
