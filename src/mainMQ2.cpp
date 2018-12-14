#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <time.h>
#include <sstream>
#include <errno.h>
#include <string.h>
#include "mathObj.hpp"

int main(int argc, char **argv){
  srand(time(0));
  unsigned int prio = 1;
  mq_attr attr;
  char buffer[8192];
  Problem* p2 = (Problem*) buffer;
  Answer a2;
  mq_unlink(problemQueueName.c_str());
  mq_unlink(answerQueueName.c_str());
  mqd_t ansList = mq_open(answerQueueName.c_str(), O_CREAT | O_RDWR, 0666, NULL);
  mqd_t proList = mq_open(problemQueueName.c_str(), O_CREAT | O_RDWR, 0666, NULL);
  if(ansList == -1){
    std::cout << "Error: " << strerror(errno) << std::endl;
  }
  std::cout << "Waiting..." << std::endl;
  
  //waiting for problems
  while(1){
    ssize_t retVal = mq_receive(proList, buffer, sizeof(buffer), NULL);
    if (retVal == -1) {
      std::cout << "Error: " << strerror(errno) << std::endl;
    }
    //std::cout << "Problem receieved: " << p2->m_op1 << " " << p2->m_opcode << " " << p2->m_op2 << std::endl;
    switch(p2->m_opcode){
      case eADD:
	a2.m_answer = p2->m_op1 + p2->m_op2;
	break;
      case eSUB:
	a2.m_answer = p2->m_op1 - p2->m_op2;
	break;
      case eMUL:
	a2.m_answer = p2->m_op1 * p2->m_op2;
	break;
      case eDIV:
	if(p2->m_op2 == 0){
	  a2.m_answer = 0;
	}
	else{
	  a2.m_answer = p2->m_op1 / p2->m_op2;
	}
	
    }
    //std::cout << "Answer to send: " << a2.m_answer << std:: endl;
    
    //send answer
    mq_send(ansList, (char*)&a2, sizeof(Answer), prio);  
  }
  return 0;
}