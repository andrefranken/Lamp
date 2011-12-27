#include "stdafx.h"
#include "os.h"
#include "ucstring.h"


bool os_CommaAsDecimalPoint()
{
   bool result = false;
   double val = atof(",1");
   if(val != 0.0)
   {
      result = true;
   }
   return result;
}



//////////////////////////////////////////////////////////////////////
//
// os_Format() tries to store the formatted string into a default 1k buffer, 
// if the buffer is not large enough _vsnwprintf() will return a negative value,
// in the error case we increment the output buffersize and try again until we 
// get a buffer large enough or we exhaust memory. 
//
// _vsnprintf and _vsnwprintf return the number of characters written, 
// not including the terminating null character, or a negative value 
// if an output error occurs. 
//
//////////////////////////////////////////////////////////////////////

#define FORMAT_BUFFER_INC (1024)

//////////////////////////////////////////////////////////////////////
//
UCChar *os_Format(const UCChar *szFormat, va_list argList)
{
   int buffersize = FORMAT_BUFFER_INC;
   UCChar *pBuff = new UCChar[buffersize];
   
   while (_vsnwprintf_s( pBuff, buffersize, buffersize, szFormat, argList) < 0) {
       buffersize += FORMAT_BUFFER_INC;
       delete []pBuff;
       pBuff = new UCChar[buffersize];
   }
   return pBuff;
}

//////////////////////////////////////////////////////////////////////
//
void os_FormatCleanup(UCChar *pBuff) 
{
   delete []pBuff;   
}

//////////////////////////////////////////////////////////////////////
//
char *os_Format8(const char *szFormat, va_list argList)
{
   int buffersize = FORMAT_BUFFER_INC;
   char *pBuff = new char[buffersize];
   
   while (_vsnprintf_s( pBuff, buffersize, buffersize, szFormat, argList) < 0) {
       buffersize += FORMAT_BUFFER_INC;
       delete []pBuff;
       pBuff = new char[buffersize];
   }
   return pBuff;
}

//////////////////////////////////////////////////////////////////////
//
void os_FormatCleanup8(char *pBuff) 
{
   delete []pBuff;   
}

// this only supports outputting to 8bit flavors right now.
size_t os_utf16_to_other( void *mbstr, const UCChar *wcstr, size_t sourcecount, size_t count, bool bStrIsPath, CharEncodingType cet )
{   
   UINT codepage = CP_ACP;
   switch(cet)
   {
   case CET_UTF7: codepage = CP_UTF7; break;
   case CET_UTF8: codepage = CP_UTF8; break;
   }

   if(sourcecount == 0)
   {
      sourcecount = wcslen(wcstr);
   }

   return WideCharToMultiByte(codepage,     // code page
                             0,             // performance and mapping flags
                             wcstr,         // wide-character string
                             sourcecount, // number of chars in string
                             (LPSTR)mbstr,         // buffer for new string
                             count,         // size of buffer
                             NULL,           // default for unmappable chars
                             NULL); // set when default char used
}

size_t os_other_to_utf16( UCChar *wcstr, const void *mbstr, size_t count, CharEncodingType cet, size_t buffersize)//( UCChar *wcstr, const char *mbstr, size_t count, bool utf8 )
{
   if(cet == CET_UTF16 ||
      cet == CET_UTF16be)
   {
      if(wcstr == NULL || buffersize == 0)
      {
         // find out how many chars are needed
         return count;
      }
      else
      {
         if(cet == CET_UTF16be)
         {
            // copy inverted bytes
            byte *read = (byte*)mbstr;
            byte *write = (byte*)wcstr;
            for(size_t i = 0; i < count; i++)
            {
               *write = *(read+1);
               write++;
               *write = *read;
               read += 2;
               write++;
            }
            wcstr[count] = L'\0';
            return count;
         }
         else
         {
            // copy
            wcsncpy_s(wcstr,buffersize,(wchar_t*)mbstr,count);
            wcstr[count] = L'\0';
            return count;
         }
      }
   }
   else if(cet == CET_UTF32 ||
           cet == CET_UTF32be)
   {
      if(wcstr == NULL || buffersize == 0)
      {
         // find out how many chars are needed
         size_t charscounted = 0;
         wchar_t *read1 = (wchar_t*)mbstr;
         wchar_t *read2 = ((wchar_t*)mbstr) + 1;

         for(size_t i = 0; i < count; i++)
         {
            if(*read1 != 0) charscounted++;
            if(*read2 != 0) charscounted++;

            read1 += 2;
            read2 += 2;
         }

         return charscounted;
      }
      else
      {
         if(cet == CET_UTF32be)
         {
            // copy inverted bytes
            size_t charsoutput = 0;
            wchar_t *read2 = (wchar_t*)mbstr;
            wchar_t *read1 = ((wchar_t*)mbstr) + 1;
            wchar_t *write = wcstr;

            for(size_t i = 0; i < count; i++)
            {
               *write = ((wchar_t)((*read1) << 8)) + ((wchar_t)((*read1) >> 8));
               charsoutput++;
               write++;
               
               if(*read2 != 0)
               {
                  *write = ((wchar_t)((*read2) << 8)) + ((wchar_t)((*read2) >> 8));
                  charsoutput++;
                  write++;
               }

               read1 += 2;
               read2 += 2;
            }

            wcstr[charsoutput] = L'\0';
            return charsoutput;
         }
         else
         {
            // copy
            size_t charsoutput = 0;
            wchar_t *read1 = (wchar_t*)mbstr;
            wchar_t *read2 = ((wchar_t*)mbstr) + 1;
            wchar_t *write = wcstr;

            for(size_t i = 0; i < count; i++)
            {
               *write = *read1;
               charsoutput++;
               write++;
               
               if(*read2 != 0)
               {
                  *write = *read2;
                  charsoutput++;
                  write++;
               }

               read1 += 2;
               read2 += 2;
            }

            wcstr[charsoutput] = L'\0';
            return charsoutput;
         }
      }
   }
   else
   {
      UINT codepage;
      switch(cet)
      {
      case CET_UTF7: codepage = CP_UTF7; break;
      case CET_UTF8: codepage = CP_UTF8; break;
      default:       codepage = CP_ACP; break; // CET_UNKNOWN and CET_ANSI
      }

      return MultiByteToWideChar(codepage,       // code page
                              0,             // performance and mapping flags
                              (LPCSTR)mbstr,         // multibyte string
                              count,         // number of chars in string
                              wcstr,         // buffer for new string
                              buffersize);   // size of buffer
   }
}


