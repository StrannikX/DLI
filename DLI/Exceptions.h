#pragma once

# include <string>
# include <typeinfo>
# include "Position.h"
# include "Token.h"
#include "AST.h"

// ���������� �������������

// ������� ����� ���������� �������������
class InterpreterException
{
public:
	// ���������, ��� ���������
	virtual std::string What() const = 0;
};

// ������� ����� ��� ���������� ������� ����������
class RuntimeException : public InterpreterException 
{
protected:
	// ���������� ������� ���������� ����� ��� �����
	// ��������������� � ���� ������� � �������� ����
	// ���������, ��� � �� �����

	// ������� � ����
	PositionInText position;
	// ����������� �� �������
	bool positionIsSet = false;

	// ������� � ����������� �� ����, ���� �� ������� ��� ���������� � ����
	std::string GetPositionPrefix() const;
public:
	RuntimeException() {}
	RuntimeException(const PositionInText& position) : position(position), positionIsSet(true) {};

	// ���������� ������� � ����, � ������� ��������� ����������
	void SetPosition(const PositionInText& position);
};

// ���������� �� ��������� ����������
class UndefinedVariableException : public RuntimeException
{
	std::string variable;
public:
	UndefinedVariableException(const std::string& var) : variable(var) {};
	UndefinedVariableException(const std::string& var, const PositionInText& position) : variable(var), RuntimeException(position) {};
	virtual std::string What() const;
};

// ������� ����� ��� ����������, ��������� � ��������� ������ AST ���������
class ExpressionException : public RuntimeException
{
protected:
	Expression* expression;
public:
	ExpressionException(Expression* expr)
		: expression(expr), RuntimeException(expr->GetPosition()) {};
};

// ���������� ��� ��������� �� ���������� ����������� ���� ��������
class ExpressionIsNotValueException : public ExpressionException
{
public:
	ExpressionIsNotValueException(Expression* expr) : ExpressionException(expr) {}
	virtual std::string What() const;
};

// ���������� ��� ��������� �� ���������� �����������, 
// ������� ����� �������������� � �������� ����������� ��������
class ExpressionIsNotCallableException : public ExpressionException
{
public:
	ExpressionIsNotCallableException(Expression* expr) : ExpressionException(expr) {}
	virtual std::string What() const;
};

// ���������� ��� ��������� � ����������� �����
class UnknownExpressionException : public ExpressionException
{
public:
	UnknownExpressionException(Expression* expr) : ExpressionException(expr) {}
	virtual std::string What() const;
};

// ������� ����� ��� ����������, ������� ����� ��������� �� ����� ������� ���������
class ParserException : public InterpreterException
{
	// ��� ����� ���������� ���������� ����� � �������� 
	// ������ ����������� ���������, � ������� ��� �������
protected:
	PositionInText position;
public:
	ParserException(const PositionInText& position) : position(position) {};
	const PositionInText& GetPosition() const;
};

// ���������� ������ ������� ��������� ���� <block>,
// ��������� � ����������� � �� ��������� ���������
class EmptyBlockException : public ParserException
{
public:
	EmptyBlockException(const PositionInText& position) : ParserException(position) {}
	virtual std::string What() const;
};

// ����������, ������������, ����� ����������� ���������� �������� �� ����
// ����������� ��� ������
class UnexpectedCharacterException : public ParserException
{
	int ch;
public:
	UnexpectedCharacterException(int ch, const PositionInText& position) : ch(ch), ParserException(position) {}
	virtual std::string What() const;
};

// ���������� ������������ ����� ����� ��� �������������� �������
class UnexpectedEndOfFileException : public ParserException
{
public:
	UnexpectedEndOfFileException(const PositionInText& position) : ParserException(position) {};
	virtual std::string What() const;
};

// ����������, ����������� �����, ����� �������������� ����������
// ��� ������ �������� �� �� �������, ������� ������
class UnexpectedTokenException : public ParserException
{
protected:
	const Token* unexpectedToken;
	const std::type_info& expectedType;

	std::string GetExpectedString() const;
public:
	UnexpectedTokenException(const Token* unexpectedToken, const std::type_info& expected, const PositionInText& position)
		: unexpectedToken(unexpectedToken), expectedType(expected), ParserException(position) {};
	virtual std::string What() const;
	const Token* GetToken() const;
	const std::type_info& GetExpectedType() const;
};

// ����������, ����������� �����, ����� �������������� ����������
// ��� ������ �������� �� �� �������, ������� ������,
// � ��������� �������� �������� ��������� �������� �����.
class UnexpectedTokenWithExpectedKeywordException : public UnexpectedTokenException
{
	const std::string expectedKeyword;
public:
	UnexpectedTokenWithExpectedKeywordException(const Token* unexpectedToken, const std::string& expected, const PositionInText& position)
		: expectedKeyword(expected), UnexpectedTokenException(unexpectedToken, typeid(KeywordToken), position) {};
	virtual std::string What() const;
};