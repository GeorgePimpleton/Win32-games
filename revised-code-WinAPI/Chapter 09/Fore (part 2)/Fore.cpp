#include "Fore.hpp"

HRESULT GameInitialize( HINSTANCE inst )
{
   g_game = std::make_unique<GameEngine>( inst, L"Fore", L"Fore b: Making Things Move with Sprites",
                                          IDI_ICON, IDI_ICON_SM, 600, 400 );

   if ( NULL == g_game )
   {
      return E_FAIL;
   }

   g_game->SetFrameRate( 30 );

   return S_OK;
}

void GameStart( HWND wnd )
{
   rtk::srand( );

   g_forestBitmap   = std::make_unique<Bitmap>( IDB_FOREST );
   g_golfBallBitmap = std::make_unique<Bitmap>( IDB_GOLFBALL );

   RECT rcBounds = { 0, 0, 600, 400 };

   g_golfBallSprite[0] = std::make_unique<Sprite>( g_golfBallBitmap.get( ), rcBounds );
   g_golfBallSprite[0]->SetVelocity( 2, 1 );

   g_golfBallSprite[1] = std::make_unique<Sprite>( g_golfBallBitmap.get( ), rcBounds, BA_WRAP );
   g_golfBallSprite[1]->SetVelocity( 3, -2 );

   g_golfBallSprite[2] = std::make_unique<Sprite>( g_golfBallBitmap.get( ), rcBounds, BA_BOUNCE );
   g_golfBallSprite[2]->SetVelocity( 7, 4 );

   g_dragging = FALSE;
   g_dragBall = -1;
}

void GameNew( )
{ }

void GameEnd( )
{ }

void GameActivate( HWND wnd )
{ }

void GameDeactivate( HWND wnd )
{ }

void GamePaint( HDC dc )
{
   g_forestBitmap->Draw( dc, 0, 0 );

   for ( int i = 0; i < 3; i++ )
   {
      g_golfBallSprite[i]->Draw( dc );
   }
}

void GameCycle( )
{
   for ( int i = 0; i < 3; i++ )
   {
      g_golfBallSprite[i]->Update( );
   }

   InvalidateRect( g_game->GetWindow( ), NULL, FALSE );
}

void GameMenu( WPARAM wParam )
{
   switch ( LOWORD( wParam ) )
   {
   case IDM_GAME_NEW:
      GameNew( );
      return;

   case IDM_GAME_EXIT:
      DestroyWindow( g_game->GetWindow() );
      return;

   case IDM_HELP_ABOUT:
      DialogBoxW( g_game->GetInstance( ), MAKEINTRESOURCEW( IDD_ABOUT ), g_game->GetWindow( ), ( DLGPROC ) DlgProc );
      return;
   }
}

void HandleKeys( )
{ }

void MouseButtonDown( LONG x, LONG y, BOOL left )
{
   if ( left && !g_dragging )
   {
      for ( int i = 0; i < 3; i++ )
      {
         if ( g_golfBallSprite[i]->IsPointInside( x, y ) )
         {
            SetCapture( g_game->GetWindow( ) );

            g_dragging = TRUE;
            g_dragBall = i;

            MouseMove( x, y );

            break;
         }
      }
   }
}

void MouseButtonUp( LONG x, LONG y, BOOL left )
{
   ReleaseCapture( );

   g_dragging = FALSE;
}

void MouseMove( LONG x, LONG y )
{
   if ( g_dragging )
   {
      g_golfBallSprite[g_dragBall]->SetPosition( x - ( g_golfBallBitmap->GetWidth( )  / 2 ),
                                                 y - ( g_golfBallBitmap->GetHeight( ) / 2 ) );

      InvalidateRect( g_game->GetWindow( ), NULL, FALSE );
   }
}

void HandleJoystick( JOYSTATE jsJoystickState )
{ }
