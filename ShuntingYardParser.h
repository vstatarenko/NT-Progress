/* Idea of the Shunting Yard to convert
the prefix math expression to postfix(Reverse Polish)
was taken from wiki :
https://en.wikipedia.org/wiki/Shunting-yard_algorithm
Here is the realization of the content
'The algorithms in detail content' that you can find
if follow by link.
*/

#pragma once
#include <iostream>
#include <stack>
#include <string>
#include <queue>
#include <stdexcept>
	
using namespace std;

/*It gets an operator and returns its priority number.
By default '+' and '-' has priority number = 1,
'*' and '/' has priority number = 2
*/
int OperatorPriority(const char* op);

bool isLeftBracket(const char* token);
bool isRightBracket(const char* token);
bool isOperator(const char* token);
bool isNumber(const char* token);

/*Returns the end position of a number
 in the string if we provide its start position. 
 E.g. string expr = "123+456"
 NumbEnd(expr, start_pos = 0) returns the end position
 of the numb '123'. Since we know the start and end positions
 we can pull down the whole numb
 */
size_t NumbEnd(const string& str, size_t start_pos); 

/*
For the given operator and two operands the function evaluates
and returns the answer of the giben operator op.
*/

float BinaryOperation(const char* op, float operand1, float operand2);

/*
Returns the result of unary operator and operand.
E.g. op = -1 operand1 = 5, then
function returns -5
*/
float UnaryOperation(const char* op, float operand1);

/*
while there is an operator at the top of the operator stack with
   greater than or equal to precedence:
       pop operators from the operator stack, onto the output queue.
push the read operator onto the operator stack.
*/
void OperatorManipulation(char op, queue<string>& output, stack<string> &operator_stack);

/*
while the operator at the top of the operator stack is not a left bracket: then
     pop operators from the operator stack onto the output queue.
pop the left bracket from the stack.
*/
void BracketManipulation(queue<string>& output, stack<string> &operator_stack);

/*
gets a reverse polish notation expression as argument and evaluate it.
returns the answer of the postfix expression. 
*/
float EvaluateReversePolishExpression(queue<string>& postfix_expr);
