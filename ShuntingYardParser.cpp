#include "ShuntingYardParser.h"

int OperatorPriority(const char* op) { 
	return (*op == '-' || *op == '+') ? 1 : 2; 
}
bool isLeftBracket(const char* token) { 
	return (*token == '(') ? 1 : 0; 
}
bool isRightBracket(const char* token) {
	return (*token == ')') ? 1 : 0; 
}
bool isOperator(const char* token) { 
	return (*token == '+' || *token == '-' || *token == '/' || *token == '*') ? 1 : 0; 
}
bool isNumber(const char* token) { 
	return (*token >= '0' && *token <= '9') ? 1 : 0; 
}

size_t NumbEnd(const string & str, size_t start_pos)
{
	while (str[start_pos] >= '0' && str[start_pos] <= '9' || str[start_pos] == '.')
		start_pos++;
	return start_pos;
}

float BinaryOperation(const char * op, float operand1, float operand2)
{
	if (*op == '+')
		return (operand1 + operand2);
	else if (*op == '-')
		return (operand1 - operand2);
	else if (*op == '*')
		return (operand1*operand2);
	else if (*op == '/')
		if (operand2 == 0.0)
			throw std::logic_error("zero divider");
		else
			return (operand1 / operand2);
	else
		throw std::logic_error("unknown operator");
}

float UnaryOperation(const char * op, float operand1)
{
	if (*op == '+')
		return operand1;
	else if (*op == '-')
		return (-1 * operand1);
	else if (*op == '*' || *op == '/')
		throw std::logic_error("unary operator not supported");
	else
		throw std::logic_error("unknown operator");
}

void OperatorManipulation(char op, queue<string>& output, stack<string>& operator_stack)
{
	string str_op{ op };
	if (operator_stack.empty())
	{
		operator_stack.push(str_op);
		return;
	}
	while (!operator_stack.empty() &&
		OperatorPriority(operator_stack.top().c_str()) >= OperatorPriority(&op) &&
		!isLeftBracket(operator_stack.top().c_str()))
	{
		output.push(operator_stack.top());
		operator_stack.pop();
	}
	operator_stack.push(str_op);
}

void BracketManipulation(queue<string>& output, stack<string>& operator_stack)
{
	char left_bracket = '(';
	while (!operator_stack.empty() && operator_stack.top().front() != left_bracket)
	{
		output.push(operator_stack.top());
		operator_stack.pop();
	}
	if (operator_stack.empty())
		throw std::logic_error("mismatched parantheses");
	operator_stack.pop();  //pop the left bracket from the stack
}

float EvaluateReversePolishExpression(queue<string>& postfix_expr)
{
	stack<float> answer;
	while (!postfix_expr.empty())
	{
		string token = postfix_expr.front();
		postfix_expr.pop();
		if (isOperator(token.c_str()))
		{
			if (answer.size() >= 2) //binary operation
			{
				float y = answer.top();
				answer.pop();
				float x = answer.top();
				answer.pop();
				answer.push(BinaryOperation(token.c_str(), x, y));
			}
			else //unary operator 
			{
				float x = answer.top();
				answer.pop();
				answer.push(UnaryOperation(token.c_str(), x));
			}
		}
		else
			answer.push(stof(token));
	}
	return answer.top();
}
