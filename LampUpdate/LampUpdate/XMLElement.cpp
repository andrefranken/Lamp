// XMLElement.cpp: implementation of the CXMLElement class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "UCString.h"
#include "XMLElement.h"

UCString blankstring;

CXMLAttribute::CXMLAttribute(const CXMLAttribute &other)
{
   (*this) = &other;
}

CXMLAttribute::CXMLAttribute(const CXMLAttribute *other)
{
   (*this) = other;
}

void CXMLAttribute::operator = (const CXMLAttribute *other)
{
   m_tag = other->m_tag;
   m_value = other->m_value;
   m_rawvalue = other->m_rawvalue;
   m_datatype = other->m_datatype;
}

void CXMLAttribute::operator = (const CXMLAttribute &other)
{
   (*this) = &other;
}

void CXMLAttribute::SetTag(const UCChar *tag)
{
   m_tag = tag;
   m_tag.TrimWhitespace();
}

const UCString &CXMLAttribute::GetTag(void) const
{
   return m_tag;
}

bool CXMLAttribute::CompareTag(const UCChar *compare)
{
   bool result = false;
   if(GetTag() == compare)
   {
      result = true;
   }
   return result;
}

void CXMLAttribute::SetValue(const UCChar *value)
{
   m_value = value;
   m_value.TrimWhitespace();
   if(m_value.Length() != wcslen(value))
   {
      m_rawvalue = value;
   }
}

UCString &CXMLAttribute::GetValue(bool bRawValue /*= false*/)
{
   if(bRawValue && !m_rawvalue.IsEmpty())
   {
      return m_rawvalue;
   }
   return m_value;
}

bool CXMLAttribute::operator == (const CXMLAttribute &other)
{
   if(m_tag == other.m_tag &&
      m_value == other.m_value &&
      m_rawvalue == other.m_rawvalue)
   {
      return true;
   }
   return false;
}

CXMLElement::~CXMLElement()
{
   Prune();
}

CXMLElement::CXMLElement(const CXMLElement &other)
{
   (*this) = &other;
}

CXMLElement::CXMLElement(const CXMLElement *other)
{
   (*this) = other;
}

void CXMLElement::operator = (const CXMLElement *other)
{
   Prune();

   (*((CXMLAttribute*)this)) = (CXMLAttribute*)other;

   m_UserDefinedFlagPack = other->m_UserDefinedFlagPack;

   for(size_t i = 0; i < other->m_children.size(); i++)
   {
      CXMLElement *pElement = new CXMLElement(other->m_children[i]);
      //assert(pElement != NULL);
      if(pElement != NULL)
      {
         pElement->SetParent(this);
         m_children.push_back(pElement);
      }
   }

   for(size_t i = 0; i < other->m_attributes.size(); i++)
   {
      CXMLAttribute *pAttribute = new CXMLAttribute(other->m_attributes[i]);
      //assert(pAttribute != NULL);
      if(pAttribute != NULL)
      {
         pAttribute->SetParent(this);
         m_attributes.push_back(pAttribute);
      }
   }
}

void CXMLElement::operator = (const CXMLElement &other)
{
   (*this) = &other;
}


bool CXMLElement::operator == (const CXMLElement &other)
{
   bool result = true;
   if(CXMLAttribute::operator == (other) &&
      m_attributes.size() == other.m_attributes.size() &&
      m_children.size() == other.m_children.size())
   {
      for(size_t i = 0; result && i < m_attributes.size(); i++)
      {
         // the order of attributes is NOT important
         bool bFound = false;
         for(size_t j = 0; !bFound && j < other.m_attributes.size(); j++)
         {
            if(*(m_attributes[i]) == *(other.m_attributes[j]))
            {
               bFound = true;
            }
         }
         result = bFound;
      }

      if(result)
      {
         for(size_t i = 0; result && i < m_children.size(); i++)
         {
            // the order of children is important
            result = (*(m_children[i]) == *(other.m_children[i]));
         }
      }
   }
   else
   {
      result = false;
   }

   return result;
}

int CXMLElement::CountAttributes(void) const
{
   return (int)m_attributes.size();
}

int CXMLElement::CountChildren(void) const
{
   return (int)m_children.size();
}

CXMLElement *CXMLElement::FindChildElement(const UCChar *tag) const
{
   CXMLElement *result = NULL;

   std::vector<CXMLElement*>::const_iterator it = m_children.begin();
   std::vector<CXMLElement*>::const_iterator end = m_children.end();

   while(it != end)
   {
      if( (*it)->CompareTag(tag) )
      {
         result = (*it);
         break;
      }
      it++;
   }

   return result;
}

CXMLElement *CXMLElement::FindCDATAChild(void)
{
   CXMLElement *result = NULL;

   std::vector<CXMLElement*>::iterator it = m_children.begin();
   std::vector<CXMLElement*>::iterator end = m_children.end();

   while(it != end)
   {
      if( (*it)->IsCDATA() )
      {
         result = (*it);
         break;
      }
      it++;
   }

   return result;
}

CXMLElement *CXMLElement::FindChildElementByAttribute(const UCChar *tag,
                                                      const UCChar *attribute,
                                                      const UCChar *att_value, 
                                                      bool bRawValue /*= false*/)
{
   CXMLElement *result = NULL;

   std::vector<CXMLElement*>::iterator it = m_children.begin();
   std::vector<CXMLElement*>::iterator end = m_children.end();

   while(it != end)
   {
      if( (*it)->CompareTag(tag) )
      {
         UCString value = (*it)->GetAttributeValue(attribute, bRawValue);
         if(value == att_value)
         {
            result = (*it);
            break;
         }
      }
      it++;
   }

   return result;
}

CXMLAttribute *CXMLElement::GetAttribute(int index)
{
   CXMLAttribute *result = NULL;

   if(m_attributes.size() > (size_t)index)
   {
      result = m_attributes[index];
   }

   return result;
}

