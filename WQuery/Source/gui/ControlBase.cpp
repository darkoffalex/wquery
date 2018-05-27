#include <wquery/stdafx.h>
#include <wquery/gui/ControlBase.h>
#include "wquery/tools/text.h"

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
	* \brief Конструктор элемента управления
	* \param window Указатель на владеющее окно
	* \param controlClassName Наименовая WinApi класса элемента управления
	* \param dwStyle Стиль отображения элемента
	*/
	ControlBase::ControlBase(Window* window, const std::string& controlClassName, DWORD dwStyle) :
		hWnd_(nullptr),
		window_(window),
		anchor_(AnchorSettings(false, false, false, false)),
	    customFont_(nullptr)
	{
		// Все элементы управления в WinApi являются окнами, отличаются их классы (controlClassName) и стили.
		// В системе есть ряд предустановленых классов окон используемых для элементов управления.
		// Наследуемые от данного класса дочерные классы, в зависимости от своего типа и предназначения, в параметре controlClassName
		// передают в базовый конструктор (этот) разные наименования WinApi классов окон (напр. Static - для лейбла, Button - для кнопки)
		if (hInstance_ && this->window_ && this->window_->GetNativeHandle() && !controlClassName.empty())
		{
			this->hWnd_ = CreateWindowA(
				controlClassName.c_str(),
				NULL,
				dwStyle,
				0, 0,
				150, 30,
				this->window_->GetNativeHandle(),
				NULL,
				hInstance_,
				NULL
			);
		}

		// Если элемент был создан
		if (this->hWnd_)
		{
			// В поле GWLP_USERDATA, созданного элемента управления, будет записан указатель на данный объект
			// Таким образом к объекту можно будет обратиться в оконной процедуре
			SetWindowLongPtr(this->hWnd_, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

			// Установить шрифт по умолчанию
			SendMessage(this->hWnd_, WM_SETFONT, reinterpret_cast<WPARAM>(GetStockObject(DEFAULT_GUI_FONT)), MAKELPARAM(TRUE, 0));
		}
	}

	/**
	* \brief Деструктор (вирутальный, уничтожает в том числе и объект-наследник)
	*/
	ControlBase::~ControlBase()
	{
		if (this->hWnd_)
			DestroyWindow(this->hWnd_);
	}

	/**
	* \brief Получить указатель на владеющее окно
	* \return Указатель
	*/
	Window* ControlBase::GetWindow() const
	{
		return this->window_;
	}

	/**
	* \brief Установить текст элемента управления
	* \param text Текст
	*/
	void ControlBase::SetText(const std::string& text) const
	{
		if (this->hWnd_)
		{
			SetWindowTextA(this->hWnd_, text.c_str());
		}
	}

	/**
	* \brief Получить текст элемента управления
	* \return Текст
	*/
	std::string ControlBase::GetText() const
	{
		std::string result;
		if (this->hWnd_)
		{
			const unsigned int textLength = GetWindowTextLengthA(this->hWnd_)+1;
			char * buffer = new char[textLength];
			GetWindowTextA(this->hWnd_, buffer, textLength);
			result.append(buffer);
			delete[] buffer;

		}
		return result;
	}

	/**
	* \brief Установить положение
	* \param position Положение
	*/
	void ControlBase::SetPosition(Vector2D<int> position) const
	{
		if (this->hWnd_) {
			SetWindowPos(
				this->hWnd_,                      // WinApi хендл элемента
				nullptr,                          // Хендл элемента, над которым текущий будет размещен (Z-уровень)
				position.X,                       // Положение левой стороны
				position.Y,                       // Положение верха
				NULL,                             // Новая ширина в пикселях (не меняется)
				NULL,                             // Новая высота в пикселях (не меняется)
				SWP_ASYNCWINDOWPOS | SWP_NOSIZE   // Асинхронное изменение (изменяет нить владеющая окном) без смены размера
			);
		}
	}

	/**
	* \brief Получить положение
	* \return Положение
	*/
	Vector2D<int> ControlBase::GetPosition() const
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

			if (this->window_ != nullptr && this->window_->GetNativeHandle() != nullptr)
			{
				ScreenToClient(this->window_->GetNativeHandle(), &posPoint);
			}

			position.X = posPoint.x;
			position.Y = posPoint.y;
		}

		return position;
	}

	/**
	* \brief Установить размеры
	* \param size Размеры
	*/
	void ControlBase::SetSize(Vector2D<int> size) const
	{
		if (this->hWnd_)
		{
			SetWindowPos(
				this->hWnd_,                      // WinApi хендл элемента
				nullptr,                          // Хендл элемента, над которым текущий будет размещен (Z-уровень)
				NULL,                             // Положение левой стороны (не меняется)
				NULL,                             // Положение верха (не меняется)
				size.X,                           // Новая ширина в пикселях
				size.Y,                           // Новая высота в пикселях
				SWP_ASYNCWINDOWPOS | SWP_NOMOVE   // Асинхронное изменение (изменяет нить владеющая окном) без смены положения
			);

			if(this->window_->GetNativeHandle())
			{
				UpdateWindow(this->window_->GetNativeHandle());
			}
		}
	}

	/**
	* \brief Получить размеры
	* \return Размеры
	*/
	Vector2D<int> ControlBase::GetSize() const
	{
		Vector2D<int> sizes;

		if (this->hWnd_)
		{
			RECT rect = {};
			GetWindowRect(this->hWnd_, &rect);
			sizes.X = rect.right - rect.left;
			sizes.Y = rect.bottom - rect.top;
		}

		return sizes;
	}

	/**
	* \brief Установить состояние элемента управления (активен/не активен)
	* \param state Состояние
	*/
	void ControlBase::SetEnabled(const bool state) const
	{
		if (this->hWnd_) EnableWindow(this->hWnd_, state);
	}

	/**
	* \brief Проверить активирован ли элемент
	* \return Статус
	*/
	bool ControlBase::IsEnabled() const
	{
		if (!this->hWnd_) return false;
		return !(GetWindowLong(this->hWnd_, GWL_STYLE) & WS_DISABLED);
	}

	/**
	* \brief Установить привязку (якорь) элемента управления к сторонам контейнера
	* \param anchor Параметры привязки (якоря)
	*/
	void ControlBase::SetAnchor(const AnchorSettings& anchor)
	{
		this->anchor_ = anchor;
	}

	/**
	* \brief Получить привязку (якорь)
	* \return Параметры привязки
	*/
	AnchorSettings ControlBase::GetAnchor() const
	{
		return this->anchor_;
	}

	/**
	* \brief Установка параметров шрифта элемента управления
	* \param font Параметры шрифта
	*/
	void ControlBase::SetFont(const FontSettings& font)
	{
		if(this->hWnd_)
		{
			// Если кастомный шрифт уже был установлен ранее - следует очистить объект этого шрифта
			if (this->customFont_) {
				DeleteObject(this->customFont_);
			}

			// Создать новый объект шрифта в памяти и получить его хендл
			this->customFont_ = CreateFontA(
				font.size,
				FALSE,
				FALSE,
				FW_DONTCARE,
				font.bold ? 600 : 1,
				font.italic,
				FALSE,
				FALSE,
				ANSI_CHARSET,
				OUT_TT_PRECIS,
				CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_DONTCARE,
				font.fontFamilyName);

			// Отправить сообщение элементу управления о смене шрифта
			SendMessage(this->hWnd_, WM_SETFONT, reinterpret_cast<WPARAM>(this->customFont_), TRUE);

			// Обновление области элемента управления
			InvalidateRect(this->hWnd_, nullptr, TRUE);
			UpdateWindow(this->window_->GetNativeHandle());
		}
	}

	/**
	* \brief Получение параметров шрифта элемента управления
	* \return Параметры шрифта
	*/
	FontSettings ControlBase::GetFont() const
	{
		// Результирующий объект с параметрами шрифта
		FontSettings result;

		// Хендл используемого шрифта (если кастомный шрифт не установлен, значит используется шрифт по умолчанию)
		HFONT hFont = this->customFont_ ? this->customFont_ : reinterpret_cast<HFONT>(GetStockObject(DEFAULT_GUI_FONT));

		// Структура с информацией о шрифте
		LOGFONT lf;

		// Если информация о шрифте была получена - вписать необходимые данные в result
		if (GetObject(hFont, sizeof(LOGFONT), &lf)) {
			result.bold = lf.lfWeight >= 600;
			result.size = static_cast<unsigned int>(lf.lfHeight);
			result.italic = !!lf.lfItalic;
			result.fontFamilyName = WideToStr(lf.lfFaceName).c_str();
		}

		return result;
	}

	/**
	* \brief Очистка шрифта (вернуть шрифт по умолчанию)
	*/
	void ControlBase::ClearFont()
	{
		if (this->hWnd_)
		{
			// Если кастомный шрифт уже был установлен ранее - следует очистить объект этого шрифта
			if (this->customFont_) {
				DeleteObject(this->customFont_);
				this->customFont_ = nullptr;
			}

			// Отправить сообщение элементу управления о смене шрифта на шрифт по умочланию
			SendMessage(this->hWnd_, WM_SETFONT, reinterpret_cast<WPARAM>(GetStockObject(DEFAULT_GUI_FONT)), TRUE);

			// Обновление области элемента управления
			InvalidateRect(this->hWnd_, nullptr, TRUE);
			UpdateWindow(this->window_->GetNativeHandle());
		}
	}
}
