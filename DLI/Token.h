# pragma once

# ifndef TOKEN_H_INCLUDED
# define TOKEN_H_INCLUDED

# include <string>
# include "Position.h"

// Лексемы, читаемые лексическим анализатором


// Базовая лексема
class Token {
protected:
	PositionInText position = { 0, 0 };
public:
	Token(const PositionInText &position) : position(position) {}
	Token() {}
	const PositionInText& GetPosition() const;
	virtual ~Token() = default;
	virtual std::string ToString();
};

// Открывающаяся скобка
class OpenBracketToken : public Token 
{
public:
	OpenBracketToken(const PositionInText& position) : Token(position) {}
	virtual std::string ToString();
};

// Закрывающаяся скобка
class CloseBracketToken : public Token 
{
public:
	CloseBracketToken(const PositionInText& position) : Token(position) {}
	virtual std::string ToString();
};

// Оператор присваивания
class AssignOperatorToken : public Token 
{
public:
	AssignOperatorToken(const PositionInText& position) : Token(position) {}
	virtual std::string ToString();
};

// Ключевое слово
class KeywordToken : public Token 
{
	std::string keyword;
public:
	KeywordToken(const KeywordToken& kw) : keyword(kw.keyword), Token(kw.position) {}
	KeywordToken(std::string str, PositionInText& position) : keyword(str), Token(position) {}
	std::string& GetKeyword();
	virtual std::string ToString();
};

// Идентификатор
class IdentifierToken : public Token 
{
	std::string id;
public:
	IdentifierToken(const IdentifierToken& id) : id(id.id), Token(id.position) {}
	IdentifierToken(std::string id, const PositionInText& position) : id(id), Token(position) {}
	std::string& GetId();
	virtual std::string ToString();
};

// Значение
class ValueToken : public Token
{
	int value;
public:
	ValueToken(const ValueToken& val) : value(val.value), Token(val.position) {}
	ValueToken(int value, const PositionInText& position) : value(value), Token(position) {}
	int GetValue();
	virtual std::string ToString();
};

#endif // !TOKEN_H_INCLUDED
