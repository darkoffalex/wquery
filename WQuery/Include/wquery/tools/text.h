/**
* \brief Набор вспомогательных функций для работы с текстом и строкам (интерфейс)
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
	* \brief Конвертация обычной string строки в "широкую" UTF-16 строку
	* \param str Исходная строка
	* \param codePage Идентификатор поддерживаемой операционной системой "кодовй страницы"
	* \param dwFlags Тип конвертации (как конвертировать простые символы в составные)
	* \return UTF-16 строка
	*/
	std::wstring StrToWide(const std::string& str, const UINT codePage = CP_ACP, const DWORD dwFlags = MB_PRECOMPOSED);

	/**
	* \brief Конвертация из "широкой" wstring-строки в обычную string
	* \param wstr Исходная строка
	* \param codePage Идентификатор поддерживаемой операционной системой "кодовй страницы"
	* \param dwFlags Тип конвертации (как конвертировать составные символы в простые)
	* \return UTF-8 строка
	*/
	std::string WideToStr(const std::wstring& wstr, const UINT codePage = CP_ACP, const DWORD dwFlags = WC_COMPOSITECHECK);

	/**
	* \brief Конвертация обычного символа в "широкий" UTF-16 символ
	* \param symbol Исходный символ
	* \param codePage Идентификатор поддерживаемой операционной системой "кодовй страницы"
	* \param dwFlags Тип конвертации (как конвертировать простые символы в составные)
	* \return UTF-16 строка
	*/
	wchar_t CharToWide(char symbol, const UINT codePage = CP_ACP, const DWORD dwFlags = MB_PRECOMPOSED);

	/**
	* \brief Конвертация из "широкого" символа в обычный char символ
	* \param wsymbol Исходный символ
	* \param codePage Идентификатор поддерживаемой операционной системой "кодовй страницы"
	* \param dwFlags Тип конвертации (как конвертировать составные символы в простые)
	* \return UTF-8 строка
	*/
	char WideToChar(wchar_t wsymbol, const UINT codePage = CP_ACP, const DWORD dwFlags = WC_COMPOSITECHECK);
}