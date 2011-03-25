// ucstring.h
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "os.h"

void InvertBytes(byte *data, size_t stride, size_t numstrides);
size_t dwcslen(const size_t *pString);

class UCString
{
public:

   /*****************  constructors **********************/
   UCString();
   UCString(const UCString &otherstring);
   UCString(const UCChar *otherstring);
   UCString(const char *otherstring);
   UCString(const UCChar *otherstring, int length);
   UCString(const char *otherstring, int length);
   UCString(const float val, bool internalUse = false);    // internalUse, force use of period regardless of local
   UCString(const double val, bool internalUse = false);   // internalUse, force use of period regardless of local
   UCString(const unsigned int val);
   UCString(const int val);
   UCString(const __int64 val);
   UCString(const bool val);

   /*****************  the destructor **********************/

   virtual ~UCString()
   {
      FreeString();
      FreeMetaStrings();
   }

   /*****************  comparison operators **********************/

   /* UCChar * */   
   bool operator == (const UCChar *otherstring) const;
   bool operator != (const UCChar *otherstring) const;
   bool operator <  (const UCChar *otherstring) const;
   bool operator >  (const UCChar *otherstring) const;

   /* UCString & */   
   bool operator == (const UCString &otherstring) const;
   bool operator != (const UCString &otherstring) const;
   bool operator <  (const UCString &otherstring) const;
   bool operator >  (const UCString &otherstring) const;

   /*****************  assignment operators **********************/

   UCString &operator = (const UCChar character);
   UCString &operator = (const UCChar *otherstring);                  
   UCString &operator = (const UCString &otherstring);
   UCString &operator = (const char *other8bitstring);
   UCString &operator = (const float val);
   UCString &operator = (const double val);
   UCString &operator = (const unsigned int val);
   UCString &operator = (const int val);
   UCString &operator = (const __int64 val);
   UCString &operator = (const bool val);

   /*****************  append operators **********************/

   UCString &operator += (const UCChar character);
   UCString &operator += (const char character);
   UCString &operator += (const UCChar *otherstring);
   UCString &operator += (const UCString &otherstring);
   UCString &operator += (const char *other8bitstring);
   UCString &operator += (const float val);
   UCString &operator += (const double val);
   UCString &operator += (const unsigned int val);
   UCString &operator += (const int val);
   UCString &operator += (const __int64 val);
   UCString &operator += (const bool val);

   /*****************  cast operators **********************/

   UCChar operator []( int nIndex ) const;
   inline operator const UCChar*() const { if(m_string == NULL)MakeValid(); return m_string; }

   operator float () const;
   operator double () const;
   operator unsigned int() const;
   operator int() const;
   operator __int64() const;
   operator bool() const;
   operator COLORREF() const;

   /********* the only way to get an 8bit string out *********/

   const char *str8(bool bStrIsPath = false, CharEncodingType cet = CET_ANSI, bool bAddBOM = false) const;

   /******************** return a string *********************/

   inline const UCChar *Str() const { MakeValid(); return m_string; }
   const UCChar *BEEncodedString();
   const UCChar *LEEncodedString();

   /****************  string compare operations **************/

   int          cmp     (const UCChar *str) const;              
   int          icmp    (const UCChar *str) const;              
   int          ncmp    (const UCChar *str, int iCount) const;  
   int          nicmp   (const UCChar *str, int iCount) const;  

   /* not really comparison but if string is a sub string of */
   const UCChar *beginswith (const UCChar *str) const;
   const UCChar *endswith (const UCChar *str) const;
   const UCChar *subcmp (const UCChar *str) const;
   const UCChar *subicmp(const UCChar *str) const;

   /****************  string operations **********************/

   inline int Length(void) const {return m_stringlength;}

   int ByteSize(void) const {return m_stringlength * sizeof(UCChar);}

   bool IsEmpty(void) const;

   bool IsWhitespace(void);

   void TrimWhitespace(bool bFront = true, bool bBack = true);

   void InsertChar(UCChar thechar, int position);

   void DeleteChar(int position);

   void TrimEnd(int numchars);

   const UCChar *Format(const UCChar *, ...);
   const char *Format(const char *, ...);

