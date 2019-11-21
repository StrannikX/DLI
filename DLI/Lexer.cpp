#include "Lexer.h"

# include <string>
# include <vector>
# include <list>
# include <iostream>
# include <cctype>
# include <algorithm>
# include <exception>

std::list<Token*> Lexer::Tokenize(std::istream &in)
{
	Tokenizer tokenizer(keywords, in);
	return tokenizer.Tokenize();
}

std::list<Token*> Tokenizer::Tokenize()
{
	while (!in.eof() && in.peek() >= 0)
	{
		switch (state)
		{
			case TokenizerState::WaitToken:
				WaitForToken();
				break;
			case TokenizerState::ReadInt:
				ReadIntState();
				break;
			case TokenizerState::ReadWord:
				ReadWordState();
		}
	}

	if (state == TokenizerState::ReadInt && buff.length() > 0)
	{
		tokens.push_back((Token*)CreateValueToken());
	}

	if (state == TokenizerState::ReadWord && buff.length() > 0)
	{
		tokens.push_back(CreateWordToken());
	}

	return tokens;
}

void Tokenizer::ReadIntState()
{
	int ch = in.get();
	if (ch < 0) return;

	if (isdigit(ch))
	{
		buff += ch;
		return;
	} 
	
	Token * token = CreateValueToken();
	tokens.push_back(token);
	in.putback(ch);
	state = TokenizerState::WaitToken;
}

void Tokenizer::ReadWordState()
{
	int ch = in.get();
	if (ch < 0) return;

	if (isalnum(ch)) {
		buff += ch;
		return;
	}

	Token* token = CreateWordToken();
	tokens.push_back(token);
	in.putback(ch);
	state = TokenizerState::WaitToken;
}

void Tokenizer::WaitForToken()
{
	int ch = in.get();
	if (ch < 0) return;

	if (isspace(ch) || isblank(ch)) return;

	if (isalnum(ch) || ch == MINUS)
	{
		buff += ch;
		state = isalpha(ch) 
			? TokenizerState::ReadWord 
			: TokenizerState::ReadInt;
		return;
	}

	if (ch == OPEN_BRACKET) 
	{
		tokens.push_back((Token*)(new OpenBracketToken()));
		return;
	}

	if (ch == CLOSE_BRACKET)
	{
		tokens.push_back((Token*)(new CloseBracketToken()));
		return;
	}

	if (ch == ASSIGN_OPERATOR)
	{
		tokens.push_back((Token*)(new AssignOperatorToken()));
		return;
	}

	throw std::exception("Unknown character: " + ch);
}

ValueToken* Tokenizer::CreateValueToken()
{
	ValueToken* token = new ValueToken(std::stoi(buff));
	buff = "";
	return token;
}

Token* Tokenizer::CreateWordToken()
{
	bool isKeyword = std::find(keywords.begin(), keywords.end(), buff) != keywords.end();
	Token* token = isKeyword
		? (Token*) new KeywordToken(buff)
		: (Token*) new IdentifierToken(buff);

	buff = "";
	return token;
}