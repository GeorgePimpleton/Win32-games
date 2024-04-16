#pragma once

#include <windows.h>
#include "Background.h"

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
   BackgroundLayer(HDC hDC, PCTSTR szFileName, int iSpeed,
                   SCROLLDIR sdDirection);
   BackgroundLayer(HDC hDC, UINT uiResID, HINSTANCE hInstance, int iSpeed = 0,
                   SCROLLDIR sdDirection = SD_LEFT);

   virtual void Update( );
   virtual void Draw(HDC hDC, int x, int y, BOOL bTrans = FALSE,
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
            ScrollingBackground(int iWidth, int iHeight);
   virtual ~ScrollingBackground( );

   virtual void Update( );
   virtual void Draw(HDC hDC, BOOL bTrans = FALSE,
                     COLORREF crTransColor = RGB(255, 0, 255));
   void         AddLayer(BackgroundLayer* pLayer);
};