CXMLElement *CXMLElement::GetChildElement(int index) const
{
   CXMLElement *result = NULL;

   if(m_children.size() > (size_t)index)
   {
      result = m_children[index];
   }

   return result;
}

int CXMLElement::GetChildElementIndex(CXMLElement *pElement)
{
   int result = -1;

   for(int i = 0; i < (int)m_children.size(); i++)
   {
      if(m_children[i] == pElement)
      {
         result = i;
         break;
      }
   }

   return result;
}

void CXMLElement::PruneChildElement(int index)
{
   CXMLElement *pChild = GetChildElement(index);
   if(pChild != NULL)
   {
      PruneChildElement(pChild);
   }
}

void CXMLElement::PruneChildElement(const UCChar *tag)
{
   CXMLElement *pChild = FindChildElement(tag);
   if(pChild != NULL)
   {
      PruneChildElement(pChild);
   }
}

void CXMLElement::PruneChildElement(CXMLElement *pChild)
{
   std::vector<CXMLElement*> newchildren;

   std::vector<CXMLElement*>::iterator it = m_children.begin();
   std::vector<CXMLElement*>::iterator end = m_children.end();

   while(it != end)
   {
      if( (*it) == pChild )
      {
         delete(*it);
         (*it) = NULL;
      }
      else
      {
         newchildren.push_back((*it));
      }

      it++;
   }

   m_children = newchildren;
}

void CXMLElement::Prune(void)
{
   std::vector<CXMLElement*>::iterator it = m_children.begin();
   std::vector<CXMLElement*>::iterator end = m_children.end();

   while(it != end)
   {
      if( (*it) != NULL )
      {
         delete(*it);
      }
      it++;
   }
   m_children.clear();

   PruneAttributes();
}

void CXMLElement::PruneAttributes(void)
{
   std::vector<CXMLAttribute*>::iterator ait = m_attributes.begin();
   std::vector<CXMLAttribute*>::iterator aend = m_attributes.end();

   while(ait != aend)
   {
      if( (*ait) != NULL )
      {
         delete(*ait);
      }
      ait++;
   }
   m_attributes.clear();
}

void CXMLElement::Duplicate(CXMLElement *pTarget)
{
   // copy my contents
   pTarget->SetTag(GetTag());
   pTarget->SetValue(GetValue(true));

   // copy my attributes
   std::vector<CXMLAttribute*>::iterator ait = m_attributes.begin();
   std::vector<CXMLAttribute*>::iterator aend = m_attributes.end();

   while(ait != aend)
   {
      CXMLAttribute *pAttr = (*ait);
      if(pAttr != NULL )
      {
         pTarget->AddAttribute(pAttr->GetTag(), pAttr->GetValue(true));
      }
      ait++;
   }

   // copy my children
   std::vector<CXMLElement*>::iterator it = m_children.begin();
   std::vector<CXMLElement*>::iterator end = m_children.end();

   while(it != end)
   {
      CXMLElement *pChild = (*it);
      if(pChild != NULL )
      {
         CXMLElement *pNewChild = pTarget->AddChildElement();
         if(pNewChild != NULL)
         {
            pChild->Duplicate(pNewChild);
         }
      }
      it++;
   }
}

UCString &CXMLElement::GetAttributeValue(const UCChar *tag, bool bRawValue /*= false*/)
{
   UCString *result = &blankstring;

   std::vector<CXMLAttribute*>::iterator it = m_attributes.begin();
   std::vector<CXMLAttribute*>::iterator end = m_attributes.end();

   while(it != end)
   {
      if( (*it)->CompareTag(tag) )
      {
         result = &(*it)->GetValue(bRawValue);
         break;
      }
      it++;
   }

   return *result;
}

bool CXMLElement::GetAttributeValue(const UCChar *tag, UCString& attributeValue)
{
   bool found = false;
   
   std::vector<CXMLAttribute*>::iterator it = m_attributes.begin();
   std::vector<CXMLAttribute*>::iterator end = m_attributes.end();

   while(it != end && !found)
   {
      if( (*it)->CompareTag(tag) )
      {
         attributeValue = (*it)->GetValue();
         found = true;
      }
      it++;
   }

   return found;
}
UCString &CXMLElement::GetElementValue(const UCChar *tag, bool bRawValue /*= false*/)
{
   UCString *result = &blankstring;

   std::vector<CXMLElement*>::iterator it = m_children.begin();
   std::vector<CXMLElement*>::iterator end = m_children.end();

   while(it != end)
   {
      if( (*it)->CompareTag(tag) )
      {
         result = &(*it)->GetValue(bRawValue);
         break;
      }
      it++;
   }

   return *result;
}

CXMLElement *CXMLElement::FindOrAddChildElement(const UCChar *tag)
{
   CXMLElement *pElement = FindChildElement(tag);
   if(pElement == NULL)
   {
      pElement = AddChildElement();
      if(pElement != NULL)
      {
         pElement->SetTag(tag);
      }
   }
   return pElement;
}

CXMLElement *CXMLElement::FindOrAddChildElementByAttribute(const UCChar *tag,
                                                           const UCChar *attribute,
                                                           const UCChar *att_value,
                                                           bool bRawValue /*= false*/)
{
   CXMLElement *pElement = FindChildElementByAttribute(tag, attribute, att_value, bRawValue);
   if(pElement == NULL)
   {
      pElement = AddChildElement();
      if(pElement != NULL)
      {
         pElement->SetTag(tag);
         pElement->AddAttribute(attribute,att_value);
      }
   }
   return pElement;
}

CXMLElement *CXMLElement::AddChildElement(void)
{
   CXMLElement *pElement = new CXMLElement();
   //assert(pElement != NULL);
   if(pElement != NULL)
   {
      pElement->SetParent(this);
      m_children.push_back(pElement);
   }
   return pElement;
}

