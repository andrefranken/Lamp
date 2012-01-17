// DCSurface.cpp : implementation file
//
#include "stdafx.h"
#include "Lamp.h"
#include "UCString.h"
#include "DCSurface.h"
#include "os.h"
#include "pnglib\png.h"        /* libpng header; includes zlib.h */

#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

extern "C" 
{
   #define XMD_H
   #define NEED_FAR_POINTERS
   //#define HAVE_BOOLEAN   
   #include "JpgLib\jpeglib.h"
}

struct MyJpegProgressMgr {
   struct jpeg_progress_mgr pub;	/* fields known to JPEG library */
   int completed_extra_passes;	/* extra passes completed */
   int total_extra_passes;	      /* total extra */

   /* last printed percentage stored here to avoid multiple printouts */
   int percent_done;
};

struct MyJpegErrorMgr {
  struct jpeg_error_mgr pub;	/* "public" fields */
  jmp_buf setjmp_buffer;	/* for return to caller */
};

extern CRITICAL_SECTION g_ThreadAccess;

bool CDCSurface::m_bPopulatedBlendTable = false;
byte CDCSurface::m_BlendTable[256][256];

/////////////////////////////////////////////////////////////////////////////
// CDCSurface


CDCSurface::CDCSurface()
{
   m_DC                = NULL;
   m_PixelWidth         = 0;
   m_PixelHeight        = 0;
   m_bitdepth           = 24;
   m_bytedepth          = 3;
   m_pBits              = NULL;
   m_hBitmap            = NULL;
   m_hPreviousBitmap    = NULL;
   m_bIsTransparent     = false;
   m_pBitmapInfo        = NULL;
   m_pCachedStretchdImage = NULL;
   m_bEnableCachedStretchImage = false;
}

CDCSurface::~CDCSurface()
{
   if(m_pCachedStretchdImage != NULL)
   {
      delete m_pCachedStretchdImage;
   }

   if(m_DC != NULL)
   {
      // if our bitmap is selected into the DC,
      // deselect it
      if(m_hPreviousBitmap != NULL)
      {
         ::SelectObject(m_DC, m_hPreviousBitmap);
      }

      ::DeleteDC( m_DC );
      m_DC = NULL;         
   }

   if( m_hBitmap != NULL )
   {
      ::DeleteObject(m_hBitmap);
      m_hBitmap = NULL;
   }

   if(m_pBitmapInfo != NULL)
   {
      free(m_pBitmapInfo);
   }

}

bool CDCSurface::Resize( int pixelwidth, int pixelheight )
{
   bool bDidResize = true;
   // first, determin if we need to resize at all,
   // this allows callers to call us redundantly, 
   // without penalty

   m_currentinsertionscanline = pixelheight - 1;

   if(m_DC != NULL)
   {
      // if our bitmap is selected into the DC,
      // deselect it
      if(m_hPreviousBitmap != NULL)
      {
         ::SelectObject(m_DC, m_hPreviousBitmap);
      }

      ::DeleteDC( m_DC );
      m_DC = NULL;         
   }

   if( m_hBitmap != NULL )
   {
      ::DeleteObject(m_hBitmap);
      m_hBitmap = NULL;
   }

   if(pixelheight > 0 && pixelwidth > 0 &&
      (m_hBitmap == NULL ||
       pixelwidth != m_PixelWidth ||
       pixelheight != m_PixelHeight))
   {
      bDidResize = true;
      HBITMAP hNewbmp = (HBITMAP)Size( pixelwidth, pixelheight, &m_pBits, &m_DC, &m_ImageByteSize );

      if( hNewbmp != NULL )
      {
         m_hPreviousBitmap = (HBITMAP)::SelectObject( m_DC, hNewbmp );
         m_hBitmap = hNewbmp;
      }
      m_PixelWidth = pixelwidth;
      m_PixelHeight = pixelheight;

      // for some reason, the bmWidthBytes value in the DIBSECTION
      // is wrong.  Dont do the following line of code:
      // m_ScanlineByteLength = dibSection.dsBm.bmWidthBytes;

      // Do this instead:
      m_ScanlineByteLength = m_ImageByteSize / pixelheight;
   }   
   else
   {
      m_PixelWidth = 0;
      m_PixelHeight = 0;
      m_pBits = NULL;
   }

   return bDidResize;
}


