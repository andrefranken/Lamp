/******************************************************************************
 *
 *  Latest Chatty Command Line Client
 *  By Electroly.
 *
 *  HTML.C
 *
 *  Decodes posts containing HTML entities, and encodes HTTP POST data into
 *  URL-safe form.
 *  
 *****************************************************************************/

#include "stdafx.h"
#include <stdlib.h>
#include <string.h>
#include "html.h"


static struct {char* name; char* decimal; char* hex; char ch;} table[] =
{
   {"&amp;",       "#&38;",        "&#x26;",       38},
   {"&lt;",        "#&60;",        "&#x3C;",       60},
   {"&gt;",        "#&62;",        "&#x3E;",       62},
   {"&AElig;",     "&#198;",       "&#xC6;",       146},
   {"&Aacute;",    "&#193;",       "&#xC1;",       65},
   {"&Acirc;",     "&#194;",       "&#xC2;",       65},
   {"&Agrave;",    "&#192;",       "&#xC0;",       65},
   {"&Aring;",     "&#197;",       "&#xC5;",       143},
   {"&Atilde;",    "&#195;",       "&#xC3;",       65},
   {"&Auml;",      "&#196;",       "&#xC4;",       142},
   {"&Ccedil;",    "&#199;",       "&#xC7;",       128},
   {"&ETH;",       "&#208;",       "&#xD0;",       68},
   {"&Eacute;",    "&#201;",       "&#xC9;",       144},
   {"&Ecirc;",     "&#202;",       "&#xCA;",       69},
   {"&Egrave;",    "&#200;",       "&#xC8;",       69},
   {"&Euml;",      "&#203;",       "&#xCB;",       69},
   {"&Iacute;",    "&#205;",       "&#xCD;",       73},
   {"&Icirc;",     "&#206;",       "&#xCE;",       73},
   {"&Igrave;",    "&#204;",       "&#xCC;",       73},
   {"&Iuml;",      "&#207;",       "&#xCF;",       73},
   {"&Ntilde;",    "&#209;",       "&#xD1;",       78},
   {"&Oacute;",    "&#211;",       "&#xD3;",       79},
   {"&Ocirc;",     "&#212;",       "&#xD4;",       79},
   {"&Ograve;",    "&#210;",       "&#xD2;",       79},
   {"&Oslash;",    "&#216;",       "&#xD8;",       79},
   {"&Otilde;",    "&#213;",       "&#xD5;",       79},
   {"&Ouml;",      "&#214;",       "&#xD6;",       79},
   {"&THORN;",     "&#222;",       "&#xDE;",       68},
   {"&Uacute;",    "&#218;",       "&#xDA;",       85},
   {"&Ucirc;",     "&#219;",       "&#xDB;",       85},
   {"&Ugrave;",    "&#217;",       "&#xD9;",       85},
   {"&Uuml;",      "&#220;",       "&#xDC;",       85},
   {"&Yacute;",    "&#221;",       "&#xDD;",       89},
   {"&aacute;",    "&#225;",       "&#xE1;",       160},
   {"&acirc;",     "&#226;",       "&#xE2;",       132},
   {"&acute;",     "&#180;",       "&#xB4;",       0},
   {"&aelig;",     "&#230;",       "&#xE6;",       145},
   {"&agrave;",    "&#224;",       "&#xE0;",       133},
   {"&aring;",     "&#229;",       "&#xE5;",       134},
   {"&atilde;",    "&#227;",       "&#xE3;",       131},
   {"&auml;",      "&#228;",       "&#xE4;",       132},
   {"&brvbar;",    "&#166;",       "&#xA6;",       124},
   {"&ccedil;",    "&#231;",       "&#xE7;",       135},
   {"&cedil;",     "&#184;",       "&#xB8;",       0},
   {"&cent;",      "&#162;",       "&#xA2;",       99},
   {"&copy;",      "&#169;",       "&#xA9;",       0},
   {"&curren;",    "&#164;",       "&#xA4;",       167},
   {"&deg;",       "&#176;",       "&#xB0;",       167},
   {"&divide;",    "&#247;",       "&#xF7;",       246},
   {"&eacute;",    "&#233;",       "&#xE9;",       130},
   {"&ecirc;",     "&#234;",       "&#xEA;",       137},
   {"&egrave;",    "&#232;",       "&#xE8;",       138},
   {"&eth;",       "&#240;",       "&#xF0;",       111},
   {"&euml;",      "&#235;",       "&#xEB;",       137},
   {"&frac12;",    "&#189;",       "&#xBD;",       171},
   {"&frac14;",    "&#188;",       "&#xBC;",       172},
   {"&frac34;",    "&#190;",       "&#xBE;",       0},
   {"&iacute;",    "&#237;",       "&#xED;",       161},
   {"&icirc;",     "&#238;",       "&#xEE;",       140},
   {"&iexcl;",     "&#161;",       "&#xA1;",       173},
   {"&igrave;",    "&#236;",       "&#xEC;",       141},
   {"&iquest;",    "&#191;",       "&#xBF;",       168},
   {"&iuml;",      "&#239;",       "&#xEF;",       139},
   {"&laquo;",     "&#171;",       "&#xAB;",       174},
   {"&macr;",      "&#175;",       "&#xAF;",       0},
   {"&micro;",     "&#181;",       "&#xB5;",       117},
   {"&middot;",    "&#183;",       "&#xB7;",       249},
   {"&nbsp;",      "&#160;",       "&#xA0;",       32},
   {"&not;",       "&#172;",       "&#xAC;",       170},
   {"&ntilde;",    "&#241;",       "&#xF1;",       164},
   {"&oacute;",    "&#243;",       "&#xF3;",       162},
   {"&ocirc;",     "&#244;",       "&#xF4;",       147},
   {"&ograve;",    "&#242;",       "&#xF2;",       149},
   {"&ordf;",      "&#170;",       "&#xAA;",       166},
   {"&ordm;",      "&#186;",       "&#xBA;",       167},
   {"&oslash;",    "&#248;",       "&#xF8;",       148},
   {"&otilde;",    "&#245;",       "&#xF5;",       147},
   {"&ouml;",      "&#246;",       "&#xF6;",       148},
   {"&para;",      "&#182;",       "&#xB6;",       80},
   {"&plusmn;",    "&#177;",       "&#xB1;",       241},
   {"&pound;",     "&#163;",       "&#xA3;",       156},
   {"&quot;",      "&#34;",        "&#x22;",       34},
   {"&raquo;",     "&#187;",       "&#xBB;",       175},
   {"&reg;",       "&#174;",       "&#xAE;",       0},
   {"&sect;",      "&#167;",       "&#xA7;",       83},
   {"&shy;",       "&#173;",       "&#xAD;",       45},
   {"&sup1;",      "&#185;",       "&#xB9;",       49},
   {"&sup2;",      "&#178;",       "&#xB2;",       253},
   {"&sup3;",      "&#179;",       "&#xB3;",       51},
   {"&szlig;",     "&#223;",       "&#xDF;",       225},
   {"&thorn;",     "&#254;",       "&#xFE;",       116},
   {"&times;",     "&#215;",       "&#xD7;",       42},
   {"&uacute;",    "&#250;",       "&#xFA;",       163},
   {"&ucirc;",     "&#251;",       "&#xFB;",       150},
   {"&ugrave;",    "&#249;",       "&#xF9;",       151},
   {"&uml;",       "&#168;",       "&#xA8;",       0},
   {"&uuml;",      "&#252;",       "&#xFC;",       129},
   {"&yacute;",    "&#253;",       "&#xFD;",       121},
   {"&yen;",       "&#165;",       "&#xA5;",       157},
   {"&yuml;",      "&#255;",       "&#xFF;",       121},
   {NULL,          NULL,           NULL,           0}
};


