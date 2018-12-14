#include "mathObj.hpp"
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <unistd.h>
#include <iostream>
#include <semaphore.h>
#include <map>
#include <functional>
#include <errno.h>
#include <string.h>
#include <mqueue.h>
#include <string>
#include <sys/wait.h>

int main(){
  int head = 0;
  
  //unlink previous Semaphores
  sem_unlink(problemNotFullSem.c_str());
  sem_unlink(problemNotEmptySem.c_str());
  sem_unlink(answerNotFullSem.c_str());
  sem_unlink(answerNotEmptySem.c_str());
  
  //unlink previous shared memory
  shm_unlink(problemMem.c_str());
  shm_unlink(answerMem.c_str());
  
  //open and create Semaphores
  sem_t *pnf = sem_open(problemNotFullSem.c_str(), O_CREAT | O_RDWR, 0666, ARRAY_SIZE);
  sem_t *pne = sem_open(problemNotEmptySem.c_str(), O_CREAT | O_RDWR, 0666, 0);
  sem_t *anf = sem_open(answerNotFullSem.c_str(), O_CREAT | O_RDWR, 0666, ARRAY_SIZE);
  sem_t *ane = sem_open(answerNotEmptySem.c_str(), O_CREAT | O_RDWR, 0666, 0);

  //open and create Problem List shared memory
  int probList = shm_open(problemMem.c_str(), O_CREAT | O_RDWR, 0666);
  if(probList == -1)
  {
    std::cout << "probList Error: " << strerror(errno) << std::endl;
  }
  ftruncate(probList, (ARRAY_SIZE*sizeof(Problem)));
  
  //open problem Address mmap
  void *probAddr = mmap(NULL, (ARRAY_SIZE*sizeof(Problem)), PROT_READ | PROT_WRITE, MAP_SHARED, probList, 0);
  Problem* p2 = (Problem*) probAddr;    
		    
  //open and create Answer List shared memory
  int answList = shm_open(answerMem.c_str(), O_CREAT | O_RDWR, 0666);
  if(answList == -1)
  {
    std::cout << "answList Error: " << strerror(errno) << std::endl;
  }
  ftruncate(answList, (ARRAY_SIZE*sizeof(Answer)));
  
  //open answer Address mmap
  void *answAddr = mmap(NULL, (ARRAY_SIZE*sizeof(Answer)), PROT_READ | PROT_WRITE, MAP_SHARED, answList, 0);
  Answer* a2 = (Answer*) answAddr;
  
  std::cout << "Waiting..." << std::endl;
  
  //read from probList, write to answList
  while(1){
      sem_wait(pne);
      sem_wait(anf);
      //std::cout << "Problem received: " << p2[head].m_op1 << " " << p2[head].m_opcode << " " << p2[head].m_op2 << std::endl;
      switch(p2[head].m_opcode){
	case eADD:
	  a2[head].m_answer = p2[head].m_op1 + p2[head].m_op2;
	  break;
	case eSUB:
	  a2[head].m_answer = p2[head].m_op1 + p2[head].m_op2;
	  break;
	case eMUL:
	  a2[head].m_answer = p2[head].m_op1 + p2[head].m_op2;
	  break;
	case eDIV:
	  if(p2[head].m_op2 == 0){
	    a2[head].m_answer = 0;
	  }
	  else{
	    a2[head].m_answer = p2[head].m_op1 + p2[head].m_op2;
	  }
	  break;
      }
    //  std::cout << "Answer to send: " << a2[head].m_answer << std::endl;
      head = (head+1) % ARRAY_SIZE;
      sem_post(pnf);
      sem_post(ane);
  }
  return 0;
}

//g++ shmServer.cpp -pthread -lrt -o test1
