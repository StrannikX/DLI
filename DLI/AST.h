#pragma once



# include <string>
# include <list>

class Expression 
{
public:
	virtual ~Expression();
	virtual Expression* Clone();
	virtual std::string ToString();
};

class ValExpression : public Expression
{
	int value;
public:
	ValExpression(int val) : value(val) {};
	int GetValue() const;
	virtual Expression* Clone();
	virtual std::string ToString();
};

class VarExpression : public Expression
{
	std::string id;
public:
	VarExpression(std::string& str) : id(str) {};
	std::string GetId() const;
	virtual Expression* Clone();
	virtual std::string ToString();
};

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

class IfExpression : public Expression
{
	Expression * left;
	Expression * right;
	Expression * trueBranch;
	Expression * elseBranch;
public:
	IfExpression(Expression* left, Expression* right, Expression* trueBranch, Expression* elseBranch) :
		left(left), right(right), trueBranch(trueBranch), elseBranch(elseBranch) {};
	~IfExpression();

	Expression * GetLeftOperand() const;
	Expression * GetRightOperand() const;
	Expression * GetTrueBranach() const;
	Expression * GetElseBranch() const;
	virtual Expression* Clone();
	virtual std::string ToString();
};

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

class FunctionExpression : public Expression
{
	std::string id;
	Expression * body;
public:
	FunctionExpression(const std::string& id, Expression* body) : id(id), body(body) {};
	~FunctionExpression();

	std::string GetId() const;
	Expression * GetBody() const;
	virtual Expression* Clone();
	virtual std::string ToString();
};

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