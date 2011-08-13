// DCSurface.h: interface for the CDCSurface class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "UCString.h"

class CDCSurface;

bool LoadPNG(const UCString &filename, CDCSurface *pImage);
bool LoadPNGViaPointer(FILE *pFile, CDCSurface *pImage);

#define REVERSBITS(in)          (((in * 0x0802LU & 0x22110LU) | (in * 0x8020LU & 0x88440LU)) * 0x10101LU >> 16)

/////////////////////////////////////////////////////////////////////////////
// CDCSurface 

class CDCSurface
{
// Construction
public:
   CDCSurface();
   virtual ~CDCSurface();

   bool Resize( int pixelwidth, int pixelheight );
   void *Size( int pixelwidth, int pixelheight, void **pBits, HDC *pDC, int *pImageByteSize );
   bool ReadPNG(const UCString &FileName, bool bBackgroundAlpha = false);
   bool ReadJpeg(const UCString &filename);

   HDC GetDC(){return m_DC;}

   int GetWidth(){return m_PixelWidth;}
   int GetHeight(){return m_PixelHeight;}

   int GetHeightForWidth(int width)
   {
      if(width < m_PixelWidth)
      {
         float scale = (float)width / (float)m_PixelWidth;
         return (int)((float)m_PixelHeight * scale);
      }
      else
      {
         return m_PixelHeight;
      }
   }

   void EnableCachedStretchImage(bool value){m_bEnableCachedStretchImage = value;}

   void StretchBlit(HDC hDC, RECT &rect);
   void Blit(HDC hDC, RECT &rect, bool fromorigin = true, int xsourceoffset = 0, int ysourceoffset = 0);
   void BlitRect2Rect(HDC hDC, RECT &fromrect, RECT &torect);
   void Fill(HDC hDC, RECT &rect);
   void AlphaBlit(HDC hDC, RECT &rect, bool fromorigin = true, byte globalalpha = 255, int xsourceoffset = 0, int ysourceoffset = 0);
   void SetWhite();
   void Fill(byte red, byte green, byte blue);
   void MakeTransparentBitmap(bool bBackgroundAlpha);

   void UseAlphaChannel(bool bValue)
   {
      if(bValue)
      {
         if(m_bitdepth != 32 ||
            m_bytedepth != 4)
         {
            m_PixelWidth = 0;
            m_PixelHeight = 0;
         }            
         m_bitdepth = 32; 
         m_bytedepth = 4;            
      }
      else
      {
         if(m_bitdepth != 24 ||
            m_bytedepth != 3)
         {
            m_PixelWidth = 0;
            m_PixelHeight = 0;
         }
         m_bitdepth = 24; 
         m_bytedepth = 3;
      }
   }

   void AddScanline(byte *pScanline, int numbytes, bool reverse = false, bool invertbits = false);

   HBITMAP GetBitmap(){return m_hBitmap;}

protected:
   void setupBlendTable(void);

   HDC          m_DC;
   int          m_PixelWidth;
   int          m_PixelHeight;
   int          m_ScanlineByteLength;   
   int          m_ImageByteSize;   
   HBITMAP      m_hBitmap;
   HBITMAP      m_hPreviousBitmap;      
   void         *m_pBits;
   void         *m_pBitmapInfo;
   int          m_bitdepth;
   int          m_bytedepth;
   int          m_currentinsertionscanline;

   bool         m_bIsTransparent;

   bool         m_bEnableCachedStretchImage;
   CDCSurface  *m_pCachedStretchdImage;

   static byte m_BlendTable[256][256];
   static bool m_bPopulatedBlendTable;
};
