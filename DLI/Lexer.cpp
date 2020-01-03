#include "Lexer.h"

# include <string>
# include <vector>
# include <list>
# include <iostream>
# include <cctype>
# include <algorithm>
# include <exception>

// Выполнить лексический анализ
std::list<Token*> Lexer::Tokenize()
{
	// Пока не достигнут конец ввода
	while (!in.eof() && in.peek() >= 0)
	{
		// Вызываем метод для текущего состояния конечного автомата
		switch (state)
		{
			case LexerState::WaitToken:
				WaitForToken();
				break;
			case LexerState::ReadInt:
				ReadIntState();
				break;
			case LexerState::ReadWord:
				ReadWordState();
		}
	}

	// Заканчиваем чтение целого
	if (state == LexerState::ReadInt && buff.length() > 0)
	{
		tokens.push_back((Token*)CreateValueToken());
	}

	// Заканчиваем чтение строки
	if (state == LexerState::ReadWord && buff.length() > 0)
	{
		tokens.push_back(CreateWordToken());
	}

	return tokens;
}

// Состояние - чтение целого числа
void Lexer::ReadIntState()
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
	state = LexerState::WaitToken;
}

// Состояние - чтение слова
void Lexer::ReadWordState()
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
	state = LexerState::WaitToken;
}

// Состояние - ожидание начала лексемы
void Lexer::WaitForToken()
{
	int ch = in.get();
	if (ch < 0) return;

	// Пропускаем пробелы, символы табуляции и перевода строки
	if (isspace(ch) || isblank(ch)) return;

	// Если буква символ или знак минуса
	if (isalnum(ch) || ch == MINUS)
	{
		buff += ch; // Добавляем символ в буфер
		state = isalpha(ch)  // И выбираем новое состояние автомата
			? LexerState::ReadWord // В зависимости от того, что будем считывать
			: LexerState::ReadInt;
		return;
	}

	// Если скобки или оператор "="
	// То просто добавляем соответсвующие им лексемы в список

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

// Создает лексему для целого числа
// На основе содержимого буфера чтения 
ValueToken* Lexer::CreateValueToken()
{
	ValueToken* token = new ValueToken(std::stoi(buff));
	buff = "";
	return token;
}

// Создаёт лексему для ключевого слова или идентификатора
// В зависимости от того, лежит ли в буфер строка
// Идентичная одному из ключевых слов
Token* Lexer::CreateWordToken()
{
	bool isKeyword = std::find(keywords.begin(), keywords.end(), buff) != keywords.end();
	Token* token = isKeyword
		? (Token*) new KeywordToken(buff)
		: (Token*) new IdentifierToken(buff);

	buff = "";
	return token;
}