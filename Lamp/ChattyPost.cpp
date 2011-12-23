
// LampDoc.cpp : implementation of the CLampDoc class
//

#include "stdafx.h"
#include "Lamp.h"
#include "ChattyPost.h"
#include "LampDoc.h"
#include "ReplyDlg.h"
#include <math.h>

UCString blank_string;

size_t HTML_GetIDAttribute(tree<htmlcxx::HTML::Node>::sibling_iterator &it, const char *attr_name/* = NULL*/)
{
   size_t result = 0;

   if(it->attributes().size() == 0)
   {
      it->parseAttributes();
   }

   std::string str;
   if(attr_name != NULL)
   {
      str = attr_name;
   }
   else
   {
      str = "id";
   }

   std::pair<bool,std::string> class_attr = it->attribute(str);
   if(class_attr.second.length() > 0)
   {
      const char *pStr = (const char *)class_attr.second.data();
      const char *pEnd = pStr + class_attr.second.length();

      while(pStr < pEnd && isdigit(*pStr) == 0)pStr++;

      if(pStr < pEnd)
      {
         result = strtoul(pStr,NULL, 10);
      }
   }

   return result;
}

bool HTML_HasAttribute(tree<htmlcxx::HTML::Node>::sibling_iterator &it, 
                       const char *attribute_name, 
                       const char *attribute_value)
{
   if(it->attributes().size() == 0)
   {
      it->parseAttributes();
   }
   std::pair<bool,std::string> class_attr = it->attribute(std::string(attribute_name));
   if(class_attr.second.length() > 0 &&
      class_attr.second == attribute_value)
   {
      return true;
   }
   return false;
}

bool HTML_StartsWithAttribute(tree<htmlcxx::HTML::Node>::sibling_iterator &it, 
                              const char *attribute_name, 
                              const char *attribute_value, 
                              std::string *attribute_value_remainder/* = 0*/)
{
   if(it->attributes().size() == 0)
   {
      it->parseAttributes();
   }
   std::pair<bool,std::string> class_attr = it->attribute(std::string(attribute_name));
   if(class_attr.second.length() > 0)
   {
      const char *pClassStr = (const char *)class_attr.second.data();
      size_t str_len = strlen(attribute_value);
      if(class_attr.second.length() >= str_len &&
         strncmp(pClassStr, attribute_value, str_len) == 0)
      {
         if(attribute_value_remainder != NULL)
         {
            *attribute_value_remainder = pClassStr + str_len;
         }
         return true;
      }
   }
   return false;
}

void HTML_GetValue(tree<htmlcxx::HTML::Node>::sibling_iterator &from_it, std::string &value)
{
   tree<htmlcxx::HTML::Node>::sibling_iterator it = from_it.begin();
   tree<htmlcxx::HTML::Node>::sibling_iterator end = from_it.end();

   while(it != end)
   {
      value += it->text();
      HTML_GetValue(it, value);
      value += it->closingText();
      it++;
   }
}

bool HTML_FindChild(tree<htmlcxx::HTML::Node>::sibling_iterator &from_it, 
                    tree<htmlcxx::HTML::Node>::sibling_iterator &result_it, 
                    const char *tag_name)
{
   tree<htmlcxx::HTML::Node>::sibling_iterator it = from_it.begin();
   tree<htmlcxx::HTML::Node>::sibling_iterator end = from_it.end();

   while(it != end)
   {
      if(it->tagName() == tag_name)
      {
         result_it = it;
         return true;
      }
      it++;
   }
   return false;
}

bool HTML_FindChild_HasAttribute(tree<htmlcxx::HTML::Node>::sibling_iterator &from_it, 
                                 tree<htmlcxx::HTML::Node>::sibling_iterator &result_it, 
                                 const char *tag_name, 
                                 const char *attribute_name, 
                                 const char *attribute_value)
{
   tree<htmlcxx::HTML::Node>::sibling_iterator it = from_it.begin();
   tree<htmlcxx::HTML::Node>::sibling_iterator end = from_it.end();

   while(it != end)
   {
      if(it->tagName() == tag_name &&
         HTML_HasAttribute(it, attribute_name, attribute_value))
      {
         result_it = it;
         return true;
      }
      it++;
   }
   return false;
}

bool HTML_FindChild_StartsWithAttribute(tree<htmlcxx::HTML::Node>::sibling_iterator &from_it, 
                                        tree<htmlcxx::HTML::Node>::sibling_iterator &result_it, 
                                        const char *tag_name, 
                                        const char *attribute_name, 
                                        const char *attribute_value, 
                                        std::string *attribute_value_remainder/* = 0*/)
{
   tree<htmlcxx::HTML::Node>::sibling_iterator it = from_it.begin();
   tree<htmlcxx::HTML::Node>::sibling_iterator end = from_it.end();

   while(it != end)
   {
      if(it->tagName() == tag_name &&
         HTML_StartsWithAttribute(it, attribute_name, attribute_value, attribute_value_remainder))
      {
         result_it = it;
         return true;
      }
      it++;
   }
   return false;
}

////////////////////////

bool HTML_FindChild(tree<htmlcxx::HTML::Node> &from_dom,
                    tree<htmlcxx::HTML::Node>::sibling_iterator &result_it, 
                    const char *tag_name)
{
   tree<htmlcxx::HTML::Node>::iterator root = from_dom.begin();
   tree<htmlcxx::HTML::Node>::iterator end = from_dom.end();

   while(root != end)
   {
      if(root->tagName() == "html")
      {
         tree<htmlcxx::HTML::Node>::sibling_iterator here = root;
         return HTML_FindChild(here, result_it, tag_name);
      }
      root++;
   }

   return false;
}

bool HTML_FindChild_HasAttribute(tree<htmlcxx::HTML::Node> &from_dom,
                                 tree<htmlcxx::HTML::Node>::sibling_iterator &result_it, 
                                 const char *tag_name, 
                                 const char *attribute_name, 
                                 const char *attribute_value)
{
   tree<htmlcxx::HTML::Node>::iterator root = from_dom.begin();
   tree<htmlcxx::HTML::Node>::iterator end = from_dom.end();

   while(root != end)
   {
      if(root->tagName() == "html")
      {
         tree<htmlcxx::HTML::Node>::sibling_iterator here = root;
         return HTML_FindChild_HasAttribute(here, result_it, tag_name, attribute_name, attribute_value);
      }
      root++;
   }

   return false;
}

bool HTML_FindChild_StartsWithAttribute(tree<htmlcxx::HTML::Node> &from_dom,
                                        tree<htmlcxx::HTML::Node>::sibling_iterator &result_it, 
                                        const char *tag_name, 
                                        const char *attribute_name, 
                                        const char *attribute_value, 
                                        std::string *attribute_value_remainder/* = 0*/)
{
   tree<htmlcxx::HTML::Node>::iterator root = from_dom.begin();
   tree<htmlcxx::HTML::Node>::iterator end = from_dom.end();

   while(root != end)
   {
      if(root->tagName() == "html")
      {
         tree<htmlcxx::HTML::Node>::sibling_iterator here = root;
         return HTML_FindChild_StartsWithAttribute(here, result_it, tag_name, attribute_name, attribute_value, attribute_value_remainder);
      }
      root++;
   }

   return false;
}

void ChattyPost::UpdateAuthorColor()
{
   if(m_author == theApp.GetUsername())
   {
      m_AuthorColor = theApp.GetMyPostColor();
      m_bIsMe = true;
   }
   else
   {
      m_AuthorColor = theApp.GetUserColor(m_author);
   }
}

void ChattyPost::ReadRootChattyFromHTML(tree<htmlcxx::HTML::Node>::sibling_iterator &root_it, 
                                        CLampDoc *pDoc,
                                        unsigned int id)
{
   SetDoc(pDoc);
   
   m_id = id;

   if(pDoc->GetDataType() == DDT_STORY)
   {
      m_bCollapsed = theApp.GetMyCollapse(m_id);
   }

   m_mylols = theApp.GetMyLol(m_id);

   UpdateLOLs();

   UCString body_temp;

   // read self

   tree<htmlcxx::HTML::Node>::sibling_iterator it;

   if(HTML_FindChild(root_it, it, "ul"))
   {
      if(HTML_FindChild(it, it, "li"))
      {
         tree<htmlcxx::HTML::Node>::sibling_iterator info_it;

         std::string value;
         if(HTML_FindChild_StartsWithAttribute(it, info_it, "div", "class", "fullpost", &value))
         {
            const char *pClassStr = (const char *)value.data();
            pClassStr = strstr(pClassStr,"mod_");
            if(pClassStr != NULL)
            {
               pClassStr += 4;
               // if(strncmp(pClassStr, "ontopic", 7) do nothing.  That is the default            
               if(strncmp(pClassStr, "informative",11) == 0)   {m_category = PCT_INF;}
               else if(strncmp(pClassStr, "nws",3) == 0)       {m_category = PCT_NWS;}
               else if(strncmp(pClassStr, "offtopic",8) == 0)  {m_category = PCT_OFFTOPIC;}
               else if(strncmp(pClassStr, "political",9) == 0) {m_category = PCT_POLITICAL;}
               else if(strncmp(pClassStr, "stupid",6) == 0)    {m_category = PCT_STUPID;}
               else if(strncmp(pClassStr, "nuked",5) == 0)     {m_category = PCT_NUKED;}
            }

            tree<htmlcxx::HTML::Node>::sibling_iterator author_it;
            if(HTML_FindChild_HasAttribute(info_it, author_it, "div", "class", "postmeta"))
            {
               if(HTML_FindChild_HasAttribute(author_it, author_it, "span", "class", "author"))
               {
                  tree<htmlcxx::HTML::Node>::sibling_iterator user_it = author_it;

                  if(HTML_FindChild_HasAttribute(user_it, user_it, "span", "class", "user") || 
                     HTML_FindChild_HasAttribute(user_it, user_it, "span", "class", "user this-user"))
                  {
                     if(HTML_FindChild_StartsWithAttribute(user_it, user_it, "a", "href", "/user/"))
                     {
                        std::string author;
                        HTML_GetValue(user_it, author);
                        m_author = (const char*)author.data();
                        m_author.TrimWhitespace();
                        RemoveSomeTags(m_author);
                        UpdateAuthorColor();
                     }
                  }

                  if(HTML_FindChild_HasAttribute(author_it, user_it, "a", "class", "lightningbolt"))
                  {
                     m_lightningbolt = true;
                  }
               }
            }

            tree<htmlcxx::HTML::Node>::sibling_iterator date_it;
            if(HTML_FindChild_HasAttribute(info_it, date_it, "div", "class", "postdate"))
            {
               std::string date;
               HTML_GetValue(date_it, date);
               m_datetext = (const char*)date.data();
               m_datetext.TrimWhitespace();
               m_datetext.MakeNormal();
               UpdateDate();
            }

            tree<htmlcxx::HTML::Node>::sibling_iterator body_it;
            if(HTML_FindChild_HasAttribute(info_it, body_it, "div", "class", "postbody"))
            {
               std::string body;
               HTML_GetValue(body_it, body);
               if(body.length() > 0)
               {
                  body_temp.AppendEncodedString((const char *)body.data(),body.length(),CET_UTF8);
               }
            }
         }

         tree<htmlcxx::HTML::Node>::sibling_iterator replies_it;

         if(HTML_FindChild_HasAttribute(it, replies_it, "div", "class", "capcontainer"))
         {
            if(HTML_FindChild(replies_it, replies_it, "ul"))
            {
               tree<htmlcxx::HTML::Node>::sibling_iterator child_it = replies_it.begin();
               tree<htmlcxx::HTML::Node>::sibling_iterator child_end = replies_it.end();

               ChattyPost *lastpost = NULL;

               while(child_it != child_end)
               {
                  if(child_it->tagName() == "li")
                  {
                     unsigned int child_id = HTML_GetIDAttribute(child_it);

                     if(child_id != 0)
                     {
                        ChattyPost *post = new ChattyPost();
                        post->SetNewness(N_OLD);
                        m_children.push_back(post);

                        post->SetParent(this);

                        post->ReadPostPreviewChattyFromHTML(child_it, pDoc, child_id);
                                                
                        post->SetPrevSibling(lastpost);
                        post->SetNextSibling(NULL);
                        if(lastpost != NULL)
                        {
                           lastpost->SetNextSibling(post);
                        }

                        if(post->IsFiltered())
                        {
                           m_children.pop_back();
                           if(lastpost != NULL)
                           {
                              lastpost->SetNextSibling(NULL);
                           }
                           delete post;
                        }
                        else
                        {
                           lastpost = post;
                        }
                     }
                  }

                  child_it++;
               }
            }
         }
      }
   }

   m_bodytext = L"";
   m_shacktags.clear();
   DecodeString(body_temp,m_bodytext,m_shacktags);
   m_bodytext.ReplaceAll(0x02C2,L'<');
   m_bodytext.ReplaceAll(0x02C3,L'>');
   //m_bodytext.MakeNormal();
   SetupCharWidths();
   m_lines_of_text.clear();
   m_charsizes.clear();
   m_linesizes.clear();
   m_linetags.clear();
   m_linetypes.clear();
   m_lasttextrectwidth = 0;
   m_textrectheight = 0;
   m_largest_line_width = 0;
   InitImageLinks();
}

void ChattyPost::ReadPostPreviewChattyFromHTML(tree<htmlcxx::HTML::Node>::sibling_iterator &post_it, 
                                               CLampDoc *pDoc,
                                               unsigned int id)
{
   SetDoc(pDoc);
   m_id = id;
   m_bIsPreview = true;
   bool bKnown = false;

   if(theApp.IsPostKnown(id))
   {
      ReadFromKnown(pDoc); // no longer a preview
   }
   else
   {
      m_datetext = L"Preview.  Just a moment...";
      m_ago_color = theApp.GetMiscPostTextColor();
   }
   
   if(!m_bIsPreview) 
      bKnown = true;

   m_mylols = theApp.GetMyLol(m_id);

   UpdateLOLs();

   UCString body_temp;

   // read self
   tree<htmlcxx::HTML::Node>::sibling_iterator it;
   std::string value;
   if(HTML_FindChild_StartsWithAttribute(post_it, it, "div", "class", "oneline",&value))
   {
      const char *pClassStr = (const char *)value.data();
      pClassStr = strstr(pClassStr,"mod_");
      if(pClassStr != NULL)
      {
         pClassStr += 4;
         // if(strncmp(pClassStr, "ontopic", 7) do nothing.  That is the default            
         if(strncmp(pClassStr, "informative",11) == 0)   {m_category = PCT_INF;}
         else if(strncmp(pClassStr, "nws",3) == 0)       {m_category = PCT_NWS;}
         else if(strncmp(pClassStr, "offtopic",8) == 0)  {m_category = PCT_OFFTOPIC;}
         else if(strncmp(pClassStr, "political",9) == 0) {m_category = PCT_POLITICAL;}
         else if(strncmp(pClassStr, "stupid",6) == 0)    {m_category = PCT_STUPID;}
         else if(strncmp(pClassStr, "nuked",5) == 0)     {m_category = PCT_NUKED;}
      }

      if(!bKnown)
      {
         tree<htmlcxx::HTML::Node>::sibling_iterator preview_it;
         if(HTML_FindChild_HasAttribute(it, preview_it, "a", "class", "shackmsg"))
         {
            if(HTML_FindChild_HasAttribute(preview_it, preview_it, "span", "class", "oneline_body"))
            {
               std::string body_preview;
               HTML_GetValue(preview_it, body_preview);
               if(body_preview.length() > 0)
               {
                  body_temp.AppendEncodedString((const char *)body_preview.data(),body_preview.length(),CET_UTF8);
                  body_temp.Replace(L"\r\n",L" ");
               }
            }
         }

         tree<htmlcxx::HTML::Node>::sibling_iterator author_it;
         if(HTML_FindChild_HasAttribute(it, author_it, "span", "class", "oneline_user") ||
            HTML_FindChild_HasAttribute(it, author_it, "span", "class", "oneline_user this_user"))
         {
            std::string author;
            HTML_GetValue(author_it, author);
            if(author.length() > 0)
            {
               m_author = (const char*)author.data();
               m_author.TrimWhitespace();
               RemoveSomeTags(m_author);
               UpdateAuthorColor();
            }

            tree<htmlcxx::HTML::Node>::sibling_iterator lightning_it = author_it;
            lightning_it++;
            if(lightning_it->tagName() == "a" &&
               HTML_HasAttribute(lightning_it, "class", "lightningbolt"))
            {
               m_lightningbolt = true;
            }
         }
      }
   }

   tree<htmlcxx::HTML::Node>::sibling_iterator replies_it;
   if(HTML_FindChild(post_it, replies_it, "ul"))
   {
      tree<htmlcxx::HTML::Node>::sibling_iterator child_it = replies_it.begin();
      tree<htmlcxx::HTML::Node>::sibling_iterator child_end = replies_it.end();

      ChattyPost *lastpost = NULL;

      while(child_it != child_end)
      {
         if(child_it->tagName() == "li")
         {
            unsigned int child_id = HTML_GetIDAttribute(child_it);

            if(child_id != 0)
            {
               ChattyPost *post = new ChattyPost();
               post->SetNewness(N_OLD);
               m_children.push_back(post);

               post->SetParent(this);

               post->ReadPostPreviewChattyFromHTML(child_it, pDoc, child_id);
               
               post->SetPrevSibling(lastpost);
               post->SetNextSibling(NULL);
               if(lastpost != NULL)
               {
                  lastpost->SetNextSibling(post);
               }

               if(post->IsFiltered())
               {
                  m_children.pop_back();
                  if(lastpost != NULL)
                  {
                     lastpost->SetNextSibling(NULL);
                  }
                  delete post;
               }
               else
               {
                  lastpost = post;
               }
            }
         }

         child_it++;
      }
   }
   
   if(!bKnown)
   {
      m_bodytext = L"";
      m_shacktags.clear();
      DecodeString(body_temp,m_bodytext,m_shacktags);
      m_bodytext.ReplaceAll(0x02C2,L'<');
      m_bodytext.ReplaceAll(0x02C3,L'>');
      //m_bodytext.MakeNormal();
      SetupCharWidths();
      m_lines_of_text.clear();
      m_charsizes.clear();
      m_linesizes.clear();
      m_linetags.clear();
      m_linetypes.clear();
      m_lasttextrectwidth = 0;
      m_textrectheight = 0;
      m_largest_line_width = 0;
      InitImageLinks();
   }
}

void ChattyPost::ReadKnownPostChattyFromHTML(tree<htmlcxx::HTML::Node>::sibling_iterator &root_it, unsigned int id)
{
   SetDoc(NULL);
   
   m_id = id;

   m_bIsPreview = false;

   UCString body_temp;

   // read self
   tree<htmlcxx::HTML::Node>::sibling_iterator info_it;
   std::string value;
   if(HTML_FindChild_StartsWithAttribute(root_it, info_it, "div", "class", "fullpost", &value))
   {
      const char *pClassStr = (const char *)value.data();
      pClassStr = strstr(pClassStr,"mod_");
      if(pClassStr != NULL)
      {
         pClassStr += 4;
         // if(strncmp(pClassStr, "ontopic", 7) do nothing.  That is the default            
         if(strncmp(pClassStr, "informative",11) == 0)   {m_category = PCT_INF;}
         else if(strncmp(pClassStr, "nws",3) == 0)       {m_category = PCT_NWS;}
         else if(strncmp(pClassStr, "offtopic",8) == 0)  {m_category = PCT_OFFTOPIC;}
         else if(strncmp(pClassStr, "political",9) == 0) {m_category = PCT_POLITICAL;}
         else if(strncmp(pClassStr, "stupid",6) == 0)    {m_category = PCT_STUPID;}
         else if(strncmp(pClassStr, "nuked",5) == 0)     {m_category = PCT_NUKED;}
      }

      tree<htmlcxx::HTML::Node>::sibling_iterator author_it;
      if(HTML_FindChild_HasAttribute(info_it, author_it, "div", "class", "postmeta"))
      {
         if(HTML_FindChild_HasAttribute(author_it, author_it, "span", "class", "author"))
         {
            tree<htmlcxx::HTML::Node>::sibling_iterator user_it = author_it;

            if(HTML_FindChild_HasAttribute(user_it, user_it, "span", "class", "user") ||
               HTML_FindChild_HasAttribute(user_it, user_it, "span", "class", "user this-user"))
            {
               if(HTML_FindChild_StartsWithAttribute(user_it, user_it, "a", "href", "/user/"))
               {
                  std::string author;
                  HTML_GetValue(user_it, author);
                  m_author = (const char*)author.data();
                  m_author.TrimWhitespace();
                  RemoveSomeTags(m_author);
                  UpdateAuthorColor();
               }
            }

            if(HTML_FindChild_HasAttribute(author_it, user_it, "a", "class", "lightningbolt"))
            {
               m_lightningbolt = true;
            }
         }
      }

      tree<htmlcxx::HTML::Node>::sibling_iterator date_it;
      if(HTML_FindChild_HasAttribute(info_it, date_it, "div", "class", "postdate"))
      {
         std::string date;
         HTML_GetValue(date_it, date);
         m_datetext = (const char*)date.data();
         m_datetext.TrimWhitespace();
         m_datetext.MakeNormal();
         UpdateDate();
      }

      tree<htmlcxx::HTML::Node>::sibling_iterator body_it;
      if(HTML_FindChild_HasAttribute(info_it, body_it, "div", "class", "postbody"))
      {
         std::string body;
         HTML_GetValue(body_it, body);
         if(body.length() > 0)
         {
            body_temp.AppendEncodedString((const char *)body.data(),body.length(),CET_UTF8);
         }
      }
   }

   m_bodytext = L"";
   m_shacktags.clear();
   DecodeString(body_temp,m_bodytext,m_shacktags);
   m_bodytext.ReplaceAll(0x02C2,L'<');
   m_bodytext.ReplaceAll(0x02C3,L'>');
}

void ChattyPost::ReadSearchResultFromHTML(tree<htmlcxx::HTML::Node>::sibling_iterator &result_it, CLampDoc *pDoc)
{
   SetDoc(pDoc);

   UCString body;

   tree<htmlcxx::HTML::Node>::sibling_iterator it;
   //if(HTML_FindChild(result_it,it,"p"))
   //{
      tree<htmlcxx::HTML::Node>::sibling_iterator author_it;
      if(HTML_FindChild_HasAttribute(result_it,author_it,"span","class","chatty-author"))
      {
         if(HTML_FindChild_HasAttribute(author_it,author_it,"a","class","more"))
         {
            std::string value;
            HTML_GetValue(author_it, value);
            if(value.length() > 0)
            {
               m_author = (const char*)value.data();
               m_author.TrimWhitespace();
               if(m_author[m_author.Length()-1] == L':')
                  m_author.TrimEnd(1);
               RemoveSomeTags(m_author);
               UpdateAuthorColor();
            }
         }
      }

      tree<htmlcxx::HTML::Node>::sibling_iterator body_it;
      if(HTML_FindChild(result_it,body_it,"a"))
      {
         m_id = HTML_GetIDAttribute(body_it,"href");

         if(m_id != 0)
         {
            std::string value;
            HTML_GetValue(body_it, value);
            if(value.length() > 0)
            {
               body = (const char*)value.data();
               body.TrimWhitespace();

               body.Replace(L"&quot;",L"\"",false);
               body.Replace(L"&amp;",L"&",false);
               body.Replace(L"&apos;",L"\'",false);
               body.Replace(L"&lt;",L"<",false);
               body.Replace(L"&gt;",L">",false);
            }
         }
      }

      // <span class="postdate">Posted Sep 19, 2011 11:08am PDT</span>

      tree<htmlcxx::HTML::Node>::sibling_iterator date_it;
      //if(HTML_FindChild_HasAttribute(result_it,date_it,"span","class","chatty-posted"))
      if(HTML_FindChild_HasAttribute(result_it,date_it,"span","class","postdate"))
      {
         std::string value;
         HTML_GetValue(date_it, value);
         if(value.length() > 7)
         {
            char *date_str = (char*)value.data();
            if(strncmp(date_str,"Posted ",7) == 0)
            {
               date_str += 7;
            }
            m_datetext = date_str;
            m_datetext.TrimWhitespace();
            m_datetext.MakeNormal();
            UpdateDate();
         }
      }
   //}
   
   m_bodytext = L"";

   DecodeShackTagsString(body);
   m_bodytext.ReplaceAll(0x02C2,L'<');
   m_bodytext.ReplaceAll(0x02C3,L'>');

   SetupCharWidths();
   m_lines_of_text.clear();
   m_charsizes.clear();
   m_linesizes.clear();
   m_linetags.clear();
   m_linetypes.clear();
   m_lasttextrectwidth = 0;
   m_textrectheight = 0;
   m_largest_line_width = 0;
   InitImageLinks();
}

