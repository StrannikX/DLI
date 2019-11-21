#include "Parser.h"

KeywordToken* Parser::GetKeyword(const std::string &str)
{
	auto token = GetToken<KeywordToken>();
	if (token->GetKeyword() != str) throw std::exception("Unexpected keyword");
	return token;
}

Token* Parser::NextToken()
{
	if (it != end)
	{
		auto token = *it;
		it++;
		return token;
	}
	throw std::exception("Unexpected end of file");
}

Parser::Parser(std::list<Token*>& tokens)
{
	it = tokens.begin();
	end = tokens.end();
}

Expression* Parser::Parse()
{
	return ParseExpression();
}

Expression* Parser::ParseExpression()
{
	static_cast<void>(GetToken<OpenBracketToken>());
	auto keyword = GetToken<KeywordToken>();
	Expression* expression = nullptr;

	const std::string& str = keyword->GetKeyword();

	if (str == "block")
	{
		expression = (Expression*) ParseBlockExpression();
	} 
	else if (str == "val") 
	{
		expression = (Expression*) ParseValExpression();
	}
	else if (str == "let") {
		expression = (Expression*) ParseLetExpression();
	}
	else if (str == "var") {
		expression = (Expression*)ParseVarExpression();
	}
	else if (str == "add") {
		expression = (Expression*)ParseAddExpression();
	}
	else if (str == "if") {
		expression = (Expression*)ParseIfxpression();
	}
	else if (str == "function") {
		expression = (Expression*)ParseFunctionExpression();
	}
	else if (str == "call") {
		expression = (Expression*)ParseCallExpression();
	}
	else if (str == "set") {
		expression = (Expression*)ParseSetExpression();
	}

	static_cast<void>(GetToken<CloseBracketToken>());
	return expression;
}

ValExpression* Parser::ParseValExpression()
{
	auto valueToken = GetToken<ValueToken>();
	int value = valueToken->GetValue();
	return new ValExpression(value);
}

BlockExpression* Parser::ParseBlockExpression()
{
	BlockExpression * expr = new BlockExpression();
	do {
		auto token = NextToken();
		auto closeBracket = dynamic_cast<CloseBracketToken*>(token);
		
		it--;

		if (closeBracket) {
			break;
		} else {
			Expression* nestedExpression = ParseExpression();
			expr->AddExpression(nestedExpression);
		}

	} while (true);

	if (expr->GetExpressions().size() == 0)
	{
		delete expr;
		throw new std::exception("Block epxression should have one nested expression at least");
	}

	return expr;
}

LetExpression* Parser::ParseLetExpression()
{
	auto id = GetToken<IdentifierToken>();
	static_cast<void>(GetToken<AssignOperatorToken>());
	auto expression = ParseExpression();
	static_cast<void>(GetKeyword("in"));
	auto body = ParseExpression();
	return new LetExpression(id->GetId(), expression, body);
}

VarExpression* Parser::ParseVarExpression()
{
	auto token = GetToken<IdentifierToken>();
	return new VarExpression(token->GetId());
}

AddExpression* Parser::ParseAddExpression()
{
	auto left = ParseExpression();
	auto right = ParseExpression();
	return new AddExpression(left, right);
}

IfExpression* Parser::ParseIfxpression()
{
	auto left = ParseExpression();
	auto right = ParseExpression();
	static_cast<void>(GetKeyword("then"));
	auto trueBranch = ParseExpression();
	static_cast<void>(GetKeyword("else"));
	auto elseBranch = ParseExpression();
	return new IfExpression(left, right, trueBranch, elseBranch);
}

FunctionExpression* Parser::ParseFunctionExpression()
{
	auto id = GetToken<IdentifierToken>();
	auto body = ParseExpression();
	return new FunctionExpression(id->GetId(), body);
}

CallExpression* Parser::ParseCallExpression()
{
	auto function = ParseExpression();
	auto argument = ParseExpression();
	return new CallExpression(function, argument);
}

SetExpression* Parser::ParseSetExpression()
{
	auto id = GetToken<IdentifierToken>();
	auto val = ParseExpression();
	return new SetExpression(id->GetId(), val);
}