CXMLElement *CXMLElement::AddChildElement(const UCChar *tag, const UCChar *value)
{
   CXMLElement *pElement = AddChildElement();   
   if(pElement != NULL)
   {
      pElement->SetTag(tag);
      if(value != NULL)
      {
         pElement->SetValue(value);
      }
   }
   return pElement;
}

CXMLElement *CXMLElement::AddChildComment(const UCChar *comment)
{
   CXMLElement *pElement = AddChildElement();   
   if(pElement != NULL)
   {
      pElement->SetAsComment();
      pElement->SetTag(L"Comment");
      if(comment != NULL)
      {
         pElement->SetValue(comment);
      }
   }
   return pElement;
}

CXMLElement *CXMLElement::AddChildElement(const UCChar *tag, const UCChar *attName, const UCChar *attValue)
{
   CXMLElement *pElement = AddChildElement();   
   if(pElement != NULL)
   {
      pElement->SetTag(tag);
      if(attName != NULL && attValue != NULL)
      {
         pElement->AddAttribute(attName,attValue);
      }
   }
   return pElement;
}

CXMLAttribute *CXMLElement::AddAttribute(void)
{
   CXMLAttribute *pAttribute = new CXMLAttribute();   
   //assert(pAttribute != NULL);
   if(pAttribute != NULL)
   {
      pAttribute->SetParent(this);
      m_attributes.push_back(pAttribute);
   }
   return pAttribute;
}

void CXMLElement::AddAttribute(const UCChar *tag, const UCChar *value)
{
   CXMLAttribute *pAttribute = AddAttribute();   
   if(pAttribute != NULL)
   {
      pAttribute->SetTag(tag);
      pAttribute->SetValue(value);
   }
}

void CXMLElement::SetAttributeValue(const UCChar *tag, const UCChar *value)
{
   bool bFoundExistingAttribute = false;

   std::vector<CXMLAttribute*>::iterator it = m_attributes.begin();
   std::vector<CXMLAttribute*>::iterator end = m_attributes.end();

   while(it != end)
   {
      if( (*it)->CompareTag(tag) )
      {
         (*it)->SetValue(value);
         bFoundExistingAttribute = true;
         break;
      }
      it++;
   }

   if(!bFoundExistingAttribute)
   {
      // if it didn't exist, add it.
      AddAttribute(tag, value);
   }
}


void CXMLElement::XMLEncode(UCString &output,UCString &input)
{
   output = L"";
   const UCChar *work = input;

   if(work != NULL)
   {
      /*
      &lt; 	   < 	less than
      &gt; 	   > 	greater than
      &amp;    & 	ampersand 
      &apos;   ' 	apostrophe
      &quot;   " 	quotation mark
      */
      while(*work != L'\0')
      {
         if(*work == L'<')
         {
            output += L"&lt;";
         }
         else if(*work == L'>')
         {
            output += L"&gt;";
         }
         else if(*work == L'&')
         {
            output += L"&amp;";
         }
         else if(*work == L'\'')
         {
            output += L"&apos;";
         }
         else if(*work == L'\"')
         {
            output += L"&quot;";
         }
         else
         {
            output += *work;
         }
         work++;
      }      
   }
}

void CXMLElement::WriteToString(UCString &buffer, int indentDepth/*= 0*/)
{
   if(IsCDATA())
   {
      buffer += L"<![CDATA[";
      buffer += m_value; // don't need to XMLEncode() the string
      buffer += L"]]>";
   }
   else if(IsComment())
   {
      buffer += L"\n";
      WriteIndention(buffer,indentDepth);
      buffer += L"<!--";
      buffer += m_value; // don't need to XMLEncode() the string
      buffer += L"-->";
   }
   else // normal
   {
      buffer += L"<";
      buffer += GetTag();

      UCString xmloutput;

      std::vector<CXMLAttribute*>::iterator ait = m_attributes.begin();
      std::vector<CXMLAttribute*>::iterator aend = m_attributes.end();

      while(ait != aend)
      {
         buffer += L' ';
         buffer += (*ait)->GetTag();
         buffer += L"=\"";

         xmloutput = L"";
         XMLEncode(xmloutput,(*ait)->GetValue(true));

         buffer += xmloutput;
         buffer += L'\"';
         ait++;
      }

      if(GetValue(true).IsEmpty() &&
         m_children.size() == 0)
      {
         buffer += L"/>";
      }
      else
      {
         buffer += L'>';

         bool bHadValue = false;

         if(GetValue(true).Length() > 0)
         {
            xmloutput = L"";
            XMLEncode(xmloutput,GetValue(true));
            buffer += xmloutput;
            bHadValue = true;
         }

         std::vector<CXMLElement*>::iterator it = m_children.begin();
         std::vector<CXMLElement*>::iterator end = m_children.end();

         indentDepth++;
         bool bDoneFirst = false;
         while(it != end)
         {
            if(!bDoneFirst)
            {
               // first child
               bDoneFirst = true;
               if(bHadValue)
               {
                  // do nothing, leave current position here and start drawing the first child
               }
               else
               {
                  // return and indent
                  buffer += L"\n";
                  WriteIndention(buffer,indentDepth);
               }
            }
            else
            {
               buffer += L"\n";
               WriteIndention(buffer,indentDepth);
            }
            (*it)->WriteToString(buffer, indentDepth );
            it++;
         }
         indentDepth--;

         if(m_children.size() > 0)
         {
            buffer += L"\n";
            WriteIndention(buffer,indentDepth);
         }
         
         buffer += L"</";
         buffer += GetTag();
         buffer += L">";
      }
   }
}

void CXMLElement::WriteIndention(UCString &buffer, int indentDepth)
{
   for(int i = 0; i < indentDepth; i++)
   {
      buffer += L"   "; // three space tabs
   }
}

