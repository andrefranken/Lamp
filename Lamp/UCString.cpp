#include "stdafx.h"
#include "UCString.h"
#include "Lamp.h"

extern UCString g_PathToMe;

UCString::UCString()
{
   Init();
   MakeValid();
}

UCString::UCString(const UCString &otherstring)
{
   Init();

   UCString *pother = (UCString *)&otherstring;
   if(pother != NULL)
   {
      AppendUnicodeString(*pother, pother->Length());
   }
}

UCString::UCString(const UCChar *otherstring)
{
   Init();
   if(otherstring != NULL)
   {
      AppendUnicodeString(otherstring, StringLength(otherstring));
   }
}

UCString::UCString(const char *otherstring)
{
   Init();
   if(otherstring != NULL)
   {
      AppendEncodedString(otherstring, strlen(otherstring));
   }
}

UCString::UCString(const UCChar *otherstring, int length)
{
   Init();
   if(otherstring != NULL)
   {
      AppendUnicodeString(otherstring, length);
   }
}

UCString::UCString(const char *otherstring, int length)
{
   Init();
   if(otherstring != NULL)
   {
      AppendEncodedString(otherstring, length);
   }
}

UCString::UCString(float val, bool internalUse /*= false*/)
{
   Init();
   if(internalUse)
   {
      FormatFloatWithPeriod(val);
   }
   else
   {
      AppendValue(val);
   }
}

UCString::UCString(double val, bool internalUse /*= false*/)
{
   Init();
   if(internalUse)
   {
      FormatFloatWithPeriod(val);
   }
   else
   {
      AppendValue(val);
   }
}

UCString::UCString(unsigned int val)
{
   Init();
   AppendValue(val);
}

UCString::UCString(int val)
{
   Init();
   AppendValue(val);
}

UCString::UCString(__int64 val)
{
    Init();
    AppendValue(val);
}

UCString::UCString(bool val)
{
   Init();
   AppendValue(val);
}

void UCString::Init(void)
{
   m_string = NULL;
   m_string_8bit = NULL;
   m_8bitIsPath = false;
   m_8bitCET = CET_UNKNOWN;
   m_8bitAddBOM = false;
   m_8bitbytesize = 0;
   m_stringlength = 0;
   m_extrachars = 0;
   m_returnBuffer = NULL;
   m_returnBufferLength = 0;
}

/*****************  comparison operators **********************/

bool UCString::operator == (const UCChar *otherstring) const
{
   bool result = false;
   if(otherstring == NULL &&
      (m_string == NULL || Length() == 0))
   {
      result = true;
   }
   else if(m_string != NULL && otherstring != NULL)
   {
      if(Length() == StringLength(otherstring))
      {
         if(_wcsicmp(m_string, otherstring) == 0)
         {
            result = true;
         }
      }
   }
   return result;
}

bool UCString::operator != (const UCChar *otherstring) const
{
  return(!(*this == otherstring));
}

bool UCString::operator == (const UCString &otherstring) const
{
   bool result = false;
   if(m_string != NULL && &otherstring != NULL)
   {
      if(Length() == otherstring.Length())
      {
         const UCChar *potherstring = otherstring;
         if(_wcsicmp(m_string, potherstring) == 0)
         {
            result = true;
         }
      }
   }
   return result;
}

bool UCString::operator != (const UCString &otherstring) const
{
  return(!(*this == otherstring));
}

bool UCString::operator < (const UCString &otherstring) const
{
  return cmp(otherstring) < 0;
}

bool UCString::operator > (const UCString &otherstring) const
{
  return cmp(otherstring) > 0;
}

bool UCString::operator < (const UCChar *otherstring) const
{
  return cmp(otherstring) < 0;
}

bool UCString::operator > (const UCChar *otherstring) const
{
  return cmp(otherstring) > 0;
}

/*****************  comparison methods **********************/

int UCString::cmp(const UCChar *str) const 
{
   return wcscmp (*this, str);
}

int UCString::icmp(const UCChar *str) const 
{
   return _wcsicmp (*this, str);
}

int UCString::ncmp(const UCChar *str, const int iCount) const 
{
   return wcsncmp (*this, str, iCount);
}

int UCString::nicmp(const UCChar *str, const int iCount) const 
{
   return _wcsnicmp(*this, str, iCount);
}

const UCChar *UCString::subicmp(const UCChar *str) const 
{
   UCString cmp1(*this), cmp2(str);

   cmp1.MakeLower();
   cmp2.MakeLower();

   return m_string + (wcsstr(cmp1, cmp2) - (const UCChar *)cmp1);
}

const UCChar *UCString::subcmp(const UCChar *str) const
{
   return wcsstr(*this, str);
}

const UCChar *UCString::endswith(const UCChar *str) const
{
   if(str != NULL)
   {
      int len = wcslen(str);
      if(len <= m_stringlength)
      {
         int offset = m_stringlength - len;
         UCChar *result = m_string + offset;
         if(wcscmp(result,str) == 0) 
            return result;
      }
   }
   return NULL;
}

const UCChar *UCString::beginswith(const UCChar *str) const
{
   const UCChar *result = wcsstr(*this, str); 
   if(result != NULL)
   {  
      if(result == m_string) return result;
      return NULL;
   }
   return NULL;
}

/*****************  assignment operators **********************/

UCString &UCString::operator = (const UCChar character)
{
   FreeString(true);   
   if(character != L'\0')
   {      
      AppendUnicodeString(&character, 1);
   }
   FreeMetaStrings();
   return(*this);
}

UCString &UCString::operator = (const UCChar *otherstring)
{
   if(m_string != otherstring)
   {  
      UCChar *pTmp = NULL;
      if(otherstring != NULL && 
         (otherstring >= m_string && 
          otherstring < m_string + m_stringlength) ||
         (m_returnBuffer != NULL && 
          otherstring >= m_returnBuffer && 
          otherstring < m_returnBuffer + m_returnBufferLength)) // if we are pointing to ourselves
      {
         int len = StringLength(otherstring);
         if(len > 0)
         {
            pTmp = new UCChar[len+1];
            memcpy(pTmp, otherstring, 2*len);
            pTmp[len] = L'\0';
            otherstring = pTmp;
         }
      }

      FreeString(true);
      if(otherstring != NULL)
      {
         AppendUnicodeString(otherstring, StringLength(otherstring));
      }
      MakeValid();
      FreeMetaStrings();
      if(pTmp != NULL)
      {
         delete []pTmp;
      }
   }
   return(*this);
}
               
UCString &UCString::operator = (const UCString &otherstring)
{
   if(this != &otherstring)
   {
      FreeString();
      UCString *pother = (UCString *)&otherstring;
      if(pother != NULL)
      {
         AppendUnicodeString(*pother, pother->Length());
      }
   }
   return(*this);
}

UCString &UCString::operator = (const char *other8bitstring)
{
   if(m_string_8bit != other8bitstring)
   {
      FreeString(true);
      if(other8bitstring != NULL)
      {
         AppendEncodedString(other8bitstring, strlen(other8bitstring));
      }
      FreeMetaStrings();
   }
   return(*this);
}      

UCString &UCString::operator = (float val)
{
   FreeString();
   AppendValue(val);
   return(*this);
}

UCString &UCString::operator = (double val)
{
   FreeString();
   AppendValue(val);
   return(*this);
}

UCString &UCString::operator = (unsigned int val)
{
   FreeString();
   AppendValue(val);
   return(*this);
}

UCString &UCString::operator = (int val)
{
   FreeString();
   AppendValue(val);
   return(*this);
}

UCString &UCString::operator = (__int64 val)
{
    FreeString();
    AppendValue(val);
    return(*this);
}

UCString &UCString::operator = (bool val)
{
   FreeString();
   AppendValue(val);
   return(*this);
}

/*****************  append operators **********************/

UCString &UCString::operator += (const UCChar character)
{
   if(character != L'\0')
   {
   AppendUnicodeString(&character, 1);
   }
   return(*this);
}

UCString &UCString::operator += (const UCChar *otherstring)
{
   if(m_string == otherstring)
   {
      if(otherstring != NULL)
      {
         UCString trueother(otherstring);
         AppendUnicodeString(trueother.Str(), trueother.Length());
      }
   }
   else
   {
      if(otherstring != NULL)
      {
         AppendUnicodeString(otherstring, StringLength(otherstring));
      }
   }
   return(*this);
}

UCString &UCString::operator += (const char character)
{
   if(character != '\0')
   {      
      UCChar ch = (unsigned char)character;
      AppendUnicodeString(&ch, 1);
   }
   return(*this);
}

UCString &UCString::operator += (const UCString &otherstring)
{
   if(this == &otherstring)
   {
      UCString trueother(otherstring);
      AppendUnicodeString(trueother.Str(), trueother.Length());
   }
   else
   {
      if(&otherstring != NULL)
      {
         AppendUnicodeString(otherstring, otherstring.Length());
      }
   }
   return(*this);
}

