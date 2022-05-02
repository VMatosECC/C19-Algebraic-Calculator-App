// C19-Algebraic-Calculator-App.cpp 
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
using namespace std;
// -------------------------------------------------------------------------
// Prototypes
queue<string> tokenize(string exp, string sep);
queue<string> tokenizeV2(string exp);
double evaluateExpression(string expression);
void   processOperator(string opr, stack<double>& stackOfValues);
string infixToPostfix(string expression);

//--------------------------------------------------------------------------
int main()
{
	string exp = "(1 + 5)*((2 + 4/2) % 3)";
	do
	{
		cout << "\nEnter an infix expression - use spaces around the operators [xxx to end]: \n";
		getline(cin, exp);
		if (exp == "xxx") break;
		string expRPN = infixToPostfix(exp);
		double result = evaluateExpression(expRPN);
		cout << "Infix:   " << exp << endl;
		cout << "Postfix: " << expRPN << endl;
		cout << "Result:  " << result << endl;
	} while (true);

}//main

// --------------------------------------------------------------------------
// Evaluate an arithmetical expression holding operators: +-*/() 
double evaluateExpression(string expression) {
	stack<double> stackOfValues; 		// operand's stack
	string separators = " +-*/%()";		// binary operators and parenthesis

	// Extract operands and operators
	// queue<string> tokenQueue = tokenize(expression, separators);

	//alternatively
	queue<string> tokenQueue = tokenizeV2(expression);

	// Scan tokenQueue
	while (!tokenQueue.empty()) {
		string token = tokenQueue.front(); 	// Extract a token
		tokenQueue.pop(); 					//remove token from queue

		if (token == "+" || token == "-" ||
			token == "%" ||
			token == "*" || token == "/") {
			processOperator(token, stackOfValues);	// add, multiply, etc.
		}
		else {  // we found an operand (convert it to double push it to stack)
			double doubleValue = stod(token);
			stackOfValues.push(doubleValue);
		}
	}

	// Return the numerical result
	return stackOfValues.top();
}

// -------------------------------------------------------------------------------------
queue<string> tokenize(string exp, string sep)
{
	string token, strOpr;
	int foundPos = exp.find_first_of(sep);
	int previous = 0;
	queue<string> result;

	// Look for the occurrence of a token followed by a separator. 
	// Each time they are found add to result the token and the separator.
	// Caution: You may not always have pairs <token:separator>
	// as it could be the case of two consecutive separators such as +(
	// Additionally, SPACE separator could be ignored (as we do).

	while (foundPos < exp.size()) //if not at the end-of-string
	{

		token = exp.substr(previous, foundPos - previous);
		
		if (token != "") result.push(token);

		char opr = exp[foundPos];
		strOpr = string() + opr;  //converting char to string

		if (strOpr != " ") result.push(strOpr);

		previous = foundPos + 1; //skip the previously found separator
		foundPos = exp.find_first_of(" +-*/%()", foundPos + 1);
	}
	token = exp.substr(previous, foundPos - previous);
	if(token != "") result.push(token);
	return result;
}

queue<string> tokenizeV2(string exp)
{
	// a much simpler tokenizer! requires spaces around operators
	queue<string> qTokens;
	istringstream sin(exp);
	string aToken;
	while (sin >> aToken) {
		qTokens.push(aToken);
	}
	return qTokens;
}

// -------------------------------------------------------------------------------------
void processOperator(string opr, stack<double>& stackOfValues) {
	// Process binary operator: pop top two arguments from the stack
	// apply to them the operator, push back the result on the stack

	if (opr == "+") {
		double val2 = stackOfValues.top(); stackOfValues.pop();
		double val1 = stackOfValues.top(); stackOfValues.pop();
		stackOfValues.push(val1 + val2);
	}
	else if (opr == "-") {
		double val2 = stackOfValues.top(); stackOfValues.pop();
		double val1 = stackOfValues.top(); stackOfValues.pop();
		stackOfValues.push(val1 - val2);
	}
	else if ((opr == "*")) {
		double val2 = stackOfValues.top(); stackOfValues.pop();
		double val1 = stackOfValues.top(); stackOfValues.pop();
		stackOfValues.push(val1 * val2);
	}
	else if (opr == "/") {
		double val2 = stackOfValues.top(); stackOfValues.pop();
		double val1 = stackOfValues.top(); stackOfValues.pop();
		stackOfValues.push(val1 / val2);
	}
	else if (opr == "%") {
		double val2 = stackOfValues.top(); stackOfValues.pop();
		double val1 = stackOfValues.top(); stackOfValues.pop();
		stackOfValues.push((int)val1 % (int)val2);
	}
}

// -------------------------------------------------------------------------------------
string infixToPostfix(string expression) 
{
	string s = ""; // this string will hold the postfix formula
	// Create operator's stack to store their values
	stack<string> operatorStack;
	// Extract operands and operators
	string separators = " ()+-/*%";
	queue<string> tokenQueue = tokenize(expression, separators);

	// Phase 1: Scan tokenQueue extracting one token at the time
	while (!tokenQueue.empty()) {
		string token = tokenQueue.front(); tokenQueue.pop(); // Extract a token
		if (token == "+" || token == "-") {
			// remove all +, -, *, / on top of the operator stack
			while (!operatorStack.empty() &&
				((operatorStack.top() == "+") ||
					(operatorStack.top() == "-") ||
					(operatorStack.top() == "*") ||
					(operatorStack.top() == "/")
					)) {
				s += operatorStack.top() + " "; //add top symbol to output
				operatorStack.pop();
			}
			// push the incoming + or - low-level operator into the operator stack
			operatorStack.push(token);
		}
		else if (token == "*" || token == "/") {
			// remove all *, / on top of the operator stack
			while (!operatorStack.empty() &&
				((operatorStack.top() == "*") ||
					(operatorStack.top() == "/")
					)) {
				s += operatorStack.top() + " ";
				operatorStack.pop();
			}
			// Push the incoming * or / operator into the operator stack
			operatorStack.push(token);
		}
		else if (token == "(") {
			operatorStack.push("("); // Push "(" to stack
		}
		else if (token == ")") {
			// remove all the operators from the stack until seeing "("			
			while (operatorStack.top() != "(") {
				s += operatorStack.top() + " ";
				operatorStack.pop();
			}
			operatorStack.pop(); // Pop the "(" symbol from the stack
		}
		else { // An operand scanned - add it to the output string s			  
			s += token + " ";
		}
	}

	// Phase 2: remove the remaining operators from the stack
	while (!operatorStack.empty()) {
		s += operatorStack.top() + " ";
		operatorStack.pop();
	}

	// Return the result
	return s;

}


// -------------------------------------------------------------------------------------


