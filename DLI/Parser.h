#pragma once

# include "Token.h"
# include "AST.h"
# include <string>
# include <vector>
# include <list>
# include <exception>
#include <memory>

// �������������� ����������
// ������ AST �� ������������������ ������
class Parser
{
	std::list<Token*> tokens;           // �������� �������
	std::list<Token*>::iterator it;     // �������� ������� �������
	std::list<Token*>::iterator end;    // ����� ������ ������
protected:
	// ������� �������� ��������� ������� ���� T
	// ���� ������� �����������, ��� ����� ��� �������� �� T, �� ����� ��������� ����������
	template<class T> T* GetToken();

	// ������� �������� ��������� ������� ��������������� ��������� ����� keyword
	// ���� ������� �����������, ����� ��� �������� �� KeywordToken ��� �������� �������� �� keyword,
	// ����� ��������� ����������
	KeywordToken* GetKeyword(const std::string& keyword);

	// ������� �������� ��������� �������
	// ���� ������� ����������� - ����� ��������� ����������
	Token* NextToken();

	// ������ ������� �������������� ����������� �����
	ValExpression* ParseValExpression();
	BlockExpression* ParseBlockExpression();
	LetExpression* ParseLetExpression();
	VarExpression* ParseVarExpression();
	AddExpression* ParseAddExpression();
	IfExpression* ParseIfxpression();
	FunctionExpression* ParseFunctionExpression();
	CallExpression* ParseCallExpression();
	SetExpression* ParseSetExpression();
	// ������� ����� ������������ ������
	Expression* ParseExpression();
public:
	Parser(std::list<Token*>& tokens);
	Expression* Parse();
};

// ������� �������� ��������� ������� ���� T
template<class T> inline T* Parser::GetToken()
{
	auto token = NextToken();
	T* t = dynamic_cast<T*>(token);
	if (t) return t;
	throw std::exception((std::string("Unexpected token: ") + token->ToString()).c_str());
}
