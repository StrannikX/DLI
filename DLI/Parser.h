#pragma once

# include "Token.h"
# include "AST.h"
# include <string>
# include <vector>
# include <list>
# include <exception>
#include <memory>


class Parser
{
	std::list<Token*> tokens;
	std::list<Expression*> expressions;
	std::list<Token*>::iterator it;
	std::list<Token*>::iterator end;
protected:
	template<class T> T* GetToken();
	KeywordToken* GetKeyword(const std::string&);
	Token* NextToken();

	ValExpression* ParseValExpression();
	BlockExpression* ParseBlockExpression();
	LetExpression* ParseLetExpression();
	VarExpression* ParseVarExpression();
	AddExpression* ParseAddExpression();
	IfExpression* ParseIfxpression();
	FunctionExpression* ParseFunctionExpression();
	CallExpression* ParseCallExpression();
	SetExpression* ParseSetExpression();
public:
	Parser(std::list<Token*>& tokens);
	Expression* Parse();
	Expression* ParseExpression();
};

template<class T> inline T* Parser::GetToken()
{
	auto token = NextToken();
	T* t = dynamic_cast<T*>(token);
	if (t) return t;
	throw std::exception((std::string("Unexpected token: ") + token->ToString()).c_str());
}