UCString &UCString::operator += (const char *other8bitstring)
{
   if(m_string_8bit == other8bitstring)
   {
      if(other8bitstring != NULL)
      {
         UCString trueother(other8bitstring);
         AppendUnicodeString(trueother.Str(), trueother.Length());
      }
   }
   else
   {
      if(other8bitstring != NULL)
      {
         AppendEncodedString(other8bitstring, strlen(other8bitstring));
      }
   }
   return(*this);
}

UCString &UCString::operator += (float val)
{
   AppendValue(val);
   return(*this);
}

UCString &UCString::operator += (double val)
{
   AppendValue(val);
   return(*this);
}

UCString &UCString::operator += (unsigned int val)
{
   AppendValue(val);
   return(*this);
}

UCString &UCString::operator += (int val)
{
   AppendValue(val);
   return(*this);
}

UCString &UCString::operator += (__int64 val)
{
    AppendValue(val);
    return(*this);
}

UCString &UCString::operator += (bool val)
{
   AppendValue(val);
   return(*this);
}

/*****************  cast operators **********************/

UCChar UCString::operator []( int nIndex ) const
{
   if(m_string != NULL && Length() > nIndex)
   {
      return m_string[nIndex];         
   }
   else return 0;
}


UCString::operator float () const
{
   return((float)GetDouble());
}

UCString::operator double () const
{
   return(GetDouble());
}

UCString::operator unsigned int() const
{
   return(GetUInt());
}

UCString::operator int() const
{
   return(GetInt());
}

UCString::operator __int64() const
{
    return(GetInt64());
}

UCString::operator bool() const
{
   return(GetBool());
}

UCString::operator COLORREF() const
{
   return(GetColor());
}


/******** encoding binary data as text support ************/

UCChar UCString::EncodeBinaryByteToWideChar(unsigned char binaryval)
{
   // map the number to an ASCII char
   
   if(binaryval >= 0 &&
      binaryval <= 25)
   {
      return (L'a' + binaryval);
   }
   else if(binaryval >= 26 &&
           binaryval <= 51)
   {
      return (L'A' + binaryval - 26);
   }
   else if(binaryval >= 52 &&
           binaryval <= 61)
   {
      return (L'0' + binaryval - 52);
   }
   else if(binaryval == 62)
   {
      return L'-';
   }
   else if(binaryval == 63)
   {
      return L'+';
   }
   else 
   {
      return 0xFFFF;
   }
}

unsigned char UCString::DecodeWideCharToBinaryByte(const UCChar asciival)
{
   // map the ASCII char to a number
   if(asciival >= L'a' &&
      asciival <= L'z')
   {
      return asciival - L'a';
   }
   else if(asciival >= L'A' &&
           asciival <= L'Z')
   {
      return 26 + asciival - L'A';
   }
   else if(asciival >= L'0' &&
           asciival <= L'9')
   {
      return 52 + asciival - L'0';
   }
   else if(asciival == L'-')
   {
      return 62;
   }
   else if(asciival == L'+')
   {
      return 63;
   }
   else 
   {
      return 0;
   }
}

void UCString::EncodeBinaryToASCIIText(void *pData, unsigned int numbytes)
{
   // every 3 bytes of original data will equal 4 bytes of encoded data.
   // (original) 11111111 11111111 11111111 
   // (encoded)  00111111 00111111 00111111 00111111
   unsigned int numbytetriplets = numbytes / 3;
   unsigned int numbytesencoded = numbytetriplets * 4;

   // now add in the remainder
   unsigned int remainingbytes = numbytes % 3;
   if(remainingbytes > 0 && remainingbytes < 3)
   {
      // remaining must be 1 or 2      
      numbytesencoded += remainingbytes + 1;
   }   

   // add in room for 6 more encoded chars.  
   // These 6 will hold the 4 bytes needed to 
   // insert an unsigned int which will contain 
   // the number of bytes in the original data.
   numbytesencoded += 6;
   
   UCChar *pString = Reserve(numbytesencoded);

   UCChar *pWrite = pString;

   // write out the 6 char "byte size" value
   unsigned char *pRead = (unsigned char *)(&numbytes);

   pWrite[0] = EncodeBinaryByteToWideChar( pRead[0] >> 2 );
   pWrite[1] = EncodeBinaryByteToWideChar( ((unsigned char)(pRead[0] << 6) >> 2) + (pRead[1] >> 4) );
   pWrite[2] = EncodeBinaryByteToWideChar( ((unsigned char)(pRead[1] << 4) >> 2) + (pRead[2] >> 6) );
   pWrite[3] = EncodeBinaryByteToWideChar( (unsigned char)(pRead[2] << 2) >> 2 );
   pWrite += 4;
   pRead += 3;      
   pWrite[0] = EncodeBinaryByteToWideChar( pRead[0] >> 2 );
   pWrite[1] = EncodeBinaryByteToWideChar( (unsigned char)(pRead[0] << 6) >> 2 );
   pWrite += 2;

   // write out all the triple byte sections
   pRead = (unsigned char *)pData;

   while(numbytetriplets)
   {
      // (original) 00000000 11111111 22222222
      // (encoded)  ..000000 ..001111 ..111122 ..222222

      pWrite[0] = EncodeBinaryByteToWideChar( pRead[0] >> 2 );
      pWrite[1] = EncodeBinaryByteToWideChar( ((unsigned char)(pRead[0] << 6) >> 2) + (pRead[1] >> 4) );
      pWrite[2] = EncodeBinaryByteToWideChar( ((unsigned char)(pRead[1] << 4) >> 2) + (pRead[2] >> 6) );
      pWrite[3] = EncodeBinaryByteToWideChar( (unsigned char)(pRead[2] << 2) >> 2 );

      numbytetriplets--;
      pWrite += 4;
      pRead += 3;      
   }   

   // finish out the last byte or two
   // (original) 00000000 11111111 22222222
   // (encoded)  ..000000 ..001111 ..111122 ..222222

   if(remainingbytes == 1)
   {      
      pWrite[0] = EncodeBinaryByteToWideChar( pRead[0] >> 2 );
      pWrite[1] = EncodeBinaryByteToWideChar( (unsigned char)(pRead[0] << 6) >> 2 );
   }
   else if(remainingbytes == 2)
   {
      pWrite[0] = EncodeBinaryByteToWideChar( pRead[0] >> 2 );
      pWrite[1] = EncodeBinaryByteToWideChar( ((unsigned char)(pRead[0] << 6) >> 2) + (pRead[1] >> 4) );
      pWrite[2] = EncodeBinaryByteToWideChar( (unsigned char)(pRead[1] << 4) >> 2 );
   }   
}

unsigned int UCString::DecodeASCIITextToBinary(void *pData, unsigned int numbytes)
{
   unsigned int numbytesdecoded = 0;

   if(Length() > 0)
   {      
      if(Length() > 6 && m_string != NULL)
      {      
         // read the byte size info from the string and return the answer
         numbytesdecoded = 0;
         unsigned char temp[6];

         unsigned char *pWrite = (unsigned char *)(&numbytesdecoded);
         UCChar *pRead = m_string;
         // (encoded) ..000000 ..001111 ..111122 ..222222
         // (decoded) 00000000 11111111 22222222
         temp[0] = DecodeWideCharToBinaryByte(pRead[0]);
         temp[1] = DecodeWideCharToBinaryByte(pRead[1]);
         temp[2] = DecodeWideCharToBinaryByte(pRead[2]);
         temp[3] = DecodeWideCharToBinaryByte(pRead[3]);
         temp[4] = DecodeWideCharToBinaryByte(pRead[4]);
         temp[5] = DecodeWideCharToBinaryByte(pRead[5]);
         pWrite[0] = (unsigned char)(temp[0] << 2) + ((unsigned char)(temp[1] << 2) >> 6);
         pWrite[1] = (unsigned char)(temp[1] << 4) + ((unsigned char)(temp[2] << 2) >> 4);
         pWrite[2] = (unsigned char)(temp[2] << 6) + temp[3];
         pWrite[3] = (unsigned char)(temp[4] << 2) + ((unsigned char)(temp[5] << 2) >> 6);
         
         if(pData != NULL && numbytes > 0)
         {
            if(numbytesdecoded <= numbytes)
            {
               unsigned int numbytetriplets = numbytesdecoded / 3;
               unsigned int remainingbytes = numbytesdecoded % 3;

               pWrite = (unsigned char *)pData;
               pRead += 6;

               // read all the triple bytes

               while(numbytetriplets)
               {
                  // (encoded) ..000000 ..001111 ..111122 ..222222
                  // (decoded) 00000000 11111111 22222222
                  temp[0] = DecodeWideCharToBinaryByte(pRead[0]);
                  temp[1] = DecodeWideCharToBinaryByte(pRead[1]);
                  temp[2] = DecodeWideCharToBinaryByte(pRead[2]);
                  temp[3] = DecodeWideCharToBinaryByte(pRead[3]);               

                  pWrite[0] = (unsigned char)(temp[0] << 2) + ((unsigned char)(temp[1] << 2) >> 6);
                  pWrite[1] = (unsigned char)(temp[1] << 4) + ((unsigned char)(temp[2] << 2) >> 4);
                  pWrite[2] = (unsigned char)(temp[2] << 6) + temp[3];

                  numbytetriplets--;
                  pWrite += 3;
                  pRead += 4;      
               }

               // finish reading out the last byte or two
               // (encoded) ..000000 ..001111 ..111122 ..222222
               // (decoded) 00000000 11111111 22222222

               if(remainingbytes == 1)
               {      
                  temp[0] = DecodeWideCharToBinaryByte(pRead[0]);
                  temp[1] = DecodeWideCharToBinaryByte(pRead[1]);               
                  pWrite[0] = (unsigned char)(temp[0] << 2) + ((unsigned char)(temp[1] << 2) >> 6);
               }
               else if(remainingbytes == 2)
               {
                  temp[0] = DecodeWideCharToBinaryByte(pRead[0]);
                  temp[1] = DecodeWideCharToBinaryByte(pRead[1]);
                  temp[2] = DecodeWideCharToBinaryByte(pRead[2]);               
                  pWrite[0] = (unsigned char)(temp[0] << 2) + ((unsigned char)(temp[1] << 2) >> 6);
                  pWrite[1] = (unsigned char)(temp[1] << 4) + ((unsigned char)(temp[2] << 2) >> 4);
               }
            }
            else
            {
               numbytesdecoded = 0;
            }
         }
      }
   }

   return numbytesdecoded;
}


