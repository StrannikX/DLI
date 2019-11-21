#pragma once

# include <string>
# include <vector>
# include <list>
# include <iostream>
# include "Token.h"

const char OPEN_BRACKET = '(';
const char CLOSE_BRACKET = ')';
const char ASSIGN_OPERATOR = '=';
const char MINUS = '-';

class Lexer 
{
	std::vector<std::string> keywords;
public:
	Lexer(std::vector<std::string> keywords) : keywords(keywords) {};
	std::list<Token*> Tokenize(std::istream&);
};

enum class TokenizerState {
	WaitToken,
	ReadInt,
	ReadWord
};

class Tokenizer
{
	std::vector<std::string> &keywords;
	std::istream &in;
	std::list<Token*> tokens;
	TokenizerState state;
	std::string buff;
public:
	Tokenizer(std::vector<std::string>& keywords, std::istream& in) : in(in), keywords(keywords), state(TokenizerState::WaitToken) {};
	std::list<Token*> Tokenize();
protected:

	void ReadIntState();
	void ReadWordState();
	void WaitForToken();

	ValueToken* CreateValueToken();
	Token* CreateWordToken();
};