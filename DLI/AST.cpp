#include "AST.h"

AddExpression::~AddExpression()
{
	delete left;
	delete right;
}

Expression* AddExpression::GetLeftOperand() const
{
	return left;
}

Expression* AddExpression::GetRightOperand() const
{
	return right;
}

Expression* AddExpression::Clone()
{
	return (Expression*) new AddExpression(left->Clone(), right->Clone());
}

std::string AddExpression::ToString()
{
	return "(add " + left->ToString() + " " + right->ToString() + ")";
}

IfExpression::~IfExpression()
{
	delete left;
	delete right;
	delete trueBranch;
	delete elseBranch;
}

Expression* IfExpression::GetLeftOperand() const
{
	return left;
}

Expression* IfExpression::GetRightOperand() const
{
	return right;
}

Expression* IfExpression::GetTrueBranach() const
{
	return trueBranch;
}

Expression* IfExpression::GetElseBranch() const
{
	return elseBranch;
}

Expression* IfExpression::Clone()
{
	return (Expression*)new IfExpression(
		left->Clone(),
		right->Clone(),
		trueBranch->Clone(),
		elseBranch->Clone()
	);
}

std::string IfExpression::ToString()
{
	return "(if " + left->ToString() + " " + right->ToString() + " then " + trueBranch->ToString() + " else " + elseBranch->ToString() + ")";
}

LetExpression::~LetExpression()
{
	delete expression;
	delete body;
}

std::string LetExpression::GetId() const
{
	return id;
}

Expression* LetExpression::GetExpression() const
{
	return expression;
}

Expression* LetExpression::GetBody() const
{
	return body;
}

Expression* LetExpression::Clone()
{
	return (Expression*)new LetExpression(id, expression->Clone(), body->Clone());
}

std::string LetExpression::ToString()
{
	return "(let " + id + " = " + expression->ToString() + " in " + body->ToString() + ")";
}

FunctionExpression::~FunctionExpression()
{
	delete body;
}

std::string FunctionExpression::GetId() const
{
	return id;
}

Expression* FunctionExpression::GetBody() const
{
	return body;
}

Expression* FunctionExpression::Clone()
{
	return (Expression*) new FunctionExpression(id, body->Clone());
}

std::string FunctionExpression::ToString()
{
	return "(function " + id + " " + body->ToString() + ")";
}

CallExpression::~CallExpression()
{
	delete callable;
	delete argument;
}

Expression* CallExpression::GetCallable() const
{
	return callable;
}

Expression* CallExpression::GetArgument() const
{
	return argument;
}

Expression* CallExpression::Clone()
{
	return (Expression*) new CallExpression(callable->Clone(), argument->Clone());
}

std::string CallExpression::ToString()
{
	return "(call " + callable->ToString() + " " + argument->ToString() + ")";
}

void BlockExpression::AddExpression(Expression * expr)
{
	expressions.push_back(expr);
}

BlockExpression::~BlockExpression()
{
	auto it = expressions.begin();
	while (it != expressions.end())
	{
		delete *it;
		it++;
	}
}

std::list<Expression*> BlockExpression::GetExpressions() const
{
	return expressions;
}

Expression* BlockExpression::Clone()
{
	auto blck = new BlockExpression();
	for (auto p : expressions)
	{
		blck->AddExpression(p->Clone());
	}
	return (Expression*)blck;
}

std::string BlockExpression::ToString()
{
	std::string blockValues;
	for (auto expr : expressions)
	{
		blockValues += expr->ToString() + " ";
	}
	return "(block " + blockValues + ")";
}

int ValExpression::GetValue() const
{
	return value;
}

Expression* ValExpression::Clone()
{
	return (Expression*) new ValExpression(value);
}

std::string ValExpression::ToString()
{
	return "(val " + std::to_string(value) + ")";
}

std::string VarExpression::GetId() const
{
	return id;
}

Expression* VarExpression::Clone()
{
	return (Expression*) new VarExpression(id);
}

std::string VarExpression::ToString()
{
	return "(var " + id + ")";
}

SetExpression::~SetExpression()
{
	delete expression;
}

std::string SetExpression::GetId() const
{
	return id;
}

Expression* SetExpression::GetExpression() const
{
	return expression;
}

Expression* SetExpression::Clone()
{
	return (Expression*) new SetExpression(id, expression->Clone());
}

std::string SetExpression::ToString()
{
	return "(set " + id + " " + expression->ToString() + ")";
}

Expression::~Expression()
{
}

Expression* Expression::Clone()
{
	return new Expression();
}

std::string Expression::ToString()
{
	return "(expr)";
}