const UCChar *CXMLElement::ParseFromString(const UCChar *buffer, const UCChar *pNullChar)
{
   buffer = EatWhitespace(buffer);

   if(*buffer != L'<') 
   {
      return NULL;
   }

   if(*buffer == L'<')
   {
      if(wcsncmp(buffer,L"<!--",4) == 0)     // if comment read and skip
      {
         SetAsComment();
         SetTag(L"Comment");
         const UCChar *pValue = buffer+4;         
         buffer = wcsstr(buffer,L"-->");
         *(UCChar*)buffer = L'\0';
         m_value = pValue;
         *(UCChar*)buffer = L'-';
         buffer += 3;
         buffer = EatWhitespace(buffer);
        
         return buffer;
      }
      else if(wcsncmp(buffer,L"<![CDATA[",9) == 0)     // if CDATA read and skip
      {
         SetAsCDATA();
         SetTag(L"CDATA");
         const UCChar *pValue = buffer+9;         
         buffer = wcsstr(buffer,L"]]>");
         *(UCChar*)buffer = L'\0';
         m_value = pValue;
         *(UCChar*)buffer = L'-';
         buffer += 3;
         buffer = EatWhitespace(buffer);
        
         return buffer;
      }
      buffer = EatWhitespace(buffer+1);
      UCString tag;
      buffer = ParseStringToken(tag, buffer, pNullChar, true);
      SetTag(tag);
      buffer = EatWhitespace(buffer);
      while(*buffer != L'>' &&
            *buffer != L'/' &&
            *buffer != L'\0')
      {
         // attribute
         UCString attributetag;
         buffer = ParseStringToken(attributetag, buffer, pNullChar, true);
         buffer = EatWhitespace(buffer);
         if(*buffer != L'=') 
         {
            return NULL;
         }
         buffer++;
         buffer = EatWhitespace(buffer);
         UCString attributevalue;
         buffer = ParseStringToken(attributevalue, buffer, pNullChar);
         buffer = EatWhitespace(buffer);

         AddAttribute(attributetag, attributevalue);
      }

      if(*buffer == L'/')
      {
         // content-less entity.  Has no end tag
         buffer = EatWhitespace(buffer+1);
         if(*buffer != L'>')
         {
            return NULL;
         }
         buffer = EatWhitespace(buffer+1);
         return buffer;
      }

      if(*buffer != L'>') 
      {
         return NULL;
      }

      const UCChar *backup = buffer+1;
      UCString white;
      buffer = EatWhitespace(buffer+1,&white);
      // find the value
      if(*buffer != L'<')
      {
         buffer = backup;
         UCString value;
         buffer = ParseStringToken(value, buffer, pNullChar, false, true);
         SetValue(value);
         buffer = EatWhitespace(buffer);
      }
      else if(white.Length() > 0)
      {
         SetValue(white);
      }

      // find all children
      bool bHasChildren = false;
      while(*buffer == L'<' &&
            *(buffer + 1) != L'/' &&
            *buffer != L'\0')
      {
         bHasChildren = true;
         CXMLElement *pChild = AddChildElement();
         buffer = pChild->ParseFromString(buffer, pNullChar);         
         if(buffer == NULL)
         {
            return NULL;
         }
      }

      if(bHasChildren && 
         m_rawvalue.IsWhitespace())
      {
         // if we have children and our value is just whitespace, clear the value
         m_rawvalue = L"";
         m_value = L"";
      }

      bool bFoundEndTag = false;
      // find my end tag
      while(!bFoundEndTag && *buffer != L'\0')
      {
         buffer = EatWhitespace(buffer);
         while(*buffer != L'<' && *buffer != L'\0')buffer++;
         if(*buffer != L'\0')buffer++;
         buffer = EatWhitespace(buffer);
         while(*buffer != L'/' && *buffer != L'\0')buffer++;
         if(*buffer != L'\0')buffer++;
         buffer = EatWhitespace(buffer);
         UCString temp;
         buffer = ParseStringToken(temp, buffer, pNullChar, true);
         buffer = EatWhitespace(buffer);
         while(*buffer != L'>' && *buffer != L'\0')buffer++;
         if(*buffer != L'\0')buffer++;
         buffer = EatWhitespace(buffer);

         if(temp == GetTag())
         {
            bFoundEndTag = true;
         }
      }
   }

   return buffer;
}

const UCChar *CXMLElement::EatWhitespace(const UCChar *buffer, UCString *pUCString /*= NULL*/)
{
   const UCChar *result = buffer;

   while(*result != L'\0' &&
          iswspace(*result) != 0)
   {
      if(pUCString != NULL)
      {
         pUCString->AppendUnicodeString(result,1);
      }
      result++;
   }

   return result;
}

bool CXMLElement::GetCharFromDigitUpToSemicolon(UCChar **read, const UCChar *pNullChar, UCChar &fromint)
{
   bool result = false;

   UCString digitbuffer;
   
   const UCChar *pread = *read;

   bool bContinue = true;
   bool bIsHex = false;
   if(*pread == L'&')
   {
      pread++;
   }
   if(*pread == L'#')
   {
      pread++;
   }
   if(*pread == L'x' ||
      *pread == L'X')
   {
      digitbuffer += L"0x";
      bIsHex = true;
      pread++;
   }

   while(bContinue)
   {
      if(pread == pNullChar)
      {
         // error
         bContinue = false;
      }
      else if(*pread == L';')
      {
         if(!digitbuffer.IsEmpty())
         {
            // done
            result = true;
            int intval;
            intval = digitbuffer;
            fromint = (UCChar)intval;
            *read = (UCChar *)pread;
         }
         else
         {
            // error
         }         
         bContinue = false;
      }
      else if((*pread >= L'0' &&
               *pread <= L'9') ||
              (bIsHex &&
             ((*pread >= L'a' &&
               *pread <= L'f') ||
              (*pread >= L'A' &&
               *pread <= L'F'))))
      {
         digitbuffer += *pread;
      }
      else
      {
         // error
         bContinue = false;
      }
      pread++;
   }

   return result;
}