void *CDCSurface::Size( int pixelwidth, int pixelheight, void **pBits, HDC *pDC, int *pImageByteSize )
{
   HBITMAP hNewbmp = NULL;

   *pImageByteSize = 0;

   int minbytesinscanline = pixelwidth * m_bytedepth;

   *pImageByteSize = (((minbytesinscanline * 8)+ 31) / 32 * 4) * pixelheight;

   if(m_pBitmapInfo == NULL)
   {
      m_pBitmapInfo = malloc(sizeof(BITMAPINFO) + (sizeof(RGBQUAD) * 2));
   }
   
   if(m_pBitmapInfo != NULL)
   {
      BITMAPINFO *pBMI = (BITMAPINFO*)m_pBitmapInfo;
      // we need to make a new bitmap.
      pBMI->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
      pBMI->bmiHeader.biPlanes = 1;
      pBMI->bmiHeader.biBitCount = m_bitdepth;
      pBMI->bmiHeader.biCompression = BI_RGB;
      pBMI->bmiHeader.biSizeImage = *pImageByteSize;

      pBMI->bmiHeader.biXPelsPerMeter = pBMI->bmiHeader.biYPelsPerMeter = 96;
      pBMI->bmiHeader.biClrUsed = 0;
      pBMI->bmiHeader.biClrImportant = 0;      
      pBMI->bmiHeader.biWidth = pixelwidth;
      pBMI->bmiHeader.biHeight = pixelheight;
      pBMI->bmiColors[0].rgbBlue = 255;
      pBMI->bmiColors[0].rgbRed = 255;
      pBMI->bmiColors[0].rgbGreen = 255;
      pBMI->bmiColors[0].rgbReserved = 0;
      pBMI->bmiColors[1].rgbBlue = 0;
      pBMI->bmiColors[1].rgbRed = 0;
      pBMI->bmiColors[1].rgbGreen = 0;
      pBMI->bmiColors[1].rgbReserved = 0;

      if( pDC != NULL && *pDC == NULL )
      {
         *pDC = ::CreateCompatibleDC( NULL );  
      }

      if( pDC != NULL && *pDC != NULL )
      {
         hNewbmp = ::CreateDIBSection( *pDC, 
                                          pBMI, 
                                          DIB_RGB_COLORS, 
                                          pBits, 
                                          NULL, 
                                          0);

         if( hNewbmp != NULL )
         {
            // now, find out exactly how long a scanline is            
            DIBSECTION dibSection;
            GetObject(hNewbmp, sizeof(DIBSECTION), &dibSection);

            *pImageByteSize = (int)dibSection.dsBmih.biSizeImage;
         }
      }
   }

   return (void*)hNewbmp;
}

void CDCSurface::BlitRect2Rect(HDC hDC, RECT &fromrect, RECT &torect)
{
   if(m_bytedepth == 4)
   {
      // AlphaBlit(hDC, rect, fromorigin, 255, xsourceoffset, ysourceoffset);
   }
   else 
   {
      if(m_bIsTransparent)
      {
         ::TransparentBlt(hDC, torect.left, torect.top, torect.right - torect.left, torect.bottom - torect.top, 
                          m_DC, fromrect.left, fromrect.top, fromrect.right - fromrect.left, fromrect.bottom - fromrect.top, RGB(255,0,255));
      }
      else
      {
         ::BitBlt(hDC, torect.left, torect.top, torect.right - torect.left, torect.bottom - torect.top, 
                  m_DC, fromrect.left, fromrect.top, SRCCOPY);
      }
   }
}

void CDCSurface::StretchBlit(HDC hDC, RECT &rect)
{
   if(m_DC != NULL &&
      m_PixelWidth > 0 &&
      m_PixelHeight > 0)
   {
      if(m_bEnableCachedStretchImage)
      {
         if(m_pCachedStretchdImage == NULL)
         {
            m_pCachedStretchdImage = new CDCSurface();
         }

         if(m_pCachedStretchdImage != NULL)
         {
            if(m_pCachedStretchdImage->GetWidth() != rect.right - rect.left || 
               m_pCachedStretchdImage->GetHeight() != rect.bottom - rect.top)
            {
               m_pCachedStretchdImage->Resize(rect.right - rect.left, rect.bottom - rect.top);
               ::SetStretchBltMode(m_pCachedStretchdImage->GetDC(),HALFTONE);
               ::SetBrushOrgEx(m_pCachedStretchdImage->GetDC(), 0, 0, NULL);
               if(::StretchBlt(m_pCachedStretchdImage->GetDC(),0,0,rect.right - rect.left,rect.bottom - rect.top,m_DC,0,0,m_PixelWidth,m_PixelHeight,SRCCOPY) == FALSE)
               {
                  ::SetStretchBltMode(m_pCachedStretchdImage->GetDC(),COLORONCOLOR);
                  ::StretchBlt(m_pCachedStretchdImage->GetDC(),0,0,rect.right - rect.left,rect.bottom - rect.top,m_DC,0,0,m_PixelWidth,m_PixelHeight,SRCCOPY);
               }
            }

            ::BitBlt(hDC, rect.left, rect.top, rect.right - rect.left,rect.bottom - rect.top, m_pCachedStretchdImage->GetDC(), 0, 0, SRCCOPY);
         }
      }
      else
      {
         if(::StretchBlt(hDC,rect.left,rect.top,rect.right - rect.left,rect.bottom - rect.top,m_DC,0,0,m_PixelWidth,m_PixelHeight,SRCCOPY) == FALSE)
         {
            POINT pt;
            ::GetBrushOrgEx(hDC,&pt);

            ::SetStretchBltMode(hDC,COLORONCOLOR);

            ::StretchBlt(hDC,rect.left,rect.top,rect.right - rect.left,rect.bottom - rect.top,m_DC,0,0,m_PixelWidth,m_PixelHeight,SRCCOPY);

            ::SetStretchBltMode(hDC,HALFTONE);
            ::SetBrushOrgEx(hDC, pt.x, pt.y, NULL);
         }
      }
   }
}

