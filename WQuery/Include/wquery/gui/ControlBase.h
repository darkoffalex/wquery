/**
* \brief Интерфейс базового абстрактного класса элемента управления
* \details Класс содержит набор методов и свойств общих для всех элементов управления
* \author Alex "DarkWolf" Nem (https://github.com/darkoffalex)
* \version 1.0
* \date 2018-2019
* \copyright (C) 2018-2019 by Alex "DarkWolf" Nem
*/

#pragma once

#include "../stdafx.h"
#include "../gui/Window.h"

namespace wquery
{
	class ControlBase
	{
	protected:
		// HWBD хендл элемента управления
		HWND hWnd_;

		// Указатель на родительское окно
		Window * window_;

		// Параметры привязки (якоря) элемента
		AnchorSettings anchor_;

		// Хендл кастомного шрифта
		// по умолчанию он равен nullptr, но будет присвоен при указании кастомного шрифта
		// чтобы при повторном указании можно было уничтожить объект относящийся к этому хендлу
		HFONT customFont_;

	public:
		/**
		* \brief Конструктор элемента управления
		* \param window Указатель на владеющее окно
		* \param controlClassName Наименовая WinApi класса элемента управления
		* \param dwStyle Стиль отображения элемента
		*/
		ControlBase(Window * window, const std::string& controlClassName, DWORD dwStyle = WS_CHILD | WS_VISIBLE);

		/**
		* \brief Деструктор (вирутальный, уничтожает в том числе и объект-наследник)
		*/
		virtual ~ControlBase();

		/**
		* \brief Получить название WinApi класса элемента управления
		* \details Полностью виртуальный метод (реализуется в классах-наследниках)
		* \return Строка с именем WinApi класса
		*/
		virtual std::string GetControlClassName() = 0;

		/**
		* \brief Получить указатель на владеющее окно
		* \return Указатель
		*/
		Window* GetWindow() const;

		/**
		* \brief Установить текст элемента управления
		* \param text Текст
		*/
		void SetText(const std::string &text) const;

		/**
		* \brief Получить текст элемента управления
		* \return Текст
		*/
		std::string GetText() const;

		/**
		* \brief Установить положение
		* \param position Положение
		*/
		void SetPosition(Vector2D<int> position) const;

		/**
		* \brief Получить положение
		* \return Положение
		*/
		Vector2D<int> GetPosition() const;

		/**
		* \brief Установить размеры
		* \param size Размеры
		*/
		void SetSize(Vector2D<int> size) const;

		/**
		* \brief Получить размеры
		* \return Размеры
		*/
		Vector2D<int> GetSize() const;

		/**
		* \brief Установить состояние элемента управления (активен/не активен)
		* \param state Состояние
		*/
		void SetEnabled(const bool state) const;

		/**
		* \brief Получить состояние элемента управления (активен/не активен)
		* \return Состояние
		*/
		bool IsEnabled() const;

		/**
		* \brief Установить привязку (якорь) элемента управления к сторонам контейнера
		* \param anchor Параметры привязки (якоря)
		*/
		void SetAnchor(const AnchorSettings& anchor);

		/**
		* \brief Получить параметры привязки (якоря) элемента управления к сторонам контейнера
		* \return Параметры привязки (якоря)
		*/
		AnchorSettings GetAnchor() const;

		/**
		 * \brief Установка параметров шрифта элемента управления
		 * \param font Параметры шрифта
		 */
		void SetFont(const FontSettings& font);

		/**
		 * \brief Получение параметров шрифта элемента управления
		 * \return Параметры шрифта
		 */
		FontSettings GetFont() const;

		/**
		 * \brief Очистка шрифта (вернуть шрифт по умолчанию)
		 */
		void ClearFont();
	};
}