void os_PushToTextClipboard(const UCChar *pText, const UCChar *format)
{
   int len = wcslen(pText);

   if (len > 0)
   {
      if(OpenClipboard(NULL)) 
      {
         EmptyClipboard();

         HANDLE hMem = GlobalAlloc(GHND, (len + 1) * sizeof(UCChar));

         if(hMem != NULL) 
         {
            UCChar *pStr = (UCChar *)GlobalLock(hMem);

            wcsncpy_s(pStr, len + 1, pText,_TRUNCATE);
            pStr[len] = L'\0';

            GlobalUnlock(hMem);

            UINT iformat = CF_UNICODETEXT;
   
            if(format != NULL &&
               wcslen(format) > 0)
            {
               iformat = RegisterClipboardFormatW(format);
            }

            SetClipboardData(iformat, hMem);
         }
         CloseClipboard();
      }
   }
}

int os_GetTextClipboardCharCount(const UCChar *format)
{
   int numchars = 0;

   if(OpenClipboard(NULL))
   {
      UINT iformat = CF_UNICODETEXT;
      if(format != NULL &&
         wcslen(format) > 0)
      {
         iformat = RegisterClipboardFormatW(format);
      }

      // try with unicode text first, cose Windows NT convert CF_TEXT to CF_UNICODETEXT
      HANDLE hData = ::GetClipboardData(iformat);
      if(NULL != hData)
      {
         numchars = GlobalSize(hData);
         numchars/=2;
         numchars--; // the returned count includes the trailing '\0'
      }
      else if(iformat == CF_UNICODETEXT)// if doing unicode, try 8-bit
      {
         hData = ::GetClipboardData( CF_TEXT );
         if(NULL != hData)
         {
            numchars = GlobalSize(hData);
            numchars--; // the returned count includes the trailing '\0'
         }
      }

      CloseClipboard();
   }

   return numchars;
}

void os_PullFromTextClipboard(UCChar *answer, int numchars, const UCChar *format)
{
   if(OpenClipboard(NULL))
   {
      UINT iformat = CF_UNICODETEXT;
      
      if(format != NULL &&
         wcslen(format) > 0)
      {
         iformat = RegisterClipboardFormatW(format);
      }

      UCChar *pWBuf;
      char *pBuf;
      DWORD nbytes;
      HANDLE hData = ::GetClipboardData(iformat);
      // try with unicode text first, cose Windows NT convert CF_TEXT to CF_UNICODETEXT
      if((NULL != hData &&
         ((nbytes = GlobalSize(hData)) > 0 ) &&
         (( pWBuf = (UCChar *)::GlobalLock(hData)) != NULL)))
      {
         nbytes/=2;
         nbytes--; // the returned count includes the trailing '\0'

         UCChar *pWrite = answer;
         UCChar *pChar = pWBuf;

         while( pChar < pWBuf + nbytes )
         {
            *pWrite = *pChar;
            pWrite++; 
            pChar++;
         }
         *pWrite = L'\0';
         ::GlobalUnlock(hData);
      }
      else if(iformat == CF_UNICODETEXT)// if doing unicode, try 8-bit
      {
         if(((( hData = ::GetClipboardData( CF_TEXT ))!= NULL ) &&
            ((nbytes = GlobalSize(hData)) > 0 ) &&
            ((pBuf = (char *)::GlobalLock(hData)) != NULL )))
         {
            nbytes--; // the returned count includes the trailing '\0'

            UCChar *pWrite = answer;
            char *pChar = pBuf;

            while( pChar < pBuf + nbytes )
            {
               *pWrite = (UCChar)*pChar;
               pWrite++; 
               pChar++;
            }
            *pWrite = L'\0';
            ::GlobalUnlock(hData);
         }
      }
      CloseClipboard();
   }
}


bool os_CreateRecursiveDirectoryPath(const UCChar* path)
{
   bool result = false;
   if(path != NULL &&
      wcslen(path) > 0)
   {
      if(_waccess(path, 00 ) == 0 )
      {
         result = true;
      }
      else
      {
         UCString pathToCreate = path;
         UCString basePath;
         UCString dir;
         int i=0;
         while(pathToCreate.GetToken(dir, i, L'\\',false))
         {
            if(dir.Length() > 0)
            {
               basePath += dir;
               basePath += L"\\";

               if(_waccess(basePath, 00 ) != 0 )
               {
                  if(!CreateDirectory(basePath, NULL))
                  {
                     result = false;
                     break;
                  }
               }
               else
               {
                  result = true;
               }
            }

            i++;
         }
      }
   }

   return result;
}
