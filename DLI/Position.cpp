#include "Position.h"

std::string PositionInText::ToString() const
{
	return "(" + std::to_string(row) + ":" + std::to_string(col) + ")";
}