void ChattyPost::ReadMessageFromHTML(tree<htmlcxx::HTML::Node>::sibling_iterator &it, 
                                     CLampDoc *pDoc, 
                                     bool bIsInbox, 
                                     size_t id, 
                                     std::string &author, 
                                     bool bHaveRead)
{
   m_bIsInbox = bIsInbox;
   SetDoc(pDoc);
   m_id = id;
   m_bHaveRead = bHaveRead;
   m_author = (char*)author.data();
   RemoveSomeTags(m_author);
   m_author.MakeNormal();
   UpdateAuthorColor();

   tree<htmlcxx::HTML::Node>::sibling_iterator subject_it;
   if(HTML_FindChild_HasAttribute(it,subject_it, "div", "class", "subject-column toggle-message"))
   {
      if(HTML_FindChild(subject_it,subject_it, "a"))
      {
         std::string title;
         HTML_GetValue(subject_it, title);
         m_subject = (char*)title.data();
         m_subject.ReplaceAll(0x02C2,L'<');
         m_subject.ReplaceAll(0x02C3,L'>');
      }
   }

   tree<htmlcxx::HTML::Node>::sibling_iterator date_it;
   if(HTML_FindChild_HasAttribute(it,date_it, "div", "class", "date-column toggle-message"))
   {
      if(HTML_FindChild(date_it,date_it, "a"))
      {
         std::string date;
         HTML_GetValue(date_it, date);
         m_datetext = (char*)date.data();
         m_datetext.MakeNormal();
         m_ago_color = theApp.GetMiscPostTextColor();
      }
   }

   UCString body;

   tree<htmlcxx::HTML::Node>::sibling_iterator body_it;
   if(HTML_FindChild_HasAttribute(it,body_it, "div", "class", "message-box"))
   {
      if(HTML_FindChild_HasAttribute(body_it,body_it, "div", "class", "top-content"))
      {
         if(HTML_FindChild_HasAttribute(body_it,body_it, "div", "class", "message-body"))
         {
            std::string body_str;
            HTML_GetValue(body_it, body_str);
            body = (char*)body_str.data();
            //body.Replace(L"<br>",L"<br/>");
            body.Replace(L"\n<br>",L"\n");
            body.Replace(L"<br>",L"\n");
            body.Replace(L"<p>",L"");
            body.Replace(L"</p>",L"");

            // find links and use the link shacktag l{...}l
            const UCChar *start = body.Str();
            const UCChar *end = start + body.Length();
            const UCChar *current = start;

            bool bContinue = true;

            while(bContinue && current < end)
            {
               const UCChar *suspect = NULL;
               const UCChar *suspecta = wcsstr(current, L"http://");
               const UCChar *suspectb = wcsstr(current, L"https://");
               if(suspecta != NULL &&
                  suspectb != NULL)
               {
                  suspect = __min(suspecta,suspectb);
               }
               else if(suspecta != NULL)
               {
                  suspect = suspecta;
               }
               else if(suspectb != NULL)
               {
                  suspect = suspectb;
               }

               if(suspect != NULL)
               {
                  // find the end of it
                  const UCChar *work = suspect;

                  while(work < end &&
                        iswspace(*work) == 0)
                  {
                     work++;
                  }

                  if(work > suspect)
                  {
                     int position = suspect - start;
                     int suspectlen = work - suspect;
                     body.InsertChar(L'l', position);
                     position++;
                     body.InsertChar(L'{', position);
                     position++;
                     position += suspectlen;
                     body.InsertChar(L'}', position);
                     position++;
                     body.InsertChar(L'l', position);
                     position++;

                     start = body.Str();
                     end = start + body.Length();
                     current = start + position;
                  }
               }
               else
               {
                  bContinue = false;
               }
            }
         }
      }
   }

   m_bodytext = L"";
   m_shacktags.clear();
   //DecodeString(body,m_bodytext,m_shacktags);
   DecodeShackTagsString(body);
   m_bodytext.ReplaceAll(0x02C2,L'<');
   m_bodytext.ReplaceAll(0x02C3,L'>');

   //m_bodytext.MakeNormal();
   SetupCharWidths();
   m_lines_of_text.clear();
   m_charsizes.clear();
   m_linesizes.clear();
   m_linetags.clear();
   m_linetypes.clear();
   m_lasttextrectwidth = 0;
   m_textrectheight = 0;
   m_largest_line_width = 0;
   InitImageLinks();
}

void ChattyPost::ReadLOL(CLampDoc *pDoc,
                         UCString &id,
                         UCString &lolcount,
                         UCString &author,
                         UCString &body)
{
   SetDoc(pDoc);
   m_datetext = L"";
   m_category = PCT_NORMAL;
   m_author = author;
   RemoveSomeTags(m_author);
   UpdateAuthorColor();
   m_id = id;
   m_bodytext = L"";
   m_shacktags.clear();
   DecodeString(body,m_bodytext,m_shacktags);
   m_bodytext.ReplaceAll(0x02C2,L'<');
   m_bodytext.ReplaceAll(0x02C3,L'>');
   SetupCharWidths();
   m_lines_of_text.clear();
   m_charsizes.clear();
   m_linesizes.clear();
   m_linetags.clear();
   m_linetypes.clear();
   m_lasttextrectwidth = 0;
   m_textrectheight = 0;
   m_largest_line_width = 0;
   InitImageLinks();
}

void ChattyPost::ReadPost(ChattyPost *pOther, CLampDoc *pDoc)
{
   if(pOther != NULL)
   {
      SetDoc(pDoc);
      m_datetext = pOther->m_datetext;
      m_tm_posttime = pOther->m_tm_posttime;
      UpdateDate();
      m_category = pOther->m_category;
      m_author = pOther->m_author;
      RemoveSomeTags(m_author);
      UpdateAuthorColor();

      m_Newness = pOther->m_Newness;
      
      m_bodytext = pOther->m_bodytext;
      SetupCharWidths();
      m_shacktags = pOther->m_shacktags;
      InitImageLinks();

      m_id = pOther->m_id;

      m_mylols = theApp.GetMyLol(m_id);

      m_lolflags = pOther->m_lolflags;
      m_lol_text = pOther->m_lol_text;
      m_inf_text = pOther->m_inf_text;
      m_unf_text = pOther->m_unf_text;
      m_tag_text = pOther->m_tag_text;
      m_wtf_text = pOther->m_wtf_text;
      
      m_lasttextrectwidth = 0;
      m_textrectheight = 0;
      m_largest_line_width = 0;
      m_bRefreshing = false;

      m_bShowTruncated = pOther->m_bShowTruncated;
      m_familysize = pOther->m_familysize;

      if(pOther->m_children.size() > 0)
      {
         ChattyPost *lastpost = NULL;
         std::list<ChattyPost*>::iterator it = pOther->m_children.begin();
         std::list<ChattyPost*>::iterator end = pOther->m_children.end();
         while(it != end)
         {
            ChattyPost *post = new ChattyPost();
            if(post != NULL)
            {
               post->ReadPost((*it), pDoc);
               post->SetParent(this);
               post->SetPrevSibling(lastpost);
               post->SetNextSibling(NULL);
               if(lastpost != NULL)
               {
                  lastpost->SetNextSibling(post);
               }
               lastpost = post;
               m_children.push_back(post);
            }
            it++;
         }
      }
   }
}

void ChattyPost::Read(CXMLElement *pElement, CLampDoc *pDoc, bool bDoingNewFlags)
{
   if(pElement != NULL)
   {
      SetDoc(pDoc);
      m_datetext = pElement->GetAttributeValue(L"date");
      m_datetext.MakeNormal();
      UpdateDate();

      UCString category = pElement->GetAttributeValue(L"category");
      category.MakeNormal();

      // if(category == L"ontopic")
      // do nothing.  That is the default
      
      if(category == L"informative")
      {
         m_category = PCT_INF;
      }
      else if(category == L"nws")
      {
         m_category = PCT_NWS;
      }
      else if(category == L"offtopic")
      {
         m_category = PCT_OFFTOPIC;
      }
      else if(category == L"political")
      {
         m_category = PCT_POLITICAL;
      }
      else if(category == L"stupid")
      {
         m_category = PCT_STUPID;
      }
      else if(category == "nuked")
      {
         m_category = PCT_NUKED;
      }

      m_author = pElement->GetAttributeValue(L"author");
      RemoveSomeTags(m_author);
      m_author.MakeNormal();
      UpdateAuthorColor();

      UCString temp = pElement->GetAttributeValue(L"id");
      m_id = temp;
      if(pDoc->GetDataType() == DDT_STORY)
      {
         m_bCollapsed = theApp.GetMyCollapse(m_id);
      }

      m_mylols = theApp.GetMyLol(m_id);

      UpdateLOLs();

      CXMLElement *pBody = pElement->FindChildElement(L"body");
      if(pBody != NULL)
      {
         UCString &temp = pBody->GetValue();
         m_bodytext = L"";
         m_shacktags.clear();
         DecodeString(temp,m_bodytext,m_shacktags);
         m_bodytext.ReplaceAll(0x02C2,L'<');
         m_bodytext.ReplaceAll(0x02C3,L'>');
         //m_bodytext.MakeNormal();
         SetupCharWidths();
         m_lines_of_text.clear();
         m_charsizes.clear();
         m_linesizes.clear();
         m_linetags.clear();
         m_linetypes.clear();
         InitImageLinks();
      }

      m_lasttextrectwidth = 0;
      m_textrectheight = 0;
      m_largest_line_width = 0;

      CXMLElement *pChildren = pElement->FindChildElement(L"comments");

      if(pChildren == NULL && 
         pElement->CountChildren() > 0)
      {
         // if we don't have a comments section, just look under ourselves.
         pChildren = pElement;
      }

      if(m_pParent == NULL &&
         pChildren != NULL && 
         !IsFiltered() &&
         pChildren->CountChildren() == 0 &&
         m_children.size() == 0)
      {
         CXMLElement *pCachedChildren = theApp.GetCachedThreadReplies(m_id);
         if(pCachedChildren != NULL &&
            pCachedChildren->CountChildren() > 0)
         {
            pChildren = pCachedChildren;
         }
      }

      if(pChildren != NULL && !IsFiltered())
      {
         // first, clear out any nuked posts we know about
         std::vector<size_t> newids;
         int count = pChildren->CountChildren();
         for(int i = 0; i < count; i++)
         {
            CXMLElement *pChild = pChildren->GetChildElement(i);
            if(pChild != NULL &&
               pChild->GetTag() == L"comment")
            {
               UCString tempid = pChild->GetAttributeValue(L"id");
               size_t this_id = tempid;
               newids.push_back(this_id);
            }
         }

         std::list<ChattyPost*>::iterator it = m_children.begin();
         std::list<ChattyPost*>::iterator end = m_children.end();
         while(it != end)
         {
            size_t this_id = (*it)->GetId();
            bool bFound = false;
            for(size_t i =0; i < newids.size(); i++)
            {
               if(this_id == newids[i])
               {
                  bFound = true;
                  break;
               }
            }

            if(!bFound)
            {
               std::list<ChattyPost*>::iterator next = it;
               next++;

               m_children.erase(it);
               it = next;
            }
            else
            {
               it++;
            }
         }

         // now load the new posts
         it = m_children.begin();
         end = m_children.end();

         ChattyPost *post = NULL;
         ChattyPost *lastpost = NULL;
         size_t last_id = 0;
         for(int i = 0; i < count; i++)
         {
            CXMLElement *pChild = pChildren->GetChildElement(i);
            if(pChild != NULL &&
               pChild->GetTag() == L"comment")
            {
               lastpost = post;

               UCString tempid = pChild->GetAttributeValue(L"id");
               size_t this_id = tempid;

               if(this_id != 0 &&
                  this_id != last_id)
               {
                  bool bAddedBeforeIt = false;

                  if(it != end &&
                     (*it)->GetId() == this_id)
                  {
                     post = (*it);
                     bAddedBeforeIt = true;
                     post->BumpNewnessDown();
                     it++;
                  }
                  else
                  {
                     /////////////////////
                     post = new ChattyPost();
                     if(post != NULL)
                     {
                        if(it != end)
                        {
                           m_children.insert(it,post);
                           bAddedBeforeIt = true;
                        }
                        else
                        {
                           m_children.push_back(post);
                        }
                     }

                     if(!bDoingNewFlags)
                     {
                        post->SetNewness(N_OLD);
                     }
                  }

                  post->SetParent(this);
                  post->SetPrevSibling(lastpost);
                  post->SetNextSibling(NULL);
                  if(lastpost != NULL)
                  {
                     lastpost->SetNextSibling(post);
                  }

                  post->Read(pChild, pDoc, bDoingNewFlags);
                  
                  if(post->IsFiltered())
                  {
                     if(bAddedBeforeIt)
                     {
                        std::list<ChattyPost*>::iterator newit = it;
                        newit--;
                        m_children.erase(newit);
                     }
                     else
                     {
                        m_children.pop_back();
                     }
                     if(lastpost != NULL)
                     {
                        lastpost->SetNextSibling(NULL);
                     }
                     delete post;
                     post = lastpost;
                     last_id = 0;
                  }
                  else
                  {
                     last_id = post->GetId();
                  }
               }
            }
         }
      }
   }
}

void ChattyPost::ReadShackMessage(CXMLElement *pElement, CLampDoc *pDoc, bool bIsInbox)
{
   if(pElement != NULL)
   {
      m_bIsInbox = bIsInbox;

      SetDoc(pDoc);
      m_datetext = pElement->GetAttributeValue(L"date");
      m_datetext.MakeNormal();
      //UpdateDate();
      m_ago_color = theApp.GetMiscPostTextColor();

      m_author = pElement->GetAttributeValue(L"author");
      RemoveSomeTags(m_author);
      m_author.MakeNormal();
      UpdateAuthorColor();

      UCString temp = pElement->GetAttributeValue(L"id");
      m_id = temp;

      m_subject = pElement->GetAttributeValue(L"subject");
      m_subject.ReplaceAll(0x02C2,L'<');
      m_subject.ReplaceAll(0x02C3,L'>');

      m_bHaveRead = false;
      temp = pElement->GetAttributeValue(L"status");
      if(temp == L"read")
      {
         m_bHaveRead = true;
      }

      UCString body = pElement->GetElementValue(L"body");
      body.Replace(L"<br>",L"<br/>");
      body.Replace(L"<p>",L"");
      body.Replace(L"</p>",L"");
            
      m_bodytext = L"";
      m_shacktags.clear();
      DecodeString(body,m_bodytext,m_shacktags);
      m_bodytext.ReplaceAll(0x02C2,L'<');
      m_bodytext.ReplaceAll(0x02C3,L'>');
      //m_bodytext.MakeNormal();
      SetupCharWidths();
      m_lines_of_text.clear();
      m_charsizes.clear();
      m_linesizes.clear();
      m_linetags.clear();
      m_linetypes.clear();

      m_lasttextrectwidth = 0;
      m_textrectheight = 0;
      m_largest_line_width = 0;

      InitImageLinks();
   }
}

void ChattyPost::ReadSearchResult(CXMLElement *pElement, CLampDoc *pDoc)
{
   if(pElement != NULL)
   {
      SetDoc(pDoc);
      m_datetext = pElement->GetAttributeValue(L"date");
      m_datetext.MakeNormal();
      UpdateDate();

      m_category = PCT_NORMAL;

      m_author = pElement->GetAttributeValue(L"author");
      RemoveSomeTags(m_author);
      m_author.MakeNormal();
      UpdateAuthorColor();

      UCString temp = pElement->GetAttributeValue(L"id");
      m_id = temp;

      m_mylols = theApp.GetMyLol(m_id);

      UpdateLOLs();

      UCString body = pElement->GetAttributeValue(L"preview");
      body.Replace(L"&quot;",L"\"",false);
      body.Replace(L"&amp;",L"&",false);
      body.Replace(L"&apos;",L"\'",false);
      body.Replace(L"&lt;",L"<",false);
      body.Replace(L"&gt;",L">",false);
      /*
      temp = pElement->GetAttributeValue(L"story_name");// "Evening Reading"

      body += L"<br/><br/><i><span class=\"jt_blue\">Posted in: ";
      body += L"";
      body += temp;
      body += L"</span></i>";
      */
      m_bodytext = L"";

      DecodeShackTagsString(body);

      //m_shacktags.clear();
      //DecodeString(body,m_bodytext,m_shacktags);
      m_bodytext.ReplaceAll(0x02C2,L'<');
      m_bodytext.ReplaceAll(0x02C3,L'>');
      //m_bodytext.MakeNormal();
      SetupCharWidths();
      m_lines_of_text.clear();
      m_charsizes.clear();
      m_linesizes.clear();
      m_linetags.clear();
      m_linetypes.clear();

      m_lasttextrectwidth = 0;
      m_textrectheight = 0;
      m_largest_line_width = 0;

      InitImageLinks();
   }
}

void ChattyPost::InitImageLinks()
{
   // see if any of the links should be converted to image links
   int begin = -1;
   int end = -1;

   if(theApp.LoadImagesInLamp())
   {
      for(size_t i=0; i < m_shacktags.size(); i++)
      {
         if(m_shacktags[i].m_tag == ST_LINK)
         {
            begin = i;
         }

         if(m_shacktags[i].m_tag == ST_LINK_END)
         {
            end = i;
         }

         if(begin != -1 && end != -1)
         {
            UCString link = m_shacktags[begin].m_href;

            unsigned int index = 0;

            if(theApp.ShowImageThumbs() &&
               theApp.HasLinkedImage(link,index) &&
               theApp.IsThumbLoaded(index))
            {
               if(!theApp.DontAutoLoadNWSThumbs() ||
                  !IsNWSPost())
               {
                  m_shacktags[begin].m_tag = ST_THUMB;
                  m_shacktags[begin].m_image_index = index;
                  m_shacktags[end].m_tag = ST_THUMB_END;
               }
               else
               {
                  m_shacktags[begin].m_tag = ST_IMAGE_LINK;
                  m_shacktags[end].m_tag = ST_IMAGE_LINK_END;
               }
            }
            else if((link.endswith(L".jpg") != NULL ||
                     link.endswith(L".jpeg") != NULL ||
                     link.endswith(L".png") != NULL) &&
                     link.beginswith(L"http://"))
            {
               if(theApp.ShowImageThumbs() &&
                  theApp.AutoLoadChattypicsThumbs() &&
                  (link.beginswith(L"http://www.chattypics.com") != NULL ||
                   link.beginswith(L"http://chattypics.com") != NULL))
               {
                  if(!theApp.DontAutoLoadNWSThumbs() ||
                     !IsNWSPost())
                  {
                     theApp.GetLinkedImage(link,index);
                     
                     m_shacktags[begin].m_image_index = index;
                     m_shacktags[begin].m_tag = ST_THUMB;
                     m_shacktags[end].m_tag = ST_THUMB_END;
                     theApp.LoadImageThumb(index,m_id);
                  }
                  else
                  {
                     m_shacktags[begin].m_tag = ST_IMAGE_LINK;
                     m_shacktags[end].m_tag = ST_IMAGE_LINK_END;
                  }
               }
               else
               {
                  m_shacktags[begin].m_tag = ST_IMAGE_LINK;
                  m_shacktags[end].m_tag = ST_IMAGE_LINK_END;
               }
            }

            begin = -1;
            end = -1;
         }
      }
   }
}

void ChattyPost::LoadAllImageLinks()
{
   // see if any of the links should be converted to image links
   int begin = -1;
   int end = -1;

   shacktag type = ST_RED;

   for(size_t i=0; i < m_shacktags.size(); i++)
   {
      if(m_shacktags[i].m_tag == ST_IMAGE_LINK)
      {
         begin = i;
         type = ST_IMAGE_LINK;
      }

      if(m_shacktags[i].m_tag == ST_THUMB)
      {
         begin = i;
         type = ST_THUMB;
      }

      if(m_shacktags[i].m_tag == ST_IMAGE_LINK_END && type == ST_IMAGE_LINK)
      {
         end = i;
      }

      if(m_shacktags[i].m_tag == ST_THUMB_END && type == ST_THUMB)
      {
         end = i;
      }

      if(begin != -1 && end != -1)
      {
         UCString link = m_shacktags[begin].m_href;

         const UCChar *ext = link.Str() + link.Length();
         while(ext > link.Str() && *ext != L'.') ext--;

         if(_wcsicmp(ext,L".png") == 0 ||
            _wcsicmp(ext,L".jpg") == 0 || 
            _wcsicmp(ext,L".jpeg") == 0)
         {
            unsigned int index = 0;
            CDCSurface *pImage = theApp.GetLinkedImage(link,index);
            if(pImage != NULL &&
               pImage->GetDC() != NULL &&
               pImage->GetWidth() > 0 &&
               pImage->GetHeight() > 0)
            {
               m_shacktags[begin].m_tag = ST_IMAGE;
               m_shacktags[begin].m_image_index = index;
               m_shacktags[end].m_tag = ST_IMAGE_END;

               if(!theApp.IsImageLoaded(index))
               {
                  theApp.LoadImage(index,m_id);
               }
            }
         }

         begin = -1;
         end = -1;
      }
   }

   // this is to trigger a recalc of the line tags
   m_lasttextrectwidth = 0;
}

void ChattyPost::UnloadAllImagesRecurse()
{
   CloseAllImageLinks();

   std::list<ChattyPost*>::iterator it = m_children.begin();
   std::list<ChattyPost*>::iterator end = m_children.end();
   while(it != end)
   {
      if((*it) != NULL)
      {
         (*it)->UnloadAllImagesRecurse();
      }
      it++;
   }
}

void ChattyPost::UnloadImageRecurse(unsigned int image_index)
{
   bool bFoundStart = false;
   for(size_t i=0; i < m_shacktags.size(); i++)
   {
      if((m_shacktags[i].m_tag == ST_IMAGE ||
          m_shacktags[i].m_tag == ST_THUMB) &&
          m_shacktags[i].m_image_index == image_index)
      {
         m_shacktags[i].m_tag = ST_IMAGE_LINK;
         bFoundStart = true;
      }

      if((m_shacktags[i].m_tag == ST_IMAGE_END ||
          m_shacktags[i].m_tag == ST_THUMB_END) &&
          bFoundStart)
      {
         m_shacktags[i].m_tag = ST_IMAGE_LINK_END;
         bFoundStart = false;
      }
   }

   // this is to trigger a recalc of the line tags
   m_lasttextrectwidth = 0;


   std::list<ChattyPost*>::iterator it = m_children.begin();
   std::list<ChattyPost*>::iterator end = m_children.end();
   while(it != end)
   {
      if((*it) != NULL)
      {
         (*it)->UnloadImageRecurse(image_index);
      }
      it++;
   }
}

void ChattyPost::CloseAllImageLinks()
{
   // see if any of the links should be converted to image links
   int begin = -1;
   int end = -1;

   shacktag type = ST_RED;

   for(size_t i=0; i < m_shacktags.size(); i++)
   {
      if(m_shacktags[i].m_tag == ST_IMAGE)
      {
         begin = i;
         type = ST_IMAGE;
      }

      if(m_shacktags[i].m_tag == ST_THUMB)
      {
         begin = i;
         type = ST_THUMB;
      }

      if(m_shacktags[i].m_tag == ST_IMAGE_END && type == ST_IMAGE)
      {
         end = i;
      }

      if(m_shacktags[i].m_tag == ST_THUMB_END && type == ST_THUMB)
      {
         end = i;
      }

      if(begin != -1 && end != -1)
      {
         m_shacktags[begin].m_tag = ST_IMAGE_LINK;
         m_shacktags[end].m_tag = ST_IMAGE_LINK_END;

         begin = -1;
         end = -1;
      }
   }

   // this is to trigger a recalc of the line tags
   m_lasttextrectwidth = 0;
}

void ChattyPost::SetupBodyText(RECT &textrect)
{
   m_pDoc->CalcBodyText(textrect,m_bodytext,m_pCharWidths,m_shacktags,m_bodytext.Length(),m_lines_of_text,m_charsizes,m_linesizes,m_linetags,m_linetypes);

   m_largest_line_width = 0;

   for(size_t i = 0; i < m_lines_of_text.size(); i++)
   {
      unsigned int thislinelength = 0;

      if(m_linetypes[i].m_bIsText)
      {
         const int* charsizes = m_charsizes[i];
         int linesize = m_linesizes[i];
         const int* work = charsizes;
         const int* end = work + linesize;
         while(work < end)
         {
            thislinelength += *work;
            work++;
         }
         thislinelength += 5;
      }
      else
      {
         thislinelength = textrect.right - textrect.left;
      }

      if(thislinelength > m_largest_line_width)
      {
         m_largest_line_width = thislinelength;
      }
   }
}


void ChattyPost::DrawTextOnly(HDC hDC, RECT &DeviceRectangle, int pos)
{
   if(m_pDoc != NULL)
   {
      RECT textrect = DeviceRectangle;
      textrect.top = 0;
      textrect.bottom = 0;
      
      if(textrect.right - textrect.left != m_lasttextrectwidth)
      {
         m_lasttextrectwidth = textrect.right - textrect.left;
         SetupBodyText(textrect);
         m_textrectheight = textrect.bottom;
      }
      else
      {
         textrect.bottom = m_textrectheight;
      }

      textrect.top += pos;
      textrect.bottom += pos;

      std::vector<RECT> spoilers;
      std::vector<RECT> links;
      std::vector<RECT> imagelinks;
      std::vector<RECT> images;
      std::vector<RECT> thumbs;
      m_pDoc->DrawBodyText(hDC,textrect,m_lines_of_text,m_charsizes,m_linesizes,m_linetags,m_linetypes,spoilers,links,imagelinks,images,thumbs, m_bComplexShapeText,&DeviceRectangle);
   }
}


