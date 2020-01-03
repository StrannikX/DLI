#pragma once
# include "AST.h"
# include <unordered_map>
# include <stack>

// �������� ��� ����������� � ��������������� �� ��������� � ���� �������

class Scope;

// ���������� FunctionExpression, ��������� �������� ������� ���������,
// � ������� ���������� �������
class ClosureExpression : public FunctionExpression
{
	Scope* scope; // ���������� ������� ���������
public:
	ClosureExpression(FunctionExpression* func, Scope* scope) : FunctionExpression(func->GetArgument(), func->GetBody()->Clone()), scope(scope) {}
	Scope* GetScope();
};

class Evaluator 
{
	std::stack<Scope*> scopeStack; // ���� �������� ���������

public:
	Evaluator(Scope * global);
	Evaluator();
	~Evaluator();

	Expression* Eval(Expression*);	// ��������� ���������

protected:
	Scope* CurrentScope() const; // ������� ������� ���������, ������� �����
	void PushScope(Scope*); // �������� ������� ��������� �� ������� �����
	Scope* PopScope(); // ������� ������� ��������� � �������

	// ������ ��� ���������� ���������� ���������
	Expression* Eval(ValExpression*);
	Expression* Eval(VarExpression*);
	Expression* Eval(AddExpression*);
	Expression* Eval(IfExpression*);
	Expression* Eval(LetExpression*);
	Expression* Eval(FunctionExpression*);
	Expression* Eval(CallExpression*);
	Expression* Eval(SetExpression*);
	Expression* Eval(BlockExpression*);
	Expression* Eval(ClosureExpression*);

	int GetValue(Expression*); // �������� �������� ���������
	Expression* FromEnv(std::string&); // �������� �������� �� ������� ������� ���������
	bool TrySetInEnv(std::string&, Expression*); // ������� ���������� �������� � ������� ������� ���������
};

// ����� - ���������� ������� ���������
class Scope
{
	Scope * parentScope = nullptr; // ������������ ������� ���������
	std::unordered_map<std::string, Expression*> values; // ��������, ���������� � ������� ���������
public:
	Scope() {}
	Scope(Scope* parent) : parentScope(parent) {}
	~Scope();

	// �������� �������� �� ������� ������� ���������
	// ���� �������� �� ����� �������, ����� ��������� ����������
	const Expression* GetValue(const std::string& key);

	// ������� ����� �������� � ������� ������� ���������
	void AddValue(const std::string& key, Expression* value);

	// ���������� �������� � ������� ������� ���������
	// ���� �������� �� ����� �������, ����� ��������� ����������
	void SetValue(const std::string& key, Expression* value);

	// ������� �������� �������� �� ������� ������� ���������
	// �������� ����������� ����� ��������� value
	// ���� ������, ������� ����� true, ����� false
	bool TryGet(const std::string& key, Expression** value);

	// ������� ���������� �������� � ������� ������� ���������
	// ���� ������, ������� ����� true, ����� false
	bool TrySet(const std::string& key, Expression* value);

	// ������������ ������� ���������
	Scope* GetParent();
};