#include "Evaluator.h"
#include <stdexcept>

// ��� �������� Scope
// ��������� ��� ���������� � �� Expression
Scope::~Scope()
{
	for (auto p : values)
	{
		auto expr = p.second;
		if (expr != nullptr)
		{
			delete expr;
		}
	}
}

// �������� �������� �� Scope
// ���� ��� ��� ����� ��������� ����������
const Expression* Scope::GetValue(const std::string& key)
{
	try {
		return values.at(key);
	}
	catch (std::out_of_range ex) {
		throw std::exception("Unknown key");
	}
}

// �������� �������� � Scope
void Scope::AddValue(const std::string& key, Expression* value)
{
	values.insert(std::pair<const std::string, Expression*>(key, value));
}

// ���������� �������� � Scope
// ���� ��� ��� ����� ��������� ����������
void Scope::SetValue(const std::string& key, Expression* value)
{
	try {
		static_cast<void>(values.at(key));
		values[key] = value;
	}
	catch (std::out_of_range ex) {
		throw std::exception("Unknown key");
	}
}

// ������� �������� �������� �� Scope, �� ��������� ����������
// �������� ����� ��������� � ������� ��������� value
// ������������ �������� ������� ����������, ������� �� ���� ��������
bool Scope::TryGet(const std::string& key, Expression** value)
{
	if (values.count(key) != 0) {
		*value = values.at(key);
		return true;
	}
	*value = nullptr;
	return false;
}

// ������� ���������� �������� � Scope, �� ��������� ����������
// ������������ �������� ������� ����������, ������� �� ���� ��������
bool Scope::TrySet(const std::string& key, Expression* value)
{
	if (values.count(key) != 0) {
		values[key] = value;
		return true;
	}
	return false;
}

// �������� ������������ ������� ���������
Scope* Scope::GetParent()
{
	return parentScope;
}

// ������� ����� ����������� � �������� ���������� �������� ���������
Evaluator::Evaluator(Scope* global)
{
	scopeStack.push(new Scope(global));
}

// ������� ����� �����������
Evaluator::Evaluator()
{
	scopeStack.push(new Scope());
}

Evaluator::~Evaluator()
{
	delete PopScope();
}

// �������� �������� ���������.
// ���� ��������� �� �������� <val>,
// �� ����� ������� ����������
int Evaluator::GetValue(Expression * expression)
{
	auto valExpression = dynamic_cast<ValExpression*>(expression);
	if (valExpression) return valExpression->GetValue();
	throw std::exception("Expression isn't value expression");
}

// �������� �������� �� ������� ��� ����������� �������� ���������
Expression* Evaluator::FromEnv(std::string& key)
{
	Expression* expr = nullptr;
	Scope* scope;

	// ���������� �� ������� �������� ���������
	// �� �������� � ������������
	for (scope = CurrentScope(); scope != nullptr; scope = scope->GetParent())
	{
		// �������� �������� ��������
		if (scope->TryGet(key, &expr)) break;
	}
	// ���� �������� �������
	// ���������� ��� �����
	if (scope != nullptr) return expr->Clone();
	// ����� ������� ����������
	std::string str = "Unknown key " + key;
	throw std::exception(str.c_str());
}

// ���������� �������� � ������� ������� ��������� ��� �����������
bool Evaluator::TrySetInEnv(std::string& key, Expression* expr)
{
	Scope* scope;
	// ���������� �� ������� �������� ���������
	// �� �������� � ������������
	for (scope = CurrentScope(); scope != nullptr; scope = scope->GetParent())
	{
		// �������� ���������� ��������
		if (scope->TrySet(key, expr)) return true;
	}
	return false;
}

// ��������� ���������
Expression* Evaluator::Eval(Expression* expr)
{
	// ���������� ��� ���������
	// � �������� ��� ���� ��������������� �������
	if (auto valExpr = dynamic_cast<ValExpression*>(expr))
		return Eval(valExpr);
	
	if (auto varExpr = dynamic_cast<VarExpression*>(expr)) 
		return Eval(varExpr);

	if (auto addExpr = dynamic_cast<AddExpression*>(expr)) 
		return Eval(addExpr);

	if (auto ifExpr = dynamic_cast<IfExpression*>(expr)) 
		return Eval(ifExpr);

	if (auto letExpr = dynamic_cast<LetExpression*>(expr)) 
		return Eval(letExpr);

	if (auto funcExpr = dynamic_cast<FunctionExpression*>(expr)) 
		return Eval(funcExpr);

	if (auto closureExpr = dynamic_cast<ClosureExpression*>(expr))
		return Eval(closureExpr);

	if (auto callExpr = dynamic_cast<CallExpression*>(expr))
		return Eval(callExpr);

	if (auto setExpr = dynamic_cast<SetExpression*>(expr)) 
		return Eval(setExpr);

	if (auto blockExpr = dynamic_cast<BlockExpression*>(expr)) 
		return Eval(blockExpr);

	throw std::exception("Unknown expression type");
}