const UCChar *CXMLElement::ParseStringToken(UCString &token, 
                                            const UCChar *buffer, 
                                            const UCChar *pNullChar, 
                                            bool bOneWord, /* = false*/
                                            bool bTagValue /* = false*/)
{
   static UCChar token_buffer[1024];
   UCChar *write = token_buffer;
   int token_size = 0;
   UCChar fromint;

   const UCChar *read = buffer;

   bool bQuoted = false;

   if(!bTagValue && *read == L'\"')
   {
      bQuoted = true;
      read++;
   }

   if(bQuoted)
   {
      /*
      &lt; 	   < 	less than
      &gt; 	   > 	greater than
      &amp;    & 	ampersand 
      &apos;   ' 	apostrophe
      &quot;   " 	quotation mark
      */
      while(*read != L'\0' &&
            *read != L'\"')
      {
         if(pNullChar - read > 3 && wcsncmp(read,L"&lt;",4) == 0)
         {
            if(token_size > 1023)
            {
               token.AppendUnicodeString(token_buffer, token_size);
               token_size = 0;
               write = token_buffer;
            }
            *write = L'<';
            token_size++;
            write++;
            read+=3;
         }
         else if(pNullChar - read > 3 && wcsncmp(read,L"&gt;",4) == 0)
         {
            if(token_size > 1023)
            {
               token.AppendUnicodeString(token_buffer, token_size);
               token_size = 0;
               write = token_buffer;
            }
            *write = L'>';
            token_size++;
            read+=3;
            write++;
         }
         else if(pNullChar - read > 4 && wcsncmp(read,L"&amp;",5) == 0)
         {
            if(token_size > 1023)
            {
               token.AppendUnicodeString(token_buffer, token_size);
               token_size = 0;
               write = token_buffer;
            }
            *write = L'&';
            token_size++;
            read+=4;
            write++;
         }
         else if(pNullChar - read > 5 && wcsncmp(read,L"&apos;",6) == 0)
         {
            if(token_size > 1023)
            {
               token.AppendUnicodeString(token_buffer, token_size);
               token_size = 0;
               write = token_buffer;
            }
            *write = L'\'';
            token_size++;
            read+=5;
            write++;
         }
         else if(pNullChar - read > 5 && wcsncmp(read,L"&quot;",6) == 0)
         {
            if(token_size > 1023)
            {
               token.AppendUnicodeString(token_buffer, token_size);
               token_size = 0;
               write = token_buffer;
            }
            *write = L'\"';
            token_size++;
            read+=5;
            write++;
         }
         else if(pNullChar - read > 4 && wcsncmp(read,L"&#",2) == 0 && GetCharFromDigitUpToSemicolon((UCChar**)&read,pNullChar,fromint))
         {
            if(token_size > 1023)
            {
               token.AppendUnicodeString(token_buffer, token_size);
               token_size = 0;
               write = token_buffer;
            }
            *write = fromint;
            token_size++;
            write++;
         }
         else
         {
            if(token_size > 1023)
            {
               token.AppendUnicodeString(token_buffer, token_size);
               token_size = 0;
               write = token_buffer;
            }
            *write = *read;
            token_size++;
            write++;
         }
         read++;
      }

      if(*read == L'\"')
      {
         read++;
      }
   }
   else if(bOneWord)
   {
      while(*read != L'\0' &&
            (iswalnum(*read) != 0 || *read == L'_' || *read == L'-' || *read == L'.' || *read == L':'))
      {
         if(token_size > 1023)
         {
            token.AppendUnicodeString(token_buffer, token_size);
            token_size = 0;
            write = token_buffer;
         }
         *write = *read;
         token_size++;
         read++;
         write++;
      }
   }
   else
   {
      /*
      &lt; 	   < 	less than
      &gt; 	   > 	greater than
      &amp;    & 	ampersand 
      &apos;   ' 	apostrophe
      &quot;   " 	quotation mark
      */
      while(*read != L'\0' &&
            *read != L'<')
      {
         if(pNullChar - read > 3 && wcsncmp(read,L"&lt;",4) == 0)
         {
            if(token_size > 1023)
            {
               token.AppendUnicodeString(token_buffer, token_size);
               token_size = 0;
               write = token_buffer;
            }
            *write = L'<';
            token_size++;
            write++;
            read+=3;
         }
         else if(pNullChar - read > 3 && wcsncmp(read,L"&gt;",4) == 0)
         {
            if(token_size > 1023)
            {
               token.AppendUnicodeString(token_buffer, token_size);
               token_size = 0;
               write = token_buffer;
            }
            *write = L'>';
            token_size++;
            read+=3;
            write++;
         }
         else if(pNullChar - read > 4 && wcsncmp(read,L"&amp;",5) == 0)
         {
            if(token_size > 1023)
            {
               token.AppendUnicodeString(token_buffer, token_size);
               token_size = 0;
               write = token_buffer;
            }
            *write = L'&';
            token_size++;
            read+=4;
            write++;
         }
         else if(pNullChar - read > 5 && wcsncmp(read,L"&apos;",6) == 0)
         {
            if(token_size > 1023)
            {
               token.AppendUnicodeString(token_buffer, token_size);
               token_size = 0;
               write = token_buffer;
            }
            *write = L'\'';
            token_size++;
            read+=5;
            write++;
         }
         else if(pNullChar - read > 5 && wcsncmp(read,L"&quot;",6) == 0)
         {
            if(token_size > 1023)
            {
               token.AppendUnicodeString(token_buffer, token_size);
               token_size = 0;
               write = token_buffer;
            }
            *write = L'\"';
            token_size++;
            read+=5;
            write++;
         }
         else if(pNullChar - read > 4 && wcsncmp(read,L"&#",2) == 0 && GetCharFromDigitUpToSemicolon((UCChar**)&read,pNullChar,fromint))
         {
            if(token_size > 1023)
            {
               token.AppendUnicodeString(token_buffer, token_size);
               token_size = 0;
               write = token_buffer;
            }
            *write = fromint;
            token_size++;
            write++;
         }
         else
         {
            if(token_size > 1023)
            {
               token.AppendUnicodeString(token_buffer, token_size);
               token_size = 0;
               write = token_buffer;
            }
            *write = *read;
            token_size++;
            write++;
         }
         read++;
      }
   }

   if(token_size > 0)
   {
      token.AppendUnicodeString(token_buffer, token_size);
   }

   return read;
}