int ChattyPost::DrawMessage(HDC hDC, RECT &DeviceRectangle, int pos, std::vector<CHotSpot> &hotspots, unsigned int current_id)
{
   if(m_pDoc != NULL)
   {
      if(m_bCollapsed)
      {
         m_pos = pos;
         m_drewtextpos = pos + 20 + theApp.GetCellHeight();
         m_bDrewTextBody = false;
         m_drewtextedge = DeviceRectangle.left + 20;

         RECT myrect = DeviceRectangle;
         myrect.left += 20;
         myrect.right -= 20;
         myrect.top = pos;
         myrect.bottom = pos + theApp.GetCellHeight();

         pos = myrect.bottom;

         m_height = pos - m_pos;

         if(myrect.bottom < DeviceRectangle.top ||
            myrect.top > DeviceRectangle.bottom)
         {
            // do nothing, we are not on the screen
         }
         else
         {
            RECT backrect = myrect;
            backrect.left = DeviceRectangle.left;
            backrect.right = backrect.left + 20;
            m_pDoc->FillBackground(hDC,backrect);

            backrect.left = DeviceRectangle.right - 20;
            backrect.right = DeviceRectangle.right;
            m_pDoc->FillBackground(hDC,backrect);

            m_pDoc->FillExpandedBackground(hDC,myrect,true,m_category,false);
            
            RECT authorrect = myrect;
            authorrect.right = authorrect.left + (theApp.GetCellHeight() + 5 + m_authorsize + 5);
            m_pDoc->DrawRootAuthor(hDC,authorrect,m_author, m_AuthorColor, false, m_bIsInbox);
            
            /*
            RECT collapsedrect = myrect;
            collapsedrect.left += (theApp.GetCellHeight() + 5 + m_authorsize + 5);
            m_pDoc->DrawCollapseNote(hDC,collapsedrect);
            */

            RECT daterect;
            daterect.left = myrect.left;
            daterect.right = myrect.right;
            daterect.top = myrect.bottom;
            daterect.bottom = myrect.bottom;
            m_pDoc->DrawDate(hDC,daterect,m_datetext,m_ago_color,true);

            RECT subjectrect;
            subjectrect.left = authorrect.right + 5;
            subjectrect.right = daterect.left - 5;
            subjectrect.top = myrect.top;
            subjectrect.bottom = myrect.bottom;

            int shade = 10;
            if(m_bHaveRead) shade = 0;
            bool clipped = false;
            std::vector<shacktagpos> emptytags;
            m_pDoc->DrawPreviewText(hDC,subjectrect,m_subject,m_pSubjectCharWidths,emptytags,shade,clipped,false);

            CHotSpot hotspot;
            hotspot.m_bAnim = false;
            hotspot.m_type = HST_EXPAND;
            hotspot.m_spot = myrect;
            hotspot.m_id = m_id;
            hotspots.push_back(hotspot);
         }
      }
      else
      {
         m_pos = pos;
         m_drewtextpos = pos + 20 + theApp.GetCellHeight();

         RECT textrect = DeviceRectangle;
         textrect.left += 40;
         textrect.right -= 40;
         textrect.top = 0;
         textrect.bottom = 0;
         
         if(textrect.right - textrect.left != m_lasttextrectwidth)
         {
            m_lasttextrectwidth = textrect.right - textrect.left;
            SetupBodyText(textrect);
            m_textrectheight = textrect.bottom;
         }
         else
         {
            textrect.bottom = m_textrectheight;
         }

         RECT myrect = DeviceRectangle;
         myrect.top = pos;
         myrect.bottom = pos + textrect.bottom + theApp.GetCellHeight() + theApp.GetCellHeight();

         if(myrect.bottom < DeviceRectangle.top ||
            myrect.top > DeviceRectangle.bottom)
         {
            // do nothing, we are not on the screen
            // but update the pos
            pos = myrect.bottom;
            m_bDrewTextBody = false;
         }
         else
         {
            m_bDrewTextBody = true;
            //m_pDoc->FillBackground(hDC,myrect);
            RECT backrect = myrect;
            backrect.right = backrect.left + 20;
            m_pDoc->FillBackground(hDC,backrect);

            backrect.left = myrect.right - 20;
            backrect.right = myrect.right;
            m_pDoc->FillBackground(hDC,backrect);

            textrect.top += pos + theApp.GetCellHeight();
            textrect.bottom += pos + theApp.GetCellHeight();
            pos = myrect.bottom;

            myrect.left += 20;
            myrect.right -= 20;
            //myrect.top += 20;

            bool bAsRoot = true;
            if(current_id == m_id)
            {
               bAsRoot = false;
            }

            m_pDoc->FillExpandedBackground(hDC,myrect,bAsRoot,m_category,!theApp.StrokeRootEdges());

            std::vector<RECT> spoilers;
            std::vector<RECT> links;
            std::vector<RECT> imagelinks;
            std::vector<RECT> images;
            std::vector<RECT> thumbs;
            m_pDoc->DrawBodyText(hDC,textrect,m_lines_of_text,m_charsizes,m_linesizes,m_linetags,m_linetypes,spoilers,links,imagelinks,images,thumbs, m_bComplexShapeText,&textrect);
            m_drewtextpos = textrect.top;
            m_drewtextedge = textrect.left;
            
            RECT authorrect;
            authorrect.left = textrect.left;
            authorrect.right = textrect.left + theApp.GetCellHeight() + 5 + m_authorsize + 5;
            authorrect.top = myrect.top;
            authorrect.bottom = textrect.top;
            m_pDoc->DrawRootAuthor(hDC,authorrect,m_author, m_AuthorColor, false, m_bIsInbox);

            authorrect.left = textrect.left + theApp.GetCellHeight() + 5;
            CHotSpot hotspot;
            hotspot.m_bAnim = false;
            hotspot.m_type = HST_AUTHOR;
            hotspot.m_spot = authorrect;
            hotspot.m_id = m_id;
            hotspots.push_back(hotspot);

            RECT subjectrect;
            subjectrect.left = authorrect.right + 5;
            subjectrect.right = textrect.right - 5;
            subjectrect.top = authorrect.top;
            subjectrect.bottom = authorrect.bottom;
            bool clipped = false;
            std::vector<shacktagpos> emptytags;
            m_pDoc->DrawPreviewText(hDC,subjectrect,m_subject,m_pSubjectCharWidths,emptytags,0,clipped,false);

            hotspot.m_type = HST_CLOSE_MESSAGE;
            hotspot.m_spot = subjectrect;
            hotspot.m_spot.right = textrect.right;
            hotspot.m_id = m_id;
            hotspots.push_back(hotspot);

            RECT daterect;
            daterect.left = textrect.left;
            daterect.right = myrect.right;
            daterect.top = textrect.bottom;
            daterect.bottom = myrect.bottom;
            m_pDoc->DrawDate(hDC,daterect,m_datetext,m_ago_color);

            hotspot.m_type = HST_REPLY_TO_MESSAGE;
            hotspot.m_spot.left = myrect.left + 20;
            hotspot.m_spot.bottom = myrect.bottom;
            hotspot.m_spot.right = hotspot.m_spot.left + theApp.GetReplyBuffer(false)->GetWidth();
            hotspot.m_spot.top = myrect.bottom - 20;
            hotspot.m_id = m_id;
            hotspots.push_back(hotspot);

            hotspot.m_type = HST_FORWARD_MESSAGE;
            hotspot.m_spot.left = hotspot.m_spot.right;
            hotspot.m_spot.right = hotspot.m_spot.left + theApp.GetForwardImage(false)->GetWidth();
            hotspot.m_id = m_id;
            hotspots.push_back(hotspot);

            if(theApp.UseShack() && theApp.GetDeleteImage(false)->GetWidth() > 0)
            {
               hotspot.m_type = HST_DELETE_MESSAGE;
               hotspot.m_spot.left = hotspot.m_spot.right;
               hotspot.m_spot.right = hotspot.m_spot.left + theApp.GetDeleteImage(false)->GetWidth();
               hotspot.m_id = m_id;
               hotspots.push_back(hotspot);
            }

            for(size_t s = 0; s < spoilers.size(); s++)
            {
               hotspot.m_type = HST_SPOILER;
               hotspot.m_spot = spoilers[s];
               hotspot.m_id = m_id;
               hotspots.push_back(hotspot);
            }

            for(size_t s = 0; s < links.size(); s++)
            {
               hotspot.m_type = HST_LINK;
               hotspot.m_spot = links[s];
               hotspot.m_id = m_id;
               hotspots.push_back(hotspot);
            }

            for(size_t s = 0; s < imagelinks.size(); s++)
            {
               hotspot.m_type = HST_IMAGE_LINK;
               hotspot.m_spot = imagelinks[s];
               hotspot.m_id = m_id;
               hotspots.push_back(hotspot);
            }

            for(size_t s = 0; s < images.size(); s++)
            {
               hotspot.m_type = HST_IMAGE;
               hotspot.m_spot = images[s];
               hotspot.m_id = m_id;
               hotspots.push_back(hotspot);
            }

            for(size_t s = 0; s < thumbs.size(); s++)
            {
               hotspot.m_type = HST_THUMB;
               hotspot.m_spot = thumbs[s];
               hotspot.m_id = m_id;
               hotspots.push_back(hotspot);
            }

            hotspot.m_type = HST_TEXT;
            hotspot.m_spot = textrect;
            hotspot.m_id = m_id;
            hotspots.push_back(hotspot);
         }

         m_height = pos - m_pos;
      }
   }

   return pos;
}


int ChattyPost::DrawRoot(HDC hDC, RECT &DeviceRectangle, int pos, std::vector<CHotSpot> &hotspots, unsigned int current_id, bool bLinkOnly, bool bAllowModTools, bool bModToolIsUp, RECT &ModToolRect, unsigned int ModToolPostID)
{
   if(m_pDoc != NULL)
   {
      if(m_bJustText)
      {
         m_pos = pos;
         RECT myrect = DeviceRectangle;
         myrect.top = pos;
         myrect.bottom = pos + theApp.GetCellHeight();
         pos = myrect.bottom;

         m_pDoc->FillBackground(hDC,myrect);

         myrect.left = ((myrect.right - myrect.left) - m_subjectwidth) / 2;

         bool clipped = false;
         m_pDoc->DrawPreviewText(hDC,myrect,m_subject, m_pSubjectCharWidths,m_shacktags,0,clipped,false);

      }
      else if(m_bCollapsed)
      {
         m_pos = pos;
         if(!theApp.HideCollapsedPosts())
         {
            m_drewtextpos = pos + 20 + theApp.GetCellHeight();
            m_bDrewTextBody = false;
            m_drewtextedge = DeviceRectangle.left + 20;

            RECT myrect = DeviceRectangle;
            myrect.left += 20;
            myrect.right -= 20;
            myrect.top = pos;
            myrect.bottom = pos + theApp.GetCellHeight();

            pos = myrect.bottom;

            m_height = pos - m_pos;

            if(myrect.bottom < DeviceRectangle.top ||
               myrect.top > DeviceRectangle.bottom)
            {
               // do nothing, we are not on the screen
            }
            else
            {
               RECT backrect = myrect;
               backrect.left = DeviceRectangle.left;
               backrect.right = backrect.left + 20;
               m_pDoc->FillBackground(hDC,backrect);

               backrect.left = DeviceRectangle.right - 20;
               backrect.right = DeviceRectangle.right;
               m_pDoc->FillBackground(hDC,backrect);

               m_pDoc->FillExpandedBackground(hDC,myrect,false,m_category,!theApp.StrokeRootEdges());
               
               RECT authorrect = myrect;
               authorrect.right += (theApp.GetCellHeight() + 5 + m_authorsize + 5);
               m_pDoc->DrawRootAuthor(hDC,authorrect,m_author, m_AuthorColor, true);
                           
               RECT collapsedrect = myrect;
               collapsedrect.left += (theApp.GetCellHeight() + 5 + m_authorsize + 5);
               m_pDoc->DrawCollapseNote(hDC,collapsedrect);

               CHotSpot hotspot;
               hotspot.m_bAnim = false;
               hotspot.m_type = HST_EXPAND;
               hotspot.m_spot = myrect;
               hotspot.m_id = m_id;
               hotspots.push_back(hotspot);
            }
         }
      }
      else
      {
         m_pos = pos;
         m_drewtextpos = pos + 20 + theApp.GetCellHeight();

         RECT textrect = DeviceRectangle;
         textrect.left += 40;
         textrect.right -= 40;
         textrect.top = 0;
         textrect.bottom = 0;
         
         if(textrect.right - textrect.left != m_lasttextrectwidth)
         {
            m_lasttextrectwidth = textrect.right - textrect.left;
            SetupBodyText(textrect);
            m_textrectheight = textrect.bottom;
         }
         else
         {
            textrect.bottom = m_textrectheight;
         }

         int trunkatingposts = 0;

         if(!bLinkOnly && 
            m_familysize > (size_t)theApp.GetNumToShowWhenTruncated() &&
            m_bShowTruncated)
         {
            trunkatingposts = (size_t)(m_familysize - theApp.GetNumToShowWhenTruncated());
         }

         RECT myrect = DeviceRectangle;
         myrect.top = pos;
         myrect.bottom = pos + textrect.bottom + theApp.GetCellHeight() + theApp.GetCellHeight();

         if(myrect.bottom < DeviceRectangle.top ||
            myrect.top > DeviceRectangle.bottom)
         {
            // do nothing, we are not on the screen
            // but update the pos

            if(bAllowModTools && bModToolIsUp && m_id == ModToolPostID)
            {
               // update the hotspot locations
               ModToolRect.top = -1;
               ModToolRect.bottom = -1;
            }

            pos = myrect.bottom;
            m_bDrewTextBody = false;
         }
         else
         {
            m_bDrewTextBody = true;

            bool bAsRoot = true;
            if(current_id == m_id &&
               theApp.ShowRootSelected())
            {
               bAsRoot = false;
            }

            //m_pDoc->FillBackground(hDC,myrect);
            RECT backrect = myrect;
            backrect.right = backrect.left + 20;
            m_pDoc->FillBackground(hDC,backrect);

            backrect.left = myrect.right - 20;
            backrect.right = myrect.right;
            m_pDoc->FillBackground(hDC,backrect);

            textrect.top += pos + theApp.GetCellHeight();
            textrect.bottom += pos + theApp.GetCellHeight();
            pos = myrect.bottom;

            myrect.left += 20;
            myrect.right -= 20;
            //myrect.top += 20;
            m_pDoc->FillExpandedBackground(hDC,myrect,bAsRoot,m_category,!theApp.StrokeRootEdges());            

            std::vector<RECT> spoilers;
            std::vector<RECT> links;
            std::vector<RECT> imagelinks;
            std::vector<RECT> images;
            std::vector<RECT> thumbs;
            m_pDoc->DrawBodyText(hDC,textrect,m_lines_of_text,m_charsizes,m_linesizes,m_linetags,m_linetypes,spoilers,links,imagelinks,images,thumbs, m_bComplexShapeText,&textrect);
            m_drewtextpos = textrect.top;
            m_drewtextedge = textrect.left;

            if(bAllowModTools)
            {
               if(bModToolIsUp && m_id == ModToolPostID)
               {
                  CDCSurface *pModImage = NULL;
                  switch(m_category)
                  {
                  case PCT_NORMAL:        pModImage = theApp.GetOnTopicImage(true);break;
                  case PCT_INF:           pModImage = theApp.GetINFImage(true);break;
                  case PCT_NWS:           pModImage = theApp.GetNWSImage(true);break;
                  case PCT_STUPID:        pModImage = theApp.GetStupidImage(true);break;
                  case PCT_OFFTOPIC:      pModImage = theApp.GetOffTopicImage(true);break;
                  case PCT_POLITICAL:     pModImage = theApp.GetPoliticalImage(true);break;
                  case PCT_NUKED:         pModImage = theApp.GetNukedImage(true);break;
                  }
                  
                  if(pModImage != NULL)
                  {
                     RECT modrect;
                     modrect.right = myrect.right - 60;
                     modrect.left = modrect.right - pModImage->GetWidth();
                     modrect.top = myrect.top;
                     modrect.bottom = myrect.top + 20;
                     pModImage->Blit(hDC, modrect);
                     
                     // update the hotspot locations
                     int modheight = pModImage->GetHeight();
                     ModToolRect = modrect;
                     ModToolRect.top = ModToolRect.bottom;
                     ModToolRect.bottom = ModToolRect.top + (modheight * 7);
                  }
               }
               else
               {
                  CDCSurface *pCatImage = NULL;
                  switch(m_category)
                  {
                  case PCT_NORMAL:        pCatImage = theApp.GetOnTopicImage(true);break;
                  case PCT_INF:           pCatImage = theApp.GetINFImage(true);break;
                  case PCT_NWS:           pCatImage = theApp.GetNWSImage(true);break;
                  case PCT_STUPID:        pCatImage = theApp.GetStupidImage(true);break;
                  case PCT_OFFTOPIC:      pCatImage = theApp.GetOffTopicImage(true);break;
                  case PCT_POLITICAL:     pCatImage = theApp.GetPoliticalImage(true);break;
                  case PCT_NUKED:         pCatImage = theApp.GetNukedImage(true);break;
                  }
                  if(pCatImage != NULL)
                  {
                     CHotSpot hotspot;
                     hotspot.m_bAnim = false;
                     hotspot.m_type = HST_MOD_TOOL;

                     hotspot.m_spot.right = myrect.right - 60;
                     hotspot.m_spot.left = hotspot.m_spot.right - pCatImage->GetWidth();
                     hotspot.m_spot.top = myrect.top;
                     hotspot.m_spot.bottom = myrect.top + 20;

                     hotspot.m_id = m_id;
                     hotspot.m_cat_type = m_category;
                     hotspots.push_back(hotspot);
                  }
               }
            }
            else
            {
               if(m_category == PCT_INF)
               {
                  CDCSurface *pINFIMage = theApp.GetINFImage(false);
                  if(pINFIMage != NULL)
                  {
                     RECT infrect;
                     infrect.right = myrect.right - 60;
                     infrect.left = infrect.right - pINFIMage->GetWidth();
                     infrect.top = myrect.top;
                     infrect.bottom = myrect.top + 20;
                     pINFIMage->Blit(hDC, infrect);
                  }
               }
               else if(m_category == PCT_NWS)
               {
                  CDCSurface *pNWSIMage = theApp.GetNWSImage(false);
                  if(pNWSIMage != NULL)
                  {
                     RECT nwsrect;
                     nwsrect.right = myrect.right - 60;
                     nwsrect.left = nwsrect.right - pNWSIMage->GetWidth();
                     nwsrect.top = myrect.top;
                     nwsrect.bottom = myrect.top + 20;
                     pNWSIMage->Blit(hDC, nwsrect);
                  }
               }
               else if(m_category == PCT_OFFTOPIC)
               {
                  CDCSurface *pOTIMage = theApp.GetOffTopicImage(false);
                  if(pOTIMage != NULL)
                  {
                     RECT otrect;
                     otrect.right = myrect.right - 60;
                     otrect.left = otrect.right - pOTIMage->GetWidth();
                     otrect.top = myrect.top;
                     otrect.bottom = myrect.top + 20;
                     pOTIMage->Blit(hDC, otrect);
                  }
               }
               else if(m_category == PCT_POLITICAL)
               {
                  CDCSurface *pPIMage = theApp.GetPoliticalImage(false);
                  if(pPIMage != NULL)
                  {
                     RECT prect;
                     prect.right = myrect.right - 60;
                     prect.left = prect.right - pPIMage->GetWidth();
                     prect.top = myrect.top;
                     prect.bottom = myrect.top + 20;
                     pPIMage->Blit(hDC, prect);
                  }
               }
               else if(m_category == PCT_STUPID)
               {
                  CDCSurface *pSIMage = theApp.GetStupidImage(false);
                  if(pSIMage != NULL)
                  {
                     RECT srect;
                     srect.right = myrect.right - 60;
                     srect.left = srect.right - pSIMage->GetWidth();
                     srect.top = myrect.top;
                     srect.bottom = myrect.top + 20;
                     pSIMage->Blit(hDC, srect);
                  }
               }
            }
            
            RECT authorrect;
            authorrect.left = textrect.left;
            authorrect.right = textrect.left + theApp.GetCellHeight() + 5 + m_authorsize + 5;
            authorrect.bottom = textrect.top;
            authorrect.top = authorrect.bottom - theApp.GetTextHeight();
            m_pDoc->DrawRootAuthor(hDC,authorrect,m_author, m_AuthorColor);

            int rightofauthor = authorrect.right;

            authorrect.left = textrect.left + theApp.GetCellHeight() + 5;
            CHotSpot hotspot;
            hotspot.m_bAnim = false;
            hotspot.m_type = HST_AUTHOR;
            hotspot.m_spot = authorrect;
            hotspot.m_id = m_id;
            hotspots.push_back(hotspot);

            if(m_lightningbolt)
            {
               CDCSurface *pLImage = theApp.GetLightningBoltImage(false);
               if(pLImage != NULL)
               {
                  //pLImage->Blit(hDC, lightningrect);
                  hotspot.m_type = HST_LIGHTNINGBOLT;
                  hotspot.m_spot.left = authorrect.right;
                  hotspot.m_spot.top = authorrect.top;
                  hotspot.m_spot.right = hotspot.m_spot.left + pLImage->GetWidth();
                  hotspot.m_spot.bottom = hotspot.m_spot.top + pLImage->GetHeight();
                  hotspot.m_id = 0;
                  hotspots.push_back(hotspot);

                  rightofauthor = hotspot.m_spot.right;
               }
            }

            bool bThread = (m_pDoc->GetDataType() == DDT_THREAD);

            RECT daterect;
            daterect.left = textrect.left;
            daterect.right = myrect.right;
            daterect.top = textrect.bottom;
            daterect.bottom = myrect.bottom;
            m_pDoc->DrawDate(hDC,daterect,m_datetext,m_ago_color);

            if(!bLinkOnly)
            {
               hotspot.m_type = HST_COLLAPSEPOST;
               hotspot.m_spot.left = myrect.left;
               hotspot.m_spot.right = myrect.left + 20;
               hotspot.m_spot.bottom = myrect.bottom;
               hotspot.m_spot.top = myrect.bottom - 20;
               hotspot.m_id = m_id;
               hotspots.push_back(hotspot);
            }

            if(!bLinkOnly)
            {
               hotspot.m_type = HST_REFRESH;
               if(m_bRefreshing)
               {
                  hotspot.m_bAnim = true;
               }
               hotspot.m_spot.left = myrect.left;
               hotspot.m_spot.top = myrect.top + 20;
               hotspot.m_spot.right = myrect.left + 20;
               hotspot.m_spot.bottom = myrect.top + 40;
               hotspot.m_id = m_id;
               hotspots.push_back(hotspot);
               hotspot.m_bAnim = false;
            }

            if(trunkatingposts > 0)
            {
               RECT hintrect = daterect;
               //hintrect.right = hintrect.left + theApp.GetTextHeight() * 9;
               m_pDoc->DrawRepliesHint(hDC,hintrect,(int)m_familysize);

               hotspot.m_type = HST_REPLIESTOROOTPOSTHINT;
               hotspot.m_spot = hintrect;
               hotspot.m_id = m_id;
               hotspots.push_back(hotspot);
            }

            if(!bThread)
            {
               if(theApp.IsPinningInStories() &&
                  m_pDoc->GetDataType() == DDT_STORY)
               {
                  hotspot.m_type = HST_PIN;
                  hotspot.m_bOn = m_bPinned;
               }
               else
               {
                  hotspot.m_type = HST_OPENINTAB;
               }
               hotspot.m_spot.left = myrect.left;
               hotspot.m_spot.top = myrect.top;
               hotspot.m_spot.right = myrect.left + 20;
               hotspot.m_spot.bottom = myrect.top + 20;
               hotspot.m_id = m_id;
               hotspots.push_back(hotspot);
            }

            if(!bLinkOnly)
            {
               hotspot.m_type = HST_CREATEREPLY;
               hotspot.m_spot.left = myrect.right - 60;
               hotspot.m_spot.bottom = myrect.top + 20;
               hotspot.m_spot.right = myrect.right;
               hotspot.m_spot.top = myrect.top;
               hotspot.m_id = m_id;
               hotspots.push_back(hotspot);

               if(theApp.ShowLOLButtons())
               {
                  RECT lolrect;
                  lolrect.left = rightofauthor + 10;
                  lolrect.right = lolrect.left + theApp.GetLOLFieldWidth();
                  if(theApp.ShowThomWLOLS())
                  {
                     lolrect.right = lolrect.left + m_lol_width + 4;
                  }
                  lolrect.bottom = authorrect.bottom;
                  if(theApp.ShowSmallLOL())
                  {
                     lolrect.bottom -= (theApp.GetCellHeight() / 3);
                  }
                  lolrect.top = authorrect.top;

                  hotspot.m_type = HST_LOLTAG;
                  hotspot.m_spot = lolrect;
                  hotspot.m_loltext = m_lol_text;
                  hotspot.m_lolvoted = (m_mylols & LTT_LOL)?true:false;
                  hotspot.m_lolroot = bAsRoot;
                  hotspot.m_haslols = m_lolflags.m_LOLd > 0?true:false;
                  if(!hotspot.m_lolvoted)hotspots.push_back(hotspot);
                  m_pDoc->DrawLOLField(hDC, LTT_LOL, lolrect, hotspot.m_loltext,false,hotspot.m_lolvoted, bAsRoot, hotspot.m_haslols);
                  
                  if(theApp.ShowThomWLOLS())
                  {
                     lolrect.left = lolrect.right;
                     lolrect.right = lolrect.left + m_inf_width + 4;
                  }
                  else
                  {
                     lolrect.left += theApp.GetLOLFieldWidth() + 5;
                     lolrect.right = lolrect.left + theApp.GetLOLFieldWidth();
                  }

                  hotspot.m_type = HST_INFTAG;
                  hotspot.m_spot = lolrect;
                  hotspot.m_loltext = m_inf_text;
                  hotspot.m_lolvoted = (m_mylols & LTT_INF)?true:false;
                  hotspot.m_lolroot = bAsRoot;
                  hotspot.m_haslols = m_lolflags.m_INFd > 0?true:false;
                  if(!hotspot.m_lolvoted)hotspots.push_back(hotspot);
                  m_pDoc->DrawLOLField(hDC, LTT_INF, lolrect, hotspot.m_loltext,false,hotspot.m_lolvoted, bAsRoot, hotspot.m_haslols);
                                                                                
                  if(theApp.ShowThomWLOLS())
                  {
                     lolrect.left = lolrect.right;
                     lolrect.right = lolrect.left + m_unf_width + 4;
                  }
                  else
                  {
                     lolrect.left += theApp.GetLOLFieldWidth() + 5;
                     lolrect.right = lolrect.left + theApp.GetLOLFieldWidth();
                  }

                  hotspot.m_type = HST_UNFTAG;
                  hotspot.m_spot = lolrect;
                  hotspot.m_loltext = m_unf_text;
                  hotspot.m_lolvoted = (m_mylols & LTT_UNF)?true:false;
                  hotspot.m_lolroot = bAsRoot;
                  hotspot.m_haslols = m_lolflags.m_UNFd > 0?true:false;
                  if(!hotspot.m_lolvoted)hotspots.push_back(hotspot);
                  m_pDoc->DrawLOLField(hDC, LTT_UNF, lolrect, hotspot.m_loltext,false,hotspot.m_lolvoted, bAsRoot, hotspot.m_haslols);
                                                                                
                  if(theApp.ShowThomWLOLS())
                  {
                     lolrect.left = lolrect.right;
                     lolrect.right = lolrect.left + m_tag_width + 4;
                  }
                  else
                  {
                     lolrect.left += theApp.GetLOLFieldWidth() + 5;
                     lolrect.right = lolrect.left + theApp.GetLOLFieldWidth();
                  }

                  hotspot.m_type = HST_TAGTAG;
                  hotspot.m_spot = lolrect;
                  hotspot.m_loltext = m_tag_text;
                  hotspot.m_lolvoted = (m_mylols & LTT_TAG)?true:false;
                  hotspot.m_lolroot = bAsRoot;
                  hotspot.m_haslols = m_lolflags.m_TAGd > 0?true:false;
                  if(!hotspot.m_lolvoted)hotspots.push_back(hotspot);
                  m_pDoc->DrawLOLField(hDC, LTT_TAG, lolrect, hotspot.m_loltext,false,hotspot.m_lolvoted, bAsRoot, hotspot.m_haslols);
                                                                                
                  if(theApp.ShowThomWLOLS())
                  {
                     lolrect.left = lolrect.right;
                     lolrect.right = lolrect.left + m_wtf_width + 4;
                  }
                  else
                  {
                     lolrect.left += theApp.GetLOLFieldWidth() + 5;
                     lolrect.right = lolrect.left + theApp.GetLOLFieldWidth();
                  }

                  hotspot.m_type = HST_WTFTAG;
                  hotspot.m_spot = lolrect;
                  hotspot.m_loltext = m_wtf_text;
                  hotspot.m_lolvoted = (m_mylols & LTT_WTF)?true:false;
                  hotspot.m_lolroot = bAsRoot;
                  hotspot.m_haslols = m_lolflags.m_WTFd > 0?true:false;
                  if(!hotspot.m_lolvoted)hotspots.push_back(hotspot);
                  m_pDoc->DrawLOLField(hDC, LTT_WTF, lolrect, hotspot.m_loltext,false,hotspot.m_lolvoted, bAsRoot, hotspot.m_haslols);

                  hotspot.m_loltext = L"";
               }
            }

            for(size_t s = 0; s < spoilers.size(); s++)
            {
               hotspot.m_type = HST_SPOILER;
               hotspot.m_spot = spoilers[s];
               hotspot.m_id = m_id;
               hotspots.push_back(hotspot);
            }

            for(size_t s = 0; s < links.size(); s++)
            {
               hotspot.m_type = HST_LINK;
               hotspot.m_spot = links[s];
               hotspot.m_id = m_id;
               hotspots.push_back(hotspot);
            }

            for(size_t s = 0; s < imagelinks.size(); s++)
            {
               hotspot.m_type = HST_IMAGE_LINK;
               hotspot.m_spot = imagelinks[s];
               hotspot.m_id = m_id;
               hotspots.push_back(hotspot);
            }

            for(size_t s = 0; s < images.size(); s++)
            {
               hotspot.m_type = HST_IMAGE;
               hotspot.m_spot = images[s];
               hotspot.m_id = m_id;
               hotspots.push_back(hotspot);
            }

            for(size_t s = 0; s < thumbs.size(); s++)
            {
               hotspot.m_type = HST_THUMB;
               hotspot.m_spot = thumbs[s];
               hotspot.m_id = m_id;
               hotspots.push_back(hotspot);
            }

            hotspot.m_type = HST_TEXT;
            hotspot.m_spot = textrect;
            hotspot.m_id = m_id;
            hotspots.push_back(hotspot);
         }

         m_height = pos - m_pos;

         // reserve space for reply dialog
         if(m_pReplyDlg != NULL)
         {
            RECT replydlgrect;
            replydlgrect.left = DeviceRectangle.left + 20;
            replydlgrect.right = DeviceRectangle.right - 20;
            replydlgrect.top = pos;
            replydlgrect.bottom = pos + m_pReplyDlg->GetHeight();
            m_pReplyDlg->SetDlgRect(replydlgrect);
            pos += m_pReplyDlg->GetHeight();

            if(replydlgrect.bottom < DeviceRectangle.top ||
               replydlgrect.top > DeviceRectangle.bottom)
            {
               // do nothing, we are not on the screen
            }
            else
            {
               RECT backrect = replydlgrect;
               backrect.left = DeviceRectangle.left;
               backrect.right = replydlgrect.left;
               m_pDoc->FillBackground(hDC,backrect);

               backrect.left = replydlgrect.right;
               backrect.right = DeviceRectangle.right;
               m_pDoc->FillBackground(hDC,backrect);
            }
         }

         // draw children
         int indent = 1;
         std::list<ChattyPost*>::iterator it = m_children.begin();
         std::list<ChattyPost*>::iterator end = m_children.end();
         while(it != end)
         {
            if((*it) != NULL)
            {
               UCString &rootauthor = blank_string;
               if(theApp.GetHighlightOP())
               {
                  rootauthor = m_author;
               }                  
               pos = (*it)->DrawReply(hDC, DeviceRectangle, pos, hotspots, indent, current_id, trunkatingposts, rootauthor, bAllowModTools, bModToolIsUp, ModToolRect, ModToolPostID);
            }
            it++;
         }
      }
   }   

   return pos;
}

