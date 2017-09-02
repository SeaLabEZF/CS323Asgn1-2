// Member Names:
// Alexander Edgar
// Daniel Domingo
// Sean Wulwick

#include <exception>
#include <iostream>
#include <map>
#include <stack>
#include <string>

using namespace std;

/***************************************************/
/*Function: isOperator                             */
/*Input: char                                      */
/*Output: boolean true false                       */
/*                                                 */
/*This function takes in a char checking if it is a*/
/*operation character, +, -, *, /, and returns a   */
/*value of true or false based on the input.       */
/***************************************************/

bool isOperator(char c)
{
	if (c == '+' || c == '-' || c == '*' || c == '/')
		return true;
	else
		return false;
}

/***************************************************/
/*Function: promptContinue                         */
/*Input: N/A                                       */
/*Output: boolean true false                       */
/*                                                 */
/*This function asks the user via a prompt if they */
/*would like to continue running the program. If   */
/*the user replys with a 'y' the program prompts   */
/*the user for another postfix string. If any other*/
/*input is recieved the program terminates         */
/***************************************************/

bool promptContinue()
{
	cout << "Continue (y/n)?";
	char c;
	cin.get(c);
	cin.ignore();
	if (c == 'y') { return true; }
	return false;
}

int main(int argc, char* argv[]) {
	
	//Variable Declaration
	stack<int> myStack;
	map<char, int> myMap;
	int value, tempValueOne, tempValueTwo;
	string expr;
	string::iterator myStringIt;
	bool noErrors = true;

	while (1) {
		//Ask for initial user input of postfix statment
		cout << "Enter a postfix expression with a $ at the end:";
		
		//Store postfix statment
		cin >> expr;
		cin.ignore();

		//Ensure postfix statment ends with '$'
		if (expr.back() != '$') {
			cout << "\tInvalid expression: does not end with $" << endl;
			//In the event that the statmenet does not end with a '$' the user is asked
			//if they would like to continue or to terminate the program
			if (promptContinue()) { continue; }
			return 0;
		}

		//Use string iterator to parse through the postfix statment
		//for non-operation characters
		for (myStringIt = expr.begin(); myStringIt != expr.end(); ++myStringIt) {
			
			//Leaves for loop upon reaching '$'
			if (*myStringIt == '$') { break; }
			
			//Checks if the current character is in the map
			//if the character is not, the user is asked what
			//the value of the character is.
			if (!isOperator(*myStringIt) && myMap.find(*myStringIt) == myMap.end()) {
				cout << "\tEnter the value of " << *myStringIt << ":";
				
				cin >> value;
				cin.ignore();
				myMap.emplace(*myStringIt, value);
			}
		}
		
		//Use string iterator to parse through postfix statment
		//if the character is a non-operator it's value is pushed onto 
		//the stack if the character is an operator the operation is 
		//done on the top two values on the stack, the result is pushed 
		//back onto the stack
		for (myStringIt = expr.begin(); myStringIt != expr.end(); ++myStringIt) {
			
			//Leaves for loop upon reaching '$'
			if (*myStringIt == '$') { break; }

			//Checks if current character is a non-operator
			//if it is a non-operator the value is pushed
			//to the stack
			if (!isOperator(*myStringIt)) {
				try {
					myStack.push(myMap.at(*myStringIt));
				}
				catch (out_of_range& err) {
					cout << "\tAn error occured during evaluation:" << err.what() << endl;
					noErrors = false;
					break;
				}
			}
			else {
				//Error check if not enough values pushed to
				//the stack to do operation on
				if (myStack.size() < 2) {
					cout << "\tInvalid expression: the operator " << *myStringIt << " at position " << myStringIt - expr.begin() << " did not have two elements to act upon." << endl;
					noErrors = false;
					break;
				}

				//Store top two values from the stack
				//into temporary variables
				//pop the stack to remove the top two
				//variables
				tempValueTwo = myStack.top();
				myStack.pop();
				tempValueOne = myStack.top();
				myStack.pop();

				//based on operation symbol do
				//the corresponding operation
				//to the temporary variables
				if (*myStringIt == '+') { myStack.push(tempValueOne + tempValueTwo); }
				else if (*myStringIt == '-') { myStack.push(tempValueOne - tempValueTwo); }
				else if (*myStringIt == '*') { myStack.push(tempValueOne*tempValueTwo); }
				else if (*myStringIt == '/') { myStack.push(tempValueOne / tempValueTwo); }
			}
		}

		//Error checking to ensure that there is only one value
		//on the stack as a final value
		if (myStack.size() != 1) {
			cout << "\tInvalid expression: a single result value was not obtained." << endl;
		}

		//Check for previous errors
		else if (noErrors) {
			cout << "\t\tFinal value:" << myStack.top() << endl;
			myStack.pop();
		}

		//Prompt user if they would like to continue or not
		if (promptContinue()) { 
			myMap.clear();
			continue; 
		}
		return 0;
	}
	return 0;
}