// ��������� ��������� <val>
// ������ �������� ���
Expression* Evaluator::Eval(ValExpression* val)
{
	return (ValExpression*) val->Clone();
}

// ��������� ��������� <var>
Expression* Evaluator::Eval(VarExpression* var)
{
	auto id = var->GetId();
	// �������� �������� ���������� �� � ��������������
	return FromEnv(id);
}

// ��������� ��������� <add>
Expression* Evaluator::Eval(AddExpression* expr)
{
	// ������� �������� ���������
	auto left = GetValue(Eval(expr->GetLeftOperand()));
	auto right = GetValue(Eval(expr->GetRightOperand()));
	// � ��������� ��������
	return new ValExpression(left + right);
}

// ��������� ��������� <if>
Expression* Evaluator::Eval(IfExpression* expr)
{
	// �������� �������� ���������
	auto val1 = GetValue(Eval(expr->GetLeftOperand()));
	auto val2 = GetValue(Eval(expr->GetRightOperand()));
	// ���������� ��, �������� ��������� � ��������� ���
	return Eval(val1 > val2 ? expr->GetThenBranch() : expr->GetElseBranch());
}

// ��������� �������� <let>
Expression* Evaluator::Eval(LetExpression* expr)
{
	// �������� ������������� ����������
	auto id = expr->GetId(); 
	// � � ��������
	auto value = Eval(expr->GetExpression());
	// ������ ����� ������� ���������, � ������� � �������� ������������
	Scope* scope = new Scope(CurrentScope());
	// �������� � � ����
	PushScope(scope);
	// � ������ � ��� ����������
	scope->AddValue(id, value);
	// ����� ��������� � ��� ���� ���������
	auto result = Eval(expr->GetBody());
	// � ������� ������� ���������
	delete PopScope();
	return result;
}

// ��������� ��������� <function>
Expression* Evaluator::Eval(FunctionExpression* func)
{
	// �������� ������� ���������, � ������� ������� ������������
	return new ClosureExpression(func, CurrentScope());
}

// ��������� ��������� <call>
Expression* Evaluator::Eval(CallExpression* expr)
{
	// �������� ���� �������
	auto res = Eval(expr->GetCallable());
	// � ���������, ������������� �� ��� ����
	auto collable = dynamic_cast<FunctionExpression*>(res);
	if (!collable) throw std::exception("Can't call not function expression");
	// ���������, �������� �� ��� ����������
	auto closure = dynamic_cast<ClosureExpression*>(res);

	// �������� �������� ���������
	auto argument = Eval(expr->GetArgument());

	// �������� ������������ ������� ���������
	// ���� ��������� - ���������, �� �� ����� ������� ���������,
	// ��� ������� ����������, ���� ���, �� ��, � ������� ���������� �����
	Scope* parentScope = closure ? closure->GetScope() : CurrentScope();
	
	// ������ ������� ���������, � ������� ����� ����������� ���� �������
	Scope* scope = new Scope(parentScope);
	// ������ � ��� ����������-��������
	scope->AddValue(collable->GetArgument(), argument);
	// � ��������� � ���� �������� ���������
	PushScope(scope);

	// ����� ��������� � ��� ���� �������
	auto result = Eval(collable->GetBody());

	// � ����� ������� ������� ���������
	delete PopScope();
	return result;
}

// ��������� ��������� <set>
Expression* Evaluator::Eval(SetExpression* expr)
{
	auto id = expr->GetId();
	auto value = Eval(expr->GetExpression());
	if (!TrySetInEnv(id, value))
	{
		std::string message = "Variable doesn't exists ";
		message += id;
		throw std::exception(message.c_str());
	}
	return new Expression();
}

// ��������� ��������� <block>
Expression* Evaluator::Eval(BlockExpression* block)
{
	Expression* result = nullptr; // ��������� ���������� �����
	Scope* scope = new Scope(CurrentScope()); // ������ ������� ��������� ��� �����
	PushScope(scope);
	// ��� ������� ��������� � �����
	for (auto expr : block->GetExpressions())
	{
		// ���� ��� �� ���������, �� ������� ���������� ���������
		if (result) delete result;
		// � ��������� ���������
		result = Eval(expr);
	}
	delete PopScope();
	return result;
}

// ��������� ��������� <closure>
// ������ ������ �����
Expression* Evaluator::Eval(ClosureExpression* expression)
{
	return expression->Clone();
}

// ������� ������� ��������� - ������� ����� ��������
Scope* Evaluator::CurrentScope() const
{
	return scopeStack.top();
}

// �������� ����� ������� ��������� � ����
void Evaluator::PushScope(Scope* scope)
{
	scopeStack.push(scope);
}

// ������� ������� ����� �������� ���������
Scope* Evaluator::PopScope()
{
	auto result = scopeStack.top();
	scopeStack.pop();
	return result;
}

// �������� ������� ���������, � ������� ���������� ������� 
Scope* ClosureExpression::GetScope()
{
	return scope;
}
