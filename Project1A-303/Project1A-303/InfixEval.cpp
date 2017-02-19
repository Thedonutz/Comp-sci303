#include "InfixEval.h"
#include "Error.h"
#include <sstream>
#include <cctype>
#include <cmath>
using std::stack;
using std::string;
using std::istringstream;
using std::isdigit;


const string InfixEval::OPERATORS = { "+-*/%^-!{}()[]" };
const int InfixEval::Preced[] = { 1, 1, 2, 2, 2, 3, 4, 4, -1, -1, -1, -1, -1, -1 };


InfixEval::InfixEval()
{
	//Constructor	
	int infix = 0;
	operator_stack = {};
	integer_stack = {};
}


int InfixEval::eval(const string expression)
{
	int answer = 0;
	int infix = 0;
/*
To be completed by John.
*/
// Be sure the stack is empty
	while (!operator_stack.empty()) 
	{
		operator_stack.pop();
	}

	// Process each token
	istringstream tokens(expression);
	char next_char;
	while (tokens >> next_char)
	{
		if (isdigit(next_char)) 
		{
			//
			// add to integer_stack
			//
			tokens.putback(next_char);
			int value;
			tokens >> value;
			integer_stack.push(value);
		}
		else if (is_operator(next_char)) 
		{
			//
			// add to operator stack
			// and check precedence
			//
			process_OP(next_char);
			
		}
		else 
		{
			throw Syntax_Error("Invalid character encountered");
		}
	}
	while (!operator_stack.empty()) 
	{
		answer = eval_op(operator_stack.top());
		infix += answer;
		integer_stack.push(infix);
		if (operator_stack.empty()) 
		{
			return answer;
		}
	}
	if (operator_stack.empty())
	{
		throw Syntax_Error("Stack is empty");
	}
}


void InfixEval::process_OP(char op) {
	if (operator_stack.empty() || (op == '(') || (op == '[') || (op == '{')) 
	{
		if (op == ')' || op == ']' || op == '}')
			throw Syntax_Error("Unmatched close parenthesis");
		operator_stack.push(op);
	}
	else 
	{
		if (precedence(op) > precedence(operator_stack.top())) 
		{
			operator_stack.push(op);
		}
		else 
		{
			// Pop all stacked operators with equal
			// or higher precedence than op.
			while (!operator_stack.empty() && (operator_stack.top() != '(') && (operator_stack.top() != '[')
				&& (operator_stack.top() != '{') && (precedence(op) <= precedence(operator_stack.top()))) 
			{
				infix += eval_op(operator_stack.top());
				integer_stack.push(infix);
				infix = 0;			
			}
			// assert: Operator stack is empty or 
			//         top of stack is '(' or current
			//         operator precedence > top of stack operator
			//         precedence;
			if (op == ')') 
			{
				if (!operator_stack.empty() && (operator_stack.top() == '(')) 
				{
					operator_stack.pop();
				}
				else
				{
					throw Syntax_Error("Unmatched close parentheses");
				}
			}
			else if (op == ']')
			{
				if (!operator_stack.empty() && (operator_stack.top() == '['))
				{
					operator_stack.pop();
				}
				else
				{
					throw Syntax_Error("Unmatched close parentheses");
				}
			}
			else if (op == '}') 
			{
				if (!operator_stack.empty() && (operator_stack.top() == '{')) 
				{
					operator_stack.pop();
				}
				else 
				{
					throw Syntax_Error("Unmatched close parentheses");
				}
			}
			else 
			{
				operator_stack.push(op);
			}
		}
	}
}
int InfixEval::eval_op(char op) {
	if (operator_stack.empty())
		throw Syntax_Error("Stack is empty");
	int rhs = integer_stack.top();
	integer_stack.pop();
	char oper = operator_stack.top();
	operator_stack.pop();
	int lhs = integer_stack.top();
	integer_stack.pop();
	int result = 0;
	switch (oper) {
	case '+': result = lhs + rhs;
		break;
	case '-': result = lhs - rhs;
		break;
	case '*': result = lhs * rhs;
		break;
	case '/': result = lhs / rhs;
		break;
	case '%': result = lhs % rhs;
		break;
	case '^': result = pow(lhs, rhs);
		break;
	}
	return result;
}

//Other Methods to be completed by Jeff/Murad