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
#include <time.h>
#include <cstdlib>
#include <sstream>

int main(int argc, char **argv){
  srand(time(0));
  int phead = 0;
  int ahead = 0;
  
  std::string size = argv[1];
  int itter = atoi(size.c_str());
  
  //open Semaphores
  sem_t *pnf = sem_open(problemNotFullSem.c_str(), O_RDWR);
  sem_t *pne = sem_open(problemNotEmptySem.c_str(), O_RDWR);
  sem_t *anf = sem_open(answerNotFullSem.c_str(), O_RDWR);
  sem_t *ane = sem_open(answerNotEmptySem.c_str(), O_RDWR);
 
  //open Problem shared memory
  int probList = shm_open(problemMem.c_str(), O_RDWR, 0666);
  if(probList == -1)
  {
    std::cout << "probList Error: " << strerror(errno) << std::endl;
  }
  
  //open problem Address mmap
  void *probAddr = mmap(NULL, (ARRAY_SIZE*sizeof(Problem)), PROT_READ | PROT_WRITE, MAP_SHARED, probList, 0);
  Problem* p1 = (Problem*) probAddr;    
  
 //open Answer shared memory
 int answList = shm_open(answerMem.c_str(), O_RDWR, 0666);
  if(answList == -1)
  {
    std::cout << "answList Error: " << strerror(errno) << std::endl;
  }
  
   //open answer Address mmap
  void *answAddr = mmap(NULL, (ARRAY_SIZE*sizeof(Answer)), PROT_READ | PROT_WRITE, MAP_SHARED, answList, 0);
  Answer* a1 = (Answer*) answAddr;
  
  //sender and receiever
  for(int i = 0; i < (ARRAY_SIZE*itter); i++){
    
    //send problem
      sem_wait(pnf);
      int opcode = 0;
      switch(opcode = rand()%4){
	case 0: p1[phead].m_opcode = eADD;
		break;
	case 1: p1[phead].m_opcode = eSUB;
		break;
	case 2: p1[phead].m_opcode = eMUL;
		break;
	case 3: p1[phead].m_opcode = eDIV;
		break;
      }
      p1[phead].m_op1 = rand()%100;
      p1[phead].m_op2 = rand()%100;
     // std::cout << "Problem to send: " << p1[phead].m_op1 << " " << p1[phead].m_opcode << " " << p1[phead].m_op2 << std::endl;
      //std::cout << "problem number sent: " << phead+1 << std::endl;
      //std::cout << "head: " << phead << std::endl;
      phead = (phead+1) % ARRAY_SIZE;
      sem_post(pne);
      
      //receieve answers
      sem_wait(ane);
      //std::cout << "Answer received: " << a1[ahead].m_answer << std::endl;
      ahead = (ahead+1) % ARRAY_SIZE;
      sem_post(anf);

  }
	
  return 0;
}
