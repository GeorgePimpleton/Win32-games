#include "UFO 2.hpp"

HRESULT GameInitialize(HINSTANCE inst)
{
   g_game = std::make_unique<GameEngine>(inst, L"UFO 2", L"UFO 2: Improving Input with Joysticks",
                                         IDI_ICON, IDI_ICON_SM, 500, 400);

   if ( NULL == g_game )
   {
      return E_FAIL;
   }

   g_game->SetFrameRate(30);

   g_game->InitJoystick( );

   return S_OK;
}

void GameStart(HWND wnd)
{
   rtk::srand( );

   HINSTANCE inst = g_game->GetInstance( );

   g_background  = std::make_unique<Bitmap>(IDB_BACKGROUND, inst);
   g_saucer[ 0 ] = std::make_unique<Bitmap>(IDB_SAUCER, inst);
   g_saucer[ 1 ] = std::make_unique<Bitmap>(IDB_SAUCERFLAME, inst);

   g_saucerX = 250 - (g_saucer[ 0 ]-> GetWidth( ) / 2);
   g_saucerY = 200 - (g_saucer[ 0 ]-> GetHeight( ) / 2);
   g_speedX  = 0;
   g_speedY  = 0;
}

void GameNew( )
{ }

void GameEnd( )
{ }

void GameActivate(HWND wnd)
{
   g_game->CaptureJoystick( );
}

void GameDeactivate(HWND wnd)
{
   g_game->ReleaseJoystick( );
}

void GamePaint(HDC dc)
{
   g_background->Draw(dc, 0, 0);
   g_saucer[ g_saucerFlame ? 1 : 0 ]->Draw(dc, g_saucerX, g_saucerY, TRUE);
}

void GameCycle( )
{
   g_saucerX = min(500 - (LONG) g_saucer[ 0 ]->GetWidth( ), max(0, g_saucerX + g_speedX));
   g_saucerY = min(320, max(0, g_saucerY + g_speedY));

   InvalidateRect(g_game->GetWindow( ), NULL, FALSE);
}

void GameMenu(WPARAM wParam)
{
   switch ( LOWORD(wParam) )
   {
   case IDM_GAME_NEW:
      GameNew( );
      return;

   case IDM_GAME_EXIT:
      GameEnd( );
      PostQuitMessage(0);
      return;

   case IDM_HELP_ABOUT:
      DialogBoxW(g_game->GetInstance( ), MAKEINTRESOURCEW(IDD_ABOUT), g_game->GetWindow( ), (DLGPROC) DlgProc);
      return;
   }
}

void HandleKeys( )
{
   if ( GetAsyncKeyState(VK_LEFT) < 0 )
   {
      g_speedX = max(-g_MAXSPEED, --g_speedX);
   }
   else if ( GetAsyncKeyState(VK_RIGHT) < 0 )
   {
      g_speedX = min(g_MAXSPEED, ++g_speedX);
   }

   if ( GetAsyncKeyState(VK_UP) < 0 )
   {
      g_speedY = max(-g_MAXSPEED, --g_speedY);
   }
   else if ( GetAsyncKeyState(VK_DOWN) < 0 )
   {
      g_speedY = min(g_MAXSPEED, ++g_speedY);
   }
}

void MouseButtonDown(LONG x, LONG y, BOOL left)
{
   if ( left )
   {
      g_saucerX = x - (g_saucer[ 0 ]->GetWidth( ) / 2);
      g_saucerY = y - (g_saucer[ 0 ]->GetHeight( ) / 2);
   }
   else
   {
      g_speedX = 0;
      g_speedY = 0;
   }
}

void MouseButtonUp(LONG x, LONG y, BOOL left)
{ }

void MouseMove(LONG x, LONG y)
{ }

void HandleJoystick(JOYSTATE joyState)
{
   if ( joyState & JOY_LEFT )
   {
      g_speedX = max(-g_MAXSPEED, g_speedX - 2);
   }
   else if ( joyState & JOY_RIGHT )
   {
      g_speedX = min(g_MAXSPEED, g_speedX + 2);
   }

   if ( joyState & JOY_UP )
   {
      g_speedY = max(-g_MAXSPEED, g_speedY - 2);
   }
   else if ( joyState & JOY_DOWN )
   {
      g_speedY = min(g_MAXSPEED, g_speedY + 2);
   }

   g_saucerFlame = (joyState & JOY_FIRE1);

   if ( joyState & JOY_FIRE2 )
   {
      g_saucerX = rtk::rand(0, (500 - g_saucer[ 0 ]->GetWidth( )));
      g_saucerY = rtk::rand(0, (400 - g_saucer[ 0 ]->GetHeight( )));
   }
}