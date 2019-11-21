#include <iostream>

# include "Parser.h"
# include "Lexer.h"
# include "Evaluator.h"

# include <sstream>
# include <vector>
#include <fstream>

int main()
{
	std::vector<std::string> keywords = {"val", "var", "add", "if", "then", "else", "let", "in", "function", "call", "set", "block"};

	std::ifstream in("input.txt", std::ifstream::in);

	if (!in.is_open())
	{
		throw std::exception("File doesn't exist");
	}

	try {

		Lexer lex(keywords);
		auto tokens = lex.Tokenize(in);
		Parser parser(tokens);
		auto expr = parser.Parse();
		Evaluator vm;
		auto expr2 = vm.Eval(expr);
		std::cout << expr2->ToString() << std::endl;

		delete expr;
		delete expr2;

		for (auto token : tokens)
		{
			delete token;
		}
	}
	catch (std::exception e)
	{
		std::cout << "ERROR" << std::endl;
	}
	return 0;
}