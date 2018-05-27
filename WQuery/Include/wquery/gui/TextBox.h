/**
* \brief Класс элемента управления "кнопка" (интерфейс)
* \author Alex "DarkWolf" Nem (https://github.com/darkoffalex)
* \version 1.0
* \date 2018-2019
* \copyright (C) 2018-2019 by Alex "DarkWolf" Nem
*/

#pragma once

#include "../stdafx.h"
#include "../gui/Window.h"
#include "../gui/ControlBase.h"

namespace wquery
{
	class TextBox : public ControlBase
	{
	private:

	public:
		struct
		{
			std::function<void()> onChanged;
			std::function<void()> onClicked;
		} events;

		/**
		* \brief Конструктор
		* \param window Владеющее окно
		*/
		TextBox(Window * window);

		/**
		* \brief Деструктор (унаследован от частично-вирутального)
		*/
		~TextBox();

		/**
		* \brief Получение имени класса (переопредление полного виртуального метода)
		* \return Строка с именем класса
		*/
		std::string GetControlClassName() override;

		/**
		 * \brief Стиль поля для ввода пароля (да или нет)
		 * \param status Статус
		 */
		void SetIsPassowrd(bool status) const;

		/**
		 * \brief Является ли поле - полем для пароля
		 * \return Статус
		 */
		bool IsPassword() const;
	};
}