CXMLTree::CXMLTree(const CXMLTree &other)
{
   (*this) = &other;
}

CXMLTree::CXMLTree(const CXMLTree *other)
{
   (*this) = other;
}

void CXMLTree::operator = (const CXMLTree *other)
{
   (*((CXMLElement*)this)) = (CXMLElement*)other;

   bHasStylesheet = other->bHasStylesheet;
   m_stylesheet_type = other->m_stylesheet_type;
   m_stylesheet_href = other->m_stylesheet_href;
}

void CXMLTree::operator = (const CXMLTree &other)
{
   (*this) = &other;
}


bool CXMLTree::ReadFileToBuffer(const UCChar *Filename, UCString &databuffer)
{
   bool result = false;

   FILE *stream = NULL;
   _wfopen_s(&stream,Filename, L"rb");
   if(stream != NULL)
   {
      fseek(stream, 0, SEEK_END);
      int filesize = ftell(stream);
      fseek(stream, 0, SEEK_SET);

      if(filesize > 0)
      {
         char *buffer = (char *)malloc(filesize + 4);
         
         if(buffer != NULL)
         {
            memset(buffer, 0, filesize + 4);

            if(fread( buffer, 1, filesize, stream ) == filesize)
            {
               if((((byte*)buffer)[0] == 0xFF &&
                   ((byte*)buffer)[1] == 0xFE &&
                   ((byte*)buffer)[2] == 0x00 &&
                   ((byte*)buffer)[3] == 0x00) ||
                  (((byte*)buffer)[0] == 0x00 &&
                   ((byte*)buffer)[1] == 0x00 &&
                   ((byte*)buffer)[2] == 0xFE &&
                   ((byte*)buffer)[3] == 0xFF))
               {
                  // 32 bit unicode 
                  size_t *dwbuffer = (size_t *)(buffer + 4);

                  bool be = false;

                  if(((byte*)buffer)[0] == 0x00 &&
                     ((byte*)buffer)[1] == 0x00 &&
                     ((byte*)buffer)[2] == 0xFE &&
                     ((byte*)buffer)[3] == 0xFF)
                  {
                     // big-endian
                     be = true;
                  }

                  UCString encoding;

                  if(Validate32bit(dwbuffer, encoding, be))
                  {
                     CharEncodingType cet = CET_UNKNOWN;

                     if(encoding == L"UTF-32" || 
                        encoding == L"ISO-10646-UCS-4")
                     {
                        if(be)
                        {
                           cet = CET_UTF32be;
                        }
                        else
                        {
                           cet = CET_UTF32;
                        }
                     }

                     if(cet != CET_UNKNOWN)
                     {
                        databuffer.AppendEncodedString(dwbuffer, dwcslen(dwbuffer), cet);
                        databuffer.RemoveCollapse(L'\r');
                        result = true;
                     }
                  }
               }
               else if((((byte*)buffer)[0] == 0xFF &&
                        ((byte*)buffer)[1] == 0xFE) ||
                       (((byte*)buffer)[0] == 0xFE &&
                        ((byte*)buffer)[1] == 0xFF))
               {
                  // unicode 
                  UCChar *wbuffer = (UCChar *)(buffer + 2);

                  bool be = false;

                  if(((byte*)buffer)[0] == 0xFE &&
                     ((byte*)buffer)[1] == 0xFF)
                  {
                     // big-endian
                     be = true;
                  }

                  UCString encoding;

                  if(Validate16bit(wbuffer, encoding, be))
                  {
                     CharEncodingType cet = CET_UNKNOWN;

                     if(encoding == L"UTF-16" ||
                        encoding == L"ISO-10646-UCS-2" ||
                        encoding.IsEmpty())
                     {
                        if(be)
                        {
                           cet = CET_UTF16be;
                        }
                        else
                        {
                           cet = CET_UTF16;
                        }
                     }

                     if(cet != CET_UNKNOWN)
                     {
                        databuffer.AppendEncodedString(wbuffer, wcslen(wbuffer), cet);
                        databuffer.RemoveCollapse(L'\r');
                        result = true;
                     }
                  }
               }
               else
               {
                  // 8 bit
                  char *sbuffer = buffer;
                  bool bUTF8_hint = false;

                  if((((byte*)sbuffer)[0] == 0xEF &&
                      ((byte*)sbuffer)[1] == 0xBB &&
                      ((byte*)sbuffer)[2] == 0xBF))
                  {
                     // UTF-8 file marker
                     sbuffer += 3;
                     bUTF8_hint = true;
                  }

                  UCString encoding;
                  
                  if(Validate8bit(sbuffer, encoding))
                  {
                     CharEncodingType cet = CET_UNKNOWN;

                     if(encoding == L"UTF-7")
                     {
                        // encoded with UTF 7
                        cet = CET_UTF7;
                     }
                     else if(encoding == L"UTF-8")
                     {
                        // encoded with UTF 8
                        cet = CET_UTF8;
                     }
                     else if(encoding == L"ISO-8859-1")
                     {
                        // encoded with windows multi-byte
                        cet = CET_ANSI;
                     }
                     else if(encoding.IsEmpty())
                     {
                        // not encoded
                        if(bUTF8_hint)
                        {
                           cet = CET_UTF8;
                        }
                        else
                        {
                           cet = CET_ANSI;
                        }
                     }

                     if(cet != CET_UNKNOWN)
                     {
                        databuffer.AppendEncodedString(sbuffer,strlen(sbuffer),cet);
                        databuffer.RemoveCollapse(L'\r');
                        result = true;
                     }
                  }
               }
            }
            free(buffer);
         }
      }

      fclose( stream );
   }
   return result;
}

