#pragma once

# include <string>

struct PositionInText
{
	unsigned int row, col;
	PositionInText(unsigned int row, unsigned int col) : row(row), col(col) {}
	PositionInText(const PositionInText& pos) : row(pos.row), col(pos.col) {}
	PositionInText() : row(0), col(0) {}
	std::string ToString() const;
};