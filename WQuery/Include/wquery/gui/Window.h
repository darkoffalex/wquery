/**
* \brief Интерфейс класса окна
* \details Обертка над WinApi окном, позволяет управлять основными свойствами и событиями окна
* \author Alex "DarkWolf" Nem (https://github.com/darkoffalex)
* \version 1.0
* \date 2018-2019
* \copyright (C) 2018-2019 by Alex "DarkWolf" Nem
*/

#pragma once

#include "../stdafx.h"
#include "../types/common.h"

namespace wquery
{
	class Window
	{

	private:
		HWND hWnd_;                         // Хендл окна WinApi
		Window * parent_;                   // Указатель на родительский объект (родительское окно)
		ColorRGB backgroundColor_;          // Цвет фона
		bool closesProgram_;                // Инициирует ли выход из приложения закрытие данного окна

		Vector2D<int> oldClientAreaSize_;   // Старые размеры (до изменения размеров)
		Vector2D<int> maxSizes_;            // Максимальные размеры
		Vector2D<int> minSizes_;            // Минимальные размеры

	public:

		/**
		* \brief Набор функций обратного вызова для различных событий
		*/
		struct
		{
			std::function<bool()> onClose;
			std::function<void()> onPaint;
			std::function<void(unsigned int type, Vector2D<int> newSizes)> onResized;
			std::function<void(unsigned int code)> onKeyDown;
			std::function<void(unsigned int code)> onKeyUp;
			std::function<void(char symbol)> onTyping;
			std::function<void(Vector2D<int> cursor)> onMouseMove;
			std::function<void(Vector2D<int> cursor, MouseKeys type)> onMouseKeyDown;
			std::function<void(Vector2D<int> cursor, MouseKeys type)> onMouseKeyUp;
		} events;

		/**
		* \brief Конструктор
		* \param parent Родительское окно (не обязательно)
		*/
		Window(Window * parent = nullptr);

		/**
		* \brief Деструктор. Уничтожение окна
		*/
		~Window();

		/**
		* \brief Показывает окно (приводит вид в нормальное состояние)
		*/
		void Show() const;

		/**
		* \brief Прячет окно (делает невидимым, то же что и при закрытии)
		*/
		void Hide() const;

		/**
		* \brief Получить WinApi хендл окна
		* \return Хендл окна
		*/
		HWND GetNativeHandle() const;

		/**
		* \brief Получить родительский объект
		* \return Указатель на родителя
		*/
		Window* GetParent() const;

		/**
		* \brief Установка заголовка окна
		* \param title Заголовок (строка)
		*/
		void SetTitle(const std::string& title) const;

		/**
		* \brief Получение заголовка окна
		* \return Строка с заголовком
		*/
		std::string GetTitle() const;

		/**
		* \brief Установка размеров окна
		* \param size Размеры
		* \param clientArea Изменять клиентскую область (размеры рассчитываются уже по ней)
		*/
		void SetSize(const Vector2D<int>& size, const bool clientArea = false) const;

		/**
		* \brief Получение размеров окна
		* \param clientArea Получить размеры клиентской области (не всего окна)
		* \return Размеры
		*/
		Vector2D<int> GetSize(const bool clientArea = false) const;

		/**
		* \brief Установка положения
		* \param position Положение верхнего левого угла окна
		*/
		void SetPosition(const Vector2D<int>& position) const;

		/**
		* \brief Получение положения
		* \param relative Получать относительное положение (относительно родительского окна)
		* \return Положение верхнего левого угла окна
		*/
		Vector2D<int> GetPosition(const bool relative) const;

		/**
		* \brief Установка цвета фона
		* \param color Цвет фона RGB
		*/
		void SetBgColor(const ColorRGB& color);

		/**
		* \brief Получение цвета фона
		* \return Цвет фона RGB
		*/
		ColorRGB GetBgColor() const;

		/**
		* \brief Закртывает ли программу закрытие окна (установка параметра)
		* \param status Статус
		*/
		void SetClosesProgram(const bool status);

		/**
		* \brief Закрывает ли програму закрытие окна (получение параметра)
		* \return Статус
		*/
		bool IsClosesProgram() const;

		/**
		* \brief Установка максимального размера окна
		* \param sizes Размер
		* \param clientArea Для клиентской области (размер окна вычилсяется)
		*/
		void SetMaxSizes(const Vector2D<int>& sizes, const bool clientArea = false);

		/**
		* \brief Получить максимальный размер окна
		* \param clientArea Размер клиентской области (с учетом размера окна)
		* \return Размер
		*/
		Vector2D<int> GetMaxSizes(const bool clientArea = false) const;

		/**
		* \brief Установка минимального размера окна
		* \param sizes Размер
		* \param clientArea Для клиентской области (размер окна вычилсяется)
		*/
		void SetMinSizes(const Vector2D<int>& sizes, const bool clientArea = false);

		/**
		* \brief Получить минимальный размер окна
		* \param clientArea Размер клиентской области (с учетом размера окна)
		* \return Размер
		*/
		Vector2D<int> GetMinSizes(const bool clientArea = false) const;

		/**
		* \brief Установить состояние кнопки закрытия
		* \param enabled Статус
		*/
		void SetCloseButtonStatus(const bool enabled) const;

		/**
		* \brief Установить состояние кнопки минимизации
		* \param enabled Статус
		*/
		void SetMinimizeButtonStatus(const bool enabled) const;

		/**
		* \brief Установить состояние кнопки максимизации
		* \param enabled Статус
		*/
		void SetMaximizeButtonStatus(const bool enabled) const;

		/**
		* \brief Установить состояние (видимость) системного меню окна
		* \param visible Статус
		*/
		void SetSysMenuStatus(const bool visible) const;

		/**
		 * \brief Установка иконки приложения из файла
		 * \param iconFilename Путь к .ico файлу
		 * \param width Ширина загружаемой иконки
		 * \param height Высота загружаемой иконки
		 */
		void SetIcon(const std::string &iconFilename, int width = 16, int height = 16) const;

		/**
		* \brief Максимизировать окно
		*/
		void Maximize() const;

		/**
		* \brief Минимизировать окно
		*/
		void Minimize() const;

		/**
		* \brief Оконная процедура - обработчик оконных сообщений (событий) системы.
		* \param hWnd Хендл окна, которому адресовано сообщение
		* \param message Идентификатор сообщения
		* \param wParam Один из параметров перданных окну с сообщением
		* \param lParam Один из параметров перданных окну с сообщением
		* \return Код состояния
		*/
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		/**
		* \brief Используется для обновление размеров адаптивных (якорных) элементов
		* \details Метод является callback-ом для перечисления дочерних элементов в момент изменения размера кона
		* \param hWnd Хендл элемента WinApi
		* \param lParam Параметр переданный из функции перечисления
		* \return Состояние
		*/
		static BOOL CALLBACK EnumerateResizingControls(HWND hWnd, LPARAM lParam);
	};
}