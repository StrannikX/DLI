# pragma once

# ifndef TOKEN_H_INCLUDED
# define TOKEN_H_INCLUDED

#include <string>

// Лексемы, читаемые лексическим анализатором


// Базовая лексема
class Token {
public:
	Token() {}
	virtual ~Token() = default;
	virtual std::string ToString();
};

// Открывающаяся скобка
class OpenBracketToken : public Token 
{
public:
	virtual std::string ToString();
};

// Закрывающаяся скобка
class CloseBracketToken : public Token 
{
public:
	virtual std::string ToString();
};

// Оператор присваивания
class AssignOperatorToken : public Token 
{
public:
	virtual std::string ToString();
};

// Ключевое слово
class KeywordToken : public Token 
{
	std::string keyword;
public:
	KeywordToken(const KeywordToken& kw) : keyword(kw.keyword) {}
	KeywordToken(std::string str) : keyword(str) {}
	std::string& GetKeyword();
	virtual std::string ToString();
};

// Идентификатор
class IdentifierToken : public Token 
{
	std::string id;
public:
	IdentifierToken(const IdentifierToken& id) : id(id.id) {}
	IdentifierToken(std::string id) : id(id) {}
	std::string& GetId();
	virtual std::string ToString();
};

// Значение
class ValueToken : public Token
{
	int value;
public:
	ValueToken(const ValueToken& val) : value(val.value) {}
	ValueToken(int value) : value(value) {}
	int GetValue();
	virtual std::string ToString();
};

#endif // !TOKEN_H_INCLUDED
