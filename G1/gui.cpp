#include "gui.h"

button::button(vec position, double width, double height, string title) : uielement(UI_BUTTON)
{
	this->position = position;
	this->halfWidth = width / 2.;
	this->halfHeight = height / 2.;
	this->title = title;

	isClickable = true;
}

void button::OnClick()
{
	if (action != nullptr)
		action();
}

void gui::Input(vec position)
{
	if (!isShown) return;

	for (uielement* element : elements)
	{
		if (
			element->IsClickable() &&
			position.x < element->position.x + element->halfWidth + this->position.x - this->halfWidth &&
			position.y < element->position.y + element->halfHeight + this->position.y - this->halfHeight &&
			position.x > element->position.x - element->halfWidth + this->position.x - this->halfWidth &&
			position.y > element->position.y - element->halfHeight + this->position.y - this->halfHeight
			)
		{
			element->OnClick();
			break;
		}
	}
}

void gui::RegisterElement(uielement* element)
{
	element->context = this;
	elements.push_back(element);
}

gui::gui(vec pos)
{
	position = pos;
}

void gui::Dispose()
{
	for (uielement* element : elements)
	{
		delete element;
	}
	elements.clear();
}

gui::~gui()
{
	Dispose();
}

uielement::uielement(uitype type)
{
	this->type = type;
}

bool uielement::IsClickable()
{
	return isClickable;
}

int uielement::GetType()
{
	return type;
}