void CDCSurface::Blit(HDC hDC, RECT &rect, bool fromorigin/*=true*/, int xsourceoffset/*= 0*/, int ysourceoffset/*= 0*/)
{
   if(m_bytedepth == 4)
   {
      AlphaBlit(hDC, rect, fromorigin, 255, xsourceoffset, ysourceoffset);
   }
   else 
   {
      int x = xsourceoffset;
      int y = ysourceoffset;
      int width = __min(m_PixelWidth,rect.right - rect.left);
      int height = __min(m_PixelHeight,rect.bottom - rect.top);
      if(!fromorigin)
      {
         x = rect.left + xsourceoffset;
         y = rect.top + ysourceoffset;
      }

      if(m_bIsTransparent)
      {
         ::TransparentBlt(hDC, rect.left, rect.top, width, height, 
                          m_DC, x, y, width, height, RGB(255,0,255));
      }
      else
      {
         ::BitBlt(hDC, rect.left, rect.top, width, height, m_DC, x, y, SRCCOPY);
      }
   }
}

void CDCSurface::AlphaBlit(HDC hDC, RECT &rect, bool fromorigin/*=true*/, byte globalalpha/*=255*/, int xsourceoffset/*= 0*/, int ysourceoffset/*= 0*/)
{
   int x = xsourceoffset;
   int y = ysourceoffset;
   int width = __min(m_PixelWidth,rect.right - rect.left);
   int height = __min(m_PixelHeight,rect.bottom - rect.top);
   if(!fromorigin)
   {
      x = rect.left + xsourceoffset;
      y = rect.top + ysourceoffset;
   }

   BLENDFUNCTION blendFunction;

   if(m_bytedepth == 4)
   {
      blendFunction.AlphaFormat = AC_SRC_ALPHA;
      blendFunction.BlendFlags = 0;
      blendFunction.BlendOp = AC_SRC_OVER;
      blendFunction.SourceConstantAlpha = globalalpha;
   }
   else
   {
      blendFunction.AlphaFormat = 0;
      blendFunction.BlendFlags = 0;
      blendFunction.BlendOp = 0;
      blendFunction.SourceConstantAlpha = globalalpha;
   }
   BOOL result = ::AlphaBlend(hDC, rect.left, rect.top, width, height, 
                              m_DC, x, y, width, height, blendFunction);
}

void CDCSurface::Fill(HDC hDC, RECT &rect)
{
   HBRUSH newbrush = ::CreatePatternBrush(m_hBitmap);
   ::SetBrushOrgEx(hDC, rect.left, rect.top, NULL);
   ::FillRect(hDC,&rect,newbrush);
   ::DeleteObject(newbrush);
}

void CDCSurface::SetWhite()
{
   memset(m_pBits,255,m_ImageByteSize);
}

void CDCSurface::Fill(byte red, byte green, byte blue)
{
   byte *pWriteScanline = (byte *)m_pBits;
   byte *pWriteScanlineEnd = pWriteScanline + (m_ScanlineByteLength * m_PixelHeight);

   while(pWriteScanline < pWriteScanlineEnd)
   {
      byte *pWrite = (byte *)pWriteScanline;
      byte *pWriteEnd = pWrite + (m_PixelWidth * m_bytedepth);

      while(pWrite < pWriteEnd)
      {
         pWrite[0] = blue;
         pWrite[1] = green;
         pWrite[2] = red;

         if(m_bytedepth == 4)
         {
            pWrite[3] = red;
         }

         pWrite += m_bytedepth;
      }

      pWriteScanline += m_ScanlineByteLength;
   }
}

bool CDCSurface::ReadPNG(const UCString &FileName, bool bBackgroundAlpha/*= false*/)
{
   bool result = false;

   if(FileName.Length() > 0)
   {
      if(_waccess(FileName,0) != -1)
      {
         result = LoadPNG(FileName, this);
      }
   }

   if(result && 
      m_bytedepth == 4 
      /* && OS is XP */)
   {
      MakeTransparentBitmap(bBackgroundAlpha);
   }

   return result;
}

