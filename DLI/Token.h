# pragma once

# ifndef TOKEN_H_INCLUDED
# define TOKEN_H_INCLUDED


#include <string>

class Token {
public:
	Token() {}
	virtual ~Token() = default;
	virtual std::string ToString();
};

class OpenBracketToken : public Token 
{
public:
	virtual std::string ToString();
};
class CloseBracketToken : public Token 
{
public:
	virtual std::string ToString();
};
class AssignOperatorToken : public Token 
{
public:
	virtual std::string ToString();
};

class KeywordToken : public Token 
{
	std::string keyword;
public:
	KeywordToken(const KeywordToken& kw) : keyword(kw.keyword) {}
	KeywordToken(std::string str) : keyword(str) {}
	std::string& GetKeyword();
	virtual std::string ToString();
};

class IdentifierToken : public Token 
{
	std::string id;
public:
	IdentifierToken(const IdentifierToken& id) : id(id.id) {}
	IdentifierToken(std::string id) : id(id) {}
	std::string& GetId();
	virtual std::string ToString();
};

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
