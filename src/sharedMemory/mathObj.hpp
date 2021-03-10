#ifndef MATHOBJ_HPP
#define MATHOBJ_HPP
#include <string>
/**
*  @enum Operation
*
*  The purpose of this enumeration is to describe one of the
*  four elementary arithmetic operations that should be be
*  applied to the operands of the problem.
*/
enum Operation
{
eADD,
eSUB,
eMUL,
eDIV,
};
/**
*  @struct Problem
*
*  This class describes a problem that the server should execute.
*  It should accept the problem, execute the operation, and construct
*  and answer.
*/
struct Problem
{
Operation m_opcode;
float m_op1;
float m_op2;
};
/**
*  @struct Answer
*
*  This trivial class represents the solution to a problem object
*/
struct Answer
{
float m_answer;
};
/****************** MSG QUEUE ITEMS*************************/
static const std::string problemQueueName = "/problemQueue";
static const std::string answerQueueName = "/answerQueue";
/****************** SHARED MEM ITEMS*************************/
#define ARRAY_SIZE 1024
static const std::string problemMem = "/problems";
static const std::string answerMem = "/answers";
static const std::string problemNotFullSem = "/problemNotFull";
static const std::string problemNotEmptySem = "/problemNotEmpty";
static const std::string answerNotFullSem = "/answerNotFull";
static const std::string answerNotEmptySem = "/answerNotEmpty";
#endif /* MATHOBJ_HPP */