int ChattyPost::DrawReply(HDC hDC, RECT &DeviceRectangle, int pos, std::vector<CHotSpot> &hotspots, int indent, unsigned int current_id, int &trunkatingposts, const UCString &rootauthor, bool bAllowModTools, bool bModToolIsUp, RECT &ModToolRect, unsigned int ModToolPostID)
{
   if(m_pDoc != NULL)
   {
      m_pos = pos;
      m_drewtextpos = pos + 20 + theApp.GetCellHeight();

      if(trunkatingposts > 0)
      {
         trunkatingposts--;
      }
      else
      {
         RECT replyrect;

         if(current_id == m_id)
         {
            // draw expanded
            RECT textrect = DeviceRectangle;
            textrect.left += 40 + (indent * 20);
            textrect.right -= 40;
            textrect.top = 0;
            textrect.bottom = 0;
            replyrect = textrect;
            
            if(textrect.right - textrect.left != m_lasttextrectwidth)
            {
               m_lasttextrectwidth = textrect.right - textrect.left;
               SetupBodyText(textrect);
               m_textrectheight = textrect.bottom;
            }
            else
            {
               textrect.bottom = m_textrectheight;
            }

            RECT myrect = DeviceRectangle;
            myrect.top = pos;
            myrect.bottom = pos + textrect.bottom + theApp.GetCellHeight() + theApp.GetCellHeight();

            if(myrect.bottom < DeviceRectangle.top ||
               myrect.top > DeviceRectangle.bottom)
            {
               // do nothing, we are not on the screen
               // but update the pos

               if(bAllowModTools && bModToolIsUp && m_id == ModToolPostID)
               {
                  // update the hotspot locations
                  ModToolRect.top = -1;
                  ModToolRect.bottom = -1;
               }

               pos = myrect.bottom;
               m_bDrewTextBody = false;
            }
            else
            {
               m_bDrewTextBody = true;
               m_pDoc->FillBackground(hDC,myrect);

               textrect.top += pos + theApp.GetCellHeight();
               textrect.bottom += pos + theApp.GetCellHeight();
               pos = myrect.bottom;

               myrect.left = textrect.left - 20;
               myrect.right -= 20;
               m_pDoc->FillExpandedBackground(hDC,myrect,false,m_category, false);

               std::vector<RECT> spoilers;
               std::vector<RECT> links;
               std::vector<RECT> imagelinks;
               std::vector<RECT> images;
               std::vector<RECT> thumbs;
               m_pDoc->DrawBodyText(hDC,textrect,m_lines_of_text,m_charsizes,m_linesizes,m_linetags,m_linetypes,spoilers,links,imagelinks,images,thumbs, m_bComplexShapeText,&textrect);
               m_drewtextpos = textrect.top;
               m_drewtextedge = textrect.left;

               if(bAllowModTools)
               {
                  if(bModToolIsUp && m_id == ModToolPostID)
                  {
                     CDCSurface *pModImage = NULL;
                     switch(m_category)
                     {
                     case PCT_NORMAL:        pModImage = theApp.GetOnTopicImage(true);break;
                     case PCT_INF:           pModImage = theApp.GetINFImage(true);break;
                     case PCT_NWS:           pModImage = theApp.GetNWSImage(true);break;
                     case PCT_STUPID:        pModImage = theApp.GetStupidImage(true);break;
                     case PCT_OFFTOPIC:      pModImage = theApp.GetOffTopicImage(true);break;
                     case PCT_POLITICAL:     pModImage = theApp.GetPoliticalImage(true);break;
                     case PCT_NUKED:         pModImage = theApp.GetNukedImage(true);break;
                     }
                     
                     if(pModImage != NULL)
                     {
                        RECT modrect;
                        modrect.right = myrect.right - 60;
                        modrect.left = modrect.right - pModImage->GetWidth();
                        modrect.top = myrect.top;
                        modrect.bottom = myrect.top + 20;
                        pModImage->Blit(hDC, modrect);
                        
                        // update the hotspot locations
                        int modheight = pModImage->GetHeight();
                        ModToolRect = modrect;
                        ModToolRect.top = ModToolRect.bottom;
                        ModToolRect.bottom = ModToolRect.top + (modheight * 7);
                     }
                  }
                  else
                  {
                     CDCSurface *pCatImage = NULL;
                     switch(m_category)
                     {
                     case PCT_NORMAL:        pCatImage = theApp.GetOnTopicImage(true);break;
                     case PCT_INF:           pCatImage = theApp.GetINFImage(true);break;
                     case PCT_NWS:           pCatImage = theApp.GetNWSImage(true);break;
                     case PCT_STUPID:        pCatImage = theApp.GetStupidImage(true);break;
                     case PCT_OFFTOPIC:      pCatImage = theApp.GetOffTopicImage(true);break;
                     case PCT_POLITICAL:     pCatImage = theApp.GetPoliticalImage(true);break;
                     case PCT_NUKED:         pCatImage = theApp.GetNukedImage(true);break;
                     }
                     if(pCatImage != NULL)
                     {
                        CHotSpot hotspot;
                        hotspot.m_bAnim = false;
                        hotspot.m_type = HST_MOD_TOOL;

                        hotspot.m_spot.right = myrect.right - 60;
                        hotspot.m_spot.left = hotspot.m_spot.right - pCatImage->GetWidth();
                        hotspot.m_spot.top = myrect.top;
                        hotspot.m_spot.bottom = myrect.top + 20;

                        hotspot.m_id = m_id;
                        hotspot.m_cat_type = m_category;
                        hotspots.push_back(hotspot);
                     }
                  }
               }
               else
               {
                  if(m_category == PCT_INF)
                  {
                     CDCSurface *pINFIMage = theApp.GetINFImage(false);
                     if(pINFIMage != NULL)
                     {
                        RECT infrect;
                        infrect.right = myrect.right - 60;
                        infrect.left = infrect.right - pINFIMage->GetWidth();
                        infrect.top = myrect.top;
                        infrect.bottom = myrect.top + 20;
                        pINFIMage->Blit(hDC, infrect);
                     }
                  }
                  else if(m_category == PCT_NWS)
                  {
                     CDCSurface *pNWSIMage = theApp.GetNWSImage(false);
                     if(pNWSIMage != NULL)
                     {
                        RECT nwsrect;
                        nwsrect.right = myrect.right - 60;
                        nwsrect.left = nwsrect.right - pNWSIMage->GetWidth();
                        nwsrect.top = myrect.top;
                        nwsrect.bottom = myrect.top + 20;
                        pNWSIMage->Blit(hDC, nwsrect);
                     }
                  }
                  else if(m_category == PCT_OFFTOPIC)
                  {
                     CDCSurface *pOTIMage = theApp.GetOffTopicImage(false);
                     if(pOTIMage != NULL)
                     {
                        RECT otrect;
                        otrect.right = myrect.right - 60;
                        otrect.left = otrect.right - pOTIMage->GetWidth();
                        otrect.top = myrect.top;
                        otrect.bottom = myrect.top + 20;
                        pOTIMage->Blit(hDC, otrect);
                     }
                  }
                  else if(m_category == PCT_POLITICAL)
                  {
                     CDCSurface *pPIMage = theApp.GetPoliticalImage(false);
                     if(pPIMage != NULL)
                     {
                        RECT prect;
                        prect.right = myrect.right - 60;
                        prect.left = prect.right - pPIMage->GetWidth();
                        prect.top = myrect.top;
                        prect.bottom = myrect.top + 20;
                        pPIMage->Blit(hDC, prect);
                     }
                  }
                  else if(m_category == PCT_STUPID)
                  {
                     CDCSurface *pSIMage = theApp.GetStupidImage(false);
                     if(pSIMage != NULL)
                     {
                        RECT srect;
                        srect.right = myrect.right - 60;
                        srect.left = srect.right - pSIMage->GetWidth();
                        srect.top = myrect.top;
                        srect.bottom = myrect.top + 20;
                        pSIMage->Blit(hDC, srect);
                     }
                  }
               }
               
               RECT authorrect;
               authorrect.left = textrect.left;
               authorrect.right = textrect.left + theApp.GetCellHeight() + 5 + m_authorsize + 5;
               authorrect.bottom = textrect.top;
               authorrect.top = authorrect.bottom - theApp.GetTextHeight();
               m_pDoc->DrawRootAuthor(hDC,authorrect,m_author, m_AuthorColor);

               int rightofauthor = authorrect.right;

               authorrect.left = textrect.left + theApp.GetCellHeight() + 5;
               CHotSpot hotspot;
               hotspot.m_bAnim = false;
               hotspot.m_type = HST_AUTHOR;
               hotspot.m_spot = authorrect;
               hotspot.m_id = m_id;
               hotspots.push_back(hotspot);

               if(m_lightningbolt)
               {
                  CDCSurface *pLImage = theApp.GetLightningBoltImage(false);
                  if(pLImage != NULL)
                  {
                     //pLImage->Blit(hDC, lightningrect);
                     hotspot.m_type = HST_LIGHTNINGBOLT;
                     hotspot.m_spot.left = authorrect.right;
                     hotspot.m_spot.top = authorrect.top;
                     hotspot.m_spot.right = hotspot.m_spot.left + pLImage->GetWidth();
                     hotspot.m_spot.bottom = hotspot.m_spot.top + pLImage->GetHeight();
                     hotspot.m_id = 0;
                     hotspots.push_back(hotspot);

                     rightofauthor = hotspot.m_spot.right;
                  }
               }

               RECT daterect;
               daterect.left = textrect.left;
               daterect.right = myrect.right;
               daterect.top = textrect.bottom;
               daterect.bottom = myrect.bottom;
               m_pDoc->DrawDate(hDC,daterect,m_datetext,m_ago_color);

               hotspot.m_type = HST_CLOSEREPLY;
               hotspot.m_spot.left = myrect.left;
               hotspot.m_spot.right = myrect.left + 20;
               hotspot.m_spot.bottom = myrect.bottom;
               hotspot.m_spot.top = myrect.bottom - 20;
               hotspot.m_id = m_id;
               hotspots.push_back(hotspot);

               hotspot.m_type = HST_REFRESH;
               if(m_bRefreshing)
               {
                  hotspot.m_bAnim = true;
               }
               hotspot.m_spot.left = myrect.left;
               hotspot.m_spot.top = myrect.top + 20;
               hotspot.m_spot.right = myrect.left + 20;
               hotspot.m_spot.bottom = myrect.top + 40;
               hotspot.m_id = m_id;
               hotspots.push_back(hotspot);
               hotspot.m_bAnim = false;

               if(m_pDoc->GetDataType() != DDT_THREAD &&
                  !theApp.IsPinningInStories())
               {
                  hotspot.m_type = HST_OPENINTAB;
                  hotspot.m_spot.left = myrect.left;
                  hotspot.m_spot.top = myrect.top;
                  hotspot.m_spot.right = myrect.left + 20;
                  hotspot.m_spot.bottom = myrect.top + 20;
                  hotspot.m_id = m_id;
                  hotspots.push_back(hotspot);
               }

               hotspot.m_type = HST_CREATEREPLY;
               hotspot.m_spot.left = myrect.right - 60;
               hotspot.m_spot.bottom = myrect.top + 20;
               hotspot.m_spot.right = myrect.right;
               hotspot.m_spot.top = myrect.top;
               hotspot.m_id = m_id;
               hotspots.push_back(hotspot);

               if(theApp.ShowLOLButtons())
               {
                  RECT lolrect;
                  lolrect.left = rightofauthor + 10;
                  lolrect.right = lolrect.left + theApp.GetLOLFieldWidth();
                  if(theApp.ShowThomWLOLS())
                  {
                     lolrect.right = lolrect.left + m_lol_width + 4;
                  }
                  lolrect.bottom = authorrect.bottom;
                  if(theApp.ShowSmallLOL())
                  {
                     lolrect.bottom -= (theApp.GetCellHeight() / 3);
                  }
                  lolrect.top = authorrect.top;
                                                                                
                  hotspot.m_type = HST_LOLTAG;
                  hotspot.m_spot = lolrect;
                  hotspot.m_loltext = m_lol_text;
                  hotspot.m_lolvoted = (m_mylols & LTT_LOL)?true:false;
                  hotspot.m_lolroot = false;
                  hotspot.m_haslols = m_lolflags.m_LOLd > 0?true:false;
                  if(!hotspot.m_lolvoted)hotspots.push_back(hotspot);
                  m_pDoc->DrawLOLField(hDC, LTT_LOL, lolrect, hotspot.m_loltext,false,hotspot.m_lolvoted, false, hotspot.m_haslols);
                                                                                
                  if(theApp.ShowThomWLOLS())
                  {
                     lolrect.left = lolrect.right;
                     lolrect.right = lolrect.left + m_inf_width + 4;
                  }
                  else
                  {
                     lolrect.left += theApp.GetLOLFieldWidth() + 5;
                     lolrect.right = lolrect.left + theApp.GetLOLFieldWidth();
                  }

                  hotspot.m_type = HST_INFTAG;
                  hotspot.m_spot = lolrect;
                  hotspot.m_loltext = m_inf_text;
                  hotspot.m_lolvoted = (m_mylols & LTT_INF)?true:false;
                  hotspot.m_lolroot = false;
                  hotspot.m_haslols = m_lolflags.m_INFd > 0?true:false;
                  if(!hotspot.m_lolvoted)hotspots.push_back(hotspot);
                  m_pDoc->DrawLOLField(hDC, LTT_INF, lolrect, hotspot.m_loltext,false,hotspot.m_lolvoted, false, hotspot.m_haslols);
                                                                                
                  if(theApp.ShowThomWLOLS())
                  {
                     lolrect.left = lolrect.right;
                     lolrect.right = lolrect.left + m_unf_width + 4;
                  }
                  else
                  {
                     lolrect.left += theApp.GetLOLFieldWidth() + 5;
                     lolrect.right = lolrect.left + theApp.GetLOLFieldWidth();
                  }

                  hotspot.m_type = HST_UNFTAG;
                  hotspot.m_spot = lolrect;
                  hotspot.m_loltext = m_unf_text;
                  hotspot.m_lolvoted = (m_mylols & LTT_UNF)?true:false;
                  hotspot.m_lolroot = false;
                  hotspot.m_haslols = m_lolflags.m_UNFd > 0?true:false;
                  if(!hotspot.m_lolvoted)hotspots.push_back(hotspot);
                  m_pDoc->DrawLOLField(hDC, LTT_UNF, lolrect, hotspot.m_loltext,false,hotspot.m_lolvoted, false, hotspot.m_haslols);
                                                                                
                  if(theApp.ShowThomWLOLS())
                  {
                     lolrect.left = lolrect.right;
                     lolrect.right = lolrect.left + m_tag_width + 4;
                  }
                  else
                  {
                     lolrect.left += theApp.GetLOLFieldWidth() + 5;
                     lolrect.right = lolrect.left + theApp.GetLOLFieldWidth();
                  }

                  hotspot.m_type = HST_TAGTAG;
                  hotspot.m_spot = lolrect;
                  hotspot.m_loltext = m_tag_text;
                  hotspot.m_lolvoted = (m_mylols & LTT_TAG)?true:false;
                  hotspot.m_lolroot = false;
                  hotspot.m_haslols = m_lolflags.m_TAGd > 0?true:false;
                  if(!hotspot.m_lolvoted)hotspots.push_back(hotspot);
                  m_pDoc->DrawLOLField(hDC, LTT_TAG, lolrect, hotspot.m_loltext,false,hotspot.m_lolvoted, false, hotspot.m_haslols);
                                                                                
                  if(theApp.ShowThomWLOLS())
                  {
                     lolrect.left = lolrect.right;
                     lolrect.right = lolrect.left + m_wtf_width + 4;
                  }
                  else
                  {
                     lolrect.left += theApp.GetLOLFieldWidth() + 5;
                     lolrect.right = lolrect.left + theApp.GetLOLFieldWidth();
                  }

                  hotspot.m_type = HST_WTFTAG;
                  hotspot.m_spot = lolrect;
                  hotspot.m_loltext = m_wtf_text;
                  hotspot.m_lolvoted = (m_mylols & LTT_WTF)?true:false;
                  hotspot.m_lolroot = false;
                  hotspot.m_haslols = m_lolflags.m_WTFd > 0?true:false;
                  if(!hotspot.m_lolvoted)hotspots.push_back(hotspot);
                  m_pDoc->DrawLOLField(hDC, LTT_WTF, lolrect, hotspot.m_loltext,false,hotspot.m_lolvoted, false, hotspot.m_haslols);

                  hotspot.m_loltext = L"";
               }

               for(size_t s = 0; s < spoilers.size(); s++)
               {
                  hotspot.m_type = HST_SPOILER;
                  hotspot.m_spot = spoilers[s];
                  hotspot.m_id = m_id;
                  hotspots.push_back(hotspot);
               }

               for(size_t s = 0; s < links.size(); s++)
               {
                  hotspot.m_type = HST_LINK;
                  hotspot.m_spot = links[s];
                  hotspot.m_id = m_id;
                  hotspots.push_back(hotspot);
               }

               for(size_t s = 0; s < imagelinks.size(); s++)
               {
                  hotspot.m_type = HST_IMAGE_LINK;
                  hotspot.m_spot = imagelinks[s];
                  hotspot.m_id = m_id;
                  hotspots.push_back(hotspot);
               }

               for(size_t s = 0; s < images.size(); s++)
               {
                  hotspot.m_type = HST_IMAGE;
                  hotspot.m_spot = images[s];
                  hotspot.m_id = m_id;
                  hotspots.push_back(hotspot);
               }

               for(size_t s = 0; s < thumbs.size(); s++)
               {
                  hotspot.m_type = HST_THUMB;
                  hotspot.m_spot = thumbs[s];
                  hotspot.m_id = m_id;
                  hotspots.push_back(hotspot);
               }

               hotspot.m_type = HST_TEXT;
               hotspot.m_spot = textrect;
               hotspot.m_id = m_id;
               hotspots.push_back(hotspot);

               // draw branches
               RECT branchrect;
               branchrect.top = myrect.top;
               branchrect.bottom = myrect.bottom;
               branchrect.left = myrect.left - 20;
               branchrect.right = myrect.left;

               if(m_pNextSibling == NULL)
               {
                  m_pDoc->DrawBranch(hDC, branchrect, IT_TO_ME_END, m_previewshade, GetNewness());
               }
               else
               {
                  m_pDoc->DrawBranch(hDC, branchrect, IT_TO_ME_FORK, m_previewshade, GetNewness());
               }

               ChattyPost *parent = m_pParent;
               
               while(parent != NULL &&
                     parent->m_pParent != NULL)
               {
                  branchrect.left -= 20;
                  branchrect.right -= 20;

                  if(parent->m_pNextSibling == NULL)
                  {
                     // gap
                  }
                  else
                  {
                     m_pDoc->DrawBranch(hDC, branchrect, IT_VERT, m_previewshade, parent->m_pNextSibling->GetNewness());
                  }
                  
                  parent = parent->m_pParent;
               }
            }
         }
         else
         {
            m_bDrewTextBody = false;
            // draw as preview

            int author_info_size = 20;
            author_info_size += m_authorsize;
            if(theApp.ShowLOLButtons() && m_bHaveLOLPreview)
            {
               author_info_size += m_lol_preview_size;
            }

            if(m_lightningbolt)
            {
               CDCSurface *pLImage = theApp.GetLightningBoltImage(false);
               if(pLImage != NULL)
               {
                  author_info_size += pLImage->GetWidth();
               }
            }

            RECT myrect = DeviceRectangle;
            myrect.top = pos;
            
            myrect.bottom = pos + theApp.GetTextHeight();

            pos = myrect.bottom;
            
            if(myrect.bottom < DeviceRectangle.top ||
               myrect.top > DeviceRectangle.bottom)
            {
               // do nothing, we are not on the screen
            }
            else
            {
               m_pDoc->FillBackground(hDC,myrect);

               myrect.left += 20 + (indent * 20);

               m_last_left = myrect.left;
                             
            
               myrect.right -= 20;
               myrect.right -= m_authorsize;
               myrect.right -= (abs(theApp.GetCellHeight()) + 5);// "... : "
               if(theApp.ShowLOLButtons() && m_bHaveLOLPreview)
               {
                  myrect.right -= m_lol_preview_size;
               }

               if(m_lightningbolt)
               {
                  CDCSurface *pLImage = theApp.GetLightningBoltImage(false);
                  if(pLImage != NULL)
                  {
                     myrect.right -= pLImage->GetWidth();
                  }
               }

               RECT textrect = myrect;

               if(m_category == PCT_INF)
               {
                  CDCSurface *pINFIMage = theApp.GetINFStar();
                  if(pINFIMage != NULL)
                  {
                     RECT infrect = textrect;
                     infrect.right = textrect.left + 16;
                     pINFIMage->Blit(hDC, infrect);
                  }
                  textrect.left += 16;
               }
               else if(m_category == PCT_NWS)
               {
                  CDCSurface *pNWSIMage = theApp.GetNWSStar();
                  if(pNWSIMage != NULL)
                  {
                     RECT nwsrect = textrect;
                     nwsrect.right = textrect.left + 16;
                     pNWSIMage->Blit(hDC, nwsrect);
                  }
                  textrect.left += 16;
               }
               else if(m_category == PCT_OFFTOPIC)
               {
                  CDCSurface *pOTIMage = theApp.GetOffTopicStar();
                  if(pOTIMage != NULL)
                  {
                     RECT otrect = textrect;
                     otrect.right = textrect.left + 16;
                     pOTIMage->Blit(hDC, otrect);
                  }
                  textrect.left += 16;
               }
               else if(m_category == PCT_POLITICAL)
               {
                  CDCSurface *pPIMage = theApp.GetPoliticalStar();
                  if(pPIMage != NULL)
                  {
                     RECT prect = textrect;
                     prect.right = textrect.left + 16;
                     pPIMage->Blit(hDC, prect);
                  }
                  textrect.left += 16;
               }
               else if(m_category == PCT_STUPID)
               {
                  CDCSurface *pSIMage = theApp.GetStupidStar();
                  if(pSIMage != NULL)
                  {
                     RECT srect = textrect;
                     srect.right = textrect.left + 16;
                     pSIMage->Blit(hDC, srect);
                  }
                  textrect.left += 16;
               }
               
               bool clipped = false;

               if(m_previewshade == 10)
               {
                  RECT boldrect = textrect;
                  boldrect.left++;
                  boldrect.right++;
                  m_pDoc->DrawPreviewText(hDC,boldrect,m_bodytext,m_pCharWidths,m_shacktags,0,clipped, m_bComplexShapeText);
                  clipped = false;
               }
               m_pDoc->DrawPreviewText(hDC,textrect,m_bodytext,m_pCharWidths,m_shacktags,m_previewshade,clipped, m_bComplexShapeText);

               m_drewtextpos = myrect.top;
               m_drewtextedge = myrect.left;

               CHotSpot hotspot;
               hotspot.m_bAnim = false;
               hotspot.m_type = HST_REPLYPREVIEW;
               hotspot.m_spot = textrect;
               hotspot.m_id = m_id;
               hotspots.push_back(hotspot);

               RECT authorrect = textrect;
               authorrect.left = textrect.right;
               if(clipped)
               {
                  authorrect.right = authorrect.left + abs(theApp.GetCellHeight()) + 5 + m_authorpreviewsize + 5;
               }
               else
               {
                  authorrect.right = authorrect.left + abs(theApp.GetCellHeight() / 3) + 5 + m_authorpreviewsize + 5;
               }
               m_pDoc->DrawPreviewAuthor(hDC, authorrect, m_author, clipped, m_previewshade, m_AuthorColor, rootauthor);

               if(clipped)
               {
                  authorrect.left += abs(theApp.GetCellHeight());
               }
               else
               {
                  authorrect.left += abs(theApp.GetCellHeight() / 3);
               }
               hotspot.m_type = HST_AUTHORPREVIEW;
               hotspot.m_spot = authorrect;
               hotspot.m_id = m_id;
               hotspots.push_back(hotspot);

               int rightofauthor = authorrect.right;

               if(m_lightningbolt)
               {
                  CDCSurface *pLImage = theApp.GetLightningBoltImage(false);
                  if(pLImage != NULL)
                  {
                     //pLImage->Blit(hDC, lightningrect);
                     hotspot.m_type = HST_LIGHTNINGBOLT;
                     hotspot.m_spot.left = authorrect.right;
                     hotspot.m_spot.top = authorrect.top;
                     hotspot.m_spot.right = hotspot.m_spot.left + pLImage->GetWidth();
                     hotspot.m_spot.bottom = hotspot.m_spot.top + pLImage->GetHeight();
                     hotspot.m_id = 0;
                     hotspots.push_back(hotspot);

                     rightofauthor = hotspot.m_spot.right;
                  }
               }

               //
               if(theApp.ShowLOLButtons() && m_bHaveLOLPreview)
               {
                  RECT lolpreviewrect = authorrect;
                  lolpreviewrect.left = rightofauthor;
                  lolpreviewrect.right = lolpreviewrect.left + m_lol_preview_size;

                  m_pDoc->DrawPreviewText(hDC,lolpreviewrect,m_lol_preview_text,m_plol_preview_charwidths,m_lol_preview_shacktags,10,clipped,false);
               }

               // draw branches
               RECT branchrect;
               branchrect.top = myrect.top;
               branchrect.bottom = myrect.bottom;
               branchrect.left = myrect.left - 20;
               branchrect.right = myrect.left;

               if(m_pNextSibling == NULL)
               {
                  m_pDoc->DrawBranch(hDC, branchrect, IT_TO_ME_END, m_previewshade, GetNewness());
               }
               else
               {
                  m_pDoc->DrawBranch(hDC, branchrect, IT_TO_ME_FORK, m_previewshade, GetNewness());
               }

               ChattyPost *parent = m_pParent;
               
               while(parent != NULL &&
                     parent->m_pParent != NULL)
               {
                  branchrect.left -= 20;
                  branchrect.right -= 20;

                  if(parent->m_pNextSibling == NULL)
                  {
                     // gap
                  }
                  else
                  {
                     m_pDoc->DrawBranch(hDC, branchrect, IT_VERT, m_previewshade, parent->m_pNextSibling->GetNewness());
                  }
                  
                  parent = parent->m_pParent;
               }
            }
         }

         m_height = pos - m_pos;

         // reserve space for reply dialog
         if(m_pReplyDlg != NULL)
         {
            RECT replydlgrect;
            replydlgrect.left = DeviceRectangle.left + 20 + (indent * 20);
            replydlgrect.right = DeviceRectangle.right - 20;
            replydlgrect.top = pos;
            replydlgrect.bottom = pos + m_pReplyDlg->GetHeight();
            m_pReplyDlg->SetDlgRect(replydlgrect);
            pos += m_pReplyDlg->GetHeight();

            if(replydlgrect.bottom < DeviceRectangle.top ||
               replydlgrect.top > DeviceRectangle.bottom)
            {
               // do nothing, we are not on the screen
            }
            else
            {
               RECT backrect = replydlgrect;
               backrect.left = DeviceRectangle.left;
               backrect.right = replydlgrect.left;
               m_pDoc->FillBackground(hDC,backrect);

               backrect.left = replydlgrect.right;
               backrect.right = DeviceRectangle.right;
               m_pDoc->FillBackground(hDC,backrect);

               // draw branches
               RECT branchrect;
               branchrect.top = replydlgrect.top;
               branchrect.bottom = replydlgrect.bottom;
               branchrect.left = replydlgrect.left - 20;
               branchrect.right = replydlgrect.left;

               if(m_pNextSibling == NULL)
               {
                  // gap
               }
               else
               {
                  m_pDoc->DrawBranch(hDC, branchrect, IT_VERT, m_previewshade, GetNewness());
               }

               ChattyPost *parent = m_pParent;
               
               while(parent != NULL &&
                     parent->m_pParent != NULL)
               {
                  branchrect.left -= 20;
                  branchrect.right -= 20;

                  if(parent->m_pNextSibling == NULL)
                  {
                     // gap
                  }
                  else
                  {
                     m_pDoc->DrawBranch(hDC, branchrect, IT_VERT, m_previewshade, parent->m_pNextSibling->GetNewness());
                  }
                  
                  parent = parent->m_pParent;
               }
            }
         }
      }

      // draw children
      indent++;
      std::list<ChattyPost*>::iterator it = m_children.begin();
      std::list<ChattyPost*>::iterator end = m_children.end();
      while(it != end)
      {
         if((*it) != NULL)
         {
            pos = (*it)->DrawReply(hDC, DeviceRectangle, pos, hotspots, indent, current_id, trunkatingposts, rootauthor, bAllowModTools, bModToolIsUp, ModToolRect, ModToolPostID);
         }
         it++;
      }
   }

   return pos;
}

