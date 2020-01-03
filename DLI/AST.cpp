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
	return (Expression*) new AddExpression(left->Clone(), right->Clone(), GetPosition());
}

std::string AddExpression::ToString()
{
	return "(add " + left->ToString() + " " + right->ToString() + ")";
}

IfExpression::~IfExpression()
{
	delete left;
	delete right;
	delete thenBranch;
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

Expression* IfExpression::GetThenBranch() const
{
	return thenBranch;
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
		thenBranch->Clone(),
		elseBranch->Clone(),
		GetPosition()
	);
}

std::string IfExpression::ToString()
{
	return "(if " + left->ToString() + " " + right->ToString() + " then " + thenBranch->ToString() + " else " + elseBranch->ToString() + ")";
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
	return (Expression*)new LetExpression(id, expression->Clone(), body->Clone(), GetPosition());
}

std::string LetExpression::ToString()
{
	return "(let " + id + " = " + expression->ToString() + " in " + body->ToString() + ")";
}

FunctionExpression::~FunctionExpression()
{
	delete body;
}

std::string FunctionExpression::GetArgument() const
{
	return argument;
}

Expression* FunctionExpression::GetBody() const
{
	return body;
}

Expression* FunctionExpression::Clone()
{
	return (Expression*) new FunctionExpression(argument, body->Clone(), GetPosition());
}

std::string FunctionExpression::ToString()
{
	return "(function " + argument + " " + body->ToString() + ")";
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
	return (Expression*) new CallExpression(callable->Clone(), argument->Clone(), GetPosition());
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
	auto blck = new BlockExpression(GetPosition());
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
	return (Expression*) new ValExpression(value, GetPosition());
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
	return (Expression*) new VarExpression(id, GetPosition());
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
	return (Expression*) new SetExpression(id, expression->Clone(), GetPosition());
}

std::string SetExpression::ToString()
{
	return "(set " + id + " " + expression->ToString() + ")";
}

const PositionInText& Expression::GetPosition() const
{
	return position;
}

Expression::~Expression()
{
}

Expression* Expression::Clone()
{
	return new Expression(GetPosition());
}

std::string Expression::ToString()
{
	return "(expr)";
}
