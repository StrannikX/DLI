#include "Evaluator.h"
#include <stdexcept>

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

const Expression* Scope::GetValue(const std::string& key)
{
	try {
		return values.at(key);
	}
	catch (std::out_of_range ex) {
		throw std::exception("Unknown key");
	}
}

void Scope::AddValue(const std::string& key, Expression* value)
{
	values.insert(std::pair<const std::string, Expression*>(key, value));
}

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

bool Scope::TryGet(const std::string& key, Expression** value)
{
	if (values.count(key) != 0) {
		*value = values.at(key);
		return true;
	}
	return false;
}

bool Scope::TrySet(const std::string& key, Expression* value)
{
	return false;
}

Scope* Scope::GetParent()
{
	return parentScope;
}

Evaluator::Evaluator(Scope* global)
{
	scopeStack.push(new Scope(global));
}

Evaluator::Evaluator()
{
	scopeStack.push(new Scope());
}

Evaluator::~Evaluator()
{
	delete PopScope();
}

int Evaluator::GetValue(Expression * expression)
{
	auto valExpression = dynamic_cast<ValExpression*>(expression);
	if (valExpression) return valExpression->GetValue();
	throw std::exception("Expression isn't value expression");
}

Expression* Evaluator::FromEnv(std::string& key)
{
	Expression* expr = nullptr;
	Scope* scope;
	for (scope = CurrentScope(); scope != nullptr; scope = scope->GetParent())
	{
		if (scope->TryGet(key, &expr)) break;
	}
	if (scope != nullptr) return expr->Clone();
	std::string str = "Unknown key " + key;
	throw std::exception(str.c_str());
}

Expression* Evaluator::Eval(Expression* expr)
{
	auto valExpr = dynamic_cast<ValExpression*>(expr);
	if (valExpr) return (Expression *) Eval(valExpr);
	
	auto varExpr = dynamic_cast<VarExpression*>(expr);
	if (varExpr) return Eval(varExpr);

	auto addExpr = dynamic_cast<AddExpression*>(expr);
	if (addExpr) return (Expression*)Eval(addExpr);

	auto ifExpr = dynamic_cast<IfExpression*>(expr);
	if (ifExpr) return Eval(ifExpr);

	auto letExpr = dynamic_cast<LetExpression*>(expr);
	if (letExpr) return Eval(letExpr);

	auto funcExpr = dynamic_cast<FunctionExpression*>(expr);
	if (funcExpr) return (Expression*)Eval(funcExpr);

	auto callExpr = dynamic_cast<CallExpression*>(expr);
	if (callExpr) return (Expression*)Eval(callExpr);

	auto setExpr = dynamic_cast<SetExpression*>(expr);
	if (setExpr) return (Expression*)Eval(setExpr);

	auto blockExpr = dynamic_cast<BlockExpression*>(expr);
	if (blockExpr) return Eval(blockExpr);

	throw std::exception("Unknown expression type");
}

ValExpression* Evaluator::Eval(ValExpression* val)
{
	return (ValExpression*) val->Clone();
}

Expression* Evaluator::Eval(VarExpression* var)
{
	auto id = var->GetId();
	return FromEnv(id);
}

ValExpression* Evaluator::Eval(AddExpression* expr)
{
	auto leftExpr = Eval(expr->GetLeftOperand());
	auto rightExpr = Eval(expr->GetRightOperand());
	auto value = GetValue(leftExpr) + GetValue(rightExpr);
	return new ValExpression(value);
}

Expression* Evaluator::Eval(IfExpression* expr)
{
	auto val1 = GetValue(Eval(expr->GetLeftOperand()));
	auto val2 = GetValue(Eval(expr->GetRightOperand()));
	return Eval(val1 > val2 ? expr->GetTrueBranach() : expr->GetElseBranch());
}

Expression* Evaluator::Eval(LetExpression* expr)
{
	auto id = expr->GetId();
	auto value = Eval(expr->GetExpression());
	Scope* scope = new Scope(CurrentScope());
	PushScope(scope);
	CurrentScope()->AddValue(id, value);
	auto result = Eval(expr->GetBody());
	delete PopScope();
	return result;
}

ClosureExpression* Evaluator::Eval(FunctionExpression* func)
{
	return new ClosureExpression(func, CurrentScope());
}


Expression* Evaluator::Eval(CallExpression* expr)
{
	auto res = Eval(expr->GetCallable());
	auto collable = dynamic_cast<FunctionExpression*>(res);
	if (!collable) throw std::exception("Can't call not function expression");
	auto closure = dynamic_cast<ClosureExpression*>(res);

	auto argument = Eval(expr->GetArgument());

	Scope* parentScope = closure ? closure->GetScope() : CurrentScope();
	Scope* scope = new Scope(parentScope);
	scope->AddValue(collable->GetId(), argument);

	PushScope(scope);

	auto result = Eval(collable->GetBody());

	delete PopScope();
	return result;
}

Expression* Evaluator::Eval(SetExpression* expr)
{
	auto id = expr->GetId();
	auto value = Eval(expr->GetExpression());
	if (!CurrentScope()->TrySet(id, value))
	{
		std::string message = "Variable doesn't exists ";
		message += id;
		throw std::exception(message.c_str());
	}
	return new Expression();
}

Expression* Evaluator::Eval(BlockExpression* block)
{
	Expression* result = nullptr;
	Scope* scope = new Scope(CurrentScope());
	PushScope(scope);
	for (auto expr : block->GetExpressions())
	{
		if (result) delete result;
		result = Eval(expr);
	}
	delete PopScope();
	return result;
}

Scope* Evaluator::CurrentScope() const
{
	return scopeStack.top();
}

void Evaluator::PushScope(Scope* scope)
{
	scopeStack.push(scope);
}

Scope* Evaluator::PopScope()
{
	auto result = scopeStack.top();
	scopeStack.pop();
	return result;
}

Scope* ClosureExpression::GetScope()
{
	return scope;
}