void UCString::EncodeBinaryToBase64Text(void *pData, unsigned int numbytes)
{
	 // every 3 bytes of original data will equal 4 bytes of encoded data.
   // (original) 11111111 11111111 11111111 
   // (encoded)  00111111 00111111 00111111 00111111
   unsigned int numbytetriplets = numbytes / 3;
   unsigned int numbytesencoded = numbytetriplets * 4;

   // now add in the remainder
   unsigned int remainingbytes = numbytes % 3;
   if(remainingbytes > 0 && remainingbytes < 3)
   {
      // remaining must be 1 or 2      
      numbytesencoded += remainingbytes+1;  // if 1 byte we write 2, 2 bytes, we write 3
   }   

   numbytesencoded += 1; // for '=' base64 terminator.

   int padding = ((numbytesencoded % 4) == 0) ? 0 : 4 - (numbytesencoded % 4);

   numbytesencoded += padding;
   
   UCChar *pString = Reserve(numbytesencoded);  // add one more for null terminator

   UCChar *pWrite = pString;

   unsigned char *pRead = (unsigned char *)pData;

   while(numbytetriplets)
   {
      // (original) 00000000 11111111 22222222
      // (encoded)  ..000000 ..001111 ..111122 ..222222

      pWrite[0] = EncodeBinaryByteToBase64WideChar( pRead[0] >> 2 );
      pWrite[1] = EncodeBinaryByteToBase64WideChar( ((unsigned char)(pRead[0] << 6) >> 2) + (pRead[1] >> 4) );
      pWrite[2] = EncodeBinaryByteToBase64WideChar( ((unsigned char)(pRead[1] << 4) >> 2) + (pRead[2] >> 6) );
      pWrite[3] = EncodeBinaryByteToBase64WideChar( (unsigned char)(pRead[2] << 2) >> 2 );

      numbytetriplets--;
      pWrite += 4;
      pRead += 3;      
   }

   // finish out the last byte or two
   // (original) 00000000 11111111 22222222
   // (encoded)  ..000000 ..001111 ..111122 ..222222

   if(remainingbytes == 1)
   {      
      pWrite[0] = EncodeBinaryByteToBase64WideChar( pRead[0] >> 2 );
      pWrite[1] = EncodeBinaryByteToBase64WideChar( (unsigned char)(pRead[0] << 6) >> 2 );
      pWrite+=2;
   }
   else if(remainingbytes == 2)
   {
      pWrite[0] = EncodeBinaryByteToBase64WideChar( pRead[0] >> 2 );
      pWrite[1] = EncodeBinaryByteToBase64WideChar( ((unsigned char)(pRead[0] << 6) >> 2) + (pRead[1] >> 4) );
      pWrite[2] = EncodeBinaryByteToBase64WideChar( (unsigned char)(pRead[1] << 4) >> 2 );
      pWrite+=3;
   }

   *pWrite = L'=';  // write terminator
   pWrite++;

   for(int i=0;i<padding;i++)   // base64 must be 4 byte padded. Issue 38127 for flash.
   {
      *pWrite = L'=';  // write terminator
      pWrite++;
   }

   //assert(pWrite - m_string == numbytesencoded);
   m_string[numbytesencoded] = L'\0';
}


	// Take the given letter and find its position offset (0-63) within 
	// ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/
	
unsigned char UCString::DecodeBase64WideCharToBinaryByte(UCChar ch)
{
	int result = -1;
	if(ch >= L'A' && ch <= L'Z')
	{
		result = ch - 'A';
	}
	else if(ch >= L'a' && ch <= L'z')
	{
		result = ch - 'a' + 26;		// 26 is offset in A to / master base64 string
	}
	else if (ch >= L'0' && ch <= L'9')
	{
		result = ch - '0' + 52;		// 52 is offset in A to / base64 master string
	}
	else if(ch == '+')
	{
		result = 62;
	}
	else if(ch == '/')
	{
		result = 63;
	}

	return result;
}

UCChar UCString::EncodeBinaryByteToBase64WideChar(unsigned char binval)
{
   UCChar result = 0xFFFF;
   if(binval >= 0 &&
      binval <= 25)
   {
      result = (L'A' + binval);
   }
   else if(binval >= 26 &&
           binval <= 51)
   {
      result = (L'a' + binval - 26);
   }
   else if(binval >= 52 &&
           binval <= 61)
   {
      result = (L'0' + binval - 52);
   }
   else if(binval == 62)
   {
      result = L'+';
   }
   else if(binval == 63)
   {
      result = L'/';
   }

   return result;
   
}

unsigned int UCString::DecodeBase64TextToBinary(void *pData, unsigned int numbytes)
{
   unsigned int numbytesdecoded = 0;

   // read the byte size info from the string and return the answer
   numbytesdecoded = 0;
   unsigned char temp[4] = {0};

   UCChar *pRead = m_string;
   
   unsigned char *pWrite = (unsigned char *)pData;
   // read all the triple bytes

   bool foundTerminator = false;
   while(!foundTerminator)
   {
      // (encoded) ..000000 ..001111 ..111122 ..222222
      // (decoded) 00000000 11111111 22222222

      int numCharsRead = 0;

      while(numCharsRead < 4)
      {
         if(pRead[numCharsRead] == L'=')
         {
            foundTerminator = true;
            break;
         }

         temp[numCharsRead] = DecodeBase64WideCharToBinaryByte(pRead[numCharsRead]);
         numCharsRead++;
      }
      if(numbytesdecoded+numCharsRead-1 > numbytes)
      {
         break;
      }
      switch(numCharsRead)
      {
      case 4:
         {
            pWrite[0] = (unsigned char)(temp[0] << 2) + ((unsigned char)(temp[1] << 2) >> 6);
            pWrite[1] = (unsigned char)(temp[1] << 4) + ((unsigned char)(temp[2] << 2) >> 4);
            pWrite[2] = (unsigned char)(temp[2] << 6) + temp[3];  
            numbytesdecoded+=3;             
         }
         break;
      case 3:
         {
            pWrite[0] = (unsigned char)(temp[0] << 2) + ((unsigned char)(temp[1] << 2) >> 6);
            pWrite[1] = (unsigned char)(temp[1] << 4) + ((unsigned char)(temp[2] << 2) >> 4); 
            numbytesdecoded+=2;
         }
         break;
      case 2:
         {
            pWrite[0] = (unsigned char)(temp[0] << 2) + ((unsigned char)(temp[1] << 2) >> 6); 
            numbytesdecoded++;
         }
      }
      if(!foundTerminator)
      {
         pWrite += 3;
         pRead += 4;
      }
   }
   //assert(foundTerminator);

   return numbytesdecoded;
}


const UCChar *UCString::LEEncodedString()
{
   FreeMetaStrings();
   if(Length() > 0)
   {
      UCChar *buffer = new UCChar[Length() + 2];
      memset(buffer, 0, sizeof(UCChar) * (Length() + 2));
      buffer[0] = 0xFEFF;
      memcpy((buffer+1), m_string, sizeof(UCChar) * Length());
      UCString tmp = buffer;
      SetReturnBuffer(tmp);
      delete []buffer;
   }
   return m_returnBuffer;
}