int ChattyPost::GetReplyPreviewHeight(RECT &DeviceRectangle)
{
   if(m_pDoc != NULL)
   {
      int author_info_size = 20;
      author_info_size += m_authorsize;
      if(theApp.ShowLOLButtons() && m_bHaveLOLPreview)
      {
         author_info_size += m_lol_preview_size;
      }

      if(m_lightningbolt)
      {
         CDCSurface *pLImage = theApp.GetLightningBoltImage(false);
         if(pLImage != NULL)
         {
            author_info_size += pLImage->GetWidth();
         }
      }

      RECT myrect = DeviceRectangle;
      myrect.top = 0;
      myrect.bottom = 10000;
      myrect.left = m_last_left;
      myrect.right -= author_info_size;
                  
      RECT textrect = myrect;
      textrect.left -= 5;
      textrect.top = 0;
      textrect.bottom = 0;
      
      if(textrect.right - textrect.left != m_lasttextrectwidth)
      {
         m_lasttextrectwidth = textrect.right - textrect.left;
         SetupBodyText(textrect);
         m_textrectheight = textrect.bottom - 8;
         //m_textrectheight = theApp.GetTextHeight() * m_lines_of_text.size();
      }
   }

   return m_textrectheight;
}

void ChattyPost::DrawReplyPreview(HDC hDC, RECT &DeviceRectangle, int top, int bottom, const UCString &rootauthor)
{
   if(m_pDoc != NULL)
   {
      // draw as an expanded preview
      ::SetBkMode(hDC,TRANSPARENT);

      int author_info_size = 20;
      author_info_size += m_authorsize;
      if(theApp.ShowLOLButtons() && m_bHaveLOLPreview)
      {
         author_info_size += m_lol_preview_size;
      }

      if(m_lightningbolt)
      {
         CDCSurface *pLImage = theApp.GetLightningBoltImage(false);
         if(pLImage != NULL)
         {
            author_info_size += pLImage->GetWidth();
         }
      }

      RECT myrect = DeviceRectangle;
      myrect.top = top;
      myrect.bottom = bottom;
      myrect.left = m_last_left;

      m_pDoc->FillBackground(hDC,myrect);

      myrect.right = myrect.left + m_largest_line_width;

      if(m_textrectheight > theApp.GetTextHeight())
      {
         m_pDoc->StrokeShapedRect(hDC, myrect, theApp.GetTextHeight() / 2);
      }

      m_pDoc->FillExpandedBackground(hDC, myrect, true, m_category, false);

      RECT textrect = myrect;
      textrect.left -= 5;
      textrect.right = textrect.left + m_largest_line_width;
      textrect.top = myrect.top - 4;
      textrect.bottom = myrect.bottom;
      
      std::vector<RECT> spoilers;
      std::vector<RECT> links;
      std::vector<RECT> imagelinks;
      std::vector<RECT> images;
      std::vector<RECT> thumbs;


      HRGN hCR = ::CreateRectRgn(myrect.left,myrect.top,myrect.right,myrect.bottom);
      ::ExtSelectClipRgn(hDC, hCR, RGN_COPY);

      m_pDoc->DrawBodyText(hDC,textrect,m_lines_of_text,m_charsizes,m_linesizes,m_linetags,m_linetypes,spoilers,links,imagelinks,images,thumbs, m_bComplexShapeText,&textrect);

      ::ExtSelectClipRgn(hDC, NULL, RGN_COPY);

      myrect.right -= 5;

      RECT authorrect;
      
      authorrect.left = myrect.right;
      authorrect.right = authorrect.left + author_info_size;
      authorrect.bottom = myrect.bottom;
      authorrect.top = myrect.bottom - theApp.GetTextHeight();

      RECT authornamerect = authorrect;

      authornamerect.right = authornamerect.left + abs(theApp.GetCellHeight() / 3) + 5 + m_authorpreviewsize + 5;
      
      m_pDoc->DrawPreviewAuthor(hDC, authornamerect, m_author, false, m_previewshade, m_AuthorColor, rootauthor);

      int rightofauthor = authornamerect.right;

      if(m_lightningbolt)
      {
         CDCSurface *pLImage = theApp.GetLightningBoltImage(false);
         if(pLImage != NULL)
         {
            RECT lightningrect = authorrect;
            
            lightningrect.left = authornamerect.right;
            lightningrect.top = authornamerect.top;
            lightningrect.right = lightningrect.left + pLImage->GetWidth();
            lightningrect.bottom = lightningrect.top + pLImage->GetHeight();
            pLImage->Blit(hDC, lightningrect);
            rightofauthor = lightningrect.right;
         }
      }

      //
      if(theApp.ShowLOLButtons() && m_bHaveLOLPreview)
      {
         RECT lolpreviewrect = authornamerect;
         lolpreviewrect.left = rightofauthor;
         lolpreviewrect.right = lolpreviewrect.left + m_lol_preview_size;
         bool clipped = false;
         m_pDoc->DrawPreviewText(hDC,lolpreviewrect,m_lol_preview_text,m_plol_preview_charwidths,m_lol_preview_shacktags,10,clipped,false);
      }
   }
}


ChattyPost::~ChattyPost()
{
   if(m_pCharWidths != NULL)
   {
      free(m_pCharWidths);
      m_pCharWidths = NULL;
   }

   if(m_pAuthorCharWidths != NULL)
   {
      free(m_pAuthorCharWidths);
      m_pAuthorCharWidths = NULL;
   }

   if(m_pSubjectCharWidths != NULL)
   {
      free(m_pSubjectCharWidths);
      m_pSubjectCharWidths = NULL;
   }

   if(m_plol_preview_charwidths != NULL)
   {
      free(m_plol_preview_charwidths);
      m_plol_preview_charwidths = NULL;
   }
   
   std::list<ChattyPost*>::iterator it = m_children.begin();
   std::list<ChattyPost*>::iterator end = m_children.end();
   while(it != end)
   {
      if((*it) != NULL)
      {
         delete (*it);
         (*it) = NULL;
      }
      it++;
   }
   m_children.clear();
}

void ChattyPost::ClearChildren()
{
   std::list<ChattyPost*>::iterator it = m_children.begin();
   std::list<ChattyPost*>::iterator end = m_children.end();
   while(it != end)
   {
      if((*it) != NULL)
      {
         delete (*it);
         (*it) = NULL;
      }
      it++;
   }
   m_children.clear();
}

void ChattyPost::SetupCharWidths()
{
   if(m_pCharWidths != NULL)
   {
      free(m_pCharWidths);
      m_pCharWidths = NULL;
   }

   if(m_pAuthorCharWidths != NULL)
   {
      free(m_pAuthorCharWidths);
      m_pAuthorCharWidths = NULL;
   }

   if(m_pSubjectCharWidths != NULL)
   {
      free(m_pSubjectCharWidths);
      m_pSubjectCharWidths = NULL;
   }

   if(m_plol_preview_charwidths != NULL)
   {
      free(m_plol_preview_charwidths);
      m_plol_preview_charwidths = NULL;
   }

   m_bComplexShapeText = false;
      
   if(!m_bodytext.IsEmpty() && 
      m_pDoc != NULL)
   {
      int numchars = m_bodytext.Length();
      m_pCharWidths = (int*)malloc(sizeof(int) * numchars);

      // go through and massage based on shacktags
      // also, give newlines a width so that they look ok in previews
      std::vector<shacktagpos> fonttags;

      for(int i = 0; i < (int)m_shacktags.size(); i++)
      {
         switch(m_shacktags[i].m_tag)
         {
         case ST_ITALIC:
         case ST_ITALIC_END:
         case ST_BOLD:
         case ST_BOLD_END:
         case ST_CODE: 
         case ST_CODE_END:
         case ST_SAMPLE: 
         case ST_SAMPLE_END:
         case ST_QUOTE: 
         case ST_QUOTE_END:
            fonttags.push_back(m_shacktags[i]);
         }
      }
      
      bool italic = false;
      bool bold = false;
      bool code = false;
      bool quote = false;
      bool sample = false;
      
      int lastpos = 0;
      for(int i = 0; i < (int)fonttags.size(); i++)
      {
         int thispos = __min(fonttags[i].m_pos,numchars);
         if(thispos > lastpos)
         {
            const UCChar *fontname = theApp.GetNormalFontName();
            if(quote) fontname = theApp.GetQuotedFontName();
            else if(code) fontname = theApp.GetCodeFontName();

            GetCharWidths(m_bodytext.Str() + lastpos, m_pCharWidths + lastpos, thispos - lastpos, italic, bold, sample, fontname, &m_bComplexShapeText);
            lastpos = thispos;
         }

         switch(fonttags[i].m_tag)
         {
         case ST_ITALIC:      italic = true; break;
         case ST_ITALIC_END:  italic = false; break;
         case ST_BOLD:        bold = true; break;
         case ST_BOLD_END:    bold = false; break;
         case ST_CODE:        code = true; break;
         case ST_CODE_END:    code = false; break;
         case ST_SAMPLE:      sample = true; break;
         case ST_SAMPLE_END:  sample = false; break;
         case ST_QUOTE:       quote = true; break;
         case ST_QUOTE_END:   quote = false; break;
         }
      }

      if(numchars > lastpos)
      {
         const UCChar *fontname = theApp.GetNormalFontName();
         if(quote) fontname = theApp.GetQuotedFontName();
         else if(code) fontname = theApp.GetCodeFontName();
         GetCharWidths(m_bodytext.Str() + lastpos, m_pCharWidths + lastpos, numchars - lastpos, italic, bold, sample, fontname, &m_bComplexShapeText);
      }
      
      for(int i = 0; i < numchars; i++)
      {
         if(m_bodytext[i] == L'\n')
         {
            m_pCharWidths[i] = 4;
         }

         /*
         if(m_pCharWidths[i] == 0)
         {
            if(m_bodytext[i] > 255)
            {
               if(sample)
               {
                  m_pCharWidths[i] = abs(theApp.GetSampleFontHeight());
               }
               else
               {
                  m_pCharWidths[i] = abs(theApp.GetFontHeight());
               }
            }
            else
            {
               if(iswprint(m_bodytext[i]) == 0)
               {
                  if(sample)
                  {
                     m_pCharWidths[i] = abs(theApp.GetSampleFontHeight()) / 2;
                  }
                  else
                  {
                     m_pCharWidths[i] = abs(theApp.GetFontHeight()) / 2;
                  }
               }
               else if(iswalnum(m_bodytext[i]) == 0)
               {
                  // is printable, but isn't alphanumeric
                  // must be punctuation
                  // give it some width
                  if(sample)
                  {
                     m_pCharWidths[i] = abs(theApp.GetSampleFontHeight()) / 2;
                  }
                  else
                  {
                     m_pCharWidths[i] = abs(theApp.GetFontHeight()) / 2;
                  }
               }
            }
         }
         */
      }
   }

   m_pAuthorCharWidths = (int*)malloc(sizeof(int) * m_author.Length());

   GetCharWidths(m_author, m_pAuthorCharWidths, m_author.Length(), false, false, false, theApp.GetNormalFontName());
   m_authorpreviewsize = 0;
   for(size_t i = 0; i < (size_t)m_author.Length(); i++)
   {
      m_authorpreviewsize += m_pAuthorCharWidths[i];
   }

   GetCharWidths(m_author, m_pAuthorCharWidths, m_author.Length(), false, true, false, theApp.GetNormalFontName());
   m_authorsize = 0;
   for(size_t i = 0; i < (size_t)m_author.Length(); i++)
   {
      m_authorsize += m_pAuthorCharWidths[i];
   }

   if(!m_subject.IsEmpty())
   {
      m_pSubjectCharWidths = (int*)malloc(sizeof(int) * m_subject.Length());

      GetCharWidths(m_subject, m_pSubjectCharWidths, m_subject.Length(), false, false, false, theApp.GetNormalFontName());
   }

   UpdateLOLs();
}

bool PopTag(std::vector<shacktag> &shacktags, shacktag who)
{         
   bool badscope = false;
   if(shacktags.size() > 0)
   {
      if(shacktags[shacktags.size()-1] == who)
      {
         shacktags.pop_back();
      }
      else
      {
         badscope = true;
         std::vector<shacktag>::iterator begin = shacktags.begin();
         std::vector<shacktag>::iterator it = shacktags.end();
         it--;
         while(1)
         {
            if((*it) == who)
            {
               shacktags.erase(it);
               break;
            }

            if(it == begin)
            {
               break;
            }
            else
            {
               it--;
            }
         }               
      }
   }
   else
   {
      badscope = true;
   }

   return badscope;
}


void MergeTags(std::vector<shacktagpos> &ina, std::vector<shacktagpos> &inb, std::vector<shacktagpos> &out)
{
   if(ina.size() == 0)
   {
      out = inb;
   }
   else if(inb.size() == 0)
   {
      out = ina;
   }
   else
   {
      int a = 0;
      int a_end = ina.size();
      int b = 0;
      int b_end = inb.size();

      while(a < a_end && b < b_end)
      {
         if(ina[a].m_pos <= inb[b].m_pos)
         {
            out.push_back(ina[a]);
            a++;
            if(ina[a].m_tag == ina[a-1].m_tag + 30)
            {
               out.push_back(ina[a]);
               if(ina[a].m_pos > inb[b].m_pos)
               {
                  inb[b].m_pos = ina[a].m_pos;
               }
               a++;
            }
         }
         else 
         {
            out.push_back(inb[b]);
            b++;
            if(inb[b].m_tag == inb[b-1].m_tag + 30)
            {
               out.push_back(inb[b]);
               if(inb[b].m_pos > ina[a].m_pos)
               {
                  ina[a].m_pos = inb[b].m_pos;
               }
               b++;
            }
         }
      }

      while(a < a_end)
      {
         out.push_back(ina[a]);
         a++;
      }

      while(b < b_end)
      {
         out.push_back(inb[b]);
         b++;
      }
   }
}

void FindBadShackTagsString(UCString &from, std::vector<shacktagpos> &shacktags)
{
   shacktags.clear();

   std::vector<shacktag> tagstack;

   const UCChar *readstart = from;
   const UCChar *read = readstart;
   const UCChar *readend = read + from.Length();

   while(read < readend)
   {
      if(_wcsnicmp(read,L"r{",2) == 0 ||
         _wcsnicmp(read,L"r[",2) == 0)
      {
         tagstack.push_back(ST_RED);
         read+=2;
      }
      else if(_wcsnicmp(read,L"g{",2) == 0 ||
              _wcsnicmp(read,L"g[",2) == 0)
      {
         tagstack.push_back(ST_GREEN);
         read+=2;
      }
      else if(_wcsnicmp(read,L"b{",2) == 0 ||
              _wcsnicmp(read,L"d[",2) == 0)
      {
         tagstack.push_back(ST_BLUE);
         read+=2;
      }
      else if(_wcsnicmp(read,L"y{",2) == 0 ||
              _wcsnicmp(read,L"y[",2) == 0)
      {
         tagstack.push_back(ST_YELLOW);
         read+=2;
      }
      else if(_wcsnicmp(read,L"e[",2) == 0)
      {
         tagstack.push_back(ST_OLIVE);
         read+=2;
      }
      else if(_wcsnicmp(read,L"l[",2) == 0 ||
              _wcsnicmp(read,L"f[",2) == 0)
      {
         tagstack.push_back(ST_LIME);
         read+=2;
      }
      else if(_wcsnicmp(read,L"n[",2) == 0)
      {
         tagstack.push_back(ST_ORANGE);
         read+=2;
      }
      else if(_wcsnicmp(read,L"p[",2) == 0)
      {
         tagstack.push_back(ST_PINK);
         read+=2;
      }
      else if(_wcsnicmp(read,L"q[",2) == 0)
      {
         tagstack.push_back(ST_QUOTE);
         read+=2;
      }
      else if(_wcsnicmp(read,L"s[",2) == 0)
      {
         tagstack.push_back(ST_SAMPLE);
         read+=2;
      }
      else if(_wcsnicmp(read,L"-[",2) == 0)
      {
         tagstack.push_back(ST_STRIKE);
         read+=2;
      }
      else if(_wcsnicmp(read,L"/[",2) == 0 ||
              _wcsnicmp(read,L"i[",2) == 0)
      {
         tagstack.push_back(ST_ITALIC);
         read+=2;
      }
      else if(_wcsnicmp(read,L"]/",2) == 0 ||
              _wcsnicmp(read,L"]i",2) == 0)
      {
         if(PopTag(tagstack, ST_ITALIC))
         {
            shacktags.push_back(shacktagpos(ST_PURPLE,read - readstart));
            shacktags.push_back(shacktagpos(ST_PURPLE_END,read - readstart + 2));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"b[",2) == 0 ||
              _wcsnicmp(read,L"*[",2) == 0)
      {
         tagstack.push_back(ST_BOLD);
         read+=2;
      }
      else if(_wcsnicmp(read,L"]b", 2) == 0 ||
              _wcsnicmp(read,L"]*", 2) == 0)
      {
         if(PopTag(tagstack, ST_BOLD))
         {
            shacktags.push_back(shacktagpos(ST_PURPLE,read - readstart));
            shacktags.push_back(shacktagpos(ST_PURPLE_END,read - readstart + 2));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"}r",2) == 0 ||
              _wcsnicmp(read,L"]r",2) == 0)
      {
         if(PopTag(tagstack, ST_RED))
         {
            shacktags.push_back(shacktagpos(ST_PURPLE,read - readstart));
            shacktags.push_back(shacktagpos(ST_PURPLE_END,read - readstart + 2));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"}g",2) == 0 ||
              _wcsnicmp(read,L"]g",2) == 0)
      {
         if(PopTag(tagstack, ST_GREEN))
         {
            shacktags.push_back(shacktagpos(ST_PURPLE,read - readstart));
            shacktags.push_back(shacktagpos(ST_PURPLE_END,read - readstart + 2));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"}b",2) == 0 ||
              _wcsnicmp(read,L"]d",2) == 0)
      {
         if(PopTag(tagstack, ST_BLUE))
         {
            shacktags.push_back(shacktagpos(ST_PURPLE,read - readstart));
            shacktags.push_back(shacktagpos(ST_PURPLE_END,read - readstart + 2));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"}y",2) == 0 ||
              _wcsnicmp(read,L"]y",2) == 0)
      {
         if(PopTag(tagstack, ST_YELLOW))
         {
            shacktags.push_back(shacktagpos(ST_PURPLE,read - readstart));
            shacktags.push_back(shacktagpos(ST_PURPLE_END,read - readstart + 2));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"]e",2) == 0)
      {
         if(PopTag(tagstack, ST_OLIVE))
         {
            shacktags.push_back(shacktagpos(ST_PURPLE,read - readstart));
            shacktags.push_back(shacktagpos(ST_PURPLE_END,read - readstart + 2));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"]l",2) == 0 ||
              _wcsnicmp(read,L"]f",2) == 0)
      {
         if(PopTag(tagstack, ST_LIME))
         {
            shacktags.push_back(shacktagpos(ST_PURPLE,read - readstart));
            shacktags.push_back(shacktagpos(ST_PURPLE_END,read - readstart + 2));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"]n",2) == 0)
      {
         if(PopTag(tagstack, ST_ORANGE))
         {
            shacktags.push_back(shacktagpos(ST_PURPLE,read - readstart));
            shacktags.push_back(shacktagpos(ST_PURPLE_END,read - readstart + 2));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"]p",2) == 0)
      {
         if(PopTag(tagstack, ST_PINK))
         {
            shacktags.push_back(shacktagpos(ST_PURPLE,read - readstart));
            shacktags.push_back(shacktagpos(ST_PURPLE_END,read - readstart + 2));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"]q",2) == 0)
      {
         if(PopTag(tagstack, ST_QUOTE))
         {
            shacktags.push_back(shacktagpos(ST_PURPLE,read - readstart));
            shacktags.push_back(shacktagpos(ST_PURPLE_END,read - readstart + 2));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"]s",2) == 0)
      {
         if(PopTag(tagstack, ST_SAMPLE))
         {
            shacktags.push_back(shacktagpos(ST_PURPLE,read - readstart));
            shacktags.push_back(shacktagpos(ST_PURPLE_END,read - readstart + 2));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"]-",2) == 0)
      {
         if(PopTag(tagstack, ST_STRIKE))
         {
            shacktags.push_back(shacktagpos(ST_PURPLE,read - readstart));
            shacktags.push_back(shacktagpos(ST_PURPLE_END,read - readstart + 2));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"]o",2) == 0)
      {
         if(PopTag(tagstack, ST_SPOILER))
         {
            shacktags.push_back(shacktagpos(ST_PURPLE,read - readstart));
            shacktags.push_back(shacktagpos(ST_PURPLE_END,read - readstart + 2));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"_[",2) == 0 ||
              _wcsnicmp(read,L"u[",2) == 0)
      {
         tagstack.push_back(ST_UNDERLINE);
         read+=2;
      }
      else if(_wcsnicmp(read,L"]_",2) == 0 ||
              _wcsnicmp(read,L"]u",2) == 0)
      {
         if(PopTag(tagstack, ST_UNDERLINE))
         {
            shacktags.push_back(shacktagpos(ST_PURPLE,read - readstart));
            shacktags.push_back(shacktagpos(ST_PURPLE_END,read - readstart + 2));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"o[",2) == 0)
      {
         tagstack.push_back(ST_SPOILER);
         read+=2;
      }
      else if(_wcsnicmp(read,L"/{{",3) == 0)
      {
         tagstack.push_back(ST_CODE);
         read+=3;
      }
      else if(_wcsnicmp(read,L"}}/",3) == 0)
      {
         if(PopTag(tagstack, ST_CODE))
         {
            shacktags.push_back(shacktagpos(ST_PURPLE,read - readstart));
            shacktags.push_back(shacktagpos(ST_PURPLE_END,read - readstart + 3));
         }
         read+=3;
      }
      else
      {
         read++;
      }
   }
}


