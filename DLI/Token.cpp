#include "Token.h"

std::string& KeywordToken::GetKeyword()
{
	return keyword;
}

std::string KeywordToken::ToString()
{
	return "Keyword(" + keyword + ")";
}

std::string& IdentifierToken::GetId()
{
	return id;
}

std::string IdentifierToken::ToString()
{
	return "Identifier(" + id + ")";
}

int ValueToken::GetValue()
{
	return value;
}

std::string ValueToken::ToString()
{
	return "Value(" + std::to_string(value) + ")";
}

std::string Token::ToString()
{
	return std::string("Token");
}

std::string OpenBracketToken::ToString()
{
	return std::string("(");
}

std::string CloseBracketToken::ToString()
{
	return std::string(")");
}

std::string AssignOperatorToken::ToString()
{
	return std::string("=");
}
