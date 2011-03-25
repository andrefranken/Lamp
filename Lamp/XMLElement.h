// XMLElement.h: interface for the CXMLElement class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "UCString.h"
#include <vector>

class CXMLElement;

typedef enum 
{
   XDT_NORMAL        = 0,
   XDT_CDATA         = 1,
   XDT_COMMENT       = 2
} XMLDataType;

class CXMLAttribute
{
public:
   CXMLAttribute()
   {
      m_pParent = NULL;
      m_datatype = XDT_NORMAL;
   }
   virtual ~CXMLAttribute(){}
   // copy constructors
   CXMLAttribute(const CXMLAttribute &other);
   CXMLAttribute(const CXMLAttribute *other);

   // assignment operators
   void operator = (const CXMLAttribute *other);                  
   void operator = (const CXMLAttribute &other);

   void SetParent(CXMLElement *pParent){m_pParent = pParent;}
   CXMLElement *GetParent(void){return m_pParent;}

   void SetTag(const UCChar *tag);
   const UCString &GetTag(void) const;
   bool CompareTag(const UCChar *compare);

   void SetValue(const UCChar *value);
   UCString &GetValue(bool bRawValue = false);
   /*
   bool operator < (const CXMLAttribute& t) const{return false;}
   bool operator == (const CXMLAttribute& t) const{return false;}
   */
   bool operator == (const CXMLAttribute &other);
protected:
   UCString             m_tag;
   UCString             m_value;
   UCString             m_rawvalue;
   CXMLElement         *m_pParent;
   XMLDataType          m_datatype;
};

template class std::allocator<CXMLElement*>;
template class std::vector<CXMLElement*>;
template class std::allocator<CXMLAttribute*>;
template class std::vector<CXMLAttribute*>;

class CXMLElement : public CXMLAttribute
{
public:
   CXMLElement() : CXMLAttribute()
   {
      m_UserDefinedFlagPack = 0;
   }
   virtual ~CXMLElement();

   // copy constructors
   CXMLElement(const CXMLElement &other);
   CXMLElement(const CXMLElement *other);

   // assignment operators
   void operator = (const CXMLElement *other);                  
   void operator = (const CXMLElement &other);

   void SetAsCDATA(void){m_datatype = XDT_CDATA;}
   void SetAsComment(void){m_datatype = XDT_COMMENT;}
   bool IsCDATA(void){return m_datatype == XDT_CDATA;}
   bool IsComment(void){return m_datatype == XDT_COMMENT;}

   int CountAttributes(void) const;
   int CountChildren(void) const;

   CXMLElement   *FindChildElement(const UCChar *tag) const;
   CXMLElement   *FindCDATAChild(void);
   CXMLElement   *FindChildElementByAttribute(const UCChar *tag,
                                              const UCChar *attribute,
                                              const UCChar *att_value, 
                                              bool bRawValue = false);

   CXMLAttribute *GetAttribute(int index);
   CXMLElement   *GetChildElement(int index) const;
   int GetChildElementIndex(CXMLElement *pElement);
   UCString      &GetAttributeValue(const UCChar *tag, bool bRawValue = false);
   bool           GetAttributeValue(const UCChar *tag, UCString& attributeValue);
   UCString      &GetElementValue(const UCChar *tag, bool bRawValue = false);
   
   CXMLElement *FindOrAddChildElement(const UCChar *tag);
   CXMLElement *FindOrAddChildElementByAttribute(const UCChar *tag,
                                                 const UCChar *attribute,
                                                 const UCChar *att_value, 
                                                 bool bRawValue = false);
   CXMLElement *AddChildElement(void);
   CXMLElement *AddChildElement(const UCChar *tag, const UCChar *value);
   CXMLElement *AddChildElement(const UCChar *tag, const UCChar *attName, const UCChar *attValue);
   CXMLElement *AddChildComment(const UCChar *comment);
   CXMLAttribute *AddAttribute(void);
   void AddAttribute(const UCChar *tag, const UCChar *value);
   void SetAttributeValue(const UCChar *tag, const UCChar *value);

   void Prune(void);
   void PruneAttributes(void);
   void PruneChildElement(int index);
   void PruneChildElement(const UCChar *tag);
   void PruneChildElement(CXMLElement *pChild);

   void Duplicate(CXMLElement *pTarget);

   bool operator == (const CXMLElement &other);

   /*
   bool operator < (const CXMLElement& t) const
   {
      return false;
   }
   bool operator == (const CXMLElement& t) const
   {
      return false;
   }
   */
   void         AddUserDefinedFlag    (unsigned int UserDefinedFlag){m_UserDefinedFlagPack |= UserDefinedFlag;}
   void         RemoveUserDefinedFlag (unsigned int UserDefinedFlag){m_UserDefinedFlagPack ^= UserDefinedFlag;}
   unsigned int GetUserDefinedFlag    (unsigned int UserDefinedFlag){return m_UserDefinedFlagPack & UserDefinedFlag;}

   void         SetUserDefinedFlagPack(unsigned int UserDefinedFlagPack){m_UserDefinedFlagPack = UserDefinedFlagPack;}
   unsigned int GetUserDefinedFlagPack(void){return m_UserDefinedFlagPack;}

   void WriteToString(UCString &buffer, int indentDepth = 0);

protected:
   void WriteIndention(UCString &buffer, int indentDepth);

   const UCChar *ParseFromString(const UCChar *buffer, const UCChar *pNullChar);
   const UCChar *EatWhitespace(const UCChar *buffer, UCString *pUCString = NULL);
   const UCChar *ParseStringToken(UCString &token, 
                                  const UCChar *buffer, 
                                  const UCChar *pNullChar, 
                                  bool bOneWord = false,
                                  bool bTagValue = false);
   void XMLEncode(UCString &output,UCString &input);
   bool GetCharFromDigitUpToSemicolon(UCChar **read, const UCChar *pNullChar, UCChar &fromint);

   std::vector<CXMLElement*>    m_children;
   std::vector<CXMLAttribute*>  m_attributes;

   unsigned int                m_UserDefinedFlagPack;
};   

class CXMLTree : public CXMLElement
{
public:
   CXMLTree() : CXMLElement()
   {
      bHasStylesheet = false;
   }

   // copy constructors
   CXMLTree(const CXMLTree &other);
   CXMLTree(const CXMLTree *other);

   // assignment operators
   void operator = (const CXMLTree *other);                  
   void operator = (const CXMLTree &other);

   bool Read(const UCChar *Filename);
   bool ReadFromBuffer(UCString &buffer);
   void Write(const UCChar *Filename, bool bUTF16 = false);
   void WriteToBuffer(UCString &buffer);
   void WriteToBufferUTF8Style(UCString &buffer);

   void UseStyleSheet(const UCChar *type, const UCChar *href)
   {
      bHasStylesheet = true;
      m_stylesheet_type = type;
      m_stylesheet_href = href;
   }
   
protected:
   bool ReadFileToBuffer(const UCChar *Filename, UCString &databuffer);

private:
   bool Validate8bit(const char *buffer, UCString &encoding);
   bool Validate16bit(const UCChar *buffer, UCString &encoding, bool be);
   bool Validate32bit(const size_t *buffer, UCString &encoding, bool be);

   void WriteStylesheet(UCString &buffer);

   bool bHasStylesheet;
   UCString m_stylesheet_type;
   UCString m_stylesheet_href;
};
