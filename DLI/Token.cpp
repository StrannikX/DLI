#include "Token.h"

std::string& KeywordToken::GetKeyword()
{
	return keyword;
}

std::string KeywordToken::ToString()
{
	return GetPosition().ToString() + " Keyword(" + keyword + ")";
}

std::string& IdentifierToken::GetId()
{
	return id;
}

std::string IdentifierToken::ToString()
{
	return GetPosition().ToString() + " Identifier(" + id + ")";
}

int ValueToken::GetValue()
{
	return value;
}

std::string ValueToken::ToString()
{
	return GetPosition().ToString() + " Value(" + std::to_string(value) + ")";
}

const PositionInText& Token::GetPosition() const
{
	return position;
}

std::string Token::ToString()
{
	return GetPosition().ToString() + " Token";
}

std::string OpenBracketToken::ToString()
{
	return GetPosition().ToString() + " (";
}

std::string CloseBracketToken::ToString()
{
	return GetPosition().ToString() + " )";
}

std::string AssignOperatorToken::ToString()
{
	return GetPosition().ToString() + " =";
}
