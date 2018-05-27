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
	class Button : public ControlBase
	{
	private:

	public:
		struct
		{
			std::function<void()> onClicked;
		} events;

		/**
		* \brief Конструктор
		* \param window Владеющее окно
		*/
		Button(Window * window);

		/**
		* \brief Деструктор (унаследован от частично-вирутального)
		*/
		~Button();

		/**
		* \brief Получение имени класса (переопредление полного виртуального метода)
		* \return Строка с именем класса
		*/
		std::string GetControlClassName() override;
	};
}