void CDCSurface::MakeTransparentBitmap(bool bBackgroundAlpha)
{
   if(m_bytedepth == 4)
   {
      setupBlendTable();

      int width = m_PixelWidth;
      int height = m_PixelHeight;
      int oldscanlinesize = m_ScanlineByteLength;   
      byte *pOldPixels = (byte*)malloc(m_PixelHeight * m_ScanlineByteLength);
      memcpy(pOldPixels, m_pBits, m_PixelHeight * m_ScanlineByteLength);

      COLORREF crf = theApp.GetPostBackgroundColor();
      byte bkclr[3];
      bkclr[0] = GetBValue(crf);
      bkclr[1] = GetGValue(crf);
      bkclr[2] = GetRValue(crf);

      // destroy old
      if(m_DC != NULL)
      {
         // if our bitmap is selected into the DC,
         // deselect it
         if(m_hPreviousBitmap != NULL)
         {
            ::SelectObject(m_DC, m_hPreviousBitmap);
         }

         ::DeleteDC( m_DC );
         m_DC = NULL;         
      }

      if( m_hBitmap != NULL )
      {
         ::DeleteObject(m_hBitmap);
         m_hBitmap = NULL;
      }

      UseAlphaChannel(false);

      Resize(width, height);

      // make mask

      // copy RGBA data into RGB and flag which ones are transparent
      byte *pWriteScanline = (byte *)m_pBits;
      byte *pWriteScanlineEnd = pWriteScanline + (m_ScanlineByteLength * m_PixelHeight);

      byte *pReadScanline = pOldPixels;

      byte ipercent, invpercent;

      while(pWriteScanline < pWriteScanlineEnd)
      {
         byte *pWrite = (byte *)pWriteScanline;
         byte *pWriteEnd = pWrite + (m_PixelWidth * 3);
         byte *pRead = (byte *)pReadScanline;

         while(pWrite < pWriteEnd)
         {
            if(bBackgroundAlpha)
            {
               ipercent = pRead[3];
               invpercent = 255 - ipercent;
               
               pWrite[0] = m_BlendTable[pRead[0]][ipercent] + m_BlendTable[bkclr[0]][invpercent];
               pWrite[1] = m_BlendTable[pRead[1]][ipercent] + m_BlendTable[bkclr[1]][invpercent];
               pWrite[2] = m_BlendTable[pRead[2]][ipercent] + m_BlendTable[bkclr[2]][invpercent];
            }
            else
            {
               if(pRead[3] > 127)
               {
                  pWrite[0] = pRead[0];
                  pWrite[1] = pRead[1];
                  pWrite[2] = pRead[2];
               }
               else
               {
                  pWrite[0] = 255;
                  pWrite[1] = 0;
                  pWrite[2] = 255;
               }
            }

            pWrite += 3;
            pRead += 4;
         }

         pWriteScanline += m_ScanlineByteLength;
         pReadScanline += oldscanlinesize;
      }

      free(pOldPixels);
      m_bIsTransparent = true;
   }
}

void CDCSurface::MakeDecal(CDCSurface *from, byte red, byte green, byte blue)
{
   if(from != NULL &&
      from->GetWidth() > 0 &&
      from->GetHeight() > 0)
   {
      int width = from->GetWidth();
      int height = from->GetHeight();

      UseAlphaChannel(false);
      Resize(width,height);
            
      int oldscanlinesize = from->m_ScanlineByteLength;   
      // make mask

      byte *pWriteScanline = (byte *)m_pBits;
      byte *pWriteScanlineEnd = pWriteScanline + (m_ScanlineByteLength * m_PixelHeight);

      byte *pReadScanline = (byte *)from->m_pBits;

      while(pWriteScanline < pWriteScanlineEnd)
      {
         byte *pWrite = (byte *)pWriteScanline;
         byte *pWriteEnd = pWrite + (m_PixelWidth * 3);
         byte *pRead = (byte *)pReadScanline;

         while(pWrite < pWriteEnd)
         {
            if(*pRead > 127)
            {
               pWrite[0] = blue;
               pWrite[1] = green;
               pWrite[2] = red;
            }
            else
            {
               pWrite[0] = 255;
               pWrite[1] = 0;
               pWrite[2] = 255;
            }

            pWrite += 3;
            pRead += 3;
         }

         pWriteScanline += m_ScanlineByteLength;
         pReadScanline += oldscanlinesize;
      }

      m_bIsTransparent = true;
   }
}

