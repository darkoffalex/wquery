/**
* \brief Реализация класса окна
* \details Обертка над WinApi окном, позволяет управлять основными свойствами и событиями окна
* \author Alex "DarkWolf" Nem (https://github.com/darkoffalex)
* \version 1.0
* \date 2018-2019
* \copyright (C) 2018-2019 by Alex "DarkWolf" Nem
*/

#include <wquery/stdafx.h>
#include <wquery/gui/Window.h>
#include <wquery/gui/ControlBase.h>
#include <wquery/gui/Button.h>
#include <wquery/gui/TextBox.h>
#include <wquery/tools/text.h>

#define DEFAULT_WINDOW_W 350
#define DEFAULT_WINDOW_H 200

namespace wquery
{
	/**
	* \brief Глобальный хендл приложения, инициализируется в методе wquery::Begin()
	* \see wquery.cpp
	*/
	extern HINSTANCE hInstance_;

	/**
	* \brief Глобальная структура параметров оконного класса, инициализируется в методе wquery::Begin()
	* \see wquery.cpp
	*/
	extern WNDCLASSEX classInfo_;

	/**
	* \brief Конструктор
	* \param parent Родительское окно (не обязательно)
	*/
	Window::Window(Window * parent) :
		hWnd_(nullptr),
		parent_(parent),
		backgroundColor_(ColorRGB(240, 240, 240)),
		closesProgram_(true),
		oldClientAreaSize_({ 0,0 }),
		maxSizes_({ 0,0 }),
		minSizes_({ 0,0 })
	{
		// Создание окна WinApi
		// Принадлежность и стиль окна меняются в зависимости от того
		// передан ли указатель на родительский объект (родительское окно)
		this->hWnd_ = CreateWindow(
			classInfo_.lpszClassName,
			L"WQueryWindow",
			(this->parent_ && this->parent_->hWnd_ ? (WS_OVERLAPPED | WS_CAPTION | WS_CHILDWINDOW | WS_SYSMENU) : WS_OVERLAPPEDWINDOW),
			0, 0,
			DEFAULT_WINDOW_W, DEFAULT_WINDOW_H,
			(this->parent_ && this->parent_->hWnd_ ? this->parent_->hWnd_ : NULL),
			NULL,
			hInstance_,
			NULL);

		// Если окно было создано
		if (this->hWnd_)
		{
			// В поле GWLP_USERDATA, в созданном окне, будет записан указатель на данный объект
			// Таким образом к объекту можно будет обратиться в оконной процедуре
			SetWindowLongPtr(this->hWnd_, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

			// Установить "старые размеры" клиенсткой области
			this->oldClientAreaSize_ = this->GetSize(true);
		}
	}

	/**
	* \brief Деструктор. Уничтожение окна
	*/
	Window::~Window()
	{
		// Уничтожение окна WinApi
		if (this->hWnd_) {
			DestroyWindow(this->hWnd_);
		}
	}

	/**
	* \brief Показывает окно (приводит вид в нормальное состояние)
	*/
	void Window::Show() const
	{
		if (this->hWnd_) {
			ShowWindow(this->hWnd_, SW_SHOWNORMAL);
			UpdateWindow(this->hWnd_);
		}
	}

	/**
	* \brief Прячет окно (делает невидимым, то же что и при закрытии)
	*/
	void Window::Hide() const
	{
		if (this->hWnd_) {
			ShowWindow(this->hWnd_, SW_HIDE);
			UpdateWindow(this->hWnd_);
		}
	}

	/**
	* \brief Получить WinApi хендл окна
	* \return Хендл окна
	*/
	HWND Window::GetNativeHandle() const
	{
		return this->hWnd_;
	}

	/**
	* \brief Получить родительский объект
	* \return Указатель на родителя
	*/
	Window* Window::GetParent() const
	{
		return this->parent_;
	}

	/**
	* \brief Установка заголовка окна
	* \param title Заголовок (строка)
	*/
	void Window::SetTitle(const std::string& title) const
	{
		if (this->hWnd_) SetWindowTextA(this->hWnd_, title.c_str());
	}

	/**
	* \brief Получение заголовка окна
	* \return Строка с заголовком
	*/
	std::string Window::GetTitle() const
	{
		std::string result;
		if (this->hWnd_)
		{
			const unsigned int textLength = GetWindowTextLengthA(this->hWnd_) + 1;
			char * buffer = new char[textLength];
			GetWindowTextA(this->hWnd_, buffer, textLength);
			result.append(buffer);
			delete[] buffer;

		}
		return result;
	}

	/**
	* \brief Установка размеров окна
	* \param size Размеры
	* \param clientArea Изменять клиентскую область (размеры рассчитываются уже по ней)
	*/
	void Window::SetSize(const Vector2D<int>& size, const bool clientArea) const
	{
		if (this->hWnd_)
		{
			// Разница медлу размерами клиенской области и окна в целом
			auto deltaWidth = 0, deltaHeight = 0;

			// Если размер должен вычисляться по размеру клиенской области
			// то нужно получить разницу между размерами оконной и клиенской области
			if (clientArea)
			{
				RECT windowRect = {};
				RECT clientRect = {};

				if (GetWindowRect(this->hWnd_, &windowRect) && GetClientRect(this->hWnd_, &clientRect))
				{
					deltaWidth = (windowRect.right - windowRect.left) - (clientRect.right - clientRect.left);
					deltaHeight = (windowRect.bottom - windowRect.top) - (clientRect.bottom - clientRect.top);
				}
			}

			SetWindowPos(
				this->hWnd_,                      // Хендл окна
				nullptr,                          // Хендл окна, над которым окно будет размещено (Z-уровень)
				NULL,                             // Положение левой стороны окна (не меняется)
				NULL,                             // Положение верха окна (не меняется)
				size.X + deltaWidth,              // Новая ширина окна в пикселях
				size.Y + deltaHeight,             // Новая высота окна в пикселях
				SWP_ASYNCWINDOWPOS | SWP_NOMOVE   // Асинхронное изменение (изменяет нить владеющая окном) без смены положения
			);
		}
	}

	/**
	* \brief Получение размеров окна
	* \param clientArea Получить размеры клиентской области (не всего окна)
	* \return Размеры
	*/
	Vector2D<int> Window::GetSize(const bool clientArea) const
	{
		Vector2D<int> sizes;

		if (this->hWnd_)
		{
			RECT rect = {};

			if (clientArea)
			{
				GetClientRect(this->hWnd_, &rect);
			}
			else
			{
				GetWindowRect(this->hWnd_, &rect);
			}

			sizes.X = rect.right - rect.left;
			sizes.Y = rect.bottom - rect.top;
		}

		return sizes;
	}

	/**
	* \brief Установка положения
	* \param position Положение верхнего левого угла окна
	*/
	void Window::SetPosition(const Vector2D<int>& position) const
	{
		if (this->hWnd_) {
			SetWindowPos(
				this->hWnd_,                      // Хендл окна
				nullptr,                          // Хендл окна, над которым окно будет размещено (Z-уровень)
				position.X,                       // Положение левой стороны окна
				position.Y,                       // Положение верха окна
				NULL,                             // Новая ширина окна в пикселях (не меняется)
				NULL,                             // Новая высота окна в пикселях (не меняется)
				SWP_ASYNCWINDOWPOS | SWP_NOSIZE   // Асинхронное изменение (изменяет нить владеющая окном) без смены размера
			);
		}
	}

	/**
	* \brief Получение положения
	* \param relative Получать относительное положение (относительно родительского окна)
	* \return Положение верхнего левого угла окна
	*/
	Vector2D<int> Window::GetPosition(const bool relative) const
	{
		Vector2D<int> position;

		if (this->hWnd_)
		{
			RECT posRect = {};
			POINT posPoint = {};

			if (GetWindowRect(this->hWnd_, &posRect))
			{
				posPoint.x = posRect.left;
				posPoint.y = posRect.top;
			}

			if (relative && this->parent_ != nullptr && this->parent_->hWnd_ != nullptr)
			{
				ScreenToClient(this->parent_->hWnd_, &posPoint);
			}

			position.X = posPoint.x;
			position.Y = posPoint.y;
		}

		return position;
	}

	/**
	* \brief Установка цвета фона
	* \param color Цвет фона RGB
	*/
	void Window::SetBgColor(const ColorRGB& color)
	{
		this->backgroundColor_ = color;

		if (this->hWnd_)
		{
			RECT clientAreaRect;
			GetClientRect(this->hWnd_, &clientAreaRect);
			InvalidateRect(this->hWnd_, &clientAreaRect, true);
		}
	}

	/**
	* \brief Получение цвета фона
	* \return Цвет фона RGB
	*/
	ColorRGB Window::GetBgColor() const
	{
		return this->backgroundColor_;
	}

	/**
	* \brief Закртывает ли программу закрытие окна (установка параметра)
	* \param status Статус
	*/
	void Window::SetClosesProgram(const bool status)
	{
		this->closesProgram_ = status;
	}

	/**
	* \brief Закрывает ли програму закрытие окна (получение параметра)
	* \return Статус
	*/
	bool Window::IsClosesProgram() const
	{
		return this->closesProgram_;
	}

	/**
	* \brief Установка максимального размера окна
	* \param sizes Размер
	* \param clientArea Для клиентской области (размер окна вычилсяется)
	*/
	void Window::SetMaxSizes(const Vector2D<int>& sizes, const bool clientArea)
	{
		if (this->hWnd_)
		{
			// Разница между размерами клиенской области и окна в целом
			auto deltaWidth = 0, deltaHeight = 0;

			// Если размер должен вычисляться по размеру клиенской области
			// то нужно получить разницу между размерами оконной и клиенской области
			if (clientArea)
			{
				RECT windowRect = {};
				RECT clientRect = {};

				if (GetWindowRect(this->hWnd_, &windowRect) && GetClientRect(this->hWnd_, &clientRect))
				{
					deltaWidth = (windowRect.right - windowRect.left) - (clientRect.right - clientRect.left);
					deltaHeight = (windowRect.bottom - windowRect.top) - (clientRect.bottom - clientRect.top);
				}
			}

			// Установить размер с учетом разницы между размером окна и размером кл. области
			this->maxSizes_.X = sizes.X + deltaWidth;
			this->maxSizes_.Y = sizes.Y + deltaHeight;
		}
	}

	/**
	* \brief Получить максимальный размер окна
	* \param clientArea Размер клиентской области (с учетом размера окна)
	* \return Размер
	*/
	Vector2D<int> Window::GetMaxSizes(const bool clientArea) const
	{
		// Разница между размерами клиенской области и окна в целом
		auto deltaWidth = 0, deltaHeight = 0;

		// Если размер должен вычисляться по размеру клиенской области
		// то нужно получить разницу между размерами оконной и клиенской области
		if (clientArea && this->hWnd_)
		{
			RECT windowRect = {};
			RECT clientRect = {};

			if (GetWindowRect(this->hWnd_, &windowRect) && GetClientRect(this->hWnd_, &clientRect))
			{
				deltaWidth = (windowRect.right - windowRect.left) - (clientRect.right - clientRect.left);
				deltaHeight = (windowRect.bottom - windowRect.top) - (clientRect.bottom - clientRect.top);
			}
		}

		// Вернуть раземр с учетом разницы между клиентской областью или окном
		// Если clientArea было установлено в true, значит запрашивается размер клиенсткой
		// области (окно - минус разница), но не факт что именно размер клиентской области а не 
		// окна целиком был установлен через set-функцию, из-за чего возможно отрицательное значение.
		// Чтобы этого избежать - используется max между полученым значением и нулем
		return{
			max(this->maxSizes_.X - deltaWidth,0),
			max(this->maxSizes_.Y - deltaHeight,0)
		};
	}

	/**
	* \brief Установка минимального размера окна
	* \param sizes Размер
	* \param clientArea Для клиентской области (размер окна вычилсяется)
	*/
	void Window::SetMinSizes(const Vector2D<int>& sizes, const bool clientArea)
	{
		if (this->hWnd_)
		{
			// Разница между размерами клиенской области и окна в целом
			auto deltaWidth = 0, deltaHeight = 0;

			// Если размер должен вычисляться по размеру клиенской области
			// то нужно получить разницу между размерами оконной и клиенской области
			if (clientArea)
			{
				RECT windowRect = {};
				RECT clientRect = {};

				if (GetWindowRect(this->hWnd_, &windowRect) && GetClientRect(this->hWnd_, &clientRect))
				{
					deltaWidth = (windowRect.right - windowRect.left) - (clientRect.right - clientRect.left);
					deltaHeight = (windowRect.bottom - windowRect.top) - (clientRect.bottom - clientRect.top);
				}
			}

			// Установить размер с учетом разницы между размером окна и размером кл. области
			this->minSizes_.X = sizes.X + deltaWidth;
			this->minSizes_.Y = sizes.Y + deltaHeight;
		}
	}

	/**
	* \brief Получить минимальный размер окна
	* \param clientArea Размер клиентской области (с учетом размера окна)
	* \return Размер
	*/
	Vector2D<int> Window::GetMinSizes(const bool clientArea) const
	{
		// Разница между размерами клиенской области и окна в целом
		auto deltaWidth = 0, deltaHeight = 0;

		// Если размер должен вычисляться по размеру клиенской области
		// то нужно получить разницу между размерами оконной и клиенской области
		if (clientArea && this->hWnd_)
		{
			RECT windowRect = {};
			RECT clientRect = {};

			if (GetWindowRect(this->hWnd_, &windowRect) && GetClientRect(this->hWnd_, &clientRect))
			{
				deltaWidth = (windowRect.right - windowRect.left) - (clientRect.right - clientRect.left);
				deltaHeight = (windowRect.bottom - windowRect.top) - (clientRect.bottom - clientRect.top);
			}
		}

		// Вернуть раземр с учетом разницы между клиентской областью или окном
		// Если clientArea было установлено в true, значит запрашивается размер клиенсткой
		// области (окно - минус разница), но не факт что именно размер клиентской области а не 
		// окна целиком был установлен через set-функцию, из-за чего возможно отрицательное значение.
		// Чтобы этого избежать - используется max между полученым значением и нулем
		return{
			max(this->minSizes_.X - deltaWidth,0),
			max(this->minSizes_.Y - deltaHeight,0)
		};
	}

	/**
	* \brief Установить состояние кнопки закрытия
	* \param enabled Статус
	*/
	void Window::SetCloseButtonStatus(const bool enabled) const
	{
		if (this->hWnd_)
		{
			if (!enabled)
				EnableMenuItem(GetSystemMenu(this->hWnd_, FALSE), SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			else
				EnableMenuItem(GetSystemMenu(this->hWnd_, FALSE), SC_CLOSE, MF_BYCOMMAND | MF_ENABLED);
		}
	}

	/**
	* \brief Установить состояние кнопки минимизации
	* \param enabled Статус
	*/
	void Window::SetMinimizeButtonStatus(const bool enabled) const
	{
		if (this->hWnd_)
		{
			if (!enabled)
				SetWindowLong(this->hWnd_, GWL_STYLE, GetWindowLong(this->hWnd_, GWL_STYLE) & ~WS_MINIMIZEBOX);
			else
				SetWindowLong(this->hWnd_, GWL_STYLE, GetWindowLong(this->hWnd_, GWL_STYLE) | WS_MINIMIZEBOX);
		}
	}

	/**
	* \brief Установить состояние кнопки максимизации
	* \param enabled Статус
	*/
	void Window::SetMaximizeButtonStatus(const bool enabled) const
	{
		if (this->hWnd_)
		{
			if (!enabled)
				SetWindowLong(this->hWnd_, GWL_STYLE, GetWindowLong(this->hWnd_, GWL_STYLE) & ~WS_MAXIMIZEBOX);
			else
				SetWindowLong(this->hWnd_, GWL_STYLE, GetWindowLong(this->hWnd_, GWL_STYLE) | WS_MAXIMIZEBOX);
		}
	}

	/**
	* \brief Установить состояние (видимость) системного меню окна
	* \param visible Статус
	*/
	void Window::SetSysMenuStatus(const bool visible) const
	{
		if (this->hWnd_)
		{
			if (!visible)
				SetWindowLong(this->hWnd_, GWL_STYLE, GetWindowLong(this->hWnd_, GWL_STYLE) & ~(WS_CAPTION | WS_SIZEBOX));
			else
				SetWindowLong(this->hWnd_, GWL_STYLE, GetWindowLong(this->hWnd_, GWL_STYLE) | WS_CAPTION | WS_SIZEBOX);
		}

	}

	/**
	* \brief Максимизировать окно
	*/
	void Window::Maximize() const
	{
		if (this->hWnd_)
			ShowWindow(this->hWnd_, SW_SHOWMAXIMIZED);
	}

	/**
	* \brief Минимизировать окно
	*/
	void Window::Minimize() const
	{
		if (this->hWnd_)
			ShowWindow(this->hWnd_, SW_SHOWMINIMIZED);
	}


	/**
	* \brief Оконная процедура - обработчик оконных сообщений (событий) системы.
	* \param hWnd Хендл окна, которому адресовано сообщение
	* \param message Идентификатор сообщения
	* \param wParam Один из параметров перданных окну с сообщением
	* \param lParam Один из параметров перданных окну с сообщением
	* \return Код состояния
	*/
	LRESULT Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		// Получить указатель на wQuery объект
		Window * window = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

		// Основной swicth-case оконной процедуры
		switch (message)
		{
		case WM_COMMAND:
			if(HIWORD(wParam) == EN_CHANGE)
			{
				HWND controlHwnd = reinterpret_cast<HWND>(lParam);
				ControlBase * pControl = reinterpret_cast<ControlBase*>(GetWindowLongPtr(controlHwnd, GWLP_USERDATA));

				if (pControl)
				{
					const std::string className = pControl->GetControlClassName();

					if (className == "TextBox")
					{
						TextBox * pTextBox = dynamic_cast<TextBox*>(pControl);
						if (pTextBox->events.onChanged) pTextBox->events.onChanged();
					}
				}
			}
			else
			{
				HWND controlHwnd = reinterpret_cast<HWND>(lParam);
				ControlBase * pControl = reinterpret_cast<ControlBase*>(GetWindowLongPtr(controlHwnd, GWLP_USERDATA));

				if (pControl)
				{
					const std::string className = pControl->GetControlClassName();

					if (className == "Button")
					{
						Button * pButton = dynamic_cast<Button*>(pControl);
						if (pButton->events.onClicked) pButton->events.onClicked();
					}
				}
			}
			break;

		case WM_DESTROY:
			return DefWindowProc(hWnd, message, wParam, lParam);

		case WM_ERASEBKGND:
			if (window)
			{
				RECT clientAreaRect;
				GetClientRect(hWnd, &clientAreaRect);
				FillRect(reinterpret_cast<HDC>(wParam), &clientAreaRect, window->backgroundColor_.GetNativeBrush());
			}
			break;

		case WM_SIZE:
			if (window)
			{
				if (window->events.onResized)
				{
					auto const type = static_cast<unsigned int>(wParam);
					const Vector2D<int> newSizes(static_cast<unsigned int>(LOWORD(lParam)), static_cast<unsigned int>(HIWORD(lParam)));

					window->events.onResized(type, newSizes);
				}

				Vector2D<int> sizingDelta((LOWORD(lParam)) - window->oldClientAreaSize_.X, (HIWORD(lParam)) - window->oldClientAreaSize_.Y);
				EnumChildWindows(window->hWnd_, Window::EnumerateResizingControls, reinterpret_cast<LPARAM>(&sizingDelta));

				window->oldClientAreaSize_ = Vector2D<int>(
					static_cast<unsigned int>(LOWORD(lParam)),
					static_cast<unsigned int>(HIWORD(lParam)));
			}
			return DefWindowProc(hWnd, message, wParam, lParam);

		case WM_KEYDOWN:
			if (window && window->events.onKeyDown)
			{
				window->events.onKeyDown(wParam);
			}
			return DefWindowProc(hWnd, message, wParam, lParam);

		case WM_KEYUP:
			if (window && window->events.onKeyUp)
			{
				window->events.onKeyUp(wParam);
			}
			return DefWindowProc(hWnd, message, wParam, lParam);

		case WM_CHAR:
			if (window && window->events.onTyping)
			{
				window->events.onTyping(wquery::WideToChar(wParam));
			}
			return 0;

		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
			if (window && window->events.onMouseKeyDown)
			{
				Vector2D<int> position;
				position.X = static_cast<int>(LOWORD(lParam));
				position.Y = static_cast<int>(HIWORD(lParam));

				MouseKeys keyType;
				if (message == WM_LBUTTONDOWN) keyType = MouseKeys::LEFT;
				else if (message == WM_MBUTTONDOWN) keyType = MouseKeys::MIDDLE;
				else keyType = MouseKeys::RIGHT;

				window->events.onMouseKeyDown(position, keyType);
			}
			return DefWindowProc(hWnd, message, wParam, lParam);

		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
			if (window && window->events.onMouseKeyUp)
			{
				Vector2D<int> position;
				position.X = static_cast<int>(LOWORD(lParam));
				position.Y = static_cast<int>(HIWORD(lParam));

				MouseKeys keyType;
				if (message == WM_LBUTTONDOWN) keyType = MouseKeys::LEFT;
				else if (message == WM_MBUTTONDOWN) keyType = MouseKeys::MIDDLE;
				else keyType = MouseKeys::RIGHT;

				window->events.onMouseKeyUp(position, keyType);
			}
			return DefWindowProc(hWnd, message, wParam, lParam);

		case WM_MOUSEMOVE:
			if (window && window->events.onMouseMove)
			{
				Vector2D<int> position;
				position.X = static_cast<int>(LOWORD(lParam));
				position.Y = static_cast<int>(HIWORD(lParam));
				window->events.onMouseMove(position);
			}
			return DefWindowProc(hWnd, message, wParam, lParam);

		case WM_PAINT:
			if (window && window->events.onPaint)
			{
				window->events.onPaint();
			}
			return DefWindowProc(hWnd, message, wParam, lParam);

		case WM_CLOSE:
			if (window)
			{
				if (window->events.onClose)
				{
					if (!window->events.onClose()) return 0;
				}

				if (window->closesProgram_) PostQuitMessage(0);
			}
			ShowWindow(hWnd, SW_HIDE);
			break;

		case WM_GETMINMAXINFO:
			if (window)
			{
				reinterpret_cast<LPMINMAXINFO>(lParam)->ptMinTrackSize.x = window->minSizes_.X;
				reinterpret_cast<LPMINMAXINFO>(lParam)->ptMinTrackSize.y = window->minSizes_.Y;

				if (window->maxSizes_.X > 0 && window->maxSizes_.Y > 0 &&
					window->maxSizes_.X >= window->minSizes_.X &&
					window->maxSizes_.Y >= window->minSizes_.Y)
				{
					reinterpret_cast<LPMINMAXINFO>(lParam)->ptMaxTrackSize.x = window->maxSizes_.X;
					reinterpret_cast<LPMINMAXINFO>(lParam)->ptMaxTrackSize.y = window->maxSizes_.Y;
				}
			}

			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		return 0;
	}

	/**
	* \brief Используется для обновление размеров адаптивных (якорных) элементов
	* \details Метод является callback-ом для перечисления дочерних элементов в момент изменения размера кона
	* \param hWnd Хендл элемента WinApi
	* \param lParam Параметр переданный из функции перечисления
	* \return Состояние
	*/
	BOOL Window::EnumerateResizingControls(HWND hWnd, LPARAM lParam)
	{
		// Наименование класса элемента управления (строка на 255 символов)
		char className[255];

		// Если удалось получить наименование класса
		if (GetClassNameA(hWnd, className, 255))
		{
			// Интерпретировать значение lParam как указатель на Vector2D<int>* (дельта смены размеров)
			Vector2D<int>* sizingDelta = reinterpret_cast<Vector2D<int>*>(lParam);

			// Если это не окно
			if (strcmp(className, "WQueryWndClass") != 0)
			{
				// Получить указатель на wQuery объект через значение GWLP_USERDATA у элемента
				ControlBase * pControl = reinterpret_cast<ControlBase*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

				// Исходные размеры и положение элемента
				Vector2D<int> sizes = pControl->GetSize();
				Vector2D<int> position = pControl->GetPosition();

				// Если есть якорь для левого и правого края одновременно - элемент растягивается (меняется размер)
				if (pControl->GetAnchor().left && pControl->GetAnchor().right)
				{
					sizes.X += sizingDelta->X;
				}
				// Если есть якорь только правого края - элемент перемещается вместе с краем
				else if (pControl->GetAnchor().right)
				{
					position.X += sizingDelta->X;
				}

				// Если есть якорь для верха и низа - элемент растягивается (по вертикали, меняется размер)
				if(pControl->GetAnchor().top && pControl->GetAnchor().bottom)
				{
					sizes.Y += sizingDelta->Y;
				}
				// Если есть якорь только для низа - элемент перемещается вместе с низом
				else if(pControl->GetAnchor().bottom)
				{
					position.Y += sizingDelta->Y;
				}

				// Установка новых параметров
				pControl->SetSize(sizes);
				pControl->SetPosition(position);
			}

		}
		return TRUE;
	}
}