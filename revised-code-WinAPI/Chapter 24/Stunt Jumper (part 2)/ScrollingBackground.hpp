#pragma once

#include <windows.h>
#include "Background.hpp"

typedef WORD    SCROLLDIR;
const SCROLLDIR SD_UP     = 0,
                SD_RIGHT  = 1,
                SD_DOWN   = 2,
                SD_LEFT   = 3;

class BackgroundLayer : Bitmap
{
protected:
   RECT      m_viewport;
   int       m_speed;
   SCROLLDIR m_direction;

public:
   BackgroundLayer( PCWSTR, int,
                    SCROLLDIR );
   BackgroundLayer( UINT, int = 0,
                    SCROLLDIR = SD_LEFT );

   virtual void Update( );
   virtual void Draw( HDC, int, int, BOOL = FALSE,
                      COLORREF = RGB( 255, 0, 255 ) );

   void SetSpeed( int speed ) { m_speed = speed; };
   void SetDirection( SCROLLDIR direction ) { m_direction = direction; };
   void SetViewport( RECT& viewport ) { CopyRect( &m_viewport, &viewport ); };
};

class ScrollingBackground : Background
{
protected:
   int               m_numLayers;
   BackgroundLayer* m_layers[10];

public:
            ScrollingBackground( int, int );
   virtual ~ScrollingBackground( );

   virtual void Update( );
   virtual void Draw( HDC, BOOL = FALSE,
                      COLORREF = RGB( 255, 0, 255 ) );

   void         AddLayer( BackgroundLayer* );
};
