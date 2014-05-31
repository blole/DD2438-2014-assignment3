#pragma once
#include <BWAPI.h>

class Rect
{
public:
	int x;
	int y;
	int width;
	int height;

public:
	Rect();

	void enclose(BWAPI::Position point);
	void shrink(int units);

	bool isEmpty() const;
};