bool CXMLTree::Read(const UCChar *Filename)
{
   Prune();
   bool result = false;

   UCString xmldatabuffer;

   if(ReadFileToBuffer(Filename,xmldatabuffer))
   {
      result = ReadFromBuffer(xmldatabuffer);
   }
   return result;
}


// this method assumes UTF16 encoding.
bool CXMLTree::ReadFromBuffer(UCString &buffer)
{
   bool result = false;

   const UCChar *from = buffer.Str();
   const UCChar *pNullChar = from + buffer.Length();

   if(*from == 0xFFFE)
   {
      from++;
   }

   from = EatWhitespace(from);

   if(wcsncmp(from,L"<?xml version",13) == 0)
   {
      // skip the xml header.
      from+=13;
      // find the next '<' char
      while(from < pNullChar)
      {
         if(*from == L'<')
         {
            break;
         }
         else
         {
            from++;
         }
      }
   }

   if(wcsncmp(from,L"<?xml-stylesheet",16) == 0)
   {
      // parse the stylesheet.
      bHasStylesheet = true;
      from+=16;

      const UCChar *start = from;
      const UCChar *end = from;
      // find the next '<' char
      while(from < pNullChar)
      {
         if(*from == L'<')
         {
            end = from;
            break;
         }
         else
         {
            from++;
         }
      }
      from = start;

      // find the type
      m_stylesheet_type = L"";
      const UCChar *find = wcsstr(from, L"type=\"");
      
      if(find != NULL &&
         find < end)
      {
         from = find + 6;
         // find the next '\"' char
         while(from < end)
         {
            if(*from == L'\"')
            {
               break;
            }
            else
            {
               m_stylesheet_type += *from;
               from++;
            }
         }
      }

      // find the href
      m_stylesheet_href = L"";
      find = wcsstr(from, L"href=\"");
      
      if(find != NULL &&
         find < end)
      {
         from = find + 6;
         // find the next '\"' char
         while(from < end)
         {
            if(*from == L'\"')
            {
               break;
            }
            else
            {
               m_stylesheet_href += *from;
               from++;
            }
         }
      }

      from = end;
   }

   while(wcsncmp(from,L"<?",2) == 0 ||
         wcsncmp(from,L"<!--",4) == 0)
   {
      // skip other xml headers and comments.
      from+=2;
      // find the next '<' char
      while(from < pNullChar)
      {
         if(*from == L'<')
         {
            break;
         }
         else
         {
            from++;
         }
      }
   }

   if(ParseFromString(from,pNullChar) != NULL)
   {
      result = true;
   }

   return result;
}


void CXMLTree::Write(const UCChar *Filename, bool bUTF16/* = false*/)
{
   FILE *stream = NULL;
   _wfopen_s(&stream, Filename, L"wb");

   if(stream != NULL)
   {
      UCString XMLSummary;

      void *data = NULL;
      size_t charsize = 0;
      size_t numchars = 0;

      if(bUTF16)
      {
         XMLSummary += (wchar_t)0xFEFF;
         WriteToBuffer(XMLSummary);
         XMLSummary.NtoRN();
         data = (void*)XMLSummary.Str();
         charsize = 2;
         numchars = wcslen((wchar_t*)data);
      }
      else
      {
         WriteToBufferUTF8Style(XMLSummary);
         XMLSummary.NtoRN();
         data = (void*)XMLSummary.str8(false,CET_UTF8,true);
         charsize = 1;
         numchars = strlen((char*)data);
      }
      
      fwrite(data, charsize, numchars, stream );
      
      fclose(stream);
   }   
}

void CXMLTree::WriteToBuffer(UCString &buffer)
{
   buffer += L"<?xml version=\"1.0\" encoding=\"UTF-16\"?>\n";

   WriteStylesheet(buffer);

   WriteToString(buffer);
}

void CXMLTree::WriteToBufferUTF8Style(UCString &buffer)
{
   buffer += L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";

   WriteStylesheet(buffer);

   WriteToString(buffer);
}

void CXMLTree::WriteStylesheet(UCString &buffer)
{
   if(bHasStylesheet)
   {
      buffer += L"<?xml-stylesheet type=\"";
      buffer += m_stylesheet_type;
      buffer += L"\" href=\"";
      buffer += m_stylesheet_href;
      buffer += L"\"?>\n";
   }
}

bool CXMLTree::Validate8bit(const char *buffer, UCString &encoding)
{
   bool result = false;

   // example:
   // <?xml version = "1.0" encoding = "UTF-8" ?>

   // skip leading "<?xml version" string
   if(strncmp(buffer, "<?xml version", 13) == 0)
   {
      buffer += 13;
      // skip whitespace
      while(*buffer != '\0' && iswspace( *buffer )) buffer++;

      // skip equal sign
      if(*buffer != '\0' && *buffer == '=')
      {
         buffer++;

         // skip whitespace
         while(*buffer != '\0' && iswspace( *buffer )) buffer++;

         // skip ""1.0"" string
         if(strncmp(buffer, "\"1.0\"", 5) == 0)
         {
            // we are now valid
            // now look for the encoding string
            result = true;

            buffer += 5;

            // skip whitespace
            while(*buffer != '\0' && iswspace( *buffer )) buffer++;

            // skip "encoding" string
            if(strncmp(buffer, "encoding", 8) == 0)
            {
               buffer += 8;

               // skip whitespace
               while(*buffer != '\0' && iswspace( *buffer )) buffer++;

               // skip equal sign
               if(*buffer != '\0' && *buffer == '=')
               {
                  buffer++;

                  // skip whitespace
                  while(*buffer != '\0' && iswspace( *buffer )) buffer++;

                  // skip doublequote
                  if(*buffer != '\0' && *buffer == '\"')
                  {
                     buffer++;
                     // copy everything up to the next doublequote
                     while(*buffer != '\0' &&
                           *buffer != '\"')
                     {
                        encoding += *buffer;
                        buffer++;
                     }
                  }
               }
            }
         }
      }
   }

   return result;
}

