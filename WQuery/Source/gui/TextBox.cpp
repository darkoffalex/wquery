/**
* \brief Класс элемента управления "кнопка" (реализация)
* \author Alex "DarkWolf" Nem (https://github.com/darkoffalex)
* \version 1.0
* \date 2018-2019
* \copyright (C) 2018-2019 by Alex "DarkWolf" Nem
*/

#include <wquery/stdafx.h>
#include <wquery/gui/TextBox.h>

namespace wquery
{
	/**
	* \brief Конструктор
	* \param window Владеющее окно
	*/
	TextBox::TextBox(Window * window) : ControlBase(window, "Edit", WS_CHILD | WS_VISIBLE | WS_BORDER, {150,20}) {}

	/**
	* \brief Деструктор (унаследован от частично-вирутального)
	*/
	TextBox::~TextBox() = default;

	/**
	* \brief Получение имени класса (переопредление полного виртуального метода)
	* \return Строка с именем класса
	*/
	std::string TextBox::GetControlClassName()
	{
		return "TextBox";
	}

	/**
	* \brief Стиль поля для ввода пароля (да или нет)
	* \param status Статус
	*/
	void TextBox::SetIsPassowrd(bool status) const
	{
		if(this->hWnd_)
		{
			if (status) {
				SetWindowLong(this->hWnd_, GWL_STYLE, GetWindowLong(this->hWnd_, GWL_STYLE) | ES_PASSWORD);
				SendMessage(this->hWnd_, EM_SETPASSWORDCHAR, '*', FALSE);
			}
			else {
				SetWindowLong(this->hWnd_, GWL_STYLE, GetWindowLong(this->hWnd_, GWL_STYLE) ^ ES_PASSWORD);
				SendMessage(this->hWnd_, EM_SETPASSWORDCHAR, NULL, FALSE);
			}

			InvalidateRect(this->hWnd_, nullptr, TRUE);
			UpdateWindow(this->hWnd_);
		}
	}

	/**
	* \brief Является ли поле - полем для пароля
	* \return Статус
	*/
	bool TextBox::IsPassword() const
	{
		if (this->hWnd_)
			return !!(GetWindowLong(this->hWnd_, GWL_STYLE) & ES_PASSWORD);

		return false;
	}
};