void ChattyPost::DecodeShackTagsString(UCString &from, bool bAllowCustomTags/* = false*/)
{
   m_bodytext = L"";
   m_shacktags.clear();
   m_lasttextrectwidth = 0;
   from.Replace(L"\t",L"");// ?
   
   int red = 0;
   int green = 0;
   int blue = 0;
   int yellow = 0;
   int olive = 0;
   int lime = 0;
   int orange = 0;
   int pink = 0;
   int purple = 0;
   int quote = 0;
   int sample = 0;
   int strike = 0;
   int italic = 0;
   int bold = 0;
   int underline = 0;
   int spoiler = 0;
   int code = 0;
   int link = 0;

   std::vector<shacktag> tagstack;

   const UCChar *readstart = from;
   const UCChar *read = readstart;
   const UCChar *readend = read + from.Length();

   const UCChar *linkstart = NULL;

   // skip leading whitespace
   bool bAtStartOfALine = true;

   while(read < readend)
   {
      if(bAtStartOfALine && code <= 0)
      {
         while(read < readend &&
               iswspace(*read))
         {
            read++;
         }
      }

      if(_wcsnicmp(read,L"r{",2) == 0 ||
         _wcsnicmp(read,L"r[",2) == 0)
      {
         red++;
         tagstack.push_back(ST_RED);
         if(red == 1)
         {
            m_shacktags.push_back(shacktagpos(ST_RED,m_bodytext.Length()));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"g{",2) == 0 ||
              _wcsnicmp(read,L"g[",2) == 0)
      {
         green++;
         tagstack.push_back(ST_GREEN);
         if(green == 1)
         {
            m_shacktags.push_back(shacktagpos(ST_GREEN,m_bodytext.Length()));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"b{",2) == 0 ||
              _wcsnicmp(read,L"d[",2) == 0)
      {
         blue++;
         tagstack.push_back(ST_BLUE);
         if(blue == 1)
         {
            m_shacktags.push_back(shacktagpos(ST_BLUE,m_bodytext.Length()));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"y{",2) == 0 ||
              _wcsnicmp(read,L"y[",2) == 0)
      {
         yellow++;
         tagstack.push_back(ST_YELLOW);
         if(yellow == 1)
         {
            m_shacktags.push_back(shacktagpos(ST_YELLOW,m_bodytext.Length()));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"e[",2) == 0)
      {
         olive++;
         tagstack.push_back(ST_OLIVE);
         if(olive == 1)
         {
            m_shacktags.push_back(shacktagpos(ST_OLIVE,m_bodytext.Length()));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"l[",2) == 0 ||
              _wcsnicmp(read,L"f[",2) == 0)
      {
         lime++;
         tagstack.push_back(ST_LIME);
         if(lime == 1)
         {
            m_shacktags.push_back(shacktagpos(ST_LIME,m_bodytext.Length()));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"n[",2) == 0)
      {
         orange++;
         tagstack.push_back(ST_ORANGE);
         if(orange == 1)
         {
            m_shacktags.push_back(shacktagpos(ST_ORANGE,m_bodytext.Length()));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"p[",2) == 0)
      {
         pink++;
         tagstack.push_back(ST_PINK);
         if(pink == 1)
         {
            m_shacktags.push_back(shacktagpos(ST_PINK,m_bodytext.Length()));
         }
         read+=2;
      }
      else if(bAllowCustomTags, _wcsnicmp(read,L"p{",2) == 0)
      {
         purple++;
         tagstack.push_back(ST_PURPLE);
         if(purple == 1)
         {
            m_shacktags.push_back(shacktagpos(ST_PURPLE,m_bodytext.Length()));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"q[",2) == 0)
      {
         quote++;
         tagstack.push_back(ST_QUOTE);
         if(quote == 1)
         {
            m_shacktags.push_back(shacktagpos(ST_QUOTE,m_bodytext.Length()));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"s[",2) == 0)
      {
         sample++;
         tagstack.push_back(ST_SAMPLE);
         if(sample == 1)
         {
            m_shacktags.push_back(shacktagpos(ST_SAMPLE,m_bodytext.Length()));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"-[",2) == 0)
      {
         strike++;
         tagstack.push_back(ST_STRIKE);
         if(strike == 1)
         {
            m_shacktags.push_back(shacktagpos(ST_STRIKE,m_bodytext.Length()));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"/[",2) == 0 ||
              _wcsnicmp(read,L"i[",2) == 0)
      {
         italic++;
         tagstack.push_back(ST_ITALIC);
         if(italic == 1)
         {
            m_shacktags.push_back(shacktagpos(ST_ITALIC,m_bodytext.Length()));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"]/",2) == 0 ||
              _wcsnicmp(read,L"]i",2) == 0)
      {
         italic--;
         if(PopTag(tagstack, ST_ITALIC))
         {
            m_bodytext.AppendUnicodeString(read, 2);
         }
         else if(italic == 0)
         {
            m_shacktags.push_back(shacktagpos(ST_ITALIC_END,m_bodytext.Length()));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"b[",2) == 0 ||
              _wcsnicmp(read,L"*[",2) == 0)
      {
         bold++;
         tagstack.push_back(ST_BOLD);
         if(bold == 1)
         {
            m_shacktags.push_back(shacktagpos(ST_BOLD,m_bodytext.Length()));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"]b", 2) == 0 ||
              _wcsnicmp(read,L"]*", 2) == 0)
      {
         bold--;
         if(PopTag(tagstack, ST_BOLD))
         {
            m_bodytext.AppendUnicodeString(read, 2);
         }
         else if(bold == 0)
         {
            m_shacktags.push_back(shacktagpos(ST_BOLD_END,m_bodytext.Length()));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"}r",2) == 0 ||
              _wcsnicmp(read,L"]r",2) == 0)
      {
         red--;
         if(PopTag(tagstack, ST_RED))
         {
            m_bodytext.AppendUnicodeString(read, 2);
         }
         else if(red == 0)
         {
            m_shacktags.push_back(shacktagpos(ST_RED_END,m_bodytext.Length()));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"}g",2) == 0 ||
              _wcsnicmp(read,L"]g",2) == 0)
      {
         green--;
         if(PopTag(tagstack, ST_GREEN))
         {
            m_bodytext.AppendUnicodeString(read, 2);
         }
         else if(green == 0)
         {
            m_shacktags.push_back(shacktagpos(ST_GREEN_END,m_bodytext.Length()));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"}b",2) == 0 ||
              _wcsnicmp(read,L"]d",2) == 0)
      {
         blue--;
         if(PopTag(tagstack, ST_BLUE))
         {
            m_bodytext.AppendUnicodeString(read, 2);
         }
         else if(blue == 0)
         {
            m_shacktags.push_back(shacktagpos(ST_BLUE_END,m_bodytext.Length()));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"}y",2) == 0 ||
              _wcsnicmp(read,L"]y",2) == 0)
      {
         yellow--;
         if(PopTag(tagstack, ST_YELLOW))
         {
            m_bodytext.AppendUnicodeString(read, 2);
         }
         else if(yellow == 0)
         {
            m_shacktags.push_back(shacktagpos(ST_YELLOW_END,m_bodytext.Length()));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"]e",2) == 0)
      {
         olive--;
         if(PopTag(tagstack, ST_OLIVE))
         {
            m_bodytext.AppendUnicodeString(read, 2);
         }
         else if(olive == 0)
         {
            m_shacktags.push_back(shacktagpos(ST_OLIVE_END,m_bodytext.Length()));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"]l",2) == 0 ||
              _wcsnicmp(read,L"]f",2) == 0)
      {
         lime--;
         if(PopTag(tagstack, ST_LIME))
         {
            m_bodytext.AppendUnicodeString(read, 2);
         }
         else if(lime == 0)
         {
            m_shacktags.push_back(shacktagpos(ST_LIME_END,m_bodytext.Length()));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"]n",2) == 0)
      {
         orange--;
         if(PopTag(tagstack, ST_ORANGE))
         {
            m_bodytext.AppendUnicodeString(read, 2);
         }
         else if(orange == 0)
         {
            m_shacktags.push_back(shacktagpos(ST_ORANGE_END,m_bodytext.Length()));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"]p",2) == 0)
      {
         pink--;
         if(PopTag(tagstack, ST_PINK))
         {
            m_bodytext.AppendUnicodeString(read, 2);
         }
         else if(pink == 0)
         {
            m_shacktags.push_back(shacktagpos(ST_PINK_END,m_bodytext.Length()));
         }
         read+=2;
      }
      else if(bAllowCustomTags, _wcsnicmp(read,L"}p",2) == 0)
      {
         purple--;
         if(PopTag(tagstack, ST_PURPLE))
         {
            m_bodytext.AppendUnicodeString(read, 2);
         }
         else if(purple == 0)
         {
            m_shacktags.push_back(shacktagpos(ST_PURPLE_END,m_bodytext.Length()));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"]q",2) == 0)
      {
         quote--;
         if(PopTag(tagstack, ST_QUOTE))
         {
            m_bodytext.AppendUnicodeString(read, 2);
         }
         else if(quote == 0)
         {
            m_shacktags.push_back(shacktagpos(ST_QUOTE_END,m_bodytext.Length()));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"]s",2) == 0)
      {
         sample--;
         if(PopTag(tagstack, ST_SAMPLE))
         {
            m_bodytext.AppendUnicodeString(read, 2);
         }
         else if(sample == 0)
         {
            m_shacktags.push_back(shacktagpos(ST_SAMPLE_END,m_bodytext.Length()));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"]-",2) == 0)
      {
         strike--;
         if(PopTag(tagstack, ST_STRIKE))
         {
            m_bodytext.AppendUnicodeString(read, 2);
         }
         else if(strike == 0)
         {
            m_shacktags.push_back(shacktagpos(ST_STRIKE_END,m_bodytext.Length()));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"]o",2) == 0)
      {
         spoiler--;
         if(PopTag(tagstack, ST_SPOILER))
         {
            m_bodytext.AppendUnicodeString(read, 2);
         }
         else if(spoiler == 0)
         {
            m_shacktags.push_back(shacktagpos(ST_SPOILER_END,m_bodytext.Length()));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"_[",2) == 0 ||
              _wcsnicmp(read,L"u[",2) == 0)
      {
         underline++;
         tagstack.push_back(ST_UNDERLINE);
         if(underline == 1)
         {
            m_shacktags.push_back(shacktagpos(ST_UNDERLINE,m_bodytext.Length()));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"]_",2) == 0 ||
              _wcsnicmp(read,L"]u",2) == 0)
      {
         underline--;
         if(PopTag(tagstack, ST_UNDERLINE))
         {
            m_bodytext.AppendUnicodeString(read, 2);
         }
         else if(underline == 0)
         {
            m_shacktags.push_back(shacktagpos(ST_UNDERLINE_END,m_bodytext.Length()));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"o[",2) == 0)
      {
         spoiler++;
         tagstack.push_back(ST_SPOILER);
         if(spoiler == 1)
         {
            m_shacktags.push_back(shacktagpos(ST_SPOILER,m_bodytext.Length()));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"l{",2) == 0)
      {
         link++;
         tagstack.push_back(ST_LINK);
         if(link == 1)
         {
            m_shacktags.push_back(shacktagpos(ST_LINK,m_bodytext.Length()));
            linkstart = read + 2;
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"}l", 2) == 0)
      {
         link--;
         if(PopTag(tagstack, ST_LINK))
         {
            m_bodytext.AppendUnicodeString(read, 2);
         }
         else if(link == 0)
         {
            UCString linktext;
            if(m_shacktags[m_shacktags.size() - 1].m_tag == ST_LINK)
            {
               linktext.AppendUnicodeString(linkstart, read - linkstart);
               m_shacktags[m_shacktags.size() - 1].m_href = linktext;
            }
            m_shacktags.push_back(shacktagpos(ST_LINK_END,m_bodytext.Length()));
         }
         read+=2;
      }
      else if(_wcsnicmp(read,L"/{{",3) == 0)
      {
         code++;
         tagstack.push_back(ST_CODE);
         if(code == 1)
         {
            if(!bAtStartOfALine)
            {
               while(m_bodytext.Length() > 0 &&
                  m_bodytext[m_bodytext.Length()-1] == L' ')
               {
                  m_bodytext.TrimEnd(1);
               }
               m_bodytext += L'\n';
               bAtStartOfALine = true;
            }
            m_shacktags.push_back(shacktagpos(ST_CODE,m_bodytext.Length()));
         }
         read+=3;
      }
      else if(_wcsnicmp(read,L"}}/",3) == 0)
      {
         code--;
         if(PopTag(tagstack, ST_CODE))
         {
            m_bodytext.AppendUnicodeString(read, 3);
         }
         else if(code == 0)
         {
            while(m_bodytext.Length() > 0 &&
               m_bodytext[m_bodytext.Length()-1] == L' ')
            {
               m_bodytext.TrimEnd(1);
            }
            m_shacktags.push_back(shacktagpos(ST_CODE_END,m_bodytext.Length()));
            m_bodytext += L'\n';
            bAtStartOfALine = true;
         }
         read+=3;
      }
      else
      {
         m_bodytext += *read;
         if(*read == L' ' && code <= 0)
         {
            // eat space series's's's
            while(*read == L' ') read++;
         }
         else
         {
            read++;
         }
         bAtStartOfALine = false;
      }
   }

   // trim trailing whitespace
   while(m_bodytext.Length() > 0 &&
         iswspace(m_bodytext[m_bodytext.Length()-1]))
   {
      m_bodytext.TrimEnd(1);
   }

   SetupCharWidths();
   m_lines_of_text.clear();
   m_charsizes.clear();
   m_linesizes.clear();
   m_linetags.clear();
   m_linetypes.clear();
}

void ChattyPost::RemoveSomeTags(UCString &str)
{
   str.Replace(L"&lt;",L"<",false);
   str.Replace(L"&gt;",L">",false);
   str.Replace(L"&apos;",L"\'",false);
   str.Replace(L"&quot;",L"\"",false);
   str.Replace(L"&amp;",L"&",false);
   str.Replace(L"&#10;",L"\n",false);
}

void ChattyPost::DecodeString(UCString &from, UCString &to, std::vector<shacktagpos> &shacktags)
{
   from.Replace(L"\t",L"");// ?
   from.Replace(L"\r",L"");// ?
   from.Replace(L"\n",L"");// ?
   from.Replace(L"&#13;",L"");

   UCChar bom[2];
   bom[0] = 0xFEFF;
   bom[1] = 0;
   from.Replace(bom,L"");
   bom[0] = 0xFFFE;
   bom[1] = 0;
   from.Replace(bom,L"");

   RemoveSomeTags(from);
   
   int red = 0;
   int green = 0;
   int blue = 0;
   int yellow = 0;
   int olive = 0;
   int lime = 0;
   int orange = 0;
   int purple = 0;
   int pink = 0;
   int quote = 0;
   int sample = 0;
   int strike = 0;
   int italic = 0;
   int bold = 0;
   int underline = 0;
   int spoiler = 0;
   int code = 0;
   int link = 0;

   std::vector<shacktag> tagstack;

   const UCChar *read = from;
   const UCChar *readend = read + from.Length();

   // skip leading whitespace
   

   bool bAtStartOfALine = true;

   while(read < readend)
   {
      if(bAtStartOfALine && code <= 0)
      {
         while(read < readend &&
               iswspace(*read))
         {
            read++;
         }
      }

      if(_wcsnicmp(read,L"<span class=\"jt_red\">",21) == 0)
      {
         red++;
         tagstack.push_back(ST_RED);
         if(red == 1)
         {
            shacktags.push_back(shacktagpos(ST_RED,to.Length()));
         }
         read+=21;
      }
      else if(_wcsnicmp(read,L"<span class=\"jt_green\">",23) == 0)
      {
         green++;
         tagstack.push_back(ST_GREEN);
         if(green == 1)
         {
            shacktags.push_back(shacktagpos(ST_GREEN,to.Length()));
         }
         read+=23;
      }
      else if(_wcsnicmp(read,L"<span class=\"jt_fuchsia\">",25) == 0)
      {
         lime++;
         tagstack.push_back(ST_LIME);
         if(lime == 1)
         {
            shacktags.push_back(shacktagpos(ST_LIME,to.Length()));
         }
         read+=25;
      }
      else if(_wcsnicmp(read,L"<span class=\"jt_blue\">",22) == 0)
      {
         blue++;
         tagstack.push_back(ST_BLUE);
         if(blue == 1)
         {
            shacktags.push_back(shacktagpos(ST_BLUE,to.Length()));
         }
         read+=22;
      }
      else if(_wcsnicmp(read,L"<span class=\"jt_yellow\">",24) == 0)
      {
         yellow++;
         tagstack.push_back(ST_YELLOW);
         if(yellow == 1)
         {
            shacktags.push_back(shacktagpos(ST_YELLOW,to.Length()));
         }
         read+=24;
      }
      else if(_wcsnicmp(read,L"<span class=\"jt_olive\">",23) == 0)
      {
         olive++;
         tagstack.push_back(ST_OLIVE);
         if(olive == 1)
         {
            shacktags.push_back(shacktagpos(ST_OLIVE,to.Length()));
         }
         read+=23;
      }
      else if(_wcsnicmp(read,L"<span class=\"jt_lime\">",22) == 0)
      {
         lime++;
         tagstack.push_back(ST_LIME);
         if(lime == 1)
         {
            shacktags.push_back(shacktagpos(ST_LIME,to.Length()));
         }
         read+=22;
      }
      else if(_wcsnicmp(read,L"<span class=\"jt_orange\">",24) == 0)
      {
         orange++;
         tagstack.push_back(ST_ORANGE);
         if(orange == 1)
         {
            shacktags.push_back(shacktagpos(ST_ORANGE,to.Length()));
         }
         read+=24;
      }
      else if(_wcsnicmp(read,L"<span class=\"jt_purple\">",24) == 0)
      {
         purple++;
         tagstack.push_back(ST_PURPLE);
         if(purple == 1)
         {
            shacktags.push_back(shacktagpos(ST_PURPLE,to.Length()));
         }
         read+=24;
      }
      else if(_wcsnicmp(read,L"<span class=\"jt_pink\">",22) == 0)
      {
         pink++;
         tagstack.push_back(ST_PINK);
         if(pink == 1)
         {
            shacktags.push_back(shacktagpos(ST_PINK,to.Length()));
         }
         read+=22;
      }
      else if(_wcsnicmp(read,L"<span class=\"jt_quote\">",23) == 0)
      {
         quote++;
         tagstack.push_back(ST_QUOTE);
         if(quote == 1)
         {
            shacktags.push_back(shacktagpos(ST_QUOTE,to.Length()));
         }
         read+=23;
      }
      else if(_wcsnicmp(read,L"<span class=\"jt_sample\">",24) == 0)
      {
         sample++;
         tagstack.push_back(ST_SAMPLE);
         if(sample == 1)
         {
            shacktags.push_back(shacktagpos(ST_SAMPLE,to.Length()));
         }
         read+=24;
      }
      else if(_wcsnicmp(read,L"<span class=\"jt_strike\">",24) == 0)
      {
         strike++;
         tagstack.push_back(ST_STRIKE);
         if(strike == 1)
         {
            shacktags.push_back(shacktagpos(ST_STRIKE,to.Length()));
         }
         read+=24;
      }
      else if(_wcsnicmp(read,L"<i>",3) == 0)
      {
         italic++;
         tagstack.push_back(ST_ITALIC);
         if(italic == 1)
         {
            shacktags.push_back(shacktagpos(ST_ITALIC,to.Length()));
         }
         read+=3;
      }
      else if(_wcsnicmp(read,L"</i>",4) == 0)
      {
         italic--;
         PopTag(tagstack, ST_ITALIC);
         if(italic == 0)
         {
            shacktags.push_back(shacktagpos(ST_ITALIC_END,to.Length()));
         }
         read+=4;
      }
      else if(_wcsnicmp(read,L"<b>",3) == 0)
      {
         bold++;
         tagstack.push_back(ST_BOLD);
         if(bold == 1)
         {
            shacktags.push_back(shacktagpos(ST_BOLD,to.Length()));
         }
         read+=3;
      }
      else if(_wcsnicmp(read,L"</b>", 4) == 0)
      {
         bold--;
         PopTag(tagstack, ST_BOLD);
         if(bold == 0)
         {
            shacktags.push_back(shacktagpos(ST_BOLD_END,to.Length()));
         }
         read+=4;
      }
      else if(_wcsnicmp(read,L"</span>",7) == 0)
      {
         if(tagstack.size() > 0)
         {
            shacktag who = tagstack[tagstack.size()-1];
            tagstack.pop_back();
            
            switch(who)
            {
            case ST_RED:red--;if(red==0)shacktags.push_back(shacktagpos(ST_RED_END,to.Length()));break;
            case ST_GREEN:green--;if(green==0)shacktags.push_back(shacktagpos(ST_GREEN_END,to.Length()));break;
            case ST_BLUE:blue--;if(blue==0)shacktags.push_back(shacktagpos(ST_BLUE_END,to.Length()));break;
            case ST_YELLOW:yellow--;if(yellow==0)shacktags.push_back(shacktagpos(ST_YELLOW_END,to.Length()));break;
            case ST_OLIVE:olive--;if(olive==0)shacktags.push_back(shacktagpos(ST_OLIVE_END,to.Length()));break;
            case ST_LIME:lime--;if(lime==0)shacktags.push_back(shacktagpos(ST_LIME_END,to.Length()));break;
            case ST_ORANGE:orange--;if(orange==0)shacktags.push_back(shacktagpos(ST_ORANGE_END,to.Length()));break;
            case ST_PURPLE:purple--;if(purple==0)shacktags.push_back(shacktagpos(ST_PURPLE_END,to.Length()));break;
            case ST_PINK:pink--;if(pink==0)shacktags.push_back(shacktagpos(ST_PINK_END,to.Length()));break;
            case ST_QUOTE:quote--;if(quote==0)shacktags.push_back(shacktagpos(ST_QUOTE_END,to.Length()));break;
            case ST_SAMPLE:sample--;if(sample==0)shacktags.push_back(shacktagpos(ST_SAMPLE_END,to.Length()));break;
            case ST_STRIKE:strike--;if(strike==0)shacktags.push_back(shacktagpos(ST_STRIKE_END,to.Length()));break;
            case ST_SPOILER:spoiler--;if(spoiler==0)shacktags.push_back(shacktagpos(ST_SPOILER_END,to.Length()));break;
            }
         }

         read+=7;
      }
      else if(_wcsnicmp(read,L"\r",1) == 0)
      {
         // throw these away
         read+=1;
      }
      else if(_wcsnicmp(read,L"<u>",3) == 0)
      {
         underline++;
         tagstack.push_back(ST_UNDERLINE);
         if(underline == 1)
         {
            shacktags.push_back(shacktagpos(ST_UNDERLINE,to.Length()));
         }
         read+=3;
      }
      else if(_wcsnicmp(read,L"</u>",4) == 0)
      {
         underline--;
         PopTag(tagstack, ST_UNDERLINE);
         if(underline == 0)
         {
            shacktags.push_back(shacktagpos(ST_UNDERLINE_END,to.Length()));
         }
         read+=4;
      }
      else if(_wcsnicmp(read,L"<span class=\"jt_spoiler\"",24) == 0)
      {
         spoiler++;
         tagstack.push_back(ST_SPOILER);
         if(spoiler == 1)
         {
            shacktags.push_back(shacktagpos(ST_SPOILER,to.Length()));
         }
         read+=24;
         while(*read != L'>')read++;
         read++;
      }
      else if(_wcsnicmp(read,L"<pre class=\"jt_code\">",21) == 0)
      {
         code++;
         tagstack.push_back(ST_CODE);
         if(code == 1)
         {
            if(!bAtStartOfALine)
            {
               while(to.Length() > 0 &&
                  to[to.Length()-1] == L' ')
               {
                  to.TrimEnd(1);
               }
               to += L'\n';
               bAtStartOfALine = true;
            }
            shacktags.push_back(shacktagpos(ST_CODE,to.Length()));
         }
         read+=21;
      }
      else if(_wcsnicmp(read,L"</pre>",6) == 0)
      {
         code--;
         PopTag(tagstack, ST_CODE);
         if(code == 0)
         {
            while(to.Length() > 0 &&
               to[to.Length()-1] == L' ')
            {
               to.TrimEnd(1);
            }
            shacktags.push_back(shacktagpos(ST_CODE_END,to.Length()));
            to += L'\n';
            bAtStartOfALine = true;
         }
         read+=6;
      }
      else if(_wcsnicmp(read,L"<br/>",5) == 0)
      {
         while(to.Length() > 0 &&
            to[to.Length()-1] == L' ')
         {
            to.TrimEnd(1);
         }
         to += L'\n';
         read+=5;
         // skip leading whitespace of next line
         bAtStartOfALine = true;
      }
      else if(_wcsnicmp(read,L"<br />",6) == 0)
      {
         while(to.Length() > 0 &&
            to[to.Length()-1] == L' ')
         {
            to.TrimEnd(1);
         }
         to += L'\n';
         read+=6;
         // skip leading whitespace of next line
         bAtStartOfALine = true;
      }
      else if(_wcsnicmp(read,L"<a",2) == 0)
      {
         UCString href;
         // found start, now find end
         read += 2;
         
         // find end
         while(read < readend)
         {
            // see if this has an href
            if(_wcsnicmp(read,L"href=\"",6) == 0)
            {
               read+=6;
               // save the reference
               while(*read != L'\"')
               {
                  href += *read;
                  read++;
               }
            }

            if(*read == L'>')
            {
               read++;
               break;
            }
            else
            {
               read++;
            }
         }
         link++;
         tagstack.push_back(ST_LINK);
         if(link == 1)
         {
            if(!href.IsEmpty() &&
               href[0] == L'/')
            {
               UCString temp = L"http://www.shacknews.com";
               temp += href;
               href = temp;
            }

            shacktags.push_back(shacktagpos(ST_LINK,to.Length(),href));
         }
      }
      else if(_wcsnicmp(read,L"</a>",4) == 0)
      {
         read+=4;
         link--;
         PopTag(tagstack, ST_LINK);
         if(link == 0)
         {
            shacktags.push_back(shacktagpos(ST_LINK_END,to.Length()));
         }
      }
      else if(_wcsnicmp(read,L"<span class=",12) == 0)
      {
         // unknown element
         read+=12;
         while(read < readend)
         {
            if(*read == L'>')
            {
               read++;
               break;
            }
            read++;
         }
      }
      else
      {
         to += *read;
         if(*read == L' ' && code <= 0)
         {
            // eat space series's's's
            while(*read == L' ') read++;
         }
         else
         {
            read++;
         }
         bAtStartOfALine = false;
      }
   }

   to.Replace(L"<i/>",L"");
   to.Replace(L"<b/>",L"");
   to.Replace(L"<u/>",L"");

   // trim trailing whitespace
   while(to.Length() > 0 &&
         iswspace(to[to.Length()-1]))
   {
      to.TrimEnd(1);
   }
}