const UCChar *UCString::BEEncodedString()
{  
   FreeMetaStrings();
   if(Length() > 0)
   {
      UCChar *buffer = new UCChar[Length()+2];
      memset(buffer, 0, sizeof(UCChar)*(Length()+2));
      buffer[0] = 0xFFFE;
      for(int i=0;i<Length();i++)
      {
         buffer[i+1] = (m_string[i] >> 8) + ((m_string[i] & 0x00FF) << 8);
      }
      UCString tmp = buffer;
      SetReturnBuffer(tmp);
   }
   return m_returnBuffer;
}



/********* the only way to get an 8bit multibyte string out *********/

const char *UCString::str8(bool bStrIsPath /*= false*/, 
                           CharEncodingType cet/* = CET_ANSI*/, 
                           bool bAddBOM /*= false*/) const
{
   if(cet == CET_UTF16 ||
      cet == CET_UTF16be ||
      cet == CET_UTF32 ||
      cet == CET_UTF32be)
   {
      // this function only does 8bit encodings
      return NULL;
   }

   if(m_8bitIsPath != bStrIsPath ||
      m_8bitCET != cet ||
      m_8bitAddBOM != bAddBOM)
   {
      Free8bitString();
   }

   m_8bitIsPath = bStrIsPath;
   m_8bitCET = cet;
   m_8bitAddBOM = bAddBOM;

   if(cet != CET_UTF8)
   {
      // only UTF8 supports a BOM
      bAddBOM = false;
   }

   if(m_string_8bit != NULL)
   {
      // if we have asked for this before, just return the answer.
      return m_string_8bit;
   }
   else if(m_string != NULL && m_stringlength > 0)
   {
      size_t bomsize = 0;
      if(bAddBOM)
      {
         bomsize = 3;
      }

      size_t sizeneeded = os_utf16_to_other( NULL, m_string, m_stringlength, 0, bStrIsPath, cet );            

      if(m_8bitbytesize <= (int)sizeneeded + 1)
      {
         m_8bitbytesize = (int)sizeneeded + 1 + bomsize;
         m_string_8bit = new char[m_8bitbytesize];            
      }      
      memset(m_string_8bit, 0, m_8bitbytesize);

      unsigned char *pWrite = (unsigned char *)m_string_8bit;

      if(bAddBOM)
      {
         bomsize = 3;
         pWrite[0] = 0xEF;
         pWrite[1] = 0xBB;
         pWrite[2] = 0xBF;
         pWrite += 3;
      }
   
      os_utf16_to_other( pWrite, m_string, m_stringlength, sizeneeded, bStrIsPath, cet );
      
      return m_string_8bit;      
   }
   else
   {
      m_8bitbytesize = 1;
      m_string_8bit = new char[m_8bitbytesize];            
      m_string_8bit[0] = '\0';
      return m_string_8bit;      
   }
}

/*****************  string operations **********************/


bool UCString::IsEmpty(void) const
{
   if (Length() == 0) 
      return true; 
   return false;
}

bool UCString::IsWhitespace(void)
{
   bool result = false;

   if(Length() > 0) 
   {
      result = true;
      const UCChar *work = m_string;
      const UCChar *end = m_string + m_stringlength;
      while(work < end)
      {
         if(iswspace(*work) == 0)
         {
            result = false;
            break;
         }
         work++;
      }
   }

   return result;
}

void UCString::TrimEnd(int numchars)
{
   if(m_string != NULL && m_stringlength >= numchars)
   {
      m_extrachars += numchars;
      m_stringlength -= numchars;
      m_string[m_stringlength] = L'\0';
      Free8bitString();
   }
}

void UCString::InsertChar(UCChar thechar, int position)
{
   if (Length() == position) 
   {
      AppendUnicodeString(&thechar, 1);
      Free8bitString();
   }
   else if (Length() > position && position >= 0) 
   {
      if(m_extrachars > 0)
      {
         UCChar *work = m_string + m_stringlength;
         UCChar *end = m_string + position;

         *(work+1) = L'\0';
         
         while(work != end)
         {
            *work = *(work-1);
            work--;
         }

         m_string[position] = thechar;
         m_stringlength++;
         m_extrachars--;
      }
      else
      {
         UCString begin = m_string;
         UCString end = m_string + position;

         begin.TrimEnd(m_stringlength - position);

         *this = begin;
         *this += thechar;
         *this += end;
      }
      Free8bitString();
   }
   // else invalid position
}

void UCString::DeleteChar(int position)
{
   if (Length() > position && position >= 0) 
   {
      UCChar *work = m_string + position;

      do
      {
         *work = *(work+1);
         work++;
      }while(*work != L'\0');

      m_stringlength--;
      m_extrachars++;
      
      Free8bitString();
   }
   // else invalid position
}

void UCString::TrimWhitespace(bool bFront /*= true*/, bool bBack /*= true*/)
{
   if(m_string != NULL)
   {
      int whitespace = 0;
      UCChar *pcurrentchar = m_string;
      while(bFront && iswspace(*pcurrentchar))
      {
         whitespace++;
         pcurrentchar++;
      }

      if(whitespace > 0)
      {
         Free8bitString();
         bool done = false;
         UCChar *writechar = m_string;
         m_extrachars += whitespace;
         m_stringlength -= whitespace;
         while(!done)
         {
            *writechar = *pcurrentchar;
            if(*writechar == L'\0')
            {
               break;
            }
            writechar++;
            pcurrentchar++;
         }
      }
      whitespace = 0;
      if(m_string[0] != L'\0')
      {
         pcurrentchar = m_string + wcslen(m_string) - 1;
         while(bBack && iswspace(*pcurrentchar))
         {
            whitespace++;
            *pcurrentchar = L'\0';
            pcurrentchar--;
         }
      }
      if(whitespace > 0)
      {
         Free8bitString();
         m_extrachars += whitespace;
         m_stringlength -= whitespace;
      }
   }      
}

/*****************  token operations **********************/

bool UCString::GetToken(UCString &result, int tokenindex, UCChar token /* = L' '*/, bool bTrimwhitespace/*= true*/) const
{
   bool bFoundit = false;
   if(m_string != NULL)
   {
      bool bDone = false;
      bFoundit = true;
      int timesfound = 0;
      UCChar *occurance = m_string;
      UCChar *lastchar = m_string + wcslen(m_string);
      while( timesfound != tokenindex && occurance != NULL  && bDone == false)
      {
         bFoundit = false;
         timesfound++;
         occurance = wcschr(occurance, token);
         if(occurance != NULL)
         {
            if(timesfound == tokenindex)
            {
               bFoundit = true;
               bDone = true;
            }
            occurance++;
         }
      }      
      if(bFoundit)
      {
         UCChar *nextoccurance = NULL;
         if(occurance < lastchar)
         {
            nextoccurance = wcschr(occurance, token);
         }
         if(nextoccurance == NULL)
         {
            nextoccurance = occurance + wcslen(occurance);
         }

         UCChar swapchar = *nextoccurance;
         *nextoccurance = L'\0';
         result = occurance;
         *nextoccurance = swapchar;
         if(bTrimwhitespace)
         {
            result.TrimWhitespace();
         }
      }
   }
   return bFoundit;
}

/*****************  token operations **********************/

