#pragma once


# include <string>
# include <list>

// Классы представляющие элементы абстрактного синтаксического дерева
// для синтаксических конструкций языка

// Базовый класс для выражений AST
class Expression 
{
public:
	virtual ~Expression();         
	virtual Expression* Clone();    // Создать копию узла
	virtual std::string ToString(); // Представить узел в виде строки
};

// Класс для конструкции (val <value>)
class ValExpression : public Expression
{
	int value;
public:
	ValExpression(int val) : value(val) {};
	int GetValue() const;
	virtual Expression* Clone();
	virtual std::string ToString();
};

// Класс для конструкции (var <id>)
class VarExpression : public Expression
{
	std::string id;
public:
	VarExpression(std::string& str) : id(str) {};
	std::string GetId() const;
	virtual Expression* Clone();
	virtual std::string ToString();
};

// Класс для конструкции (add <left> <right>)
class AddExpression : public Expression
{
	Expression * left;
	Expression * right;
public:
	AddExpression(Expression* left, Expression* right) : left(left), right(right) {};
	~AddExpression();

	Expression * GetLeftOperand() const;
	Expression * GetRightOperand() const;
	virtual Expression* Clone();
	virtual std::string ToString();
};

// Класс для конструкции (if <left> <right> then <then_branch> <else_branch>)
class IfExpression : public Expression
{
	Expression * left;
	Expression * right;
	Expression * thenBranch;
	Expression * elseBranch;
public:
	IfExpression(Expression* left, Expression* right, Expression* thenBranch, Expression* elseBranch) :
		left(left), right(right), thenBranch(thenBranch), elseBranch(elseBranch) {};
	~IfExpression();

	Expression * GetLeftOperand() const;
	Expression * GetRightOperand() const;
	Expression * GetThenBranch() const;
	Expression * GetElseBranch() const;
	virtual Expression* Clone();
	virtual std::string ToString();
};

// Класс для конструкции (let <id> <expression> in <body>)
class LetExpression : public Expression
{
	std::string id;
	Expression * expression;
	Expression * body;
public:
	LetExpression(const std::string& id, Expression* expression, Expression* body) :
		id(id), expression(expression), body(body) {};
	~LetExpression();

	std::string GetId() const;
	Expression * GetExpression() const;
	Expression * GetBody() const;
	virtual Expression* Clone();
	virtual std::string ToString();
};

// Класс для конструкции (function <arg> <body>)
class FunctionExpression : public Expression
{
	std::string argument;
	Expression * body;
public:
	FunctionExpression(const std::string& arg, Expression* body) : argument(argument), body(body) {};
	~FunctionExpression();

	std::string GetArgument() const;
	Expression * GetBody() const;
	virtual Expression* Clone();
	virtual std::string ToString();
};

// Класс для конструкции (call <callable> <argument>
class CallExpression : public Expression
{
	Expression * callable;
	Expression * argument;
public:
	CallExpression(Expression* callable, Expression* argument) : callable(callable), argument(argument) {};
	~CallExpression();

	Expression * GetCallable() const;
	Expression * GetArgument() const;
	virtual Expression* Clone();
	virtual std::string ToString();
};

// Класс для конструкции (block <expressions>+)
class BlockExpression : public Expression
{
	std::list<Expression*> expressions;
public:
	void AddExpression(Expression*);
	~BlockExpression();

	std::list<Expression*> GetExpressions() const;
	virtual Expression* Clone();
	virtual std::string ToString();
};

// Класс для конструкции (set <id> <expression>)
class SetExpression : public Expression
{
	std::string id;
	Expression* expression;
public:
	SetExpression(const std::string& id, Expression* expression) :
		id(id), expression(expression) {};
	~SetExpression();

	std::string GetId() const;
	Expression * GetExpression() const;
	virtual Expression* Clone();
	virtual std::string ToString();
};