void ChattyPost::SetupPreviewShades(bool bDoingNewFlags)
{
   std::list<unsigned int> ids;
   GatherIds(ids);
   if(ids.size() > 0)
   {
      ids.sort();

      std::list<unsigned int>::iterator begin = ids.begin();
      std::list<unsigned int>::iterator it = ids.end();
      it--;   

      int shade = 10;
      while(1)
      {
         ChattyPost *pChild = FindChild((*it));
         if(pChild != NULL)
         {
            if(0/*bDoingNewFlags*/)
            {
               shade = 10;
               if(pChild->IsNew())
               {
                  pChild->SetPreviewShade(10);
               }
               else
               {
                  pChild->SetPreviewShade(0);
               }
            }
            else
            {
               pChild->SetPreviewShade(shade);
            }
         }
         shade--;

         if(shade <= 0) shade = 0;

         if(it == begin)
         {
            break;
         }

         it--;
      }
   }
}

void ChattyPost::CountFamilySize()
{
   m_familysize = 0;
   std::list<ChattyPost*>::iterator it = m_children.begin();
   std::list<ChattyPost*>::iterator end = m_children.end();

   while(it != end)
   {
      (*it)->AddToFamilySize(m_familysize);
      it++;
   }
}

void ChattyPost::AddToFamilySize(size_t &familysize)
{
   familysize++;
   std::list<ChattyPost*>::iterator it = m_children.begin();
   std::list<ChattyPost*>::iterator end = m_children.end();

   while(it != end)
   {
      (*it)->AddToFamilySize(familysize);
      it++;
   }
}

void ChattyPost::GatherIds(std::list<unsigned int> &ids)
{
   std::list<ChattyPost*>::iterator it = m_children.begin();
   std::list<ChattyPost*>::iterator end = m_children.end();

   while(it != end)
   {
      ids.push_back((*it)->GetId());
      (*it)->GatherIds(ids);
      it++;
   }
}

void ChattyPost::ReadFromKnown(CLampDoc *pDoc)
{
   m_pDoc = pDoc;

   if(IsPreview())
   {
      ChattyPost *knownpost = theApp.GetKnownPost(m_id);
      if(knownpost != NULL)
      {
         m_id = knownpost->m_id;
         m_category = knownpost->m_category;
         m_tm_posttime = knownpost->m_tm_posttime;
         m_author = knownpost->m_author;
         m_bodytext = knownpost->m_bodytext;
         m_shacktags = knownpost->m_shacktags;
         m_lightningbolt = knownpost->m_lightningbolt;

         if(pDoc->GetDataType() == DDT_STORY)
         {
            m_bCollapsed = theApp.GetMyCollapse(m_id);
         }

         m_mylols = theApp.GetMyLol(m_id);
         UpdateLOLs();
         UpdateDate();
         UpdateAuthorColor();

         SetupCharWidths();
         m_lines_of_text.clear();
         m_charsizes.clear();
         m_linesizes.clear();
         m_linetags.clear();
         m_linetypes.clear();
         m_lasttextrectwidth = 0;
         m_textrectheight = 0;
         m_largest_line_width = 0;
         InitImageLinks();

         m_bIsPreview = false;
      }
   }
}

void ChattyPost::UpdatePreviewsToKnown()
{
  ReadFromKnown(m_pDoc);

  std::list<ChattyPost*>::iterator it = m_children.begin();
  std::list<ChattyPost*>::iterator end = m_children.end();
  while(it != end)
  {
     ChattyPost *pPost = (*it);
     pPost->UpdatePreviewsToKnown();
     it++;

     if(pPost->IsFiltered())
     {
        m_children.remove(pPost);

        delete(pPost);
     }
  }
}

ChattyPost *ChattyPost::FindChild(unsigned int id)
{
   ChattyPost *result = NULL;

   if(id == m_id)
   {
      result = this;
   }
   else
   {
      std::list<ChattyPost*>::iterator it = m_children.begin();
      std::list<ChattyPost*>::iterator end = m_children.end();

      while(it != end)
      {
         ChattyPost *thisresult = (*it)->FindChild(id);
         if(thisresult != NULL)
         {
            result = thisresult;
            break;
         }
         it++;
      }
   }
   return result;
}


void ChattyPost::GetTitle(UCString &title)
{
   if(m_bodytext.Length() > 0)
   {
      const UCChar *start = m_bodytext;
      const UCChar *end = m_bodytext.Str() + m_bodytext.Length();
      const UCChar *work = start;

      int spaces = 0;
      while(work < end &&
            work - start < theApp.GetTabTitleCharLimit())
      {
         if(iswspace(*work))
         {
            spaces++;
            if(spaces == theApp.GetTabTitleWordLimit())
            {
               break;
            }
         }
         title += *work;
         work++;
      }            
   }
   title.Replace(L"\n",L" ");
}

void ChattyPost::UpdateDate()
{
   if(m_datetext.Length() > 0 ||
      m_tm_posttime.tm_year != 0 ||
      m_tm_posttime.tm_mon  != 0 ||
      m_tm_posttime.tm_mday != 0 ||
      m_tm_posttime.tm_yday != 0 ||
      m_tm_posttime.tm_wday != 0 ||
      m_tm_posttime.tm_hour != 0 ||
      m_tm_posttime.tm_min  != 0 ||
      m_tm_posttime.tm_sec  != 0)
   {
      if(m_tm_posttime.tm_year == 0 &&
         m_tm_posttime.tm_mon  == 0 &&
         m_tm_posttime.tm_mday == 0 &&
         m_tm_posttime.tm_yday == 0 &&
         m_tm_posttime.tm_wday == 0 &&
         m_tm_posttime.tm_hour == 0 &&
         m_tm_posttime.tm_min  == 0 &&
         m_tm_posttime.tm_sec  == 0)
      {
         COleDateTime foo;

         bool bTrimmed = false;
         UCChar *datetext = (UCChar*)m_datetext.Str();
         UCChar *end = datetext + m_datetext.Length() - 4;
         if(end > datetext &&
            end[0] == L' ' &&
            iswalpha(end[1]) &&
            iswalpha(end[2]) &&
            iswalpha(end[3]))
         {
            *end = 0;
            bTrimmed = false;
         }

         foo.ParseDateTime(datetext,0,MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT));

         if(bTrimmed) *end = L' ';
         
         m_tm_posttime.tm_year = foo.GetYear() - 1900;
         m_tm_posttime.tm_mon  = foo.GetMonth() - 1;
         m_tm_posttime.tm_mday = foo.GetDay();
         m_tm_posttime.tm_yday = foo.GetDayOfYear() - 1;
         m_tm_posttime.tm_wday = foo.GetDayOfWeek() - 1;
         m_tm_posttime.tm_hour = foo.GetHour();
         m_tm_posttime.tm_min  = foo.GetMinute();
         m_tm_posttime.tm_sec  = foo.GetSecond();
      }

      time_t bar;
      time(&bar);

      tm tm_now;
      localtime_s(&tm_now,&bar);

      int now_seconds = tm_now.tm_sec;
      now_seconds += (tm_now.tm_min * 60);
      now_seconds += (tm_now.tm_hour * 60 * 60);
      now_seconds += (tm_now.tm_yday * 60 * 60 * 24);
      now_seconds += (tm_now.tm_year * 60 * 60 * 24 * 365);

      int post_seconds = m_tm_posttime.tm_sec;
      post_seconds += (m_tm_posttime.tm_min * 60);
      post_seconds += (m_tm_posttime.tm_hour * 60 * 60);
      post_seconds += (m_tm_posttime.tm_yday * 60 * 60 * 24);
      post_seconds += (m_tm_posttime.tm_year * 60 * 60 * 24 * 365);

      int ago_seconds = now_seconds - post_seconds;
      double diff = (double)ago_seconds;

      m_datetext = L"";
      bool bAddedsomething = false;

      double year_diff = diff / (60.0 * 60.0 * 24.0 * 365.0);
      if(year_diff > 1.0)
      {
         m_datetext += (int)year_diff;
         if((int)year_diff == 1) m_datetext += L" year";
         else m_datetext += L" years";
         bAddedsomething = true;
         diff = fmod(diff, 60.0 * 60.0 * 24.0 * 365.0);
      }

      double day_diff = diff / (60.0 * 60.0 * 24.0);
      if(day_diff > 1.0)
      {
         if(bAddedsomething)
         {
            m_datetext += L", ";
         }
         m_datetext += (int)day_diff;
         if((int)day_diff == 1) m_datetext += L" day";
         else m_datetext += L" days";
         bAddedsomething = true;
         diff = fmod(diff, 60.0 * 60.0 * 24.0);
      }

      double hour_diff = diff / (60.0 * 60.0);
      if(hour_diff > 1.0)
      {
         if(bAddedsomething)
         {
            m_datetext += L", ";
         }
         m_datetext += (int)hour_diff;
         if((int)hour_diff == 1) m_datetext += L" hour";
         else m_datetext += L" hours";
         bAddedsomething = true;
         diff = fmod(diff, 60.0 * 60.0);
      }

      double minute_diff = diff / 60.0;
      if(minute_diff > 1.0)
      {
         if(bAddedsomething)
         {
            m_datetext += L" and ";
         }
         m_datetext += (int)minute_diff;
         if((int)minute_diff == 1) m_datetext += L" minute";
         else m_datetext += L" minutes";
         bAddedsomething = true;
      }
      
      if(bAddedsomething)
      {
         m_datetext += L" ago";
      }
      else
      {
         m_datetext += L"just now";
      }

      if(ago_seconds > (theApp.GetHoursExpire() * 60 * 60))
      {
         m_ago_color = theApp.GetExpiredTextColor();
      }
      else if(ago_seconds > ((theApp.GetHoursExpire() - 1) * 60 * 60))
      {
         m_ago_color = theApp.GetExpiringTextColor();
      }
      else
      {
         m_ago_color = theApp.GetMiscPostTextColor();
      }
   }
   else
   {
      m_datetext = L"unknown";
      m_ago_color = theApp.GetMiscPostTextColor();
   }
}


int ChattyPost::GetCharPos(int x, int y, bool &off_end)
{
   int charpos = 0;

   // find the charpos
   x -= m_drewtextedge;
   y -= m_drewtextpos;
   x -= 5;
   y -= 4;
   size_t line =0;
   while(line < m_linetypes.size() && 
         y > (int)m_linetypes[line].m_height)
   {
      y -= m_linetypes[line].m_height;
      charpos += m_linesizes[line];
      line++;
   }

   int charposinthisline = 0;

   if(line < m_charsizes.size())
   {
      const int* charsizes = m_charsizes[line];
      const int numcharsinline = m_linesizes[line];
      int length = 0;

      for(int i=0; i < numcharsinline; i++)
      {
         if(length + charsizes[i] > x)
         {
            if(((length + charsizes[i]) - x) < (x - length))
            {
               charpos++;
               charposinthisline++;
            }
            break;
         }   
         else
         {
            length += charsizes[i];
            charpos++;
            charposinthisline++;
         }
      }
   }

   if(line == m_charsizes.size() || charposinthisline == m_linesizes[line])
   {
      off_end = true;
   }

   return charpos;
}

void ChattyPost::GetCharPosesForWord(int x, int y, int &selectionstart, int &selectionend)
{
   bool off_end;
   int charpos = GetCharPos(x, y, off_end);
   charpos = __max(0,__min(m_bodytext.Length(),charpos));

   const UCChar *begin = m_bodytext;
   const UCChar *here = begin + charpos;
   const UCChar *end = begin + m_bodytext.Length();
   const UCChar *work = here;

   if(off_end &&
      work > begin)
   {
      // dont wrap to the next line.
      work--;
      here = work;
   }

   if(iswspace(*work) == 0 &&
      iswalnum(*work) == 0)
   {
      // must be punctuation;
      // Select this char and only this char.
      selectionstart = work - begin;
      selectionend = selectionstart + 1;
      return;
   }

   // look left
   while(work >= begin && 
         iswspace(*work))
   {
      selectionstart = work - begin;
      work--;
   }
   while(work >= begin && 
         iswalnum(*work))
   {
      selectionstart = work - begin;
      work--;
   }
   
   work = here;

   // look right
   while(work < end && 
         iswalnum(*work))
   {
      selectionend = work - begin + 1;
      work++;
   }

   if(work < end && 
      //*work == L' ')
      iswspace(*work))
   {
      selectionend = work - begin + 1;
   }
}

void ChattyPost::GetCharPosesForPara(int x, int y, int &selectionstart, int &selectionend)
{
   bool off_end;
   int charpos = GetCharPos(x, y, off_end);
   charpos = __max(0,__min(m_bodytext.Length(),charpos));

   const UCChar *begin = m_bodytext;
   const UCChar *here = begin + charpos;
   const UCChar *end = begin + m_bodytext.Length();
   const UCChar *work = here;

   // look left
   while(work >= begin && 
         *work != L'\n')
   {
      selectionstart = work - begin;
      work--;
   }
   
   work = here;

   // look right
   while(work < end && 
         *work != L'\n')
   {
      selectionend = work - begin + 1;
      work++;
   }

   if(work < end && 
      *work == L'\n')
   {
      selectionend = work - begin + 1;
   }
}

void ChattyPost::GetTextSelectionRects(int selectionstart, int selectionend, std::vector<RECT> &selectionrects)
{
   if(selectionstart <= 0 && 
      selectionend < 0)
   {
      selectionstart *= -1;
      selectionend *= -1;

      int min = (size_t)__max(0,__min(selectionstart, selectionend));
      int max = (size_t)__min(m_author.Length(),__max(selectionstart, selectionend));

      if(min >= 0 &&
         min <= m_author.Length() &&
         max >= 0 &&
         max <= m_author.Length())
      {
         RECT rect;
         rect.top = m_drewtextpos - theApp.GetTextHeight();
         rect.bottom = m_drewtextpos;
         rect.left = m_drewtextedge + theApp.GetCellHeight() + 5;

         for(int i = 0; i < min; i++)
         {
            rect.left += m_pAuthorCharWidths[i];
         }

         rect.right = rect.left;

         for(int i = min; i < max; i++)
         {
            rect.right += m_pAuthorCharWidths[i];
         }

         selectionrects.push_back(rect);
      }
   }
   else
   {
      int min = (size_t)__max(0,__min(selectionstart, selectionend));
      int max = (size_t)__min(m_bodytext.Length(),__max(selectionstart, selectionend));

      RECT rect;
      rect.top = m_drewtextpos + 4;
      
      int charswalked = 0;
      for(size_t line = 0; line < m_charsizes.size(); line++)
      {
         rect.bottom = rect.top + m_linetypes[line].m_height;
                  
         const int* charsizes = m_charsizes[line];
         int numcharsinline = m_linesizes[line];

         bool bHaveleft = false;

         if(min - charswalked < 0)
         {
            rect.left = m_drewtextedge + 5;
            bHaveleft = true;
         }
         else if(min - charswalked < numcharsinline)
         {
            rect.left = m_drewtextedge + 5;
            for(int i=0; i < min - charswalked; i++)
            {
               rect.left += charsizes[i];
            }
            bHaveleft = true;
         }
         else
         {
            // skip.  Not there yet
         }

         if(bHaveleft)
         {
            if(max - charswalked >= 0)
            {
               if(m_linetypes[line].m_bIsText)
               {
                  rect.right = m_drewtextedge + 5;
                  for(int i=0; i < __min(numcharsinline,max - charswalked); i++)
                  {
                     rect.right += charsizes[i];
                  }
               }
               else
               {
                  if(m_linetypes[line].m_bIsThumb)
                  {
                     CDCSurface *pImage = theApp.GetLinkedImageThumb(m_linetypes[line].m_image_index);
                     if(pImage != NULL)
                     {
                        rect.left = m_drewtextedge + 5;
                        rect.right = rect.left + __min(m_lasttextrectwidth - 10,pImage->GetWidth());
                     }
                  }
                  else
                  {
                     CDCSurface *pImage = theApp.GetLinkedImage(m_linetypes[line].m_image_index);
                     if(pImage != NULL)
                     {
                        rect.left = m_drewtextedge + 5;
                        rect.right = rect.left + __min(m_lasttextrectwidth - 10,pImage->GetWidth());
                     }
                  }
               }
               
               if(rect.right > rect.left)
               {
                  selectionrects.push_back(rect);
               }
            }
            else
            {
               // skip.  It ended earlier.
            }
         }

         charswalked += m_linesizes[line];
         rect.top += m_linetypes[line].m_height;
      }
   }
}

void ChattyPost::GetSelectedText(int selectionstart, int selectionend, UCString &selectionrects)
{
   int min = (size_t)__max(0,__min(selectionstart, selectionend));
   int max = (size_t)__min(m_bodytext.Length(),__max(selectionstart, selectionend));

   const UCChar *work = m_bodytext.Str() + min;
   const UCChar *end = m_bodytext.Str() + max;

   while(work < end)
   {
      selectionrects += *work;
      work++;
   }
}

bool ChattyPost::FindNext(const UCChar *search, unsigned int &textselectionpost, int &selectionstart, int &selectionend)
{
   bool bFound = false;

   // test to see if it is me
   if(textselectionpost == m_id ||
      textselectionpost == 0)
   {
      int searchlength = wcslen(search);

      // test author name first
      if(textselectionpost == 0 ||
         selectionend < 0)
      {
         const UCChar *start = m_author.Str();
         const UCChar *end = start + m_author.Length();
         const UCChar *work = start;

         while(work < end &&
               end - work >= searchlength)
         {
            if(_wcsnicmp(work, search,searchlength) == 0)
            {
               unsigned int this_textselectionpost = m_id;
               int this_selectionstart = -( work - start );
               int this_selectionend = this_selectionstart - searchlength;

               if(textselectionpost == 0)
               {
                  textselectionpost = this_textselectionpost;
                  selectionstart = this_selectionstart;
                  selectionend = this_selectionend;
                  bFound = true;
                  break;
               }
               else
               {
                  // it's me
                  if(selectionstart == this_selectionstart &&
                     selectionend == this_selectionend)
                  {
                     // it is exactly me
                     // clear answer and look for next answer
                     textselectionpost = 0;
                     selectionstart = 0;
                     selectionend = 0;
                  }
                  else
                  {
                     if(this_selectionstart >= selectionstart)
                     {
                        // it is me, but I am early
                        // skip it and do nothing
                     }
                     else
                     {
                        // I am a better answer
                        textselectionpost = this_textselectionpost;
                        selectionstart = this_selectionstart;
                        selectionend = this_selectionend;
                        bFound = true;
                        break;
                     }
                  }
               }
            }
            work++;
         }
      }

      if(!bFound)
      {
         // test body next
         const UCChar *start = m_bodytext.Str();
         const UCChar *end = start + m_bodytext.Length();
         const UCChar *work = start;

         while(work < end &&
               end - work >= searchlength)
         {
            if(_wcsnicmp(work, search,searchlength) == 0)
            {
               unsigned int this_textselectionpost = m_id;
               int this_selectionstart = work - start;
               int this_selectionend = this_selectionstart + searchlength;

               if(textselectionpost == 0)
               {
                  textselectionpost = this_textselectionpost;
                  selectionstart = this_selectionstart;
                  selectionend = this_selectionend;
                  bFound = true;
                  break;
               }
               else
               {
                  // it's me
                  if(selectionstart == this_selectionstart &&
                     selectionend == this_selectionend)
                  {
                     // it is exactly me
                     // clear answer and look for next answer
                     textselectionpost = 0;
                     selectionstart = 0;
                     selectionend = 0;
                  }
                  else
                  {
                     if(this_selectionstart <= selectionstart)
                     {
                        // it is me, but I am early
                        // skip it and do nothing
                     }
                     else
                     {
                        // I am a better answer
                        textselectionpost = this_textselectionpost;
                        selectionstart = this_selectionstart;
                        selectionend = this_selectionend;
                        bFound = true;
                        break;
                     }
                  }
               }
            }
            work++;
         }

         if(textselectionpost == m_id &&
            !bFound)
         {
            // it is me
            // clear answer befor looking for next answer
            textselectionpost = 0;
            selectionstart = 0;
            selectionend = 0;
         }
      }
   }

   if(!bFound)
   {
      std::list<ChattyPost*>::iterator it = m_children.begin();
      std::list<ChattyPost*>::iterator end = m_children.end();

      while(it != end)
      {
         bFound = (*it)->FindNext(search, textselectionpost, selectionstart, selectionend);
         if(bFound)
         {
            break;
         }
         it++;
      }
   }
   return bFound;
}

void ChattyPost::ClearSpoilerTags(int x, int y)
{
   bool off_end;
   int charpos = GetCharPos(x, y, off_end);

   // find the last spoiler tag to come before the charpos
   shacktagpos *pSpoil = NULL;
   shacktagpos *pSpoilEnd = NULL;
   for(size_t i=0; i < m_shacktags.size(); i++)
   {
      if(m_shacktags[i].m_tag == ST_SPOILER &&
         m_shacktags[i].m_pos <= charpos)
      {
         pSpoil = &(m_shacktags[i]);
      }

      if(m_shacktags[i].m_tag == ST_SPOILER_END &&
         m_shacktags[i].m_pos >= charpos &&
         pSpoilEnd == NULL)
      {
         pSpoilEnd = &(m_shacktags[i]);
      }
   }

   if(pSpoil != NULL)
   {
      pSpoil->m_tag = ST_UNSPOILER;
   }

   if(pSpoilEnd != NULL)
   {
      pSpoilEnd->m_tag = ST_UNSPOILER_END;
   }   

   // this is to trigger a recalc of the line tags
   m_lasttextrectwidth = 0;
}

void ChattyPost::Despoil()
{
   bool despoiled = false;
   for(size_t i=0; i < m_shacktags.size(); i++)
   {
      if(m_shacktags[i].m_tag == ST_UNSPOILER)
      {
         despoiled = true;
         break;
      }
   }

   if(despoiled)
   {
      for(size_t i=0; i < m_shacktags.size(); i++)
      {
         if(m_shacktags[i].m_tag == ST_UNSPOILER)
         {
            m_shacktags[i].m_tag = ST_SPOILER;
         }

         if(m_shacktags[i].m_tag == ST_UNSPOILER_END)
         {
            m_shacktags[i].m_tag = ST_SPOILER_END;
         }
      }
   }
   else
   {
      for(size_t i=0; i < m_shacktags.size(); i++)
      {
         if(m_shacktags[i].m_tag == ST_SPOILER)
         {
            m_shacktags[i].m_tag = ST_UNSPOILER;
         }

         if(m_shacktags[i].m_tag == ST_SPOILER_END)
         {
            m_shacktags[i].m_tag = ST_UNSPOILER_END;
         }
      }
   }

   // this is to trigger a recalc of the line tags
   m_lasttextrectwidth = 0;
}



void ChattyPost::GetLink(int x, int y, UCString &link)
{
   bool off_end;
   int charpos = GetCharPos(x, y, off_end);

   // find the last spoiler tag to come before the charpos
   size_t index = 0;
   for(size_t i=0; i < m_shacktags.size(); i++)
   {
      if(m_shacktags[i].m_tag == ST_LINK &&
         m_shacktags[i].m_pos <= charpos)
      {
         index = i;
      }
   }

   link = m_shacktags[index].m_href;
}

void ChattyPost::GetImageLink(int x, int y, UCString &link)
{
   bool off_end;
   int charpos = GetCharPos(x, y, off_end);

   // find the last spoiler tag to come before the charpos
   size_t index = 0;
   for(size_t i=0; i < m_shacktags.size(); i++)
   {
      if(m_shacktags[i].m_tag == ST_IMAGE_LINK &&
         m_shacktags[i].m_pos <= charpos)
      {
         index = i;
      }
   }

   link = m_shacktags[index].m_href;
}

void ChattyPost::GetThumbLink(int x, int y, UCString &link)
{
   bool off_end;
   int charpos = GetCharPos(x, y, off_end);

   // find the last spoiler tag to come before the charpos
   size_t index = 0;
   for(size_t i=0; i < m_shacktags.size(); i++)
   {
      if(m_shacktags[i].m_tag == ST_THUMB &&
         m_shacktags[i].m_pos <= charpos)
      {
         index = i;
      }
   }

   link = m_shacktags[index].m_href;
}


