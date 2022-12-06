#include "TStack.h"
#include "Vector.h"
#include <iostream>
#include <string>
#include <map>


class Translator {
	std::string infix;
	enum lexemType { number, operation, bracket_open, bracket_close, word, begin };
	Vector<std::pair<lexemType, std::string>> lexems;
	Vector<std::pair<lexemType, std::string>> postfix;
	std::map<std::string, int> priority = { {"+", 1}, {"-", 1}, {"*", 2}, {"/", 2} };
	std::map<std::string, double> variables;

	void Parse() {
		lexemType l = begin;
		int n = 0;
		int brackets = 0;
		for (int i = 0; i < infix.size(); i++) {
			char c = infix[i];
			if (c == ' ')  continue;  // this ruins a lot of cases but still
			switch (l) {
			case (begin):
				if (c == '(') {
					l = bracket_open;
				}
				else if (Digit(c) || c == '.' || c == '-') {
					l = number;
				}
				else if (Letter(c)) {
					l = word;
				}
				else throw std::invalid_argument("L user");
				break;
			case(number):
				if (c == '.' || Digit(c))
					continue;
				lexems.push_back({ number, infix.substr(n, i - n) });
				if (c == ')') {
					n = i;
					l = bracket_close;
				}
				else if (Operation(c)) {
					n = i;
					l = operation;
				}
				else throw std::invalid_argument("L user");
				break;
			case(operation):
				lexems.push_back({ operation, infix.substr(n, i - n) });
				if (c == '(') {
					n = i;
					l = bracket_open;
				}
				else if (Digit(c) || c == '.' || c == '-') {
					n = i;
					l = number;
				}
				else if (Letter(c)) {
					n = i;
					l = word;
				}
				else throw std::invalid_argument("L user");
				break;
			case(bracket_open):
				brackets++;
				lexems.push_back({ bracket_open, "(" });
				if (c == '(') {
					n = i;
					l = bracket_open;
				}
				else if (Digit(c) || c == '.' || c == '-') {
					n = i;
					l = number;
				}
				else if (Letter(c)) {
					n = i;
					l = word;
				}
				else throw std::invalid_argument("L user");
				break;
			case(bracket_close):
				brackets--;
				lexems.push_back({ bracket_close, ")" });
				if (c == ')') {
					n = i;
					l = bracket_close;
				}
				else if (Operation(c)) {
					n = i;
					l = operation;
				}
				else throw std::invalid_argument("L user");
				break;
			case(word):
				if (Letter(c) || Digit(c))
					continue;
				lexems.push_back({ word, infix.substr(n, i - n) });
				variables.insert({ infix.substr(n, i - n), 0.0 });
				if (c == ')') {
					n = i;
					l = bracket_close;
				}
				else if (Operation(c)) {
					n = i;
					l = operation;
				}
				else throw std::invalid_argument("L user");
				break;
			}
		}
		lexems.push_back( { l,  infix.substr(n, infix.size()-n) } );
		if (l == bracket_close) brackets--;
		if (l == bracket_open) throw	std::invalid_argument("L user");
		if (l == word) variables.insert({ infix.substr(n, infix.size() - n), 0.0 });
		if (l == operation) throw std::invalid_argument("L user");
		if (brackets != 0) throw std::invalid_argument("L user");
	};

	void ToPostfix() {
		DeleteAllSpaces();
		Parse();
		TStack<std::pair<lexemType, std::string>> st;
		std::pair<lexemType, std::string> stackItem;
		for (int i = 0; i < lexems.get_size(); i++) {
			std::pair<lexemType, std::string>  item = lexems[i];
			switch (item.first) {
			case(bracket_open):
				st.push(item);
				break;
			case(bracket_close):
				stackItem = st.pop();
				while (stackItem.first != bracket_open) {
					postfix.push_back(stackItem);
					stackItem = st.pop();
				}
				break;
			case(operation):
				while (!st.empty()) {
					stackItem = st.pop();
					if (priority[item.second] <= priority[stackItem.second])
						postfix.push_back(stackItem);
					else {
						st.push(stackItem);
						break;
					}
				}
				st.push(item);
				break;
			default:
				postfix.push_back(item); // stackItem
			}
		}
		while (!st.empty()) {
			stackItem = st.pop();
			postfix.push_back(stackItem);
		}
	};

public:
	Translator(std::string infix) : infix(infix) {
		ToPostfix();	
	};

	std::string GetInfix() {
		return infix;
	};

	std::string GetPostfix() {
		std::string res;
		for (int i = 0; i < postfix.get_size(); i++) {
			res += postfix[i].second;
		}
		return res;
	};

	void getvariables(std::istream& input, std::ostream& output) {
		output << "Enter values:" << std::endl;
		for (auto& o : variables) {
			output << o.first << " = ";
			input >> o.second;
		}
	}

	double Calculate(std::istream& input = std::cin, std::ostream& output = std::cout){
		getvariables(input, output);
		TStack<double> st;
		double lOp, rOp;
		for (int i = 0; i < postfix.get_size(); i++) {
			std::pair<lexemType, std::string>  item = postfix[i];
			if (item.first == word) {
				st.push(variables[item.second]);
			}
			else if (item.first == number) {
				st.push(std::stod(item.second));
			}
			else {
				switch (item.second[0])
				{
				case '+':
					rOp = st.pop();
					lOp = st.pop();
					st.push(lOp + rOp);
					break;
				case '-':
					rOp = st.pop();
					lOp = st.pop();
					st.push(lOp - rOp);
					break;
				case '*':
					rOp = st.pop();
					lOp = st.pop();
					st.push(lOp * rOp);
					break;
				case '/':
					rOp = st.pop();
					lOp = st.pop();
					st.push(lOp / rOp);
					break;
				}
			}
		}
		return st.top();
	};

	void DeleteAllSpaces() {
		std::string res;
		for (int i = 0; i < infix.size(); i++) {
			if (infix[i] != ' ') res += infix[i];
		}
		infix = res;
	}

	bool Digit(char c) {
		return '0' <= c && c <= '9';
	}
	bool Letter(char c) {
		return 'a' <= c && c <= 'z' || 'A' <= c && c <= 'Z' || c == '_';
	}
	bool Operation(char c) {
		return c == '+' || c == '-' || c == '*' || c == '/';
	}
};