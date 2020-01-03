#pragma once

# include "Token.h"
# include "AST.h"
# include <string>
# include <vector>
# include <list>
# include <exception>
#include <memory>

// Синтаксический анализатор
// Строит AST по последовательности лексем
class Parser
{
	std::list<Token*> tokens;           // Исходные лексемы
	std::list<Token*>::iterator it;     // Итератор текущей лексемы
	std::list<Token*>::iterator end;    // Конец списка лексем
protected:
	// Попытка получить следующую лексему типа T
	// Если лексема отсутствует, или имеет тип отличный от T, то будет выбрашено исключение
	template<class T> T* GetToken();

	// Попытка получить следующую лексему соответствующую ключевому слову keyword
	// Если лексема отсутствует, имеет тип отличный от KeywordToken или значение отличное от keyword,
	// Будет выбрашено исключение
	KeywordToken* GetKeyword(const std::string& keyword);

	// Попытка получить следующую лексему
	// Если лексема отсутствует - будет выбрашено исключение
	Token* NextToken();

	// Методы разбора синтаксических конструкций языка
	ValExpression* ParseValExpression();
	BlockExpression* ParseBlockExpression();
	LetExpression* ParseLetExpression();
	VarExpression* ParseVarExpression();
	AddExpression* ParseAddExpression();
	IfExpression* ParseIfxpression();
	FunctionExpression* ParseFunctionExpression();
	CallExpression* ParseCallExpression();
	SetExpression* ParseSetExpression();
	// Базовый метод рекурсивного спуска
	Expression* ParseExpression();
public:
	Parser(std::list<Token*>& tokens);
	Expression* Parse();
};

// Попытка получить следующую лексему типа T
template<class T> inline T* Parser::GetToken()
{
	auto token = NextToken();
	T* t = dynamic_cast<T*>(token);
	if (t) return t;
	throw std::exception((std::string("Unexpected token: ") + token->ToString()).c_str());
}
