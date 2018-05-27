/**
* \brief Класс элемента управления "кнопка" (реализация)
* \author Alex "DarkWolf" Nem (https://github.com/darkoffalex)
* \version 1.0
* \date 2018-2019
* \copyright (C) 2018-2019 by Alex "DarkWolf" Nem
*/

#include <wquery/stdafx.h>
#include <wquery/gui/Button.h>

namespace wquery
{
	/**
	* \brief Конструктор
	* \param window Владеющее окно
	*/
	Button::Button(Window * window) : ControlBase(window, "Button") {}

	/**
	* \brief Деструктор (унаследован от частично-вирутального)
	*/
	Button::~Button() = default;

	/**
	* \brief Получение имени класса (переопредление полного виртуального метода)
	* \return Строка с именем класса
	*/
	std::string Button::GetControlClassName()
	{
		return "Button";
	}
};