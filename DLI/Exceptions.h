#pragma once

# include <string>
# include <typeinfo>
# include "Position.h"
# include "Token.h"
#include "AST.h"

// Исключения интепретатора

// Базовый класс исключений интепретатора
class InterpreterException
{
public:
	// Сообщение, что произошло
	virtual std::string What() const = 0;
};

// Базовый класс для исключений времени исполнения
class RuntimeException : public InterpreterException 
{
protected:
	// Исключения времени исполнения могут как иметь
	// Ассоциированную с ними позицию в исходном коде
	// Программы, так и не иметь

	// Позиция в коде
	PositionInText position;
	// Установлена ли позиция
	bool positionIsSet = false;

	// Префикс в зависимости от того, есть ли позиция для исключения в коде
	std::string GetPositionPrefix() const;
public:
	RuntimeException() {}
	RuntimeException(const PositionInText& position) : position(position), positionIsSet(true) {};

	// Установить позицию в коде, в котором произошло исключение
	void SetPosition(const PositionInText& position);
};

// Исключение не найденной переменной
class UndefinedVariableException : public RuntimeException
{
	std::string variable;
public:
	UndefinedVariableException(const std::string& var) : variable(var) {};
	UndefinedVariableException(const std::string& var, const PositionInText& position) : variable(var), RuntimeException(position) {};
	virtual std::string What() const;
};

// Базовый класс для исключений, связанных с неверными типами AST выражений
class ExpressionException : public RuntimeException
{
protected:
	Expression* expression;
public:
	ExpressionException(Expression* expr)
		: expression(expr), RuntimeException(expr->GetPosition()) {};
};

// Исключение для выражений не являющихся выражениями типа значений
class ExpressionIsNotValueException : public ExpressionException
{
public:
	ExpressionIsNotValueException(Expression* expr) : ExpressionException(expr) {}
	virtual std::string What() const;
};

// Исключение для выражений не являющихся выражениями, 
// которые могут использоваться в качестве вызываемого значения
class ExpressionIsNotCallableException : public ExpressionException
{
public:
	ExpressionIsNotCallableException(Expression* expr) : ExpressionException(expr) {}
	virtual std::string What() const;
};

// Исключение для выражений с неизвестным типом
class UnknownExpressionException : public ExpressionException
{
public:
	UnknownExpressionException(Expression* expr) : ExpressionException(expr) {}
	virtual std::string What() const;
};

// Базовый класс для исключений, которые могут произойти во время разбора программы
class ParserException : public InterpreterException
{
	// Для таких исключений существует место в исходном 
	// Тексте разбираемой программы, в которым они вызваны
protected:
	PositionInText position;
public:
	ParserException(const PositionInText& position) : position(position) {};
	const PositionInText& GetPosition() const;
};

// Исключение ошибка разбора выражения типа <block>,
// Связанная с отсутствием в нём вложенных выражений
class EmptyBlockException : public ParserException
{
public:
	EmptyBlockException(const PositionInText& position) : ParserException(position) {}
	virtual std::string What() const;
};

// Исключение, происходящее, когда лексический анализатор получает на вход
// неизвестный ему символ
class UnexpectedCharacterException : public ParserException
{
	int ch;
public:
	UnexpectedCharacterException(int ch, const PositionInText& position) : ch(ch), ParserException(position) {}
	virtual std::string What() const;
};

// Исключение неожиданного конца ввода при синтаксическом анализе
class UnexpectedEndOfFileException : public ParserException
{
public:
	UnexpectedEndOfFileException(const PositionInText& position) : ParserException(position) {};
	virtual std::string What() const;
};

// Исключение, возникающее тогда, когда синтаксический анализатор
// при чтении получает не ту лексему, которую ожидал
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

// Исключение, возникающее тогда, когда синтаксический анализатор
// при чтении получает не ту лексему, которую ожидал,
// и ожидаемой лексемой является кокретное ключевое слово.
class UnexpectedTokenWithExpectedKeywordException : public UnexpectedTokenException
{
	const std::string expectedKeyword;
public:
	UnexpectedTokenWithExpectedKeywordException(const Token* unexpectedToken, const std::string& expected, const PositionInText& position)
		: expectedKeyword(expected), UnexpectedTokenException(unexpectedToken, typeid(KeywordToken), position) {};
	virtual std::string What() const;
};