#include "Exceptions.h"

std::string UnexpectedCharacterException::What() const
{
	return position.ToString() + ": Unexpected character: " + (char)ch;
}

std::string UnexpectedEndOfFileException::What() const
{
	return position.ToString() + ": Unexpected end of file";
}

std::string UnexpectedTokenException::What() const
{
	std::string buff = "";
	buff += position.ToString();
	buff += " Unexpected token: " + unexpectedToken->ToString();
	buff += "; Expected: ";
	buff += GetExpectedString();
	return buff;
}

std::string UnexpectedTokenException::GetExpectedString() const
{
	if (expectedType == typeid(IdentifierToken))
		return "Identifier";

	if (expectedType == typeid(KeywordToken))
		return "Keyword";

	if (expectedType == typeid(ValueToken))
		return "Value";

	if (expectedType == typeid(OpenBracketToken))
		return "Operator (";

	if (expectedType == typeid(CloseBracketToken))
		return "Operator )";

	if (expectedType == typeid(AssignOperatorToken))
		return "Operator =";

	return "Unknown token";
}

const Token* UnexpectedTokenException::GetToken() const
{
	return unexpectedToken;
}

const std::type_info& UnexpectedTokenException::GetExpectedType() const
{
	return expectedType;
}

std::string UnexpectedTokenWithExpectedKeywordException::What() const
{
	std::string buff = "";
	buff += position.ToString();
	buff += " Unexpected token: " + unexpectedToken->ToString();
	buff += "; Expected: ";
	buff += "Keyword(" + expectedKeyword + ")";
	return buff;
}

const PositionInText& ParserException::GetPosition() const
{
	return position;
}

std::string EmptyBlockException::What() const
{
	return position.ToString() + "Block epxression should have one nested expression at least";
}

std::string UndefinedVariableException::What() const
{
	return GetPositionPrefix() +
		"Undefined variable: \"" + variable + "\"";
}

std::string RuntimeException::GetPositionPrefix() const
{
	return positionIsSet ? (position.ToString() + ": ") : "";
}

void RuntimeException::SetPosition(const PositionInText& position)
{
	this->position = position;
	this->positionIsSet = true;
}

std::string ExpressionIsNotValueException::What() const
{
	return GetPositionPrefix() +
		"Expression " + expression->ToString() + " isn't a value expression";
}

std::string UnknownExpressionException::What() const
{
	return GetPositionPrefix() +
		"Unknown expression " + expression->ToString();;
}

std::string ExpressionIsNotCallableException::What() const
{
	return GetPositionPrefix() +
		"Expression " + expression->ToString() + " isn't a callable expression";
}
