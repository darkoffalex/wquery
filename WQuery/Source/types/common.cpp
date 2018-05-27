/**
* \brief Общие типы данных (классы структуры) WQuery. Реализация
* \author Alex "DarkWolf" Nem (https://github.com/darkoffalex)
* \version 1.0
* \date 2018-2019
* \copyright (C) 2018-2019 by Alex "DarkWolf" Nem
*/

#include <wquery/stdafx.h>
#include <wquery/types/common.h>

namespace wquery
{
	/*
	* Ц В Е Т
	*/

	/**
	* \brief Конструктор цвета по умолчанию. По умолчанию инициализирует белый цвет
	* \param r Компонента красного цвета
	* \param g Компонента зеленого цвета
	* \param b Компонента синего цвета
	*/
	ColorRGB::ColorRGB(const unsigned char r, const unsigned char g, const unsigned char b) :R(r), G(g), B(b) {}

	/**
	* \brief Получить хендл системной (Winapi) кисти
	* \return Хендл кисти
	*/
	HBRUSH ColorRGB::GetNativeBrush() const
	{
		return CreateSolidBrush(RGB(this->R, this->G, this->B));
	}

	/**
	* \brief Получить хендл системной (Winapi) цветовой ссылки
	* \return Цветовая ссылка (хендл цвета в системе)
	*/
	COLORREF ColorRGB::GetNativeColorRef() const
	{
		return RGB(this->R, this->G, this->B);
	}

	/*
	* Я К О Р Я (П Р И В Я З К И)
	*/

	/**
	* \brief Конструктор по умолчанию
	* \param inLeft Привязка к левому краю
	* \param inTop  Привязка к верху
	* \param inRight Привязка к правому краю
	* \param inBottom Привязка к низу
	*/
	AnchorSettings::AnchorSettings(bool inLeft, bool inTop, bool inRight, bool inBottom) :
		left(inLeft), top(inTop), right(inRight), bottom(inBottom) {}

	/**
	* \brief Привязан ли хотя-бы к одной из сторон
	* \return Состояние
	*/
	bool AnchorSettings::IsAnchored() const
	{
		return this->left || this->bottom || this->right || this->top;
	}
}
