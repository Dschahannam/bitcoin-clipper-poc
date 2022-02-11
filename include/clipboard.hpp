#pragma once
#include <WinUser.h>
#include <stdexcept>
#include <WinBase.h>

class c_clipboard
{
public:
    c_clipboard()
    {
        OpenClipboard(nullptr);
    }

    ~c_clipboard()
    {
        CloseClipboard();
    }

private:
    c_clipboard(const c_clipboard&);
    c_clipboard& operator=(const c_clipboard&);
};

class c_text_global_lock
{
public:
    explicit c_text_global_lock(HANDLE hData)
        : m_hData(hData)
    {
        m_psz = static_cast<const char*>(GlobalLock(m_hData));
        if (!m_psz)
            return;
    }

    ~c_text_global_lock()
    {
        GlobalUnlock(m_hData);
    }

    const char* Get() const
    {
        return m_psz;
    }

private:
    HANDLE m_hData;
    const char* m_psz;

    c_text_global_lock(const c_text_global_lock&);
    c_text_global_lock& operator=(const c_text_global_lock&);
};