bool UCString::GetToken(UCString &result, int tokenindex, UCString &tokens, bool bTrimwhitespace, UCChar &TerminatingToken) const
{
   bool bFoundit = false;
   TerminatingToken = L'';
   if(m_string != NULL)
   {
      bool bDone = false;
      bFoundit = true;
      int timesfound = 0;
      UCChar *occurance = m_string;
      UCChar *tmpOccurance1,*tmpOccurance2;
      UCChar *lastchar = m_string + wcslen(m_string);
      while( timesfound != tokenindex && occurance != NULL  && bDone == false)
      {
         bFoundit = false;
         timesfound++;
         tmpOccurance2 = NULL;

         for(int i=0;i<tokens.Length();i++)
         {
            tmpOccurance1 = wcschr(occurance, tokens[i]);
            
            if(tmpOccurance1 != NULL)
            {
               if(tmpOccurance2 == NULL)
               {
                  tmpOccurance2 = tmpOccurance1;
               }
               else if(tmpOccurance1 < tmpOccurance2)
               {
                  tmpOccurance2 = tmpOccurance1;
               }
            }
         }
         if(tmpOccurance2 != NULL)
         {
            occurance = tmpOccurance2;
         }

         if(occurance != NULL)
         {
            if(timesfound == tokenindex)
            {
               bFoundit = true;
               bDone = true;
            }
            occurance++;
         }
      }      
      if(bFoundit)
      {
         UCChar *nextoccurance = NULL;
         if(occurance < lastchar)
         {            
            tmpOccurance2 = NULL;

            for(int i=0;i<tokens.Length();i++)
            {
               tmpOccurance1 = wcschr(occurance, tokens[i]);
            
               if(tmpOccurance1 != NULL)
               {
                  if(tmpOccurance2 == NULL)
                  {
                     tmpOccurance2 = tmpOccurance1;
                  }
                  else if(tmpOccurance1 < tmpOccurance2)
                  {
                     tmpOccurance2 = tmpOccurance1;
                  }
               }
            }
            if(tmpOccurance2 != NULL)
            {
               nextoccurance = tmpOccurance2;
            }
         }
         if(nextoccurance == NULL)
         {
            nextoccurance = occurance + wcslen(occurance);
         }

         UCChar swapchar = *nextoccurance;
         *nextoccurance = L'\0';
         result = occurance;
         *nextoccurance = swapchar;
         
         TerminatingToken = swapchar;

         if(bTrimwhitespace)
         {
            result.TrimWhitespace();
         }
      }
   }
   
   return bFoundit;
}
const UCChar *UCString::Find(const UCChar *what, const UCChar *starting/* = NULL*/, bool bPositionAfter/* = false*/) const
{
   const UCChar *result = NULL;
   const UCChar *searchtarget = starting;
   if(searchtarget == NULL)
   {
      searchtarget = m_string;
   }
   if(searchtarget != NULL)
   {
      result = wcsstr(searchtarget, what);

      if(bPositionAfter && result != NULL)
      {
         result += wcslen(what);
      }         
   }
   return result;
}

// FuzzyFind
// returns 0 - 100   pecentage of matching characters

int UCString::FuzzyFind(const UCChar* what, const UCChar *ignore) const
{
   int result = 0;
   int ignoreLength = 0;
   if(ignore)
   {
      ignoreLength = wcslen(ignore);
   }
   if(what != NULL && what[0] != 0)
   {      
      int length = wcslen(what);

      int i=0,j=0;
      bool lastWasAMatch = false;

      while(i < m_stringlength && j < length)
      {
         if(what[j] != m_string[i])
         {
            bool found = false;
            if(ignoreLength > 0)
            {
               for(int k=0;k<ignoreLength;k++)
               {
                  if(what[j] == ignore[k])
                  {
                     j++;
                     found = true;
                     break;
                  }
                  if(m_string[i] == ignore[k])
                  {
                     i++;
                     found = true;
                     break;
                  }
               }
              
               if(!found)
               {
                  if(!lastWasAMatch)
                  {
                     result--;
                  }
                  lastWasAMatch = false;
                  i++;
               }
            }
            else
            {
               i++;
            }
         }
         else
         {
            lastWasAMatch = true;
            result++;
            i++;
            j++;
         }
         if(i == m_stringlength && result != length && j < length)
         {
            j++;
            i = j;
         }
      }
      if(length > 0)      
         result = (100 * result) / length;
   }

   return result;
}

//////////////////////////////////////////////////////////////////////
//
bool UsesForwardSlash(const UCChar *path)
{
   int count = 0;

   if(path != NULL)
   {
      const UCChar *work = path;

      while(*work != NULL)
      {
         if(*work == L'\\')
         {
            count--;
         }
         else if(*work == L'/')
         {
            count++;
         }
         work++;
      }
   }

   if(count > 0)
   {
      return true;
   }
   else
   {
      return false;
   }
}



// formatting (using sprintf style formatting)
const UCChar *UCString::Format(const UCChar *szFormat, ...)
{
   //assert(szFormat != NULL);

   va_list argList;
   va_start(argList, szFormat);

   UCChar *pBuff = os_Format(szFormat, argList);

   *this = pBuff;
   os_FormatCleanup(pBuff);

   va_end(argList);
   
   return Str();
}

// formatting (using sprintf style formatting)
const char *UCString::Format(const char *szFormat, ...)
{
   //assert(szFormat != NULL);

   va_list argList;
   va_start(argList, szFormat);
   
   char *pBuff = os_Format8(szFormat, argList);

   *this = pBuff;
   os_FormatCleanup8(pBuff);

   va_end(argList);
   
   return str8();
}

void UCString::FormatFloatWithPeriod(double val)
{
   UCChar buffer[50];
   buffer[0] = L'\0';

   swprintf_s(buffer,50,L"%.6g",val);
   UCChar *pComma  = wcschr(buffer,L',');
   if(pComma != NULL)
   {
      *pComma = L'.';
   }
   int size = wcslen(buffer);
   AppendUnicodeString(buffer, size);
}

void UCString::MakeNormal(void)
{
   if(!IsEmpty())
   {
      UCChar *pUnicodeChar = m_string;
      UCChar *pEndChar = m_string + Length();

      while(pUnicodeChar < pEndChar)
      {
         char *newchar = NULL;

         if(*pUnicodeChar < 256)
         {
            newchar = (char*)pUnicodeChar;
         }
         else
         {
            if(*pUnicodeChar >= 0x2000 && 
               *pUnicodeChar <= 0x206F)
            {
               // general puctuation         
               static UCChar unicodebuffer[2];
               unicodebuffer[0] = *pUnicodeChar;
               unicodebuffer[1] = L'\0';

               static char output[2];
               output[0] = '\0';
               output[1] = '\0';

               if(CharToOemBuffW(unicodebuffer,output,2))
               {
                  *pUnicodeChar = output[0];
                  newchar = output;
               }
            }
         }

         if(newchar != NULL &&
            *newchar != '\0')
         {
            switch(*newchar)
            {
            case -110:
            case -76:
               *pUnicodeChar = L'\''; // 34
               break;
            case -111:
               *pUnicodeChar = L'`'; // 96
               break;
            case -109:
            case -108:
               *pUnicodeChar = L'\"';
               break;
            case -83:
            case -105:
            case -106:
               *pUnicodeChar = L'-'; // 45
               break;
            case -126:
               *pUnicodeChar = L','; // 44
               break;
            }
         }

         pUnicodeChar++;
      }
   }
}


UCChar *UCString::Reserve(unsigned int numChars, bool bAllocOnly /* = false*/)
{
   FreeString(true);
   FreeMetaStrings();
   UCChar *targetstring = NULL;

   if(numChars > 0)
   {
      if(bAllocOnly)
      {
         m_extrachars = numChars;
         m_stringlength = 0;
      }
      else
      {
         m_extrachars = 0;
         m_stringlength = numChars;
      }
      m_string = new UCChar[numChars+1];
      memset(m_string,0,sizeof(UCChar) * (numChars+1));
   }

   return m_string;
}


/********* internal routines **********/

void UCString::FreeString(bool bPreserveMetaStrings /*= false*/)
{
   if(m_string != NULL)
   {
      delete[] m_string;
      m_string = NULL;
   }      
   m_stringlength = 0;
   m_extrachars = 0;

   if(!bPreserveMetaStrings)
   {
      Free8bitString();

      if(m_returnBuffer != NULL)
      {
         delete[] m_returnBuffer;
         m_returnBuffer = NULL;
         m_returnBufferLength = 0;
      }
   }
}

void UCString::FreeMetaStrings(void)
{
   Free8bitString();

   if(m_returnBuffer != NULL)
   {
      delete[] m_returnBuffer;
      m_returnBuffer = NULL;
      m_returnBufferLength = 0;
   }
}

void UCString::Free8bitString(void) const
{
   m_8bitbytesize = 0;
   if(m_string_8bit != NULL)
   {
      delete[] m_string_8bit;
      m_string_8bit = NULL;
      m_8bitbytesize = 0;
   }
}

int UCString::StringLength(const UCChar *otherstring) const
{
   int count = 0;
   if(otherstring != NULL)
   {
   const UCChar *work = otherstring;
   while( *work != 0x00 ){work++;count++;}
   }
   return count;
}

void UCString::AppendUnicodeString(const UCChar *otherstring, int count)
{
   if(count > 0)
   {
      UCChar *targetstring = NULL;

      if(m_stringlength > 0 || m_extrachars > 0)
      {
         if(count <= m_extrachars)
         {
            m_extrachars -= count;
            targetstring = m_string + m_stringlength;
            m_stringlength += count;
         }
         else
         {
            UCChar *oldstring = m_string;
            m_extrachars = count + m_stringlength;
            // start out with at least this much buffer
            if(m_extrachars < 10)m_extrachars = 10; 
            // at least double the buffer size
            int malloccount = (count + m_stringlength + m_extrachars);
            m_string = new UCChar[malloccount+1];
            if(m_string != NULL)
            {
               memcpy(m_string, oldstring, ByteSize());
               delete[] oldstring;
               targetstring = m_string + m_stringlength;
            }
            m_stringlength += count;
         }
      }
      else
      {
         FreeString(true);
         int malloccount = count;
         if(count == 1)
         { // if string is initialized with a single char, 
           // the user may be trying to create the string 
           // one char at a time.  Malloc several, to optimize;
            malloccount = 10;
            m_extrachars = 9;
         }
         m_string = new UCChar[malloccount+1];
         targetstring = m_string;
         m_stringlength = count;         
      }

      if(targetstring != NULL)
      {               
         memcpy(targetstring, otherstring, count * sizeof(UCChar));
         FreeMetaStrings();
      }

      m_string[m_stringlength] = 0x00;
   }
   MakeValid();
}

