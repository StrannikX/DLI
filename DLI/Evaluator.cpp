#include "Evaluator.h"
#include <stdexcept>

// При удалении Scope
// Удаляются все хранящиеся в нём Expression
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

// Получить значение из Scope
// Если его нет будет выбрашено исключение
const Expression* Scope::GetValue(const std::string& key)
{
	try {
		return values.at(key);
	}
	catch (std::out_of_range ex) {
		throw std::exception("Unknown key");
	}
}

// Добавить значение в Scope
void Scope::AddValue(const std::string& key, Expression* value)
{
	values.insert(std::pair<const std::string, Expression*>(key, value));
}

// Установить значение в Scope
// Если его нет будет выбрашено исключение
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

// Попытка получить значение из Scope, не возбуждая исключения
// Значение будет возращено с помощью указателя value
// Возвращаемое значение функции показывает, успешна ли была операция
bool Scope::TryGet(const std::string& key, Expression** value)
{
	if (values.count(key) != 0) {
		*value = values.at(key);
		return true;
	}
	*value = nullptr;
	return false;
}

// Попытка установить значение в Scope, не возбуждая исключения
// Возвращаемое значение функции показывает, успешна ли была операция
bool Scope::TrySet(const std::string& key, Expression* value)
{
	if (values.count(key) != 0) {
		values[key] = value;
		return true;
	}
	return false;
}

// Получить родительскую область видимости
Scope* Scope::GetParent()
{
	return parentScope;
}

// Создать новый исполнитель с заданной глобальной областью видимости
Evaluator::Evaluator(Scope* global)
{
	scopeStack.push(new Scope(global));
}

// Создать новый исполнитель
Evaluator::Evaluator()
{
	scopeStack.push(new Scope());
}

Evaluator::~Evaluator()
{
	delete PopScope();
}

// Получить значение выражения.
// Если выражение не является <val>,
// то будет вызвано исключение
int Evaluator::GetValue(Expression * expression)
{
	auto valExpression = dynamic_cast<ValExpression*>(expression);
	if (valExpression) return valExpression->GetValue();
	throw std::exception("Expression isn't value expression");
}

// Получить значение из текущей или вышележащих областей видимости
Expression* Evaluator::FromEnv(std::string& key)
{
	Expression* expr = nullptr;
	Scope* scope;

	// Поднимаясь по цепочке областей видимости
	// От дочерней к родительской
	for (scope = CurrentScope(); scope != nullptr; scope = scope->GetParent())
	{
		// Пытаемся получить значение
		if (scope->TryGet(key, &expr)) break;
	}
	// Если значение найдено
	// Возвращаем его копию
	if (scope != nullptr) return expr->Clone();
	// Иначе бросаем исключение
	std::string str = "Unknown key " + key;
	throw std::exception(str.c_str());
}

// Установить значение в текущей области видимости или вышележащих
bool Evaluator::TrySetInEnv(std::string& key, Expression* expr)
{
	Scope* scope;
	// Поднимаясь по цепочке областей видимости
	// От дочерней к родительской
	for (scope = CurrentScope(); scope != nullptr; scope = scope->GetParent())
	{
		// Пытаемся установить значение
		if (scope->TrySet(key, expr)) return true;
	}
	return false;
}

