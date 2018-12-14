#include "Transformer.hpp"
#include "Fifo.hpp"
#include <iostream>
#include <stdint.h>
#include <functional>
#include <thread>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class TransBW : public Transformer {  
public:
  
  //single sided
  TransBW(Fifo<std::shared_ptr<cv::Mat> > &inFifo,
         std::function<void(std::shared_ptr<cv::Mat>)> displayFunc,
         uint32_t frameCount);
  
  //double sided
  TransBW(Fifo<std::shared_ptr<cv::Mat> > &inFifo,
         Fifo<std::shared_ptr<cv::Mat> > &outFifo,
         std::function<void(std::shared_ptr<cv::Mat>)> displayFunc,
         uint32_t frameCount);
    
  //transform
  virtual std::shared_ptr<cv::Mat> transform(std::shared_ptr<const cv::Mat> pInFrame) override;
    
};