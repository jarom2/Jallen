#include "Transformer.hpp"
#include "Fifo.hpp"
#include <iostream>
#include <stdint.h>
#include <functional>
#include <thread>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

//For double ended transformers
Transformer::Transformer(Fifo<std::shared_ptr<cv::Mat> > &inFifo,
              Fifo<std::shared_ptr<cv::Mat> > &outFifo,
              std::function<void(std::shared_ptr<cv::Mat>)> displayFunc,
              uint32_t frameCount) : m_inFifo(inFifo), m_outFifo(outFifo), m_displayFunc(displayFunc), 
				     m_last(false) {
  
  m_pProcessThread = std::shared_ptr<std::thread> (new std::thread([this, frameCount]() {this->process(frameCount);}));
  std::cout << "double sided base constructor" << std::endl;
  }


//For single ended transformers
Transformer::Transformer(Fifo<std::shared_ptr<cv::Mat> > &inFifo,
              std::function<void(std::shared_ptr<cv::Mat>)> displayFunc,
              uint32_t frameCount) : m_inFifo(inFifo), m_outFifo(inFifo), m_displayFunc(displayFunc), m_last(true){
 
  m_pProcessThread = std::shared_ptr<std::thread> (new std::thread([this, frameCount]() {this->process(frameCount);}));
  std::cout << "single sided base constructor" << std::endl;

		
	      }

void Transformer::process(uint32_t frameCount){
  std::shared_ptr<cv::Mat> temp(new cv::Mat());
  for (int i = 0; i < frameCount; i++){
    std::cout << "In process" << std::endl;
    m_inFifo.popItem(temp);
    temp = transform(temp);
    if(!(m_last)){
    m_outFifo.addItem(temp);
    }
    m_displayFunc(temp);
  }
}
void Transformer::wait(){
  m_pProcessThread->join();
}
  
