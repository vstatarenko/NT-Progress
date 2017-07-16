#include "ShuntingYardParser.h"
#include <string.h>
#include <algorithm>

//replaces all occurences of comma with dot
void ReplaceComma2Dot(std::string& str);

//removes all tabs and spaces from string
void EraseTabsSpaces(std::string& str);

//checks if the number of open brackets 
//equal to the number of closed ones
bool isSameBracketsNumb(const string& line);

//returns forbidden expression in the string
string searchForbiddenSymb(const string& line);

int main()
{
	string user_input;
	try
	{
		do
		{
			stack<string> stack_operator;
			queue<string> output_queue;
			cout << "input the expression or press 'q' to quit:\n";
			getline(cin, user_input);
			/*
			to start convert prefix expression to postfix
			we first have to make pre-parsing
			1. check if the number of open and close brackets are equal
			2. delete all needless spaces and tabs
			3. replace all commas to dots
			4. check if the string contains only digits, brackets and operators
			*/
			if (!isSameBracketsNumb(user_input)) 
				throw std::logic_error("mismatched parantheses\n");
			ReplaceComma2Dot(user_input);
			EraseTabsSpaces(user_input);
			string forbidden_symbols = searchForbiddenSymb(user_input);
			if (!forbidden_symbols.empty()) 
				throw std::logic_error("invalid expression\n");

			//Starting Shunting Yard Parsing procedure(convertateion to postfix)
			string::iterator token = user_input.begin();
			while (token != user_input.end())
			{
				if (isNumber(&(*token)))
				{
					//start and end position of the number in the string
					size_t start_pos = std::distance(user_input.begin(), token);
					size_t end_pos = NumbEnd(user_input, start_pos);

					//get whole number from the string
					string numb = user_input.substr(start_pos, end_pos - start_pos);
					output_queue.push(numb);
					if (end_pos >= user_input.size())
					{
						token = user_input.end();
						continue;
					}
					token += (end_pos - start_pos); //set iterator to the new position

				}
				else if (isOperator(&(*token)))
				{
					OperatorManipulation(*token, output_queue, stack_operator);
					token++;
				}
				else if (isLeftBracket(&(*token)))
				{
					string left_bracket{ *token };
					stack_operator.push(left_bracket);
					token++;
				}
				else if (isRightBracket(&(*token)))
				{
					BracketManipulation(output_queue, stack_operator);
					token++;
				}
				else if (*token == 'q' && user_input.size() == 1)
					break;
				else
					throw std::logic_error("unknown token\n");
			}
			if (output_queue.empty() && !stack_operator.empty())
				throw std::logic_error("No operand\n");
			/*
			there are no more tokens to read:
			while there are still operator tokens on the stack :
			     pop the operator onto the output queue.
		    */
			while (!stack_operator.empty())
			{
				output_queue.push(stack_operator.top());
				stack_operator.pop();
			}
			//Now we have the expression in the Reverse Polish notation(RPN)
			//To get the answer we just need to evaluate it
			if (!output_queue.empty())
			{
				float answ = EvaluateReversePolishExpression(output_queue);
				cout << "answer is: " << answ << endl;
			}
		} while (user_input != "q");
	}
	catch (std::logic_error const& err) {
		cout << err.what();
	}
	return 0;
}

void ReplaceComma2Dot(string& line)
{
	const string comma(",");
	const string dot(".");
	size_t comma_pos = 0;
	comma_pos = line.find(comma, comma_pos);
	while (comma_pos != string::npos)
	{
		line.replace(comma_pos, comma.size(), dot);
		comma_pos = line.find(comma, comma_pos);
	}
}

void EraseTabsSpaces(std::string & str)
{
	str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
	str.erase(std::remove(str.begin(), str.end(), '\t'), str.end());
}

bool isSameBracketsNumb(const string& line)
{
	short int count_open_brackets = 0;
	short int count_close_brackets = 0;

	string::const_iterator runner = line.begin();
	while (runner != line.end())
	{
		if (*runner == '(')
			count_open_brackets++;
		else if (*runner == ')')
			count_close_brackets++;
		else
		{
			runner++;
			continue;
		}
		runner++;
	}
	if (count_open_brackets == count_close_brackets)
		return true;
	return false;
}

string searchForbiddenSymb(const string& line)
{
	string res;
	const char correct_symbols[] = "+-*/()q.";
	string::const_iterator runner = line.begin();
	while (runner != line.end())
	{
		bool acceptable_symbol = 0;
		if ((*runner >= '0') && (*runner <= '9'))
			acceptable_symbol = 1;
		else
		{
			for (unsigned int i = 0; i < strlen(correct_symbols); i++)
				if (*runner == correct_symbols[i])
					acceptable_symbol = 1;
		}
		if (acceptable_symbol == 0)
			res.push_back(*runner);
		else
			if (!res.empty() && res.back() != ',')
				res.push_back(',');
		runner++;
	}
	return res;
}


