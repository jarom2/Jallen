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
#include <sys/wait.h>
#include "mathObj.hpp"

int main(int argc, char **argv)
{
  srand(time(0));
  unsigned int prio = 1;
  mq_attr attr;
  char buffer[8192];
  Problem* p1 = (Problem*) buffer;
  Answer* a1 = (Answer*) buffer;
  std::string size = argv[1];
  std::cout << size << std::endl;
  int arraySize = atoi(size.c_str());
  arraySize *= 1024;
  //std::cout << "Number of problems to send: " << arraySize << std::endl;
  mqd_t list = mq_open(problemQueueName.c_str(), O_RDWR, 0666, NULL);
  if (list == -1) {
    std::cout << "Error: " << strerror(errno) << std::endl;
  }
  mqd_t ansList = mq_open(answerQueueName.c_str(), O_RDWR, 0666, NULL);
  if (ansList == -1) {
    std::cout << "Error: " << strerror(errno) << std::endl;
  }
  int opcode = 0;
  int pid = fork();
  if(pid == 0)
  {
    //sender
    for(int i = 0; i < arraySize; i++){
      switch(opcode = rand()%4){
	case 0: p1->m_opcode = eADD; 
		break;
	case 1: p1->m_opcode = eSUB;
		break;
	case 2: p1->m_opcode = eMUL;
		break;
	case 3: p1->m_opcode = eDIV;
      }
      p1->m_op1 = rand()%100;
      p1->m_op2 = rand()%100;
      //std::cout << "Problem to send: " << p1->m_op1 << " " <<  p1->m_opcode  << " " << p1->m_op2 << std::endl;
      int val = mq_send(list, buffer, sizeof(Problem), 1);
      if (val == -1) {
	std::cout << "val Error: " << strerror(errno) << std::endl;
      }
    }
  }
  else
  {
    //receiver
    
 
    for(int i = 0; i <arraySize; i++){
      int retVal = mq_receive(ansList, buffer, sizeof(buffer), &prio);
	if (retVal == -1) {
	  std::cout << "retVal Error: " << strerror(errno) << std::endl;
	}
	//std::cout << "Answer received: " << a1->m_answer << std::endl;
    }
    int status;
    waitpid(pid, &status, 0);
  }
  return 0;
}