bool CXMLTree::Validate16bit(const UCChar *buffer, UCString &encoding, bool be)
{
   bool result = false;

   wchar_t xml_version[] = {L'<', L'?', L'x', L'm', L'l', L' ', L'v', L'e', L'r', L's', L'i', L'o', L'n', L'\0'};
   wchar_t equal = L'=';
   wchar_t space = L' ';
   wchar_t quote = L'\"';
   wchar_t one_zero[] = {L'\"', L'1', L'.', L'0', L'\"', L'\0'};
   wchar_t encoding_str[] = {L'e', L'n', L'c', L'o', L'd', L'i', L'n', L'g', L'\0'};

   if(be)
   {
      InvertBytes((byte*)xml_version, 2, 13);
      InvertBytes((byte*)&equal, 2, 1);
      InvertBytes((byte*)&space, 2, 1);
      InvertBytes((byte*)&quote, 2, 1);
      InvertBytes((byte*)one_zero, 2, 5);
      InvertBytes((byte*)encoding_str, 2, 8);
   }

   // example:
   // <?xml version = "1.0" encoding = "UTF-8" ?>

   // skip leading "<?xml version" string
   if(memcmp(buffer, xml_version, 13 * sizeof(wchar_t)) == 0)
   {
      buffer += 13;
      // skip whitespace
      while(*buffer != 0 && *buffer == space) buffer++;

      // skip equal sign
      if(*buffer != 0 && *buffer == equal)
      {
         buffer++;

         // skip whitespace
         while(*buffer != 0 && *buffer == space) buffer++;

         // skip ""1.0"" string
         if(memcmp(buffer, one_zero, 5 * sizeof(wchar_t)) == 0)
         {
            // we are now valid
            // now look for the encoding string
            result = true;

            buffer += 5;

            // skip whitespace
            while(*buffer != 0 && *buffer == space) buffer++;

            // skip "encoding" string
            if(memcmp(buffer, encoding_str, 8 * sizeof(wchar_t)) == 0)
            {
               buffer += 8;

               // skip whitespace
               while(*buffer != 0 && *buffer == space) buffer++;

               // skip equal sign
               if(*buffer !=0 && *buffer == equal)
               {
                  buffer++;

                  // skip whitespace
                  while(*buffer != 0 && *buffer == space) buffer++;

                  // skip doublequote
                  if(*buffer != 0 && *buffer == quote)
                  {
                     buffer++;
                     // copy everything up to the next doublequote
                     while(*buffer != 0 &&
                           *buffer != quote)
                     {
                        wchar_t character = *buffer;
                        if(be)
                        {
                           InvertBytes((byte*)&character, 2, 1);
                        }
                        encoding += character;
                        buffer++;
                     }
                  }
               }
            }
         }
      }
   }

   return result;
}


bool CXMLTree::Validate32bit(const size_t *buffer, UCString &encoding, bool be)
{
   bool result = false;

   size_t xml_version[] = {L'<', L'?', L'x', L'm', L'l', L' ', L'v', L'e', L'r', L's', L'i', L'o', L'n', L'\0'};
   size_t equal = L'=';
   size_t space = L' ';
   size_t quote = L'\"';
   size_t one_zero[] = {L'\"', L'1', L'.', L'0', L'\"', L'\0'};
   size_t encoding_str[] = {L'e', L'n', L'c', L'o', L'd', L'i', L'n', L'g', L'\0'};

   if(be)
   {
      InvertBytes((byte*)xml_version, 4, 13);
      InvertBytes((byte*)&equal, 4, 1);
      InvertBytes((byte*)&space, 4, 1);
      InvertBytes((byte*)&quote, 4, 1);
      InvertBytes((byte*)one_zero, 4, 5);
      InvertBytes((byte*)encoding_str, 4, 8);
   }

   // example:
   // <?xml version = "1.0" encoding = "UTF-8" ?>

   // skip leading "<?xml version" string
   if(memcmp(buffer, xml_version, 13 * sizeof(size_t)) == 0)
   {
      buffer += 13;
      // skip whitespace
      while(*buffer != 0 && *buffer == space) buffer++;

      // skip equal sign
      if(*buffer != 0 && *buffer == equal)
      {
         buffer++;

         // skip whitespace
         while(*buffer != 0 && *buffer == space) buffer++;

         // skip ""1.0"" string
         if(memcmp(buffer, one_zero, 5 * sizeof(size_t)) == 0)
         {
            // we are now valid
            // now look for the encoding string
            result = true;

            buffer += 5;

            // skip whitespace
            while(*buffer != 0 && *buffer == space) buffer++;

            // skip "encoding" string
            if(memcmp(buffer, encoding_str, 8 * sizeof(size_t)) == 0)
            {
               buffer += 8;

               // skip whitespace
               while(*buffer != 0 && *buffer == space) buffer++;

               // skip equal sign
               if(*buffer !=0 && *buffer == equal)
               {
                  buffer++;

                  // skip whitespace
                  while(*buffer != 0 && *buffer == space) buffer++;

                  // skip doublequote
                  if(*buffer != 0 && *buffer == quote)
                  {
                     buffer++;
                     // copy everything up to the next doublequote
                     while(*buffer != 0 &&
                           *buffer != quote)
                     {
                        size_t character = *buffer;
                        if(be)
                        {
                           InvertBytes((byte*)&character, 4, 1);
                        }
                        encoding += (wchar_t)character;
                        buffer++;
                     }
                  }
               }
            }
         }
      }
   }

   return result;
}


