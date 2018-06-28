/**
* \brief Основной включаемый файл библиотеки
* \details Файл подключает основные заголовочные файлы, содержит объявления основных методов WQuery
* \author Alex "DarkWolf" Nem (https://github.com/darkoffalex)
* \version 1.0
* \date 2018-2019
* \copyright (C) 2018-2019 by Alex "DarkWolf" Nem
*/

#pragma once

#include "stdafx.h"
#include "types/common.h"
#include "gui/Window.h"
#include "gui/Button.h"
#include "gui/TextBox.h"
#include "tools/text.h"
#include "tools/files.h"

namespace wquery
{
	/**
	* \brief Своеобразная "процедурная скобка" с которой начинается взаимодействие с библиотекой
	* \detail Функция регистрирует оконный клас и производит необходимые манипуляции
	* \param hInstance Хендл текущего приложения (модуля)
	*/
	void Begin(HINSTANCE hInstance = nullptr);

	/**
	* \brief Своеобразная "процедурная скобка" которой оканичнвается взаимодействие с библиотекой
	* \param loopType Тип цикла, который будет запущен для обработки оконных сообщений \see wquery::MainLoopType
	* \param afterIterationCallback Функция которая может быть вызвана после исполнения каждой итерации цикла
	*/
	void End(const MainLoopType loopType = MainLoopType::GET_MSG, std::function<void(Window * pWindow)> afterIterationCallback = nullptr);
}