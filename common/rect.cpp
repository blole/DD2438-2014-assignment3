#include "common/rect.hpp"

Rect::Rect()
	: x(0)
	, y(0)
	, width(-1)
	, height(-1)
{
}

void Rect::enclose(BWAPI::Position p)
{
	if (isEmpty())
	{
		x = p.x;
		y = p.y;
		width  = 0;
		height = 0;
	}
	else
	{
		if (x < p.x)
			width = std::max(width, p.x-x);
		else //x >= p.x
		{
			width += x-p.x;
			x = p.x;
		}

		if (y < p.y)
			height = std::max(height, p.y-y);
		else //y >= p.y
		{
			height += y-p.y;
			y = p.y;
		}
	}
}

bool Rect::isEmpty() const
{
	return width == -1 && height == -1;
}

void Rect::shrink(int units)
{
	if (!isEmpty())
	{
		x += std::min(units,  width/2);
		y += std::min(units, height/2);
		width  -= std::min(units*2, width);
		height -= std::min(units*2, height);
	}
}
