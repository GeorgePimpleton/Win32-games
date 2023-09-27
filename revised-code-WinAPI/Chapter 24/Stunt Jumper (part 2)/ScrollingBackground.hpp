#pragma once

#include <windows.h>
#include "Background.hpp"

typedef WORD        SCROLLDIR;
const SCROLLDIR     SD_UP     = 0,
                    SD_RIGHT  = 1,
                    SD_DOWN   = 2,
                    SD_LEFT   = 3;

class BackgroundLayer : Bitmap
{
protected:
   RECT      m_rcViewport;
   int       m_iSpeed;
   SCROLLDIR m_sdDirection;

public:
   BackgroundLayer(HDC dc, PCWSTR fileName, int iSpeed,
                   SCROLLDIR sdDirection);
   BackgroundLayer(HDC dc, UINT resID, HINSTANCE inst, int iSpeed = 0,
                   SCROLLDIR sdDirection = SD_LEFT);

   virtual void Update( );
   virtual void Draw(HDC dc, int x, int y, BOOL bTrans = FALSE,
                     COLORREF crTransColor = RGB(255, 0, 255));
   void         SetSpeed(int iSpeed)                    { m_iSpeed = iSpeed; };
   void         SetDirection(SCROLLDIR sdDirection)     { m_sdDirection = sdDirection; };
   void         SetViewport(RECT& rcViewport)           { CopyRect(&m_rcViewport, &rcViewport); };
};

class ScrollingBackground : Background
{
protected:
   int               m_iNumLayers;
   BackgroundLayer* m_pLayers[ 10 ];

public:
            ScrollingBackground(int width, int height);
   virtual ~ScrollingBackground( );

   virtual void Update( );
   virtual void Draw(HDC dc, BOOL bTrans = FALSE,
                     COLORREF crTransColor = RGB(255, 0, 255));
   void         AddLayer(BackgroundLayer* pLayer);
};