void CDCSurface::AddScanline(byte *pScanline, int numbytes, bool reverse /* = false*/, bool invertbits /* = false*/)
{
   if( m_pBits             != NULL  &&
       m_PixelWidth         > 0     &&
       m_PixelHeight        > 0     &&
       m_ScanlineByteLength > 0     &&
       numbytes             <= m_ScanlineByteLength &&
       m_currentinsertionscanline >= 0     &&
       //m_bytedepth          != 4 && // only supports 1bit color and 24bit color
       m_currentinsertionscanline < m_PixelHeight )
   {
      byte *pWrite = (byte*)m_pBits + ( m_currentinsertionscanline * m_ScanlineByteLength );

      if(reverse)
      {
         if(this->m_bitdepth == 1)
         {
            for(int i=0;i<numbytes;i++)
            {
               pWrite[i] = (byte)REVERSBITS(pScanline[i]);

               if(invertbits)
               {
                  pWrite[i] = pWrite[i] ^ 0xff;
               }                            
            }
         }
         else
         {  
            for(int i=0;i<numbytes-2;i+=m_bytedepth)
            {
               pWrite[i] = pScanline[i+2];
               pWrite[i+1] = pScanline[i+1];
               pWrite[i+2] = pScanline[i];
               if(m_bytedepth == 4)
               {                  
                  pWrite[i+3] = pScanline[i+3];
               }
            }
         }
      }
      else
      {
         // copy 24 bits (3 bytes)
         memcpy(pWrite, pScanline, numbytes);
      }
   }
   m_currentinsertionscanline--;
}

bool LoadPNG(const UCString &filename, CDCSurface *pImage)
{
   bool result = false;

   FILE *pFile;
   _wfopen_s(&pFile,filename,L"rb");

   if(pFile != NULL)
   {   
      result = LoadPNGViaPointer(pFile, pImage);
      fclose(pFile);
   }

   return result;
}

void PNG_READFUNC(png_structp png_ptr, png_bytep mem, png_size_t numbytes)
{
   fread(mem, 1, numbytes, (FILE*)png_ptr->io_ptr);
}

void PNG_WRITEFUNC(png_structp png_ptr, png_bytep mem, png_size_t numbytes)
{
   fwrite(mem, 1, numbytes, (FILE*)png_ptr->io_ptr);
}

void PNG_FLUSHFUNC(png_structp png_ptr)
{
   // flushall();
}

