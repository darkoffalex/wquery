/**
* \brief Набор вспомогательных функций для работы c файлами, директориями, путями и т.д.
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
	 * \brief Получение пути к каталогу содержащему .exe файл
	 * \return Путь к каталогу
	 */
	std::string GetExeDir();

	/**
	 * \brief Получение пути к рабочему каталогу
	 * @note Нужно учитывать что рабочий каталог может зависеть от конфигурации проекта
	 * \return Путь к каталогу
	 */
	std::string GetWorkingDir();
}