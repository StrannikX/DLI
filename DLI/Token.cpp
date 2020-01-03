#include "Token.h"

std::string& KeywordToken::GetKeyword()
{
	return keyword;
}

std::string KeywordToken::ToString() const
{
	return "Keyword(" + keyword + ")";
}

std::string& IdentifierToken::GetId()
{
	return id;
}

std::string IdentifierToken::ToString() const
{
	return "Identifier(" + id + ")";
}

int ValueToken::GetValue()
{
	return value;
}

std::string ValueToken::ToString() const
{
	return "Value(" + std::to_string(value) + ")";
}

const PositionInText& Token::GetPosition() const
{
	return position;
}

std::string Token::ToString() const
{
	return "Token";
}

std::string OpenBracketToken::ToString() const
{
	return "(";
}

std::string CloseBracketToken::ToString() const
{
	return ")";
}

std::string AssignOperatorToken::ToString() const
{
	return "=";
}