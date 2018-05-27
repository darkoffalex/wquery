/**
* \brief Общие типы данных (классы, структуры) WQuery. Интерфейс
* \author Alex "DarkWolf" Nem (https://github.com/darkoffalex)
* \version 1.0
* \date 2018-2019
* \copyright (C) 2018-2019 by Alex "DarkWolf" Nem
*/

#pragma once

#include "../stdafx.h"

namespace wquery
{
	/**
	* \brief Структура описывающая RGB цвет. Используется для задания цвета
	* различным жлементам управления
	*/
	struct ColorRGB
	{
		unsigned char R;
		unsigned char G;
		unsigned char B;

		/**
		* \brief Конструктор цвета по умолчанию. По умолчанию инициализирует белый цвет
		* \param r Компонента красного цвета
		* \param g Компонента зеленого цвета
		* \param b Компонента синего цвета
		*/
		ColorRGB(unsigned char r = 255, unsigned char g = 255, unsigned char b = 255);

		/**
		* \brief Получить хендл системной (WinApi) кисти
		* \return Хендл кисти
		*/
		HBRUSH GetNativeBrush() const;

		/**
		* \brief Получить хендл системной (WinApi) цветовой ссылки
		* \return Цветовая ссылка (хендл цвета в системе)
		*/
		COLORREF GetNativeColorRef() const;
	};

	/**
	* \brief Шаблонная структура описывающая двумерный вектор
	* \tparam T Тип используемый для компонент вектора
	*/
	template<typename T = int>
	struct Vector2D
	{
		T X;
		T Y;

		/**
		* \brief Конструктор по умолчанию. Без параметров инициализирует нулевой вектор
		* \param x Компонента X
		* \param y Копонента Y
		*/
		Vector2D(T x = 0, T y = 0) :X(x), Y(y) {}

		/**
		* \brief Оператор сложения
		* \param vector Вектор который суммируется с текущим
		* \return Новый вектор (результат суммы)
		*/
		Vector2D operator+(const Vector2D& vector) const
		{
			return{
				this->X + vector.X,
				this->Y + vector.Y
			};
		}

		/**
		* \brief Оператор вычитания
		* \param vector Вектор который будет вычтен из текущего
		* \return Новый вектор (результат разности)
		*/
		Vector2D operator-(const Vector2D& vector) const
		{
			return{
				this->X - vector.X,
				this->Y - vector.Y
			};
		}

		/**
		* \brief Получить длину вектора в том типе, который был использован при инициализации вектора
		* \return Значение длины
		*/
		T GetLength() const
		{
			return static_cast<T>(sqrt(pow(this->X, 2) + pow(this->Y, 2)));
		}
	};

	/**
	* \brief Структура описывающая основные свойства шрифтов используемых в элементах
	* управления
	*/
	struct FontSettings
	{
		const char* fontFamilyName;            // Наименования шрифта (имя семейства шрифтов)
		unsigned int size;                     // Размер шрифта
		bool bold;                             // Жирный
		bool italic;                           // Курсив

		/**
		* \brief Конструктор по умолчанию
		* \param inFontFamilyName Наименования шрифта (имя семейства шрифтов)
		* \param inSize Размер шрифта
		* \param inBold Жирный
		* \param inItalic Курсив
		*/
		FontSettings(const char* inFontFamilyName = "Arial", const unsigned int inSize = 16, const bool inBold = false, const bool inItalic = false) :
			fontFamilyName(inFontFamilyName),
			size(inSize),
			bold(inBold),
			italic(inItalic) {}
	};

	/**
	* \brief Структура описывающая привязку (якорь) элемента управления к сторонам родительского окна
	*/
	struct AnchorSettings
	{
		bool left, top, right, bottom;

		/**
		* \brief Конструктор по умолчанию
		* \param inLeft Привязка к левому краю
		* \param inTop  Привязка к верху
		* \param inRight Привязка к правому краю
		* \param inBottom Привязка к низу
		*/
		AnchorSettings(bool inLeft = false, bool inTop = false, bool inRight = false, bool inBottom = false);

		/**
		* \brief Привязан ли хотя-бы к одной из сторон
		* \return Состояние
		*/
		bool IsAnchored() const;
	};

	/**
	* \brief Тип основного цикла приложения, используется в качестве агрумента
	* функции end(), запускающей основной цикл.
	*/
	enum MainLoopType
	{
		GET_MSG,
		PEEK_MSG
	};

	/**
	* \brief Тип нажатой или отжатой кнопки мыши, который используется в качестве
	* аргемента обработчиков событий кнопок мыши на различных элементах управления
	*/
	enum MouseKeys
	{
		LEFT,
		MIDDLE,
		RIGHT
	};
}