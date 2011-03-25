/******************************************************************************
 *
 *  Latest Chatty Command Line Client
 *  By Electroly.
 *
 *  WINDOWS.C
 *
 *  An implementation of PLATFORM.H with Windows-specific API calls.
 *  
 *****************************************************************************/

#include "stdafx.h"
//#include "console.h"
#include "platform.h"

static HANDLE hStdout;

void os_sockets_init(void)
{
   WSADATA wsa_data;
   WSAStartup(MAKEWORD(2, 0), &wsa_data);
}

void os_sockets_cleanup(void)
{
   WSACleanup();
}

void os_console_init(void)
{
   CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

   hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
   
   if (hStdout == INVALID_HANDLE_VALUE)
      exit(1);
}

void os_get_console_size(unsigned int* width, 
                         unsigned int* height)
{
   CONSOLE_SCREEN_BUFFER_INFO info;

   if (GetConsoleScreenBufferInfo(hStdout, &info) == 0)
      exit(1);

   *width  = info.srWindow.Right + 1;
   *height = info.srWindow.Bottom + 1;;
}
 
void os_set_console_color(BOOL fg_red, BOOL fg_green, BOOL fg_blue, 
                          BOOL bg_red, BOOL bg_green, BOOL bg_blue, 
                          BOOL fg_bright, BOOL bg_bright)
{
   SetConsoleTextAttribute(hStdout,
      (fg_red    ? FOREGROUND_RED       : 0) |
      (fg_green  ? FOREGROUND_GREEN     : 0) |
      (fg_blue   ? FOREGROUND_BLUE      : 0) |
      (bg_red    ? BACKGROUND_RED       : 0) |
      (bg_green  ? BACKGROUND_GREEN     : 0) |
      (bg_blue   ? BACKGROUND_BLUE      : 0) |
      (fg_bright ? FOREGROUND_INTENSITY : 0) |
      (bg_bright ? BACKGROUND_INTENSITY : 0));
}

void os_move_console_cursor(unsigned int x, 
                            unsigned int y)
{
   COORD coord;

   coord.X = x;
   coord.Y = y;
   SetConsoleCursorPosition(hStdout, coord);
}

bool os_open_url(const char* url)
{
   bool result = true;
   LPTSTR t_url;
   size_t length = strlen(url);
   int    RetVal;

#ifdef _UNICODE
   t_url = (LPTSTR)malloc(length * 2 + 2);
   memset(t_url, 0, length * 2 + 2);
   //mbstowcs(t_url, url, length);
   size_t numconverted = 0;
   mbstowcs_s(&numconverted,t_url, length, url, length);

#else
   t_url = malloc(length + 1);
   memset(t_url, 0, length + 1);
   strcpy(t_url, url);
#endif

#pragma warning (disable: 4311) /* Warns when casting from HINSTANCE to int */
   RetVal = (int) ShellExecute(      
      /* hwnd         */ (HWND) NULL,
      /* lpOperation  */ TEXT("open"),
      /* lpFile       */ t_url,
      /* lpParameters */ (LPCTSTR) NULL,
      /* lpDirectory  */ (LPCTSTR) NULL,
      /* nShowCmd     */ SW_SHOW);
#pragma warning (default: 4311)

   if (RetVal <= 32)
   {
      result = false;
   }
   return result;
}