void UCString::AppendUnknownString(const void *otherstring)
{
   if(otherstring != NULL)
   {
      const byte *str = (const byte *)otherstring;
      CharEncodingType cet = CET_ANSI;
      int numchars = 0;

      if(str[0] == 0xFF &&
         str[1] == 0xFE &&
         str[2] == 0x00 &&
         str[3] == 0x00)
      {
         cet = CET_UTF32;
         str += 4;
         numchars = dwcslen((size_t*)str);
      }
      else if(str[0] == 0x00 &&
            str[1] == 0x00 &&
            str[2] == 0xFE &&
            str[3] == 0xFF)
      {
         cet = CET_UTF32be;
         str += 4;
         numchars = dwcslen((size_t*)str);
      }
      else if(str[0] == 0xFF &&
            str[1] == 0xFE)
      {
         cet = CET_UTF16;
         str += 2;
         numchars = wcslen((wchar_t*)str);
      }
      else if(str[0] == 0xFE &&
            str[1] == 0xFF)
      {
         cet = CET_UTF16be;
         str += 2;
         numchars = wcslen((wchar_t*)str);
      }
      else if(str[0] == 0xEF &&
            str[1] == 0xBB &&
            str[2] == 0xBF)
      {      
         cet = CET_UTF8;
         str += 3;
         numchars = strlen((char *)str);
      }
      else // must assume ascii
      {
         cet = CET_ANSI;
         numchars = strlen((char *)str);
      }

      AppendEncodedString(str, numchars, cet);
   }
}

void UCString::AppendEncodedString(const void *otherstring, 
                                   int count, 
                                   CharEncodingType cet/* = CET_ANSI*/)
{
   if(count > 0)
   {
      UCChar *targetstring = NULL;

      size_t sizeneeded = os_other_to_utf16( NULL, otherstring, count, cet, 0);

      if(m_stringlength > 0 || m_extrachars > 0)
      {
         if((int)sizeneeded < m_extrachars)  // remember terminating NULL character
         {
            m_extrachars -= (int)sizeneeded;
            targetstring = m_string + m_stringlength;
            m_stringlength += (int)sizeneeded;
         }
         else
         {
            UCChar *oldstring = m_string;
            m_extrachars = (int)sizeneeded + m_stringlength;
            // start out with at least this much buffer
            if(m_extrachars < 10)m_extrachars = 10; 
            // at least double the buffer size
            int malloccount = ((int)sizeneeded + m_stringlength + m_extrachars);
            m_string = new UCChar[malloccount+1];
            if(m_string != NULL)
            {
               memcpy(m_string, oldstring, ByteSize());
               delete[]oldstring;
               targetstring = m_string + m_stringlength;
            }
            m_stringlength += (int)sizeneeded;
         }
      }
      else
      {
         FreeString(true);

         int malloccount = (int)sizeneeded;
         if(sizeneeded == 1)
         { // if string is initialized with a single char, 
            // the user may be trying to create the string 
            // one char at a time.  Malloc several, to optimize;
            malloccount = 10;
            m_extrachars = 9;
         }
         m_string = new UCChar[malloccount+1];
         targetstring = m_string;
         m_stringlength = (int)sizeneeded;
         FreeMetaStrings();
      }

      if(targetstring != NULL)
      {
         memset(targetstring, 0, sizeof(UCChar) * sizeneeded + 1);
         os_other_to_utf16( targetstring, otherstring, count, cet, (size_t)sizeneeded + 1);
         Free8bitString();
      }

      m_string[m_stringlength] = 0x00;
   }
   MakeValid();
}

void UCString::AppendValue(float val)
{
   AppendValue((double)val);
}

void UCString::AppendValue(double val)
{
   UCChar buffer[50];
   buffer[0] = L'\0';

   swprintf_s(buffer,50,L"%.6g",val);

   int size = wcslen(buffer);

   if(size > 0)
   {
      // make sure there is a trailing ".0 or ,0 depending on local"
      bool commaAsDecimal = os_CommaAsDecimalPoint();
      if (!commaAsDecimal && wcschr(buffer,L'.') == NULL && size < 47)
      {
         buffer[size] = L'.';
         buffer[size+1] = L'0';
         buffer[size+2] = L'\0';
         size+=2;
      }
      else if (commaAsDecimal && wcschr(buffer,L',') == NULL && size < 47)
      {
         buffer[size] = L',';
         buffer[size+1] = L'0';
         buffer[size+2] = L'\0';
         size+=2;
      }
      AppendUnicodeString(buffer, size);
   }
}

void UCString::AppendValue(unsigned int val)
{
   UCChar buffer[50];
   buffer[0] = 0x00;

   _ultow_s( (unsigned long)val, buffer, 50, 10);

   int size = StringLength(buffer);
   if(size > 0)
   {
      AppendUnicodeString(buffer, size);
   }
}

void UCString::AppendValue(int val)
{
   UCChar buffer[50];
   buffer[0] = 0x00;

   _ltow_s( (long)val, buffer, 50, 10);

   int size = StringLength(buffer);
   if(size > 0)
   {
      AppendUnicodeString(buffer, size);
   }
}

void UCString::AppendValue(__int64 val)
{
    UCChar buffer[50];
    buffer[0] = 0x00;

    _i64tow_s(val, buffer, 50, 10);

    int size = StringLength(buffer);
    if(size > 0)
    {
        AppendUnicodeString(buffer, size);
    }
}

void UCString::AppendValue(bool val)
{
   UCChar buffer[50];
   
   if(val)
   {
      wcscpy_s(buffer, 50, L"true");
   }
   else
   {
      wcscpy_s(buffer, 50, L"false");
   }

   int size = StringLength(buffer);
   if(size > 0)
   {
      AppendUnicodeString(buffer, size);
   }
}

double UCString::GetDouble() const
{
   double result = 0.0;
   if(m_string != NULL)
   {
      UCString tmpStr = m_string; 
      bool commaAsDecimal = os_CommaAsDecimalPoint();
      if(commaAsDecimal && Find(L".")) // for languages that have a comma as the decimal point replace and convert
      {
         tmpStr.Replace(L".",L",");
      }
      else if(!commaAsDecimal && Find(L","))
      {
         tmpStr.Replace(L",",L".");
      }
      result = wcstod( tmpStr.Str(), NULL );
   }
   return result;
}

unsigned int UCString::GetUInt() const
{
   if(m_string != NULL)
   {
      return wcstoul( m_string, NULL, 10 );
   }
   return 0;
}

int UCString::GetInt() const
{
   if(m_string != NULL && m_stringlength > 0)
   {
      if(m_stringlength > 1 &&
         m_string[0] == L'0' &&
        (m_string[1] == L'x' ||
         m_string[1] == L'X'))
      {
         // is hexidecimal
         wchar_t *endptr;
         int result = wcstol(m_string,&endptr,16);
         return result;
      }
      else
      {
         // is decimal
         return _wtoi( m_string );
      }
   }
   return 0;
}

__int64 UCString::GetInt64() const
{
    if(m_string != NULL)
    {
        return _wtoi64( m_string);
    }
    return (__int64)0;
}

bool UCString::GetBool() const
{
   bool result = false;
   if(m_string != NULL)
   {
      /* 4 is the number of chars in "true" */
      if(wcsncmp(m_string, L"true", 4) == 0 ||   
         wcsncmp(m_string, L"TRUE", 4) == 0 ||
         wcsncmp(m_string, L"True", 4) == 0)
      {
         result = true;
      }
   }
   return result;
}

COLORREF UCString::GetColor() const
{
   byte red = 0;
   byte green;
   byte blue;
   UCString tmp;
   if(GetToken(tmp, 0, ','))
   {
      red = (byte)((unsigned int)tmp);
   }
   if(GetToken(tmp, 1, ','))
   {
      green = (byte)((unsigned int)tmp);
   }
   if(GetToken(tmp, 2, ','))
   {
      blue = (byte)((unsigned int)tmp);
   }

   return RGB(red,green,blue);
}

void UCString::MakeValid(void) const
{
   if(m_string == NULL)
   {
      UCString* pThis = const_cast<UCString*>( this );
      pThis->m_string = new UCChar[1];
      memset( pThis->m_string, 0, sizeof( UCChar ) );
      pThis->m_stringlength = 0;
      pThis->m_extrachars = 0;
   }
}