void ChattyPost::GetLinkToThumb(int x, int y, UCString &link)
{
   // find the line
   y -= m_drewtextpos;
   y -= 4;
   size_t line =0;
   while(line < m_linetypes.size() && 
         y > (int)m_linetypes[line].m_height)
   {
      y -= m_linetypes[line].m_height;

      line++;
   }

   if(line < m_linetypes.size() && !m_linetypes[line].m_bIsText && m_linetypes[line].m_bIsThumb)
   {
      // find the last spoiler tag to come before the charpos
      const UCChar *pLink = m_lines_of_text[line];
      const UCChar *pLinkEnd = pLink + m_linesizes[line];

      if(pLink != NULL)
      { 
         if(pLinkEnd == NULL)
         {
            pLinkEnd = m_bodytext.Str() + m_bodytext.Length();
         }

         if(*(pLinkEnd - 1) == L'\n')
         {
            pLinkEnd--;
         }

         link.AppendUnicodeString(pLink, pLinkEnd - pLink);
      }
   }
}


void ChattyPost::GetLinkToImage(int x, int y, UCString &link)
{
   // find the line
   y -= m_drewtextpos;
   y -= 4;
   size_t line =0;
   while(line < m_linetypes.size() && 
         y > (int)m_linetypes[line].m_height)
   {
      y -= m_linetypes[line].m_height;

      line++;
   }

   if(line < m_linetypes.size() && !m_linetypes[line].m_bIsText && !m_linetypes[line].m_bIsThumb)
   {
      // find the last spoiler tag to come before the charpos
      const UCChar *pLink = m_lines_of_text[line];
      const UCChar *pLinkEnd = pLink + m_linesizes[line];

      if(pLink != NULL)
      { 
         if(pLinkEnd == NULL)
         {
            pLinkEnd = m_bodytext.Str() + m_bodytext.Length();
         }

         if(*(pLinkEnd - 1) == L'\n')
         {
            pLinkEnd--;
         }

         link.AppendUnicodeString(pLink, pLinkEnd - pLink);
      }
   }
}

void ChattyPost::MakeLinkIntoImage(int x, int y, unsigned int &index)
{
   bool off_end;
   int charpos = GetCharPos(x, y, off_end);

   // find the last spoiler tag to come before the charpos
   const UCChar *pLink = m_bodytext.Str();
   const UCChar *pLinkEnd = NULL;
   int begintag = 0;
   int endtag = 0;

   for(size_t i=0; i < m_shacktags.size(); i++)
   {
      if(m_shacktags[i].m_tag == ST_IMAGE_LINK &&
         m_shacktags[i].m_pos <= charpos)
      {
         begintag = i;
      }

      if(m_shacktags[i].m_tag == ST_IMAGE_LINK_END &&
         m_shacktags[i].m_pos >= charpos &&
         pLinkEnd == NULL)
      {
         endtag = i;
         break;
      }
   }

   m_shacktags[begintag].m_tag = ST_IMAGE;
   m_shacktags[begintag].m_image_index = index;
   m_shacktags[endtag].m_tag = ST_IMAGE_END;
   
   // this is to trigger a recalc of the line tags
   m_lasttextrectwidth = 0;
}

void ChattyPost::MakeThumbIntoImage(int x, int y, unsigned int &index)
{
   bool off_end;
   int charpos = GetCharPos(x, y, off_end);

   // find the last spoiler tag to come before the charpos
   const UCChar *pLink = m_bodytext.Str();
   const UCChar *pLinkEnd = NULL;
   int begintag = 0;
   int endtag = 0;

   for(size_t i=0; i < m_shacktags.size(); i++)
   {
      if(m_shacktags[i].m_tag == ST_THUMB &&
         m_shacktags[i].m_pos <= charpos)
      {
         begintag = i;
      }

      if(m_shacktags[i].m_tag == ST_THUMB_END &&
         m_shacktags[i].m_pos >= charpos &&
         pLinkEnd == NULL)
      {
         endtag = i;
         break;
      }
   }

   m_shacktags[begintag].m_tag = ST_IMAGE;
   m_shacktags[begintag].m_image_index = index;
   m_shacktags[endtag].m_tag = ST_IMAGE_END;
   
   // this is to trigger a recalc of the line tags
   m_lasttextrectwidth = 0;
}

void ChattyPost::MakeImageIntoLink(int x, int y)
{
   // find the line
   y -= m_drewtextpos;
   y -= 4;
   size_t line =0;
   while(line < m_linetypes.size() && 
         y > (int)m_linetypes[line].m_height)
   {
      y -= m_linetypes[line].m_height;

      line++;
   }

   if(line < m_linetypes.size() && !m_linetypes[line].m_bIsText && !m_linetypes[line].m_bIsThumb)
   {
      int charpos = m_lines_of_text[line] - m_bodytext.Str();
      charpos += (m_linesizes[line] / 2);

      // find the last image tag to come before the charpos
      const UCChar *pLink = m_bodytext.Str();
      const UCChar *pLinkEnd = NULL;
      int begintag = 0;
      int endtag = 0;

      for(size_t i=0; i < m_shacktags.size(); i++)
      {
         if(m_shacktags[i].m_tag == ST_IMAGE &&
            m_shacktags[i].m_pos <= charpos)
         {
            begintag = i;
         }

         if(m_shacktags[i].m_tag == ST_IMAGE_END &&
            m_shacktags[i].m_pos >= charpos &&
            pLinkEnd == NULL)
         {
            endtag = i;
            break;
         }
      }

      m_shacktags[begintag].m_tag = ST_IMAGE_LINK;
      m_shacktags[endtag].m_tag = ST_IMAGE_LINK_END;
      
      // this is to trigger a recalc of the line tags
      m_lasttextrectwidth = 0;
   }
}


void ChattyPost::MakeImageIntoThumb(int x, int y)
{
   // find the line
   y -= m_drewtextpos;
   y -= 4;
   size_t line =0;
   while(line < m_linetypes.size() && 
         y > (int)m_linetypes[line].m_height)
   {
      y -= m_linetypes[line].m_height;

      line++;
   }

   if(line < m_linetypes.size() && !m_linetypes[line].m_bIsText && !m_linetypes[line].m_bIsThumb)
   {
      int charpos = m_lines_of_text[line] - m_bodytext.Str();
      charpos += (m_linesizes[line] / 2);

      // find the last image tag to come before the charpos
      const UCChar *pLink = m_bodytext.Str();
      const UCChar *pLinkEnd = NULL;
      int begintag = 0;
      int endtag = 0;

      for(size_t i=0; i < m_shacktags.size(); i++)
      {
         if(m_shacktags[i].m_tag == ST_IMAGE &&
            m_shacktags[i].m_pos <= charpos)
         {
            begintag = i;
         }

         if(m_shacktags[i].m_tag == ST_IMAGE_END &&
            m_shacktags[i].m_pos >= charpos &&
            pLinkEnd == NULL)
         {
            endtag = i;
            break;
         }
      }

      m_shacktags[begintag].m_tag = ST_THUMB;
      m_shacktags[endtag].m_tag = ST_THUMB_END;

      theApp.MakeThumb(m_shacktags[begintag].m_image_index);
      
      // this is to trigger a recalc of the line tags
      m_lasttextrectwidth = 0;
   }
}

void ChattyPost::InvalidateSkin()
{
   m_lasttextrectwidth = 0;
   m_textrectheight = 0;
   m_largest_line_width = 0;
   m_authorsize = 0;
   m_authorpreviewsize = 0;
   m_drewtextpos = 0;
   m_drewtextedge = 0;
   m_bDrewTextBody = false;
   m_pos = 0;

   SetupCharWidths();

   UpdateAuthorColor();

   std::list<ChattyPost*>::iterator it = m_children.begin();
   std::list<ChattyPost*>::iterator end = m_children.end();
   while(it != end)
   {
      if((*it) != NULL)
      {
         (*it)->InvalidateSkin();
      }
      it++;
   }
}


bool ChattyPost::IsFiltered()
{
   bool result = false;
   switch(m_category)
   {
   case PCT_INF:
      result = !theApp.EnableINF();
      break;
   case PCT_NWS:
      result = !theApp.EnableNWS();
      break;
   case PCT_STUPID:
      result = !theApp.EnableStupid();
      break;
   case PCT_OFFTOPIC:
      result = !theApp.EnableOffTopic();
      break;
   case PCT_POLITICAL:
      result = !theApp.EnablePolitical();
      break;
   }

   if(result == false)
   {
      std::set<UCString> &ful = theApp.GetFilteredUsernameList();
      if(ful.size() > 0)
      {
         std::set<UCString>::iterator it = ful.find(m_author);
         if(it != ful.end())
         {
            result = true;
         }
      }
   }

   if(result == false)
   {
      std::set<UCString> &fpl = theApp.GetFilteredPhraseList();
      if(fpl.size() > 0)
      {
         std::set<UCString>::iterator it = fpl.begin();
         std::set<UCString>::iterator end = fpl.end();
         while(result == false && it != end)
         {
            if(m_bodytext.Find((*it).Str(),NULL,false,true) != NULL)
            {
               result = true;
            }
            it++;
         }
      }
   }

   return result;
}

void ChattyPost::UpdateRootReplyList(std::vector<unsigned int> *list/* = NULL*/)
{
   if(list == NULL)
   {
      m_root_reply_list.clear();
      list = &m_root_reply_list;
   }

   std::list<ChattyPost*>::iterator it = m_children.begin();
   std::list<ChattyPost*>::iterator end = m_children.end();
   while(it != end)
   {
      if((*it) != NULL)
      {
         list->push_back((*it)->GetId());
         (*it)->UpdateRootReplyList(list);
      }
      it++;
   }
}

unsigned int ChattyPost::GetPrevReply(bool bSkipSelf /*= false*/)
{
   unsigned int result = GetId();

   ChattyPost *pParent = this;
   while(pParent->m_pParent != NULL)pParent = pParent->m_pParent;

   pParent->UnShowAsTruncated();
   
   for(int i = 0; i < (int)pParent->m_root_reply_list.size(); i++)
   {
      if(pParent->m_root_reply_list[i] == result)
      {
         if(i-1 >= 0)
         {
            result = pParent->m_root_reply_list[i-1];
         }
         break;
      }
   }

   if(result == m_id &&
      m_pParent != NULL)
   {
      // then the prev is my parent
      m_pParent->UnShowAsTruncated();
      result = m_pParent->GetId();
   }

   return result;
}

unsigned int ChattyPost::GetNextReply(bool bSkipSelf /*= false*/)
{
   unsigned int result = GetId();

   if(m_pParent == NULL)
   {
      UnShowAsTruncated();
      if(m_root_reply_list.size() > 0)
      {
         result = m_root_reply_list[0];
      }
   }
   else
   {
      ChattyPost *pParent = this;
      while(pParent->m_pParent != NULL)pParent = pParent->m_pParent;

      pParent->UnShowAsTruncated();
      
      for(int i = 0; i < (int)pParent->m_root_reply_list.size(); i++)
      {
         if(pParent->m_root_reply_list[i] == result)
         {
            if(i+1 < (int)pParent->m_root_reply_list.size())
            {
               result = pParent->m_root_reply_list[i+1];
            }
            break;
         }
      }
   }
   return result;
}

void ChattyPost::UpdateLOLsRecurse()
{
   UpdateLOLs();

   std::list<ChattyPost*>::iterator it = m_children.begin();
   std::list<ChattyPost*>::iterator end = m_children.end();
   while(it != end)
   {
      if((*it) != NULL)
      {
         (*it)->UpdateLOLsRecurse();
      }
      it++;
   }
}

unsigned int ChattyPost::GetLOLWidth(const UCChar *text)
{
   UCString fulltext = L"[ ";
   fulltext += text;
   fulltext += L" ]";

   int *widths = (int*)malloc(sizeof(int) * fulltext.Length());

   GetCharWidths(fulltext, widths, fulltext.Length(), false, false, theApp.ShowSmallLOL(), theApp.GetNormalFontName());

   unsigned int width = 0;

   for(int i = 0; i < fulltext.Length(); i++) width += widths[i];

   free(widths);

   return width;
}

void ChattyPost::UpdateLOLs()
{
   m_bHaveLOLPreview = false;
   m_lolflags = theApp.GetKnownLOLFlags(m_id);

   if(theApp.ShowThomWLOLS())
   {
      m_lol_text = L"lol";
      m_inf_text = L"inf";
      m_unf_text = L"unf";
      m_tag_text = L"tag";
      m_wtf_text = L"wtf";

      if(m_lolflags.m_LOLd > 0) {m_lol_text += L" × ";m_lol_text += m_lolflags.m_LOLd; m_bHaveLOLPreview = true;}
      if(m_lolflags.m_INFd > 0) {m_inf_text += L" × ";m_inf_text += m_lolflags.m_INFd; m_bHaveLOLPreview = true;}
      if(m_lolflags.m_UNFd > 0) {m_unf_text += L" × ";m_unf_text += m_lolflags.m_UNFd; m_bHaveLOLPreview = true;}
      if(m_lolflags.m_TAGd > 0) {m_tag_text += L" × ";m_tag_text += m_lolflags.m_TAGd; m_bHaveLOLPreview = true;}
      if(m_lolflags.m_WTFd > 0) {m_wtf_text += L" × ";m_wtf_text += m_lolflags.m_WTFd; m_bHaveLOLPreview = true;}

      m_lol_width = GetLOLWidth(m_lol_text);
      m_inf_width = GetLOLWidth(m_inf_text);
      m_unf_width = GetLOLWidth(m_unf_text);
      m_tag_width = GetLOLWidth(m_tag_text);
      m_wtf_width = GetLOLWidth(m_wtf_text);
   }
   else
   {
      if(m_lolflags.m_LOLd > 0) {m_lol_text = m_lolflags.m_LOLd; m_bHaveLOLPreview = true;} else m_lol_text = L"";
      if(m_lolflags.m_INFd > 0) {m_inf_text = m_lolflags.m_INFd; m_bHaveLOLPreview = true;} else m_inf_text = L"";
      if(m_lolflags.m_UNFd > 0) {m_unf_text = m_lolflags.m_UNFd; m_bHaveLOLPreview = true;} else m_unf_text = L"";
      if(m_lolflags.m_TAGd > 0) {m_tag_text = m_lolflags.m_TAGd; m_bHaveLOLPreview = true;} else m_tag_text = L"";
      if(m_lolflags.m_WTFd > 0) {m_wtf_text = m_lolflags.m_WTFd; m_bHaveLOLPreview = true;} else m_wtf_text = L"";
   }

   if(m_bHaveLOLPreview)
   {
      if(m_plol_preview_charwidths != NULL)
      {
         free(m_plol_preview_charwidths);
         m_plol_preview_charwidths = NULL;
      }

      m_lol_preview_text = L": ";
      m_lol_preview_shacktags.clear();

      if(theApp.ShowSmallLOL())
      {
         m_lol_preview_shacktags.push_back(shacktagpos(ST_SAMPLE,0));
      }

      m_lol_preview_shacktags.push_back(shacktagpos(ST_BOLD,0));

      if(m_lolflags.m_LOLd > 0)
      {
         m_lol_preview_shacktags.push_back(shacktagpos(ST_ORANGE,m_lol_preview_text.Length()));
         if(theApp.InvertedLOLPreviews())
         {
            m_lol_preview_shacktags.push_back(shacktagpos(ST_INVERT,m_lol_preview_text.Length()));
            m_lol_preview_text += L" ";
         }
         if(theApp.VerboseLOLPreviews())m_lol_preview_text += L"lol × "; 
         m_lol_preview_text += m_lolflags.m_LOLd;
         if(theApp.InvertedLOLPreviews())
         {
            m_lol_preview_text += L" ";
            m_lol_preview_shacktags.push_back(shacktagpos(ST_INVERT_END,m_lol_preview_text.Length()));
         }
         m_lol_preview_shacktags.push_back(shacktagpos(ST_ORANGE_END,m_lol_preview_text.Length()));
      }

      if(m_lolflags.m_INFd > 0)
      {
         if(!m_lol_preview_text.IsEmpty())
         {
            m_lol_preview_text += L' '; 
         }
         m_lol_preview_shacktags.push_back(shacktagpos(ST_BLUE,m_lol_preview_text.Length()));
         if(theApp.InvertedLOLPreviews())
         {
            m_lol_preview_shacktags.push_back(shacktagpos(ST_INVERT,m_lol_preview_text.Length()));
            m_lol_preview_text += L" ";
         }         
         if(theApp.VerboseLOLPreviews())m_lol_preview_text += L"inf × "; 
         m_lol_preview_text += m_lolflags.m_INFd;
         if(theApp.InvertedLOLPreviews())
         {
            m_lol_preview_text += L" ";
            m_lol_preview_shacktags.push_back(shacktagpos(ST_INVERT_END,m_lol_preview_text.Length()));
         }
         m_lol_preview_shacktags.push_back(shacktagpos(ST_BLUE_END,m_lol_preview_text.Length()));
      }

      if(m_lolflags.m_UNFd > 0)
      {
         if(!m_lol_preview_text.IsEmpty())
         {
            m_lol_preview_text += L' '; 
         }
         m_lol_preview_shacktags.push_back(shacktagpos(ST_RED,m_lol_preview_text.Length()));
         if(theApp.InvertedLOLPreviews())
         {
            m_lol_preview_shacktags.push_back(shacktagpos(ST_INVERT,m_lol_preview_text.Length()));
            m_lol_preview_text += L" ";
         }
         if(theApp.VerboseLOLPreviews())m_lol_preview_text += L"unf × "; 
         m_lol_preview_text += m_lolflags.m_UNFd;
         if(theApp.InvertedLOLPreviews())
         {
            m_lol_preview_text += L" ";
            m_lol_preview_shacktags.push_back(shacktagpos(ST_INVERT_END,m_lol_preview_text.Length()));
         }
         m_lol_preview_shacktags.push_back(shacktagpos(ST_RED_END,m_lol_preview_text.Length()));
      }

      if(m_lolflags.m_TAGd > 0)
      {
         if(!m_lol_preview_text.IsEmpty())
         {
            m_lol_preview_text += L' '; 
         }
         m_lol_preview_shacktags.push_back(shacktagpos(ST_GREEN,m_lol_preview_text.Length()));
         if(theApp.InvertedLOLPreviews())
         {
            m_lol_preview_shacktags.push_back(shacktagpos(ST_INVERT,m_lol_preview_text.Length()));
            m_lol_preview_text += L" ";
         }
         if(theApp.VerboseLOLPreviews())m_lol_preview_text += L"tag × "; 
         m_lol_preview_text += m_lolflags.m_TAGd;
         if(theApp.InvertedLOLPreviews())
         {
            m_lol_preview_text += L" ";
            m_lol_preview_shacktags.push_back(shacktagpos(ST_INVERT_END,m_lol_preview_text.Length()));
         }
         m_lol_preview_shacktags.push_back(shacktagpos(ST_GREEN_END,m_lol_preview_text.Length()));
      }

      if(m_lolflags.m_WTFd > 0)
      {
         if(!m_lol_preview_text.IsEmpty())
         {
            m_lol_preview_text += L' '; 
         }
         m_lol_preview_shacktags.push_back(shacktagpos(ST_PURPLE,m_lol_preview_text.Length()));
         if(theApp.InvertedLOLPreviews())
         {
            m_lol_preview_shacktags.push_back(shacktagpos(ST_INVERT,m_lol_preview_text.Length()));
            m_lol_preview_text += L" ";
         }
         if(theApp.VerboseLOLPreviews())m_lol_preview_text += L"wtf × "; 
         m_lol_preview_text += m_lolflags.m_WTFd;
         if(theApp.InvertedLOLPreviews())
         {
            m_lol_preview_text += L" ";
            m_lol_preview_shacktags.push_back(shacktagpos(ST_INVERT_END,m_lol_preview_text.Length()));
         }
         m_lol_preview_shacktags.push_back(shacktagpos(ST_PURPLE_END,m_lol_preview_text.Length()));
      }

      m_lol_preview_shacktags.push_back(shacktagpos(ST_BOLD_END,m_lol_preview_text.Length()));

      if(theApp.ShowSmallLOL())
      {
         m_lol_preview_shacktags.push_back(shacktagpos(ST_SAMPLE_END,m_lol_preview_text.Length()));
      }

      m_plol_preview_charwidths = (int*)malloc(sizeof(int) * m_lol_preview_text.Length());

      GetCharWidths(m_lol_preview_text, m_plol_preview_charwidths, m_lol_preview_text.Length(), false, true, theApp.ShowSmallLOL(), theApp.GetNormalFontName());
      m_lol_preview_size = 0;
      for(size_t i = 0; i < (size_t)m_lol_preview_text.Length(); i++)
      {
         m_lol_preview_size += m_plol_preview_charwidths[i];
      }
   }
}

ChattyPost *ChattyPost::GetRoot()
{
   ChattyPost *result = this;

   while(result->GetParent() != NULL)result = result->GetParent();

   return result;
}

void ChattyPost::RecordNewness(std::map<unsigned int,newness> &post_newness)
{
   post_newness[m_id] = m_Newness;

   std::list<ChattyPost*>::iterator it = m_children.begin();
   std::list<ChattyPost*>::iterator end = m_children.end();
   while(it != end)
   {
      if((*it) != NULL)
      {
         (*it)->RecordNewness(post_newness);
      }
      it++;
   }
}

void ChattyPost::EstablishNewness(std::map<unsigned int,newness> &post_newness)
{
   std::map<unsigned int,newness>::iterator me = post_newness.find(m_id);
   if(me != post_newness.end())
   {
      m_Newness = me->second;
      BumpNewnessDown();
   }   
   else
   {
      if(post_newness.size() == 0)
      {
         m_Newness = N_OLD;
      }
      else
      {
         m_Newness = N_NEW;
      }
   }

   std::list<ChattyPost*>::iterator it = m_children.begin();
   std::list<ChattyPost*>::iterator end = m_children.end();
   while(it != end)
   {
      if((*it) != NULL)
      {
         (*it)->EstablishNewness(post_newness);
      }
      it++;
   }
}

void ChattyPost::RecordTags(std::map<unsigned int,std::vector<shacktagpos>> &post_tags)
{
   post_tags[m_id] = m_shacktags;

   std::list<ChattyPost*>::iterator it = m_children.begin();
   std::list<ChattyPost*>::iterator end = m_children.end();
   while(it != end)
   {
      if((*it) != NULL)
      {
         (*it)->RecordTags(post_tags);
      }
      it++;
   }
}

void ChattyPost::EstablishTags(std::map<unsigned int,std::vector<shacktagpos>> &post_tags)
{
   std::map<unsigned int,std::vector<shacktagpos>>::iterator me = post_tags.find(m_id);
   if(me != post_tags.end() &&
      m_shacktags.size() == me->second.size())
   {
      m_shacktags = me->second;

      for(size_t i=0; i < m_shacktags.size(); i++)
      {
         if(m_shacktags[i].m_tag == ST_UNSPOILER)
         {
            m_shacktags[i].m_tag = ST_SPOILER;
         }

         if(m_shacktags[i].m_tag == ST_UNSPOILER_END)
         {
            m_shacktags[i].m_tag = ST_SPOILER_END;
         }
      }
   }

   std::list<ChattyPost*>::iterator it = m_children.begin();
   std::list<ChattyPost*>::iterator end = m_children.end();
   while(it != end)
   {
      if((*it) != NULL)
      {
         (*it)->EstablishTags(post_tags);
      }
      it++;
   }
}

void ChattyPost::SetAsText(const UCChar *text)
{
   m_bJustText = true;
   
   m_subject = text;

   if(!m_subject.IsEmpty())
   {
      m_pSubjectCharWidths = (int*)malloc(sizeof(int) * m_subject.Length());

      GetCharWidths(m_subject, m_pSubjectCharWidths, m_subject.Length(), false, false, false, theApp.GetNormalFontName());

      m_subjectwidth = 0;
      for(int i = 0; i < m_subject.Length(); i++)
      {
         m_subjectwidth += m_pSubjectCharWidths[i];
      }      
   }
}


void ChattyPost::SetAsPageBreak(size_t page)
{
   m_bJustText = true;
   
   m_subject = (UCChar)0x02C5;
   m_subject += (UCChar)0x02C5;
   m_subject += (UCChar)0x02C5;
   m_subject += L" Page ";
   m_subject += page;
   m_subject += L" ";
   m_subject += (UCChar)0x02C5;
   m_subject += (UCChar)0x02C5;
   m_subject += (UCChar)0x02C5;

   if(!m_subject.IsEmpty())
   {
      m_pSubjectCharWidths = (int*)malloc(sizeof(int) * m_subject.Length());

      GetCharWidths(m_subject, m_pSubjectCharWidths, m_subject.Length(), false, false, false, theApp.GetNormalFontName());

      m_subjectwidth = 0;
      for(int i = 0; i < m_subject.Length(); i++)
      {
         m_subjectwidth += m_pSubjectCharWidths[i];
      }      
   }
}

bool ChattyPost::IsNWSPost()
{
   if(m_category == PCT_NWS)
      return true;

   ChattyPost *root = GetRoot();
   if(root != NULL &&
      root->GetCategory() == PCT_NWS)
      return true;

   const UCChar *start = m_bodytext.Str();
   const UCChar *work = start;
   const UCChar *end = work + m_bodytext.Length();

   int status = 0;

   while(work < end)
   {
      switch(status)
      {
      case 0: // have nothing
         if(*work == L'n' ||
            *work == L'N')
         {
            status++;
         }
         else status = 0;
         break;
      case 1: // have n
         if(*work == L'w' ||
            *work == L'W')
         {
            status++;
         }
         else status = 0;
         break;
      case 2: // have w
         if(*work == L's' ||
            *work == L'S')
         {
            // check to see if the character before nws was nonalpha
            if(work - 2 == start ||
               (!iswalpha(*(work - 3))))
            {
               // check to see if the character after nws was nonalpha
               if(work + 1 == end ||
                  (!iswalpha(*(work + 1))))
               {
                  return true;
               }
            }
         }
         status = 0;
         break;
      }

      work++;
   }

   return false;
}

CReplyDlg *ChattyPost::FindReplyDlgInPostRecurse(unsigned int &who_id)
{
   CReplyDlg *result = NULL;

   if(m_pReplyDlg != NULL)
   {
      result = m_pReplyDlg;
      who_id = m_id;
   }

   if(result == NULL)
   {
      std::list<ChattyPost*>::iterator it = m_children.begin();
      std::list<ChattyPost*>::iterator end = m_children.end();
      while(it != end && result == NULL)
      {
         if((*it) != NULL)
         {
            result = (*it)->FindReplyDlgInPostRecurse(who_id);
         }
         it++;
      }
   }

   return result;
}