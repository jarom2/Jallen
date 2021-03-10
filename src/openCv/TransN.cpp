#include "Transformer.hpp"
#include "TransN.hpp"
#include "Fifo.hpp"
#include <iostream>
#include <stdint.h>
#include <functional>
#include <thread>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


  //single sided
  TransN::TransN(Fifo<std::shared_ptr<cv::Mat> > &inFifo,
         std::function<void(std::shared_ptr<cv::Mat>)> displayFunc,
         uint32_t frameCount) : Transformer(inFifo, displayFunc, frameCount){}

  //double sided
  TransN::TransN(Fifo<std::shared_ptr<cv::Mat> > &inFifo,
         Fifo<std::shared_ptr<cv::Mat> > &outFifo,
         std::function<void(std::shared_ptr<cv::Mat>)> displayFunc,
         uint32_t frameCount) : Transformer(inFifo, outFifo, displayFunc, frameCount){
	   std::cout << "Trans N constructor" << std::endl;
	}
    
  
  //transform
  std::shared_ptr<cv::Mat> TransN::transform(std::shared_ptr<const cv::Mat> pInFrame){
    std::shared_ptr<cv::Mat> temp(new cv::Mat());
    std::cout << "Transform N" << std::endl;
    pInFrame->copyTo(*temp);
    std::cout << "After Transform N" << std::endl;
    return temp;
  }
    
