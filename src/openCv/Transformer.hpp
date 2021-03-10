#ifndef TRANSFORMER_HPP
#define TRANSFORMER_HPP

//Stuff we wrote
#include "Fifo.hpp"

//library stuff
#include <stdint.h>
#include <functional>
#include <thread>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

/**
 * @class Transformer
 *
 * This class is an abstract base class that handles a number of frames.  
 * Using a pipeline of classes derived from this abstract base class 
 * allows the coder to start with an arbitrary video, modify it
 * frame by frame, and then display the modified frame at each step
 * in the pipeline.
 *
 * 1. Spawns a thread which:
 *   a. Reads from the input fifo
 *   b. Transforms the input image
 *   c. Passes the transformed image to the next pipeline stage
 *   d. displays the image
 *
 * 2. Has a callback to "display" the image
 *
 * 3. Proveds a wait method to block until all frames are handled
 */
class Transformer
{
public:
  /**
   * @brief Constructor
   *
   * The constructor accepts and stores references to the input and output
   * frame fifos.  
   *
   * @param inFifo Input frame FIFO
   * @param outFifo Output frame FIFO
   * @param displayFunc Callback to display the modified frame
   * @param frameCount How many frames the thread should process before
   *        exiting
   */
  Transformer(Fifo<std::shared_ptr<cv::Mat> > &inFifo,
              Fifo<std::shared_ptr<cv::Mat> > &outFifo,
              std::function<void(std::shared_ptr<cv::Mat>)> displayFunc,
              uint32_t frameCount);
  /**
   * @brief Constructor
   *
   * This constructor is the same as the first, but it is used in the case
   * that this trasformer is the last in the pipeline, (i.e. it has no
   * output fifo)
   *
   * @param inFifo Input frame FIFO
   * @param displayFunc Callback to display the modified frame
   * @param frameCount How many frames the thread should process before
   *        exiting
   */
  Transformer(Fifo<std::shared_ptr<cv::Mat> > &inFifo,
              std::function<void(std::shared_ptr<cv::Mat>)> displayFunc,
              uint32_t frameCount);
  /**
   * @brief Wait for thread
   * 
   * This function should wait for the thread; join it.  It
   * will be called from the main thread to block until this transformer
   * processes the correct number of frames
   */
  void wait();
private: 
  /**
   * @brief Process frames
   *
   * This function will process a number of frames by:
   *   1. Reading a frame from the input
   *   2. Transforming the frame
   *   3. Writing the frame to the next pipeline stage (if not last stage)
   *   4. Display the transformed frame by calling the display callback
   *
   *  @Note: This is the function that needs to run as a thread; I would
   *         suggest spawning it in the constructor of this object
   *
   *  @param frameCount How many frames to process before exiting
   */
  void process(uint32_t frameCount);
  /**
   * @brief Transform the image/frame
   *
   * This is an abstract function that will transform the image in some
   * arbitrary way.  Must be implemented by all derived classes
   *
   * @param pInFrame Shared pointer to input frame
   * @return Shared pointer to modified frame
   */
  virtual std::shared_ptr<cv::Mat> transform(std::shared_ptr<const cv::Mat> pInFrame) = 0;
  /** @brief Keep reference to input FIFO */
  Fifo<std::shared_ptr<cv::Mat> > &m_inFifo;
  /** @brief Keep reference to output FIFO */
  Fifo<std::shared_ptr<cv::Mat> > &m_outFifo;
  /** @brief Track the dispay callback */
  std::function<void(std::shared_ptr<cv::Mat>)> m_displayFunc;
  /** @brief Need a thread to run the process */
  std::shared_ptr<std::thread> m_pProcessThread;
  /** @brief Track if this is the last stage in pipeline */
  bool m_last;
};

#endif /* TRANSFORMER_HPP */