bool LoadPNGViaPointer(FILE *pFile, CDCSurface *pImage)
{
   bool result = false;

   if(pFile != NULL)
   {
      unsigned char sig[8];
      bool error = false;

      /* first do a quick check that the file really is a PNG image; could
      * have used slightly more general png_sig_cmp() function instead */
      fread(sig, 1, 8, pFile);

      if (png_check_sig(sig, 8))
      {
         /* could pass pointers to user-defined error handlers instead of NULLs: */
         png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
         if (png_ptr)
         {
            png_set_read_fn(png_ptr, NULL, &PNG_READFUNC);

            png_infop info_ptr = png_create_info_struct(png_ptr);
            if (info_ptr) 
            {
               /* setjmp() must be called in every function that calls a PNG-reading
               * libpng function */
               if (setjmp(png_jmpbuf(png_ptr))) 
               {
                  png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
                  error = true;
               }

               if(!error)
               {
                  png_init_io(png_ptr, pFile);
                  png_set_sig_bytes(png_ptr, 8);  /* we already read the 8 signature bytes */
                  png_read_info(png_ptr, info_ptr);  /* read all PNG info up to image data */

                  png_uint_32 width;
                  png_uint_32 height;
                  int bit_depth;
                  int color_type;
                  unsigned char *image_data;

                  /* alternatively, could make separate calls to png_get_image_width(),
                  * etc., but want m_bit_depth and m_color_type for later [don't care about
                  * compression_type and filter_type => NULLs] */
                  png_get_IHDR(png_ptr, info_ptr, &width, &height, 
                              &bit_depth, &color_type,NULL, NULL, NULL);

                  png_uint_32  i, rowbytes;
                  png_bytepp  row_pointers = NULL;
                  int Channels;
                  unsigned int Rowbytes;

                  /* expand palette images to RGB, low-bit-depth grayscale images to 8 bits,
                  * transparency chunks to full alpha channel; strip 16-bit-per-sample
                  * images to 8 bits per sample; and convert grayscale to RGB[A] */
                  if (color_type == PNG_COLOR_TYPE_PALETTE)
                     png_set_expand(png_ptr);
                  if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
                     png_set_expand(png_ptr);
                  if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
                     png_set_expand(png_ptr);
                  if (bit_depth == 16)
                     png_set_strip_16(png_ptr);
                  if (color_type == PNG_COLOR_TYPE_GRAY ||
                     color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
                     png_set_gray_to_rgb(png_ptr);

                  png_read_update_info(png_ptr, info_ptr);

                  Rowbytes = rowbytes = png_get_rowbytes(png_ptr, info_ptr);
                  Channels = (int)png_get_channels(png_ptr, info_ptr);

                  bool alpha = Channels == 4;

                  if(alpha)
                  {
                     pImage->UseAlphaChannel(true);
                  }
                  if(pImage->Resize(width,height))
                  {
                     if ((image_data = (unsigned char *)malloc(rowbytes*height)) != NULL) 
                     {                           
                        if ((row_pointers = (png_bytepp)malloc(height*sizeof(png_bytep))) != NULL) 
                        {
                           /* set the individual row_pointers to point at the correct offsets */
                           for (i = 0;  i < height;  ++i)
                              row_pointers[i] = image_data + i*rowbytes;

                           /* now we can go ahead and just read the whole image */
                           png_read_image(png_ptr, row_pointers);
                                    
                           for (i = 0;  i < height;  ++i)
                           {
                              pImage->AddScanline(row_pointers[i],rowbytes,true);     
                           }

                           /* and we're done!  (png_read_end() can be omitted if no processing of
                           * post-IDAT text/time/etc. is desired) */
                           free(row_pointers);
                           row_pointers = NULL;
                           result = true;
                        }

                        png_read_end(png_ptr, NULL);
                        free(image_data);
                        image_data = NULL;
                     }
                  }

                  if (png_ptr && info_ptr) 
                  {
                     png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
                  }
               }
            }
         }
      }
   }   

   return result;
}

METHODDEF (void)myErrorExit(j_common_ptr cinfo)
{
   /* cinfo->err DLREALly points to a MyErrorMgr struct, so coerce pointer */
   MyJpegErrorMgr *pMyErr = (MyJpegErrorMgr*) cinfo->err;

   /* Return control to the setjmp point */
   longjmp(pMyErr->setjmp_buffer, 1);
}

METHODDEF (void)progress_monitor (j_common_ptr cinfo)
{
   MyJpegProgressMgr *prog = (MyJpegProgressMgr *)cinfo->progress;
   int total_passes;
   int current_pass;
   int percent_done;

   total_passes = prog->pub.total_passes + prog->total_extra_passes;
   current_pass = prog->pub.completed_passes + prog->completed_extra_passes;

   percent_done = (int)
      ( (current_pass * 100L) +
         (prog->pub.pass_counter * 100L / prog->pub.pass_limit)
      ) /total_passes;

   if (percent_done > prog->percent_done + 5 ) {
      prog->percent_done = percent_done;
   }
}

bool SetJPEGDecompressionParameters(void* decompressStruct)
{
   bool result = true;
   struct jpeg_decompress_struct* pCinfo = (jpeg_decompress_struct*) decompressStruct;
   if(pCinfo != NULL)
   {
      if (pCinfo->out_color_space != JCS_GRAYSCALE &&
         pCinfo->out_color_space != JCS_RGB &&
         pCinfo->out_color_space != JCS_CMYK)
      {
         result =  false;
      }

      if(result)
      {
         pCinfo->scale_num = 1;		// 1:1 scaling 
         pCinfo->scale_denom = 1;
         pCinfo->output_gamma = 1.0;
         pCinfo->buffered_image = false;
         pCinfo->raw_data_out = false;
         pCinfo->dct_method = JDCT_DEFAULT;
         pCinfo->do_fancy_upsampling = true;
         pCinfo->do_block_smoothing = true;
         pCinfo->quantize_colors = false;   
         pCinfo->colormap = NULL;   
         pCinfo->dither_mode = JDITHER_FS;
         pCinfo->two_pass_quantize = false;
         pCinfo->desired_number_of_colors = 256;
         pCinfo->colormap = NULL;
         pCinfo->enable_1pass_quant = false;
         pCinfo->enable_external_quant = false;
         pCinfo->enable_2pass_quant = false;
      }
   }
   return result;
}

bool GetRGBFromCMYK(byte* cmyk, byte& red, byte& green, byte& blue)
{
   float c = (255 - cmyk[0]) / 255.0f;
   float m = (255 - cmyk[1]) / 255.0f;
   float y = (255 - cmyk[2]) / 255.0f;
   float k = (255 - cmyk[3]) / 255.0f;

   //if(isDoingAdobeCMYK)
   //{
   //   // invert PDF jpegs
   //   c = 1.0 - c;
   //   m = 1.0 - m;
   //   y = 1.0 - y;
   //   k = 1.0 - k;
   //}

   // create color extremes from values that came from 
   // photoshop's out of the box values (no gamma adjustements).

   float c0 = 1.0f - c;
   float m0 = 1.0f - m;
   float y0 = 1.0f - y;
   float k0 = 1.0f - k;
   float c1 = c;
   float m1 = m;
   float y1 = y;
   float k1 = k;

   float cm00 = c0 * m0;
   float cm10 = c1 * m0;
   float cm01 = c0 * m1;
   float cm11 = c1 * m1;
   float yk00 = y0 * k0;
   float yk10 = y1 * k0;
   float yk01 = y0 * k1;
   float yk11 = y1 * k1;

   float d0000 = cm00 * yk00;
   float d1000 = cm10 * yk00;
   float d0100 = cm01 * yk00;
   float d1100 = cm11 * yk00;
   float d0010 = cm00 * yk10;
   float d1010 = cm10 * yk10;
   float d0110 = cm01 * yk10;
   float d1110 = cm11 * yk10;
   float d0001 = cm00 * yk01;
   float d1001 = cm10 * yk01;
   float d0101 = cm01 * yk01;
   float d1101 = cm11 * yk01;
   float d0011 = cm00 * yk11;
   float d1011 = cm10 * yk11;
   float d0111 = cm01 * yk11;
   float d1111 = cm11 * yk11;

   float r = ( 255 * d0000 ) +
      ( 236 * d0100 ) +
      ( 46  * d1100 ) +
      ( 255 * d0010 ) +
      ( 237 * d0110 ) +
      ( 54  * d1110 ) +
      ( 35  * d0001 ) +
      ( 41  * d0101 ) +
      ( 13  * d1101 ) +
      ( 24  * d0011 ) +
      ( 35  * d0111 ) +
      ( 3   * d1111 );

   float g = ( 255 * d0000 ) +
      ( 174 * d1000 ) +
      ( 49  * d1100 ) +
      ( 242 * d0010 ) +
      ( 166 * d1010 ) +
      ( 28  * d0110 ) +
      ( 54  * d1110 ) +
      ( 31  * d0001 ) +
      ( 6   * d1001 ) +
      ( 26  * d0011 ) +
      ( 6   * d1011 );

   float b = ( 255 * d0000 ) +
      ( 239 * d1000 ) +
      ( 140 * d0100 ) +
      ( 146 * d1100 ) +
      ( 81  * d1010 ) +
      ( 36  * d0110 ) +
      ( 57  * d1110 ) +
      ( 32  * d0001 ) +
      ( 36  * d1001 ) +
      ( 3   * d0101 ) +
      ( 26  * d1101 ) +
      ( 1   * d0111 );               

   blue = (byte)__max(0,__min(255,b));
   green = (byte)__max(0,__min(255,g));
   red = (byte)__max(0,__min(255,r));

   return true;
}


void WriteJPEGRowData(int colorSpace, int width, int byteDepth, byte* pRead, byte* pWrite)
{
   int pixel = 0;
   if(colorSpace == JCS_RGB)
   { 
      for(pixel = 0; pixel < width; pixel++)
      {
         pWrite[0] = pRead[2];
         pWrite[1] = pRead[1];
         pWrite[2] = pRead[0];
         pWrite += byteDepth;
         pRead += 3;
      }
   }
   else if(colorSpace == JCS_GRAYSCALE)
   {
      for(pixel = 0; pixel < width; pixel++)
      {
         pWrite[0] = *pRead;
         pWrite[1] = *pRead;
         pWrite[2] = *pRead;
         pWrite += byteDepth;
         pRead++;
      }
   }
   else if(colorSpace == JCS_CMYK)
   {
      byte red;
      byte green; 
      byte blue;

      for(pixel = 0; pixel < width; pixel++)
      {  
            
         GetRGBFromCMYK(pRead, red, green, blue);
         
         pWrite[0] = blue;
         pWrite[1] = green;
         pWrite[2] = red;

         pWrite += byteDepth;
         pRead+=4;
      }
   }
}



bool CDCSurface::ReadJpeg(const UCString &filename)
{



   char cwdbuffer[1024];
   char *pcwd = _getcwd(cwdbuffer,1024);

   char path[MAX_PATH+1]={0};

   GetTempPathA(MAX_PATH, path);

   _chdir(path);



   bool result = false;

   /* cross between dui and jpeg data elements */

   /* jpeg library data elements */

   MyJpegProgressMgr myProgressMgr;

   /* This struct contains the JPEG decompression parameters and pointers to
   * working space (which is allocated as needed by the JPEG library).
   */
   MyJpegErrorMgr myErrorHandler;

   JSAMPARRAY     pBitmap;		  /* Output row buffer */
   int            iBitmapSize;

   FILE * infile;		/* source file */

   /* allocate a decompression structure */

   struct jpeg_decompress_struct *pCinfo = (struct jpeg_decompress_struct *)malloc(sizeof(struct jpeg_decompress_struct));

   if (pCinfo == NULL)
   {
      return false;
   }

   _wfopen_s(&infile,filename, L"rb");

   if(infile == NULL) 
   {
      if ( pCinfo != NULL )
      {
         free(pCinfo);
      }
      return false;
   }

   /* allocate and initialize JPEG decompression object
   **
   ** We set up the normal JPEG error routines, then override error_exit.
   */
   pCinfo->err = jpeg_std_error(&myErrorHandler.pub);
   myErrorHandler.pub.error_exit = myErrorExit;

   /* Establish the setjmp return context for my_error_exit to use. */

   if (setjmp(myErrorHandler.setjmp_buffer)) 
   {
      /* If we get here, the JPEG code has signaled an error.
      * We need to clean up the JPEG object, close the input file, and return.
      */
      if ( pCinfo != NULL )
      {
         free(pCinfo);
      }

      fclose(infile);
      return false;
   }

   /* Now we can initialize the JPEG decompression object. */

   jpeg_create_decompress(pCinfo);
   //jpeg_CreateDecompress (pCinfo, JPEG_LIB_VERSION, (size_t) sizeof(struct jpeg_decompress_struct));

   /* Enable progress display, unless trace output is on */

   myProgressMgr.pub.progress_monitor = progress_monitor;
   myProgressMgr.completed_extra_passes = 1;
   myProgressMgr.total_extra_passes = 1;
   myProgressMgr.percent_done = -1;
   pCinfo->progress = &myProgressMgr.pub;

   /* specify data source (eg, a file) */

   jpeg_stdio_src(pCinfo, infile);

   jpeg_save_markers(pCinfo, JPEG_COM, 20);

   /* read file parameters with jpeg_read_header() */

   if (jpeg_read_header(pCinfo, true) != JPEG_HEADER_OK )
   {
      jpeg_destroy_decompress(pCinfo);

      if ( pCinfo != NULL )
      {
         free(pCinfo);
      }

      fclose(infile);
      return false;
   }

   /* look for one of our markers */
   bool bDoingRGBAfromCMYK = false;
   bool bDoingAdobeCMYK = false;

   jpeg_saved_marker_ptr marker = pCinfo->marker_list;

   while(marker != NULL)
   {
      if(marker->data_length > 4 && strncmp("IGC:",(const char*)marker->data,4)==0)
      {
         if(marker->data_length > 12 && strncmp("IGC:CMYK=RGBA",(const char*)marker->data,13)==0)
         {
            bDoingRGBAfromCMYK = true;
            bDoingAdobeCMYK = false;
         }
         break;
      }
      marker = marker->next;
   }

   /* Calculate output image dimensions so we can allocate space */

   jpeg_calc_output_dimensions(pCinfo);

   /* set parameters for decompression */
   result = SetJPEGDecompressionParameters(pCinfo);   

   if (result)
   {
   if(Resize(pCinfo->output_width,pCinfo->output_height))
   {
      byte* pScanline = (byte *)m_pBits;
      int ScanlineByteLength = m_ScanlineByteLength;
      pScanline += ScanlineByteLength * (pCinfo->output_height - 1);

      /* JSAMPLEs per row in output buffer */
      /* Make a one-row-high sample array */

      iBitmapSize = pCinfo->output_width * pCinfo->output_components;
      pBitmap = (*pCinfo->mem->alloc_sarray)((j_common_ptr) pCinfo, JPOOL_IMAGE, iBitmapSize, 1);

      /* Start decompressor and init output */

      jpeg_start_decompress(pCinfo);

      /* while (scan lines remain to be read) */
      /*           jpeg_read_scanlines(...); */
      /* Here we use the library's state variable memptr->pCinfo->output_scanline as the
      * loop counter, so that we don't have to keep track ourselves.
      */
         byte *pRead;
         byte *pWrite;

      while (pCinfo->output_scanline < pCinfo->output_height) 
      {
         result = true;      
            
         if(jpeg_read_scanlines(pCinfo, pBitmap, 1) == 1)
         {
               /* insert the scanline */
               pRead = pBitmap[0];
               pWrite = pScanline;

               WriteJPEGRowData(pCinfo->out_color_space, 
                                 (int)pCinfo->output_width, m_bytedepth, 
                                 pRead, pWrite);


            pScanline -= ScanlineByteLength;
         }
         else      
         {
            result = false;
            /* break out of loop and let the jpg libs think they did their job */
            pCinfo->output_scanline = pCinfo->output_height;
            break;
         }
      }
   }
   }

   /* This is an important step since it will release a good deal of memory. */
   jpeg_destroy_decompress(pCinfo);

   if ( pCinfo != NULL )
   {
      free(pCinfo);
   }

   fclose(infile);

   /* And we're done! */

   return result;
}


void CDCSurface::setupBlendTable(void)
{
   if(!m_bPopulatedBlendTable)
   {
      ::EnterCriticalSection(&g_ThreadAccess);

      if(!m_bPopulatedBlendTable)
      {
         for(int i = 0; i < 256; i++)
         {
            for(int p = 0; p < 256; p++)
            {         
               m_BlendTable[i][p] = (byte)((float)i * ((float)p / (float)255.0));
            }
         }
         m_bPopulatedBlendTable = true;
      }

      ::LeaveCriticalSection(&g_ThreadAccess);
   }
}
