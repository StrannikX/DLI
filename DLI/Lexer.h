#pragma once

# include <string>
# include <vector>
# include <list>
# include <iostream>
# include "Token.h"


// ������������ ���������
const char OPEN_BRACKET = '(';
const char CLOSE_BRACKET = ')';
const char ASSIGN_OPERATOR = '=';
const char MINUS = '-';


// ��������� ��������������� �����������
enum class LexerState {
	WaitToken,     // �������� ������ ����� �������
	ReadInt,       // ������ ������ �����
	ReadWord       // ������ �����
};

class Lexer
{
	std::vector<std::string> &keywords;  // ������ �������� ����
	std::istream &in;                    // ������� ����� ��������
	std::list<Token*> tokens;            // �������� ������ ������
	LexerState state;                    // ��������� �����������
	std::string buff;                    // ����� ������ �������
public:
	Lexer(std::vector<std::string>& keywords, std::istream& in) : in(in), keywords(keywords), state(LexerState::WaitToken) {};
	
	// ��������� ����������� ������
	std::list<Token*> Tokenize();
protected:
	void ReadIntState();
	void ReadWordState();
	void WaitForToken();

	ValueToken* CreateValueToken();
	Token* CreateWordToken();
};