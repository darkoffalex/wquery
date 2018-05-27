/**
* \brief Набор вспомогательных функций для работы с текстом и строкам (реализация)
* \author Alex "DarkWolf" Nem (https://github.com/darkoffalex)
* \version 1.0
* \date 2018-2019
* \copyright (C) 2018-2019 by Alex "DarkWolf" Nem
*/


#include <wquery/stdafx.h>
#include <wquery/tools/text.h>

namespace wquery
{
	/**
	* \brief Конвертация обычной string строки в "широкую" UTF-16 строку
	* \param str Исходная строка
	* \param codePage Идентификатор поддерживаемой операционной системой "кодовй страницы"
	* \param dwFlags Тип конвертации (как конвертировать простые символы в составные)
	* \return UTF-16 строка
	*/
	std::wstring StrToWide(const std::string& str, const UINT codePage, const DWORD dwFlags)
	{
		std::wstring result;
		wchar_t* newString = new wchar_t[str.length() + 1];
		MultiByteToWideChar(codePage, dwFlags, str.c_str(), static_cast<int>(str.length()) + 1, newString, static_cast<int>(str.length()) + 1);
		result.append(newString);
		delete[] newString;
		return result;
	}

	/**
	* \brief Конвертация из "широкой" wstring-строки в обычную string
	* \param wstr Исходная строка
	* \param codePage Идентификатор поддерживаемой операционной системой "кодовй страницы"
	* \param dwFlags Тип конвертации (как конвертировать составные символы в простые)
	* \return UTF-8 строка
	*/
	std::string WideToStr(const std::wstring& wstr, const UINT codePage, const DWORD dwFlags)
	{
		std::string result;
		char* newString = new char[wstr.length() + 1];
		WideCharToMultiByte(codePage, dwFlags, wstr.c_str(), static_cast<int>(wstr.length()) + 1, newString, static_cast<int>(wstr.length()) + 1, NULL, FALSE);
		result.append(newString);
		delete[] newString;
		return result;
	}

	/**
	* \brief Конвертация обычного символа в "широкий" UTF-16 символ
	* \param symbol Исходный символ
	* \param codePage Идентификатор поддерживаемой операционной системой "кодовй страницы"
	* \param dwFlags Тип конвертации (как конвертировать простые символы в составные)
	* \return UTF-16 строка
	*/
	wchar_t CharToWide(char symbol, const UINT codePage, const DWORD dwFlags)
	{
		wchar_t newChar;
		MultiByteToWideChar(codePage, dwFlags, &symbol, 1, &newChar, 1);
		return newChar;
	}

	/**
	* \brief Конвертация из "широкого" символа в обычный char символ
	* \param wsymbol Исходный символ
	* \param codePage Идентификатор поддерживаемой операционной системой "кодовй страницы"
	* \param dwFlags Тип конвертации (как конвертировать составные символы в простые)
	* \return UTF-8 строка
	*/
	char WideToChar(wchar_t wsymbol, const UINT codePage, const DWORD dwFlags)
	{
		char newChar;
		WideCharToMultiByte(codePage, dwFlags, &wsymbol, 1, &newChar, 1, NULL, FALSE);
		return newChar;
	}
}
