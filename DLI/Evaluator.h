#pragma once
# include "AST.h"
# include <unordered_map>
# include <stack>

// Описание для исполнителя и вспомогательных по отношению к нему классов

class Scope;

// Расширения FunctionExpression, позволяет замкнуть область видимости,
// в которой определена функция
class ClosureExpression : public FunctionExpression
{
	Scope* scope; // Замыкаемая область видимости
public:
	ClosureExpression(FunctionExpression* func, Scope* scope) : FunctionExpression(func->GetArgument(), func->GetBody()->Clone()), scope(scope) {}
	Scope* GetScope();
};

class Evaluator 
{
	std::stack<Scope*> scopeStack; // Стэк областей видимости

public:
	Evaluator(Scope * global);
	Evaluator();
	~Evaluator();

	Expression* Eval(Expression*);	// Выполнить выражение

protected:
	Scope* CurrentScope() const; // Текущая область видимости, вершина стэка
	void PushScope(Scope*); // Добавить область видимости на вершину стэка
	Scope* PopScope(); // Удалить область видимости с вершины

	// Методы для выполнения конкретных выражений
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

	int GetValue(Expression*); // Получить значение выражения
	Expression* FromEnv(std::string&); // Получить значение из текущей области видимости
	bool TrySetInEnv(std::string&, Expression*); // Попытка установить значение в текущей области видимости
};

// Класс - абстракция области видимости
class Scope
{
	Scope * parentScope = nullptr; // Родительская области видимости
	std::unordered_map<std::string, Expression*> values; // Значения, хранящиеся в области видимости
public:
	Scope() {}
	Scope(Scope* parent) : parentScope(parent) {}
	~Scope();

	// Получить значение из текущей области видимости
	// Если значение не будет найдено, будет выбрашено исключение
	const Expression* GetValue(const std::string& key);

	// Создать новое значение в текущей области видимости
	void AddValue(const std::string& key, Expression* value);

	// Установить значение в текущей области видимости
	// Если значение не будет найдено, будет выбрашено исключение
	void SetValue(const std::string& key, Expression* value);

	// Попытка получить значение из текущей области видимости
	// Значение возращается через указатель value
	// Если удачно, функция вернёт true, иначе false
	bool TryGet(const std::string& key, Expression** value);

	// Попытка установить значение в текущей области видимости
	// Если удачно, функция вернёт true, иначе false
	bool TrySet(const std::string& key, Expression* value);

	// Родительская область видимости
	Scope* GetParent();
};