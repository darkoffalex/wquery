/**
* \brief Реализация основных методов WQuery
* \author Alex "DarkWolf" Nem (https://github.com/darkoffalex)
* \version 1.0
* \date 2018-2019
* \copyright (C) 2018-2019 by Alex "DarkWolf" Nem
*/

#include <wquery/stdafx.h>
#include <wquery/wquery.h>

namespace wquery
{
	/**
	* \brief Глобальный хендл приложения, инициализируется в методе wquery::Begin()
	* \see wquery.cpp
	*/
	HINSTANCE hInstance_;

	/**
	* \brief Глобальная структура параметров оконного класса, инициализируется в методе wquery::Begin()
	* \see wquery.cpp
	*/
	WNDCLASSEX classInfo_;

	/**
	* \brief Своеобразная "процедурная скобка" с которой начинается взаимодействие с библиотекой
	* \detail Функция регистрирует оконный клас и производит необходимые манипуляции
	* \param hInstance Хендл текущего приложения (модуля)
	*/
	void Begin(HINSTANCE hInstance)
	{
		const wchar_t * windowClassName = L"WQueryWndClass";

		// Попопытаться получить информацию об уже зарегистрированном классе
		// окон WQuery. Если удалось - прервать выполнение (класс зарегистрирован)
		WNDCLASSEX registeredClassInfo = {};
		if (GetClassInfoEx(hInstance, windowClassName, &registeredClassInfo)) {
			return;
		}

		// Если хендл приложения не был передан - использовать хендл по умолчанию
		hInstance_ = hInstance ? hInstance : GetModuleHandle(nullptr);

		// Описать новый класс окон при помощи структуры
		classInfo_ = {};
		classInfo_.cbSize = sizeof(WNDCLASSEX);
		classInfo_.style = CS_HREDRAW | CS_VREDRAW;
		classInfo_.cbClsExtra = 0;
		classInfo_.cbWndExtra = 0;
		classInfo_.hInstance = hInstance_;
		classInfo_.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
		classInfo_.hCursor = LoadCursor(nullptr, IDC_ARROW);
		classInfo_.hbrBackground = wquery::ColorRGB(240, 240, 240).GetNativeBrush();
		classInfo_.lpszMenuName = nullptr;
		classInfo_.lpszClassName = windowClassName;
		classInfo_.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);
		classInfo_.lpfnWndProc = wquery::Window::WndProc;

		// Регистрация класса окна
		if (!RegisterClassEx(&classInfo_)) {
			throw std::runtime_error("Can't register new class");
		}
	}

	/**
	* \brief Своеобразная "процедурная скобка" которой оканичнвается взаимодействие с библиотекой
	* \param loopType Тип цикла, который будет запущен для обработки оконных сообщений \see wquery::MainLoopType
	* \param afterIterationCallback Callback функция которая может быть вызвана после исполнения каждой итерации цикла
	*/
	void End(const MainLoopType loopType, std::function<void(Window * pWindow)> afterIterationCallback)
	{
		// Структура с информацией о сообщении
		MSG msg = {};

		// В зависимости от типа основного цикла использованы разные подходы
		// Если тип GET_MSG - будет реализован блокирующий подход (цикл приостанавливается до получения след. сообщения)
		// Если тип PEEK_MSG - цикл постоянно работает и проверяет сообщения (перенаправляя окну в случае получения)
		switch (loopType)
		{
		case MainLoopType::GET_MSG:
			while (GetMessage(&msg, nullptr, 0, 0))
			{
				if (msg.message == WM_QUIT) {
					break;
				}

				TranslateMessage(&msg);
				DispatchMessage(&msg);

				if (afterIterationCallback) {
					Window* pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(msg.hwnd, GWLP_USERDATA));
					afterIterationCallback(pWindow);
				}
			}
			break;

		case MainLoopType::PEEK_MSG:
			while (true)
			{
				if (msg.message == WM_QUIT) {
					break;
				}

				if (PeekMessage(&msg, nullptr, 0, 0, true)) {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				if (afterIterationCallback) {
					Window* pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(msg.hwnd, GWLP_USERDATA));
					afterIterationCallback(pWindow);
				}
			}
		}
	}
}