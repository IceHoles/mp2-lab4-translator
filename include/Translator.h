#include "TStack.h"
#include "Vector.h"
#include <iostream>
#include <string>
#include <map>

template <typename T>
class Translator {
	std::string infix;
	enum lexemType { number, operation, bracket_open, bracket_close, variable, begin };
	Vector<pair<lexemType, std::string>> lexems;
	Vector<pair<lexemType, std::string>> postfix;
	std::map <std::string, int> priority;
	std::map<std::string, T>  variables;

	void Parse() {
		enum lexemType { number, operation, bracket_open, bracket_close, word, begin };
		lexemType l = begin;
		int n = 0;
		int brackets = 0;
		for (int i = 0; i < infix.size(); i++) {
			char c = infix[i];
			if (c == ' ') continue;
			switch (l) {
			case (begin):
				if (c == '(') {
					l = bracket_open;
				}
				else if (digit(c) || c == '.' || c == '-') {
					l = number;
				}
				else if (letter(c)) {
					l = word;
				}
				else throw std::invalid_argument("L user");
				break;
			case(number):
				if (c == '.' || digit(c))
					continue;
				std::string lexem(i - n);
				for (int j = 0; j < i - n - 1; j++) {
					lexem[j] = infix[n + j];
				}
				lexems.push_back({ number, lexem });
				if (c == ')') {
					n = i;
					l = bracket_close;
				}
				else if (operation(c)) {
					n = i;
					l = operation;
				}
				else throw std::invalid_argument("L user");
				break;
			case(operation):
				std::string lexem(i - n);
				for (int j = 0; j < i - n - 1; j++) {
					lexem[j] = infix[n + j];
				}
				lexems.push_back({ operation, lexem });
				if (c == '(') {
					n = i;
					l = bracket_open;
				}
				else if (digit(c) || c == '.' || c == '-') {
					n = i;
					l = number;
				}
				else if (letter(c)) {
					n = i;
					l = word;
				}
				else throw std::invalid_argument("L user");
				break;
			case(bracket_open):
				brackets++;
				lexems.push_back({ bracket_open, '(' });
				if (c == '(') {
					n = i;
					l = bracket_open;
				}
				else if (digit(c) || c == '.' || c == '-') {
					n = i;
					l = number;
				}
				else if (letter(c)) {
					n = i;
					l = word;
				}
				else throw std::invalid_argument("L user");
				break;
			case(bracket_close):
				brackets--;
				lexems.push_back({ bracket_close,')' });
				if (c == ')') {
					n = i;
					l = bracket_close;
				}
				else if (operation(c)) {
					n = i;
					l = operation;
				}
				else throw std::invalid_argument("L user");
				break;
			case(word):
				if (letter(c) || digit(c))
					continue;
				std::string lexem(i - n);
				for (int j = 0; j < i - n - 1; j++) {
					lexem[j] = infix[n + j];
				}
				lexems.push_back({ word, lexem });
				variables.insert({ lexem, 0.0 });
				if (c == ')') {
					n = i;
					l = bracket_close;
				}
				else if (operation(c)) {
					n = i;
					l = operation;
				}
				else throw std::invalid_argument("L user");
				break;
			}
			std::string lexem(i - n);
			for (int j = 0; j < i - n - 1; j++) {
				lexem[j] = infix[n + j];
			}
			lexems.push_back({ l, infix[infix.size() - 1] });
			if (l == ')') brackets--;
			if (l == '(') throw	std::invalid_argument("L user");
			if (l == word) variables.insert({ lexem, 0.0 });
			if (l == operation) throw std::invalid_argument("L user");
			if (brackets != 0) throw std::invalid_argument("L user");
		}
	};

	void ToPostfix() {
		enum lexemType { number, operation, bracket_open, bracket_close, word, begin };
		Parse();
		TStack<pair<lexemType, std::string>> st;
		pair<lexemType, std::string> stackItem;
		for (int i = 0; i < lexems.size(); i++) {
			pair<lexemType, std::string>  item = lexems[i];
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
		priority = { {'+', 1}, {'-', 1}, {'*', 2}, {'/', 2} };
		ToPostfix();
		Calculate();
	};

	std::string GetInfix() {
		return infix;
	};

	std::string GetPostfix() {
		return postfix;
	};

	void getvariables(istream& input, ostream& output) {
		output << "Enter values:" << endl;
		for (auto& o : operands) {
			output << o.first << " = ";
			input >> o.second;
		}
	}

	T Calculate(const std::map<std::string, T>& values) {
		//for (auto& val : values) {
		//	try {
		//		variables.at(val.first) = val.second;
		//	}
		//	catch (std::out_of_range& e) {}
		//}
		getvariables();
		TStack<T> st;
		T lOp, rOp;
		for (int i = 0; i < postfix.size(); i++) {
			pair<lexemType, std::string>  item = postfix[i];
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
			default:
				if (item.first == letter) st.push(variables[item.second]);
				else st.push(std::stod(lexem.second));
			}
		}
		return st.top();
	};

	bool digit(char c) {
		return '0' <= c && c <= '9';
	}
	bool letter(char c) {
		return 'a' <= c && c <= 'z' || 'A' <= c && c <= 'Z' || c == '_';
	}
	bool operation(char c) {
		return c == '+' || c == '-' || c == '*' || c == '/';
	}
};