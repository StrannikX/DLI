#pragma once

# include <string>
# include <vector>
# include <list>
# include <stack>
# include <iostream>
# include "Position.h"
# include "Token.h"


// Используемые операторы
const char OPEN_BRACKET = '(';
const char CLOSE_BRACKET = ')';
const char ASSIGN_OPERATOR = '=';
const char MINUS = '-';


// Состояния синтаксического анализатора
enum class LexerState {
	WaitToken,     // Ожидание начала новой лексемы
	ReadInt,       // Чтение целого числа
	ReadWord       // Чтение слова
};

class Lexer
{
	std::vector<std::string> &keywords;    // Список ключевых слов
	std::istream &in;                      // Входной поток символов
	std::list<Token*> tokens;              // Выходной список лексем
	LexerState state;                      // Состояние анализатора
	std::string buff;                      // Буфер чтения лексемы
	PositionInText bufferBeginingPosition; // Позиция начала буфера в тексте
	PositionInText position = { 1, 0 };    // Позиция в тексте
	std::stack<unsigned int> rowLengths;   // Стэк длины строк
public:
	Lexer(std::vector<std::string>& keywords, std::istream& in) : in(in), keywords(keywords), state(LexerState::WaitToken){};
	
	// Выполняет лексический анализ
	std::list<Token*> Tokenize();
protected:
	void ReadIntState();
	void ReadWordState();
	void WaitForToken();

	int GetChar();
	void PutBack(int);

	ValueToken* CreateValueToken();
	Token* CreateWordToken();
};