void UCString::SetReturnBuffer( const UCString& value ) const
{
   size_t charcount = value.Length();

   if(m_returnBuffer != NULL)
   {
      if(charcount + 1 > static_cast<size_t>(m_returnBufferLength))
      {
         delete[] m_returnBuffer;
         m_returnBuffer = NULL;
         m_returnBufferLength = 0;
      }
   }

   if(m_returnBuffer == NULL)
   {
      m_returnBuffer = new UCChar[ charcount +1 ];
      memset( m_returnBuffer, 0, (sizeof( UCChar ))*(charcount + 1) );
      m_returnBufferLength = charcount + 1;
   }

   //assert( m_returnBuffer != NULL );
   wcsncpy_s( m_returnBuffer, m_returnBufferLength, value.Str(), _TRUNCATE );
}

void UCString::MakeUpper()
{
   if(m_string != NULL)
   {
      FreeMetaStrings();
      _wcsupr_s(m_string,m_stringlength + 1);
   }
}

void UCString::MakeLower()
{
   if(m_string != NULL)
   {
      FreeMetaStrings();
      _wcslwr_s(m_string,m_stringlength + 1);
   }
}

void UCString::ReplaceAll(UCChar find, UCChar replace)
{
   if(m_string != NULL)
   {
      FreeMetaStrings();
      UCChar *work = m_string;
      while(*work != L'\0')
      {
         if(*work == find) *work = replace;
         work++;
      }
   }
}

void UCString::NtoRN(void)
{
   if(m_stringlength > 0 &&
      Find(L"\n"))
   {
      UCString output;

      const UCChar *pRead = m_string;
      const UCChar *pReadEnd = m_string + m_stringlength - 1;

      while(pRead <= pReadEnd && pRead != L'\0')
      {
         bool prevIsR = (pRead > m_string) && (*(pRead-1) == L'\r');
         bool isN = *pRead == L'\n';

         if(isN && !prevIsR)
         {
            output += L"\r\n";
         }
         else
         {
            output += *pRead;
         }
         
         pRead++;
      }

#ifdef _DEBUG
      for(int i = 0; i < output.Length(); i++)
      {
         // Assert that either this char is not '\n'
         //  or it is '\n' and is preceded by '\r'.
         assert((output[i] != '\n') || ((i > 0) && (output[i-1] == '\r')));
      }      
#endif

      *this = output;
   }
}

void UCString::RNtoN(void)
{
   if(m_stringlength > 0 &&
      Find(L"\r\n"))
   {
      UCChar *pWrite = m_string;
      const UCChar *pRead = m_string;
      const UCChar *pReadEnd = m_string + m_stringlength - 1;

      while(pRead <= pReadEnd)
      {
         bool isR = (*pRead == L'\r');
         bool nextIsN = (pRead < pReadEnd) && (*(pRead+1) == L'\n');

         if(isR && nextIsN)
         {
            *pWrite = L'\n';
            pRead++;
            m_stringlength--;
            m_extrachars++;
         }
         else
         {
            *pWrite = *pRead;
         }
         pWrite++;
         pRead++;
      }
      *pWrite = L'\0';
   }

   //assert(wcslen(m_string) == (size_t)m_stringlength);

   if(Find(L"\r\n"))
   {
      RNtoN();
   }
}

int UCString::Replace(UCChar *find, UCChar *replace)
{
   int numfound = 0;
   int len1;
   len1 = wcslen(find);

   // make sure that the replace string does not contain the 
   // find string, otherwise we will recurse forever.
   if(wcsstr(replace,find) == NULL)
   {
      UCString tmpStr;
      int pos;
      while((pos = FindCharIndex(find)) >= 0)
      {
         tmpStr = SubStr(0,pos);
         tmpStr += replace;
         tmpStr += SubStr(pos+len1, Length() - (pos+len1));
         *this = tmpStr;
         numfound++;
      }
   }
   else
   {
      // cannot recurse, 
      // so just replace in a single pass
      size_t findsize = wcslen(find);
      size_t replacesize = wcslen(replace);

      UCString tmpStr;
      UCChar *read = m_string;
      while(*read != L'\0')
      {
         if(wcsncmp(read,find,findsize) == 0)
         {
            tmpStr += replace;
            read += findsize;
            numfound++;
         }
         else
         {
            tmpStr += *read;
            read++;
         }
      }
      *this = tmpStr;
   }

   return numfound;
}

int UCString::FindCharIndex(const UCChar *what)
{
   int result = -1;   
   
   const UCChar *searchtarget = m_string;

   const UCChar *pos;
   
   if(searchtarget != NULL)
   {
      pos = wcsstr(searchtarget, what);

      if(pos != NULL)
      {
         result = pos - searchtarget;
      }         
   }
   return result;

}
int UCString::FindFirstOfCharIndex(const UCChar *what)
{
   int result = -1;   
   
   const UCChar *searchtarget = m_string;

   const UCChar *pos;
   const UCChar *finalPos = NULL;

   int whatLen = wcslen(what);
   
   if(searchtarget != NULL)
   {
      for(int i=0;i<whatLen;i++)
      {
         pos = wcschr(searchtarget, what[i]);
         if(pos != NULL)
         {
            if(finalPos == NULL ||
               pos < finalPos)
            {
               finalPos = pos;
            }
         }
      }
      if(finalPos != NULL)
      {
         result = finalPos - searchtarget;
      }         
   }
   return result;
}

void UCString::RemoveCollapse(UCChar find)
{
   if(m_stringlength > 0 &&
      m_string != NULL)
   {
      UCChar *pread = m_string;
      UCChar *pwrite = m_string;

      int numremoved = 0;

      while(*pread != L'\0')
      {
         if(*pread != find)
         {
            *pwrite = *pread;
            pwrite++;
         }
         else
         {
            numremoved++;
         }
         pread++;
      }

      *pwrite = L'\0';
      
      m_stringlength -= numremoved;
      m_extrachars += numremoved;

      FreeMetaStrings();
   }
}

void UCString::PullFromClipboard(const UCChar *format/* = NULL*/)
{
   FreeString();
   int numchars = ::os_GetTextClipboardCharCount(format);
   if(numchars > 0)
   {
      Reserve((unsigned int)numchars);
      if(m_stringlength == numchars &&
         m_string != NULL)
      {
         ::os_PullFromTextClipboard(m_string, m_stringlength, format);
         RNtoN();
      }
   }   
}

void UCString::PushToClipboard(const UCChar *format/* = NULL*/)
{
   if(m_stringlength > 0 &&
      m_string != NULL)
   {
      NtoRN();
      ::os_PushToTextClipboard(m_string, format);
   }
}

UCString UCString::SubStr(int start, int length) const
{   
   UCString result = "";

   if(start + length > Length())
      return result;

   const UCChar *searchtarget = m_string;
      
   if(searchtarget != NULL)
   {
      searchtarget += start;
      result =  searchtarget;
      result.TrimEnd(result.Length() - length);
   }
   return result;

}

UCString UCString::right(int length)
{  
   return SubStr(Length() - length, length);
}
UCString UCString::left(int length)
{
   return SubStr(0,length);
}


int UCString::NumLinesOfText() const
{
   int result = 0;

   if(m_string != NULL && Length() > 0)
   {
      result = 1;
      const UCChar *work = m_string;
      while(*work != L'\0')
      {
         if(*work == L'\n') result++;
         work++;
      }
      // if the last characters in the string are 
      // new line characters, dont count them.
      work--;
      while(*work == L'\n'){result--; work--;}
   }

   return result;
}


UCString UCString::GetLineOfText(int lineindex) const
{
   UCString result;

   if(m_string != NULL && Length() > 0)
   {      
      int count = 0;
      const UCChar *work = m_string;
      while(*work != L'\0')
      {
         if(count == lineindex) break;
         if(*work == L'\n'){ count++; }
         work++;
      }

      if(work != NULL)
      {
         while(*work != L'\0')
         {            
            if(*work == L'\n') break;
            result += *work;
            work++;
         }
      }
   }

   return result;
}

void InvertBytes(byte *data, size_t stride, size_t numstrides)
{
   if(data != NULL)
   {
      byte *readdata = (byte*)malloc(stride);

      for(size_t i = 0; i < numstrides; i++)
      {
         memcpy(readdata, data, stride);
         for(size_t j = 0; j < stride; j++)
         {
            data[j] = readdata[stride - j - 1];
         }
         data += stride;
      }

      free(readdata);
   }
}



size_t dwcslen(const size_t *pString)
{
   size_t result = 0;

   if(pString != NULL)
   {
      while(*pString != 0)
      {
         result++;
         pString++;
      }
   }

   return result;
}