   void FormatFloatWithPeriod(double val);    // internal use to always format a float with a period as a decimal point

   void MakeNormal(void);

   void MakeUpper();
   void MakeLower();

   UCChar *Reserve(unsigned int numChars, bool bAllocOnly = false);

   int NumLinesOfText() const;
   UCString GetLineOfText(int lineindex) const;

   void ReplaceAll(UCChar find, UCChar replace);

   int Replace(UCChar *find, UCChar *replace);

   void NtoRN(void);
   void RNtoN(void);

   void RemoveCollapse(UCChar find);

   void PullFromClipboard(const UCChar *format = NULL);
   void PushToClipboard(const UCChar *format = NULL);

   void AppendUnicodeString(const UCChar *otherstring, int count);
   void AppendEncodedString(const void *otherstring, int count, CharEncodingType cet = CET_ANSI);
   void AppendUnknownString(const void *otherstring);

   /*****************  token operations **********************/

   bool GetToken(UCString &result, int tokenindex, UCChar token = L' ', bool bTrimwhitespace = true) const;

   bool GetToken(UCString &result, int tokenindex, UCString &tokens, bool bTrimwhitespace, UCChar &TerminatingToken) const;
  
   const UCChar *Find(const UCChar *what, const UCChar *starting = NULL, bool bPositionAfter = false) const;
  
   int FuzzyFind(const UCChar* what, const UCChar *ignore = NULL) const;

   void PathToMe( const UCChar *filename);
   void splitpath( UCString *pdrive, UCString *pdir, UCString *pfname, UCString *pext) const;
   void makepath( const UCChar *pdrive, const UCChar *pdir, const UCChar *pfname, const UCChar *pext);
   const UCChar *GetPath(void) const;
   const UCChar *GetFilename(void) const;
   const UCChar *GetExtension(void) const;
   const UCChar *GetExtensionWithoutDot() const;
   const UCChar *GetFilenameAndExtension(void) const;
   const UCChar *MakeRelativePath(const UCChar *pBasePath);

   int FindCharIndex(const UCChar *what);
   int FindFirstOfCharIndex(const UCChar *what);
   UCString SubStr(int start, int length) const;
   UCString right(int length);
   UCString left(int length);

   static bool IsDigit(UCChar c);
   static bool IsListSeperator(UCChar c);

   /******** encoding binary data as text support ************/
   void EncodeBinaryToASCIIText(void *pData, unsigned int numbytes);
   unsigned int DecodeASCIITextToBinary(void *pData, unsigned int numbytes);

   
   void EncodeBinaryToBase64Text(void *pData, unsigned int numbytes);
   unsigned int DecodeBase64TextToBinary(void *pData, unsigned int numbytes);
   static unsigned char DecodeBase64WideCharToBinaryByte(UCChar ch);
   static UCChar EncodeBinaryByteToBase64WideChar(unsigned char binval);

   static int HexToInt(const UCChar* p, int length);

protected:

   /********* internal routines **********/
   void Init(void);
   void FreeString(bool bPreserveMetaStrings = false);
   void FreeMetaStrings(void);
   void SetReturnBuffer( const UCString& value ) const;
   void MakeValid(void) const;
   void Free8bitString(void) const;
   int StringLength(const UCChar *otherstring) const;
   void AppendValue(float val);
   void AppendValue(double val);
   void AppendValue(unsigned int val);
   void AppendValue(int val);
   void AppendValue(__int64 val);
   void AppendValue(bool val);
   double GetDouble() const;
   unsigned int GetUInt() const;
   int GetInt() const;
   __int64 GetInt64() const;
   bool GetBool() const;
   COLORREF GetColor() const;
   UCChar EncodeBinaryByteToWideChar(unsigned char binaryval);
   unsigned char DecodeWideCharToBinaryByte(const UCChar encodedval);


   UCChar      *m_string;
   mutable char *m_string_8bit;
   int          m_stringlength;
   int          m_extrachars;
   mutable int  m_8bitbytesize;
   mutable bool m_8bitIsPath;
   mutable CharEncodingType m_8bitCET;
   mutable bool m_8bitAddBOM;

   mutable UCChar *m_returnBuffer;
   mutable int     m_returnBufferLength;
};
