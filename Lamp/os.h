#pragma once

typedef unsigned char byte;
typedef unsigned char *pbyte;
typedef wchar_t UCChar;

extern "C" {

    typedef enum {
       CET_UNKNOWN = 0,
       CET_ANSI    = 1,
       CET_UTF7    = 2,
       CET_UTF8    = 3,
       CET_UTF16   = 4, // is also CET_UCS2
       CET_UTF16be = 5, // is also CET_UCS2
       CET_UTF32   = 6,  // is also CET_UCS4
       CET_UTF32be = 7  // is also CET_UCS4
    }CharEncodingType;

// Prototypes

UCChar *os_Format(const UCChar *szFormat, va_list argList);
void os_FormatCleanup(UCChar *pBuff);

char *os_Format8(const char *szFormat, va_list argList);
void os_FormatCleanup8(char *pBuff);

size_t os_utf16_to_other( void *mbstr, const wchar_t *wcstr, size_t sourcecount, size_t count, bool bStrIsPath, CharEncodingType cet);

  //size_t os_other_to_utf16 returns the number of characters needed, you must alocate one character more for the null
  // which this rutine will set.
size_t os_other_to_utf16( UCChar *wcstr, const void *mbstr, size_t count, CharEncodingType cet, size_t buffersize);

void os_PushToTextClipboard(const UCChar *pText, const UCChar *format);
int os_GetTextClipboardCharCount(const UCChar *format);
void os_PullFromTextClipboard(UCChar *answer, int numchars, const UCChar *format);

bool os_CommaAsDecimalPoint();

}