/******************************************************************************
 *
 *  html_entity_decode
 *
 *  PARAMETERS: string - [in/out] String in which to make replacements.
 *  RETURNS:    The same pointer that was passed in.
 *  PURPOSE:    Search and replace all instances of &entities; in 'haystack'
 *              with normal ASCII equivalents.  Also strips out tabs,
 *              newlines, and duplicated whitespace, which would normally be
 *              done automatically by the web browser.
 *  
 *****************************************************************************/
char* html_entity_decode(char* string)
{
   char*        cursor = string;
   char*        end    = string + strlen(string);
   char*        reader = string;
   char*        writer = string;
   unsigned int i;

   /* Make replacements in-place.  Removed characters are replaced with \0. */
   while (cursor < end)
   {
      if (*cursor == '\n' || *cursor == '\r' ||
          (*cursor == ' ' && *(cursor + 1) == ' '))
      {
         *(cursor++) = 0;
      }
      else if (*cursor == '\t')
      {
         *(cursor++) = ' ';
      }
      else if (*cursor == '<')
      {
         if (strncmp(cursor, "<br/>", 5) == 0)
         {
            /* Convert <br/> into a newline. */
            memset(cursor, 0, 5);
            *cursor = '\n';
            cursor += 5;
         }
         else
         {
            /* Strip out all other HTML. */
            while (*cursor != '>' && *cursor != 0)
               *(cursor++) = 0;

            *(cursor++) = 0;
         }
      }
      else if (*cursor == '&')
      {
         char   replacement = 0;
         size_t length;

         /* This linear table lookup seems like it should be awful for
            performance, but it seems to be fast enough. */
         if (strncmp(cursor, "&#x", 3) == 0)
         {
            /* Hexadecimal entity. */
            for (i = 0; table[i].hex != NULL; i++)
            {
               length = strlen(table[i].hex);
               if (strncmp(table[i].hex, cursor, length) == 0)
               {
                  replacement = table[i].ch;
                  break;
               }
            }
         }
         else if (strncmp(cursor, "&#", 2) == 0)
         {
            /* Decimal entity. */
            for (i = 0; table[i].decimal != NULL; i++)
            {
               length = strlen(table[i].decimal);
               if (strncmp(table[i].decimal, cursor, length) == 0)
               {
                  replacement = table[i].ch;
                  break;
               }
            }
         }
         else
         {
            /* Named entity. */
            for (i = 0; table[i].name != NULL; i++)
            {
               length = strlen(table[i].name);
               if (strncmp(table[i].name, cursor, length) == 0)
               {
                  replacement = table[i].ch;
                  break;
               }
            }
         }

         if (replacement == 0) /* Unknown entity */
         {
            unsigned int i;

            /* In order to prevent a crash when data is double-decoded when
               it shouldn't be, we need to accept the possibility that this
               ampersand is not actually an HTML entity, but just an ampersand.
               Check if this is the case.  There should be a semicolon within
               8 characters of the ampersand. */
            for (i = 0; i < 8; i++)
            {
               if (cursor[i] == 0) /* Not an entity */
               {
                  cursor += i + 1;
                  break;
               }
               else if (cursor[i] == ';') /* Valid but unknown entity */
               {
                  memset(cursor, 0, i + 1);
                  cursor += i + 1;
                  break;
               }
            }
         }
         else /* Known entity */
         {
            memset(cursor, 0, length);
            *cursor = replacement;
            cursor += length;
         }
      }
      else
      {
         cursor++;
      }
   }

   /* Close all the \0 gaps. */
   while (reader < end)
   {
      if (*reader == 0)
         reader++;
      else
         *(writer++) = *(reader++);
   }

   while (writer < end)
   {
      *(writer++) = 0;
   }

   return string;
}


/******************************************************************************
 *
 *  url_encode
 *
 *  PARAMETERS: string - [in] String in which to make replacements.
 *  RETURNS:    A newly allocated buffer with the encoded string.
 *  PURPOSE:    Encode a parameter so it can be used in an HTTP query string.
 *  
 *****************************************************************************/
char* url_encode(const char* string)
{
   size_t       string_len = strlen(string);
   char*        encoded    = (char*)malloc((string_len + 1) * 3);
   char*        hex        = "0123456789ABCDEF";
   char*        unsafe     = " $&+,/:;=?@<>#%{}|\\^~[]`";
   unsigned int encoded_i, string_i;

   memset(encoded, 0, (string_len + 1) * 3);

   for (string_i = 0, encoded_i = 0; string_i < string_len; string_i++)
   {
      unsigned char ch = (unsigned char) string[string_i];

      if (ch <= 0x1F || ch >= 0x7F || strchr(unsafe, ch) != 0)
      {
         encoded[encoded_i++] = '%';
         encoded[encoded_i++] = hex[ch >> 4];
         encoded[encoded_i++] = hex[ch & 0x0F];
      }
      else
      {
         encoded[encoded_i++] = ch;
      }
   }   

   return encoded;
}