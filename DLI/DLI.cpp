#include <iostream>

# include "Parser.h"
# include "Lexer.h"
# include "Evaluator.h"
# include "Exceptions.h"

# include <sstream>
# include <vector>
# include <fstream>

/* 
* TODO: 
* Новый алгоритм управления областями видимости
*/

int main()
{
	// Список ключевых слов языка
	std::vector<std::string> keywords = {
		"val", "var", "add", "if", "then", 
		"else", "let", "in", "function", 
		"call", "set", "block"
	};

	std::ifstream in("input.txt", std::ifstream::in);

	if (!in.is_open())
	{
		throw std::exception("File doesn't exist");
	}

	try {

		// Лексический анализ
		Lexer lex(keywords, in);
		auto tokens = lex.Tokenize();

		// Синтаксический анализ
		Parser parser(tokens);
		auto expr = parser.Parse();

		// Выполнение кода
		Evaluator vm;
		auto expr2 = vm.Eval(expr);

		std::cout << expr2->ToString() << std::endl;

		// Подчищаем за собой

		delete expr;
		delete expr2;

		for (auto token : tokens)
		{
			delete token;
		}
	}
	catch (InterpreterException& e)
	{
		std::cout << "ERROR" << std::endl;
		std::cerr << e.What() << std::endl;
	}

	return 0;
}