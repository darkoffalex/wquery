/**
* \brief Набор вспомогательных функций для работы c файлами, директориями, путями и т.д.
* \author Alex "DarkWolf" Nem (https://github.com/darkoffalex)
* \version 1.0
* \date 2018-2019
* \copyright (C) 2018-2019 by Alex "DarkWolf" Nem
*/


#include <wquery/stdafx.h>
#include <wquery/tools/files.h>

#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

namespace wquery
{
	/**
	* \brief Получение пути к каталогу содержащему .exe файл
	* \return Путь к каталогу
	*/
	std::string GetExeDir()
	{
		char path[MAX_PATH] = {};
		GetModuleFileNameA(NULL, path, MAX_PATH);
		PathRemoveFileSpecA(path);
		PathAddBackslashA(path);
		return std::string(path);
	}

	/**
	* \brief Получение пути к рабочему каталогу
	* @note Нужно учитывать что рабочий каталог может зависеть от конфигурации проекта
	* \return Путь к каталогу
	*/
	std::string GetWorkingDir()
	{
		char path[MAX_PATH] = {};
		GetCurrentDirectoryA(MAX_PATH, path);
		PathAddBackslashA(path);
		return std::string(path);
	}
}