bool UCString::IsDigit(UCChar c) 
{
   bool result = false;

   if(c == '0')
   {
      result = true;
   }
   else if(c == '1')
   {
      result = true;
   }
   else if(c == '2')
   {
      result = true;
   }
   else if(c == '3')
   {
      result = true;
   }
   else if(c == '4')
   {
      result = true;
   }
   else if(c == '5')
   {
      result = true;
   }
   else if(c == '6')
   {
      result = true;
   }
   else if(c == '7')
   {
      result = true;
   }
   else if(c == '8')
   {
      result = true;
   }
   else if(c == '9')
   {
      result = true;
   }

   return(result);
}

bool UCString::IsListSeperator(UCChar c)
{
   bool result = false;

   if(c == ',')
   {
      result = true;
   }
   else if(c == '-')
   {
      result = true;
   }

   return(result);
}

int UCString::HexToInt(const UCChar* p, int length)
{
   int result = 0;
   //assert(length >=2);
   if(length >= 2)
   {
      for(int i=0;i<length;i+=2)
      {
         int a = p[i];
         int b = p[i+1];
         if(a > 0x60)  // lower case convert to upper
            a -= 0x57;
         else if(a > 0x40)            
            a -= 0x37;
         else
            a -= 0x30;
          if(b > 0x60)  // lower cbse convert to upper
            b -= 0x57;
         else if(b > 0x40)            
            b -= 0x37;
         else
            b -= 0x30;
         result += (a * 16 + b) << (i * 8);
      }
   }
   return result;
}


void UCString::PathToMe( const UCChar *filename)
{
   if(g_PathToMe.IsEmpty())
   {
      UCChar path_buffer[1024];
      path_buffer[0] = L'\0';
      
      int strlength = ::GetModuleFileNameW(theApp.m_hInstance, path_buffer, 1024);
      
      if(strlength > 0)
      {
         UCChar *pChar = path_buffer + strlength - 1;

         while(pChar > path_buffer)
         {
            if(*pChar == L'\\' ||
               *pChar == L'/')
            {
               pChar++;
               break;
            }
            else
            {         
               pChar--;
            }
         }

         if(filename != NULL &&
            1024 > pChar - path_buffer + (int)wcslen(filename))
         {
            wcsncpy_s(pChar, 1024 - (pChar - path_buffer), filename,_TRUNCATE);
         }
         else
         {
            path_buffer[0] = L'\0';
         }
      }

      FreeString(true);
      AppendUnicodeString(path_buffer, StringLength(path_buffer));
      FreeMetaStrings();
   }
   else
   {
      FreeString(true);
      AppendUnicodeString(g_PathToMe.Str(), g_PathToMe.Length());

      if(g_PathToMe[g_PathToMe.Length()-1] != L'\\')
      {
         AppendUnicodeString(L"\\", 1);
      }

      AppendUnicodeString(filename, wcslen(filename));

      FreeMetaStrings();
   }
}

void UCString::splitpath( UCString *pdrive, UCString *pdir, UCString *pfname, UCString *pext) const
{
   if(m_string != NULL && m_stringlength > 0)
   {
      UCChar *start = m_string;
      UCChar *end = start + m_stringlength;

      UCChar *drive = NULL;
      UCChar *dir = NULL;
      UCChar *fname = NULL;
      UCChar *ext = NULL;

      UCChar *work = start;

      // find the drive letter and directory
      if(iswalpha(*work) &&
         m_stringlength > 2 &&
         *(work + 1) == L':')
      {
         drive = work;
         dir = work + 2;
      }
      else
      {
         drive = NULL;
         dir = work;
      }

      // find the filename

      work = end - 1;

      while(*work != L'\\' &&
            *work != L'/' &&
            work >= start)
      {
         work--;
      }

      if(*work != L'\\' || *work != L'/') work++;

      fname = work;

      // find the extension

      work = end - 1;

      while(*work != L'.' &&
            work >= fname)
      {
         work--;
      }

      if(work > fname)
      {
         ext = work;
      }

      // make sure that dir is valid, before you assume it exists
      if(dir == fname)
      {
         dir = NULL;
      }

      // assign the answers
      int answerlen;

      if(pdrive != NULL && drive != NULL)
      {         
         if(dir != NULL && dir > drive)
         {
            answerlen = dir - drive;
         }
         else
         {
            answerlen = end - drive;
         }
         pdrive->AppendUnicodeString(drive, answerlen);
      }

      if(pdir != NULL && dir != NULL)
      {         
         if(fname != NULL && fname > dir)
         {
            answerlen = fname - dir;
         }
         else
         {
            answerlen = end - dir;
         }
         pdir->AppendUnicodeString(dir, answerlen);
      }

      if(pfname != NULL && fname != NULL)
      {         
         if(ext != NULL && ext > fname)
         {
            answerlen = ext - fname;
         }
         else
         {
            answerlen = end - fname;
         }
         pfname->AppendUnicodeString(fname, answerlen);
      }

      if(pext != NULL && ext != NULL)
      {         
         answerlen = end - ext;
         pext->AppendUnicodeString(ext, answerlen);
      }
   }
}

void UCString::makepath(const UCChar *pdrive, const UCChar *pdir, const UCChar *pfname, const UCChar *pext)
{
   FreeString();
   FreeMetaStrings();

   bool bUsesForwardSlash = UsesForwardSlash(pdir);

   bool bIsDriveLetter = false;

   // deal with the drive letter

   if(pdrive != NULL &&
      *pdrive != NULL)
   {
      bIsDriveLetter = true;
      AppendUnicodeString(pdrive,1);
      AppendUnicodeString(L":",1);
   }

   // deal with the directory

   if(pdir != NULL &&
      *pdir != NULL)
   {
      if(bIsDriveLetter)
      {
         // make sure it begins with a slash
         if(bUsesForwardSlash)
         {
            AppendUnicodeString(L"/",1);
         }
         else
         {
            AppendUnicodeString(L"\\",1);
         }

         // skip leading slash
         while(*pdir == L'\\' || *pdir == L'/') pdir++;

         // append the body
         AppendUnicodeString(pdir,wcslen(pdir));
      }
      else
      {
         // use it as is
         AppendUnicodeString(pdir,wcslen(pdir));
      }

      // make sure all the slashes go the same way
      if(bUsesForwardSlash)
      {
         ReplaceAll(L'\\', L'/');
      }
      else
      {
         ReplaceAll(L'/', L'\\');
      }

      // make sure it ends with a slash
      if(m_string != NULL && m_stringlength > 0)
      {
         if(bUsesForwardSlash && m_string[m_stringlength-1] != L'/')
         {
            AppendUnicodeString(L"/",1);
         }
         else if(!bUsesForwardSlash && m_string[m_stringlength-1] != L'\\')
         {
            AppendUnicodeString(L"\\",1);
         }
      }
   }

   // deal with the filename

   if(pfname != NULL &&
      *pfname != NULL)
   {
      // skip leading slashes
      while(*pfname == L'\\' || *pfname == L'/') pfname++;

      // calc the length of the filename, minus any trailing dots
      const UCChar *work = pfname + wcslen(pfname) - 1;
      int length = work - pfname + 1;

      // append the body
      AppendUnicodeString(pfname,length);
   }

   // deal with the extension

   if(pext != NULL &&
      *pext != NULL)
   {
      // add the dot
      AppendUnicodeString(L".",1);

      // skip leading dots
      while(*pext == L'.') pext++;

      // append the body
      AppendUnicodeString(pext,wcslen(pext));
   }

   MakeValid();
}

const UCChar *UCString::GetPath(void) const
{
   if(m_stringlength > 0)
   {
      UCString path_buffer;
      UCString drive;
      UCString dir;

      splitpath(&drive, &dir, NULL, NULL);      
      path_buffer.makepath(drive, dir, NULL, NULL);

      SetReturnBuffer( path_buffer );
      return m_returnBuffer;
   }
   return NULL;
}

const UCChar *UCString::GetFilename(void) const
{
   UCString fname;

   splitpath(NULL, NULL, &fname, NULL);      
   SetReturnBuffer( fname );

   return m_returnBuffer;
}

const UCChar *UCString::GetExtension(void) const
{
   UCString ext;

   splitpath(NULL, NULL, NULL, &ext);      

   SetReturnBuffer( ext );

   return m_returnBuffer;
}

const UCChar *UCString::GetExtensionWithoutDot() const
{
   const UCChar *result = GetExtension();
   if(result != NULL &&
      result[0] == L'.')
   {
      result++;
   }
   return result;
}

const UCChar *UCString::GetFilenameAndExtension(void) const
{
   UCString path_buffer;
   UCString fname;
   UCString ext;

   splitpath(NULL, NULL, &fname, &ext);      
   path_buffer.makepath(NULL, NULL, fname, ext);

   SetReturnBuffer( path_buffer );

   return m_returnBuffer;
}
