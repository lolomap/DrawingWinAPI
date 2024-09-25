#pragma once
#include "vec.h"
#include <vector>
#include <string>

using namespace std;

class uielement;

class gui
{
public:
	vec position;
	const int halfWidth = 150;
	const int halfHeight = 250;

	bool isShown = false;
	vector<uielement*> elements;

	void Input(vec position);
	void RegisterElement(uielement* element);

	gui(vec pos);

	void Dispose();
	~gui();
};

enum uitype
{
	UI_BUTTON
};

class uielement
{
protected:
	bool isClickable = false;
	uitype type;

public:

	gui* context;
	vec position;
	double halfHeight, halfWidth;

	uielement(uitype type);

	bool IsClickable();
	int GetType();

	virtual void OnClick() = 0;
};

class button : public uielement
{
public:
	void (*action)() = nullptr;
	string title;

	button(vec position, double width, double height, string title);
	
	void OnClick();
};