// Выполнить выражение
Expression* Evaluator::Eval(Expression* expr)
{
	// Определяем тип выражения
	// И вызываем для него соответствующую функцию
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

// Выполняем выражение <val>
// Просто копируем его
Expression* Evaluator::Eval(ValExpression* val)
{
	return (ValExpression*) val->Clone();
}

// Выполняем выражение <var>
Expression* Evaluator::Eval(VarExpression* var)
{
	auto id = var->GetId();
	// Получаем значение переменной по её идентификатору
	return FromEnv(id);
}

// Выполняем выражение <add>
Expression* Evaluator::Eval(AddExpression* expr)
{
	// Находим значение операндов
	auto left = GetValue(Eval(expr->GetLeftOperand()));
	auto right = GetValue(Eval(expr->GetRightOperand()));
	// И выполняем сложение
	return new ValExpression(left + right);
}

// Выполняем выражение <if>
Expression* Evaluator::Eval(IfExpression* expr)
{
	// Получаем значение операндов
	auto val1 = GetValue(Eval(expr->GetLeftOperand()));
	auto val2 = GetValue(Eval(expr->GetRightOperand()));
	// Сравниваем их, выбираем выражение и исполняем его
	return Eval(val1 > val2 ? expr->GetThenBranch() : expr->GetElseBranch());
}

// Выполняем выражени <let>
Expression* Evaluator::Eval(LetExpression* expr)
{
	// Получаем идентификатор переменной
	auto id = expr->GetId(); 
	// И её значение
	auto value = Eval(expr->GetExpression());
	// Создаём новую область видимости, с текущей в качестве родительской
	Scope* scope = new Scope(CurrentScope());
	// Помещаем её в стэк
	PushScope(scope);
	// И создаём в ней переменную
	scope->AddValue(id, value);
	// Затем выполняем в ней тело выражения
	auto result = Eval(expr->GetBody());
	// И удаляем область видимости
	delete PopScope();
	return result;
}

// Выполняем выражение <function>
Expression* Evaluator::Eval(FunctionExpression* func)
{
	// Замыкаем область видимости, в которой функция определяется
	return new ClosureExpression(func, CurrentScope());
}

// Выполняем выражение <call>
Expression* Evaluator::Eval(CallExpression* expr)
{
	// Получаем тело функции
	auto res = Eval(expr->GetCallable());
	// И проверяем, соответствует ли оно типу
	auto collable = dynamic_cast<FunctionExpression*>(res);
	if (!collable) throw std::exception("Can't call not function expression");
	// Проверяем, является ли она замыканием
	auto closure = dynamic_cast<ClosureExpression*>(res);

	// Получаем значение аргумента
	auto argument = Eval(expr->GetArgument());

	// Выбираем родительскую область видимости
	// Если выражение - замыкание, то ей будет область видимости,
	// где функция определена, если нет, то та, в которой происходит вызов
	Scope* parentScope = closure ? closure->GetScope() : CurrentScope();
	
	// Создаём область видимости, в которой будет исполняться тело функции
	Scope* scope = new Scope(parentScope);
	// Создаём в ней переменную-аргумент
	scope->AddValue(collable->GetArgument(), argument);
	// И добавляем в стэк областей видимости
	PushScope(scope);

	// Затем исполняем в ней тело функции
	auto result = Eval(collable->GetBody());

	// А потом удаляем область видимости
	delete PopScope();
	return result;
}

// Выполнить выражение <set>
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

// Выполнить выражение <block>
Expression* Evaluator::Eval(BlockExpression* block)
{
	Expression* result = nullptr; // Результат выполнения блока
	Scope* scope = new Scope(CurrentScope()); // Создаём область видимости для блока
	PushScope(scope);
	// Для каждого выражения в блоке
	for (auto expr : block->GetExpressions())
	{
		// Если оно не последние, то удаляем предыдущий результат
		if (result) delete result;
		// И выполняем выражение
		result = Eval(expr);
	}
	delete PopScope();
	return result;
}

// Выполняем выражение <closure>
// Просто создаём копию
Expression* Evaluator::Eval(ClosureExpression* expression)
{
	return expression->Clone();
}

// Текущая область видимости - вершина стэка областей
Scope* Evaluator::CurrentScope() const
{
	return scopeStack.top();
}

// Добавить новую область видимости в стэк
void Evaluator::PushScope(Scope* scope)
{
	scopeStack.push(scope);
}

// Удалить вершину стэка областей видимости
Scope* Evaluator::PopScope()
{
	auto result = scopeStack.top();
	scopeStack.pop();
	return result;
}

// Получить область видимости, в которой определена функция 
Scope* ClosureExpression::GetScope()
{
	return scope;
}
