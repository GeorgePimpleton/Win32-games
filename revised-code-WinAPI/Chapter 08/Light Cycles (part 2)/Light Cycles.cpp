#include "Light Cycles.hpp"

HRESULT GameInitialize( HINSTANCE inst )
{
   g_game = std::make_unique<GameEngine>( inst, L"Light Cycles", L"Light Cycles b",
                                          IDI_ICON, IDI_ICON_SM, 500, 400 );

   if ( NULL == g_game )
   {
      return E_FAIL;
   }

   g_game->SetFrameRate( 30 );

   g_game->InitJoystick( );

   return S_OK;
}

void GameStart( HWND wnd )
{
   g_background  = std::make_unique<Bitmap>( IDB_BACKGROUND );
   g_cycle[0][0] = std::make_unique<Bitmap>( IDB_CYCLEBLUE_0 );
   g_cycle[0][1] = std::make_unique<Bitmap>( IDB_CYCLEBLUE_90 );
   g_cycle[0][2] = std::make_unique<Bitmap>( IDB_CYCLEBLUE_180 );
   g_cycle[0][3] = std::make_unique<Bitmap>( IDB_CYCLEBLUE_270 );
   g_cycle[1][0] = std::make_unique<Bitmap>( IDB_CYCLEORANGE_0 );
   g_cycle[1][1] = std::make_unique<Bitmap>( IDB_CYCLEORANGE_90 );
   g_cycle[1][2] = std::make_unique<Bitmap>( IDB_CYCLEORANGE_180 );
   g_cycle[1][3] = std::make_unique<Bitmap>( IDB_CYCLEORANGE_270 );

   NewGame( );
}

void GameEnd( )
{ }

void GameActivate( HWND wnd )
{
   g_game->CaptureJoystick( );
}

void GameDeactivate( HWND wnd )
{
   g_game->ReleaseJoystick( );
}

void GamePaint( HDC dc )
{
   g_background->Draw( dc, 0, 0 );

   for ( int i = 0; i < 2; i++ )
   {
      HPEN pen = CreatePen( PS_SOLID, 5, ( i == 0 ) ? RGB( 0, 0, 255 ) : RGB( 255, 146, 73 ) );
      SelectObject( dc, pen );

      MoveToEx( dc, g_cycleTrail[i][0].x, g_cycleTrail[i][0].y, NULL );

      for ( int j = 1; j < g_trailLength[i]; j++ )
      {
         LineTo( dc, g_cycleTrail[i][j].x, g_cycleTrail[i][j].y );
      }

      DeleteObject( pen );
   }

   int direction[2] = { 0, 0 };

   for ( int i = 0; i < 2; i++ )
   {
      if ( g_cycleSpeed[i].y < 0 )
      {
         direction[i] = 0;
      }
      else if ( g_cycleSpeed[i].x > 0 )
      {
         direction[i] = 1;
      }
      else if ( g_cycleSpeed[i].y > 0 )
      {
         direction[i] = 2;
      }
      else if ( g_cycleSpeed[i].x < 0 )
      {
         direction[i] = 3;
      }
   }

   g_cycle[0][direction[0]]->Draw( dc, g_cyclePos[0].x, g_cyclePos[0].y, TRUE );
   g_cycle[1][direction[1]]->Draw( dc, g_cyclePos[1].x, g_cyclePos[1].y, TRUE );
}

void GameCycle( )
{
   if ( !g_gameOver )
   {
      UpdateCycles( );

      InvalidateRect( g_game->GetWindow( ), NULL, FALSE );
   }
}

void GameMenu( WPARAM wParam )
{
   switch ( LOWORD( wParam ) )
   {
   case IDM_GAME_NEW:
      if ( g_gameOver )
      {
         NewGame( );
      }
      return;

   case IDM_GAME_EXIT:
      DestroyWindow( g_game->GetWindow( ) );
      return;

   case IDM_HELP_ABOUT:
      DialogBoxParamW( g_game->GetInstance( ), MAKEINTRESOURCEW( IDD_ABOUT ), g_game->GetWindow( ), ( DLGPROC ) DlgProc, 0L );
      return;
   }
}

void HandleKeys( )
{
   if ( !g_gameOver )
   {
      if ( GetAsyncKeyState( VK_UP ) < 0 )
      {
         SteerCycle( 0, 0 );
      }
      else if ( GetAsyncKeyState( VK_RIGHT ) < 0 )
      {
         SteerCycle( 0, 1 );
      }
      else if ( GetAsyncKeyState( VK_DOWN ) < 0 )
      {
         SteerCycle( 0, 2 );
      }
      else if ( GetAsyncKeyState( VK_LEFT ) < 0 )
      {
         SteerCycle( 0, 3 );
      }
   }
}

void MouseButtonDown( LONG x, LONG y, BOOL left )
{
   if ( !left && g_gameOver )
   {
      NewGame( );
   }
}

void MouseButtonUp( LONG x, LONG y, BOOL left )
{ }

void MouseMove( LONG x, LONG y )
{ }

void HandleJoystick( JOYSTATE joyState )
{
   if ( !g_gameOver )
   {
      if ( joyState & JOY_UP )
      {
         SteerCycle( 1, 0 );
      }
      else if ( joyState & JOY_RIGHT )
      {
         SteerCycle( 1, 1 );
      }
      else if ( joyState & JOY_DOWN )
      {
         SteerCycle( 1, 2 );
      }
      else if ( joyState & JOY_LEFT )
      {
         SteerCycle( 1, 3 );
      }
   }
   else if ( joyState & JOY_FIRE1 )
   {
      NewGame( );
   }
}

void NewGame( )
{
   g_cyclePos[0].x   = 250 - ( g_cycle[0][0]-> GetWidth( ) / 2 );
   g_cyclePos[0].y   = 400 - g_cycle[0][0]-> GetHeight( );
   g_cycleSpeed[0].x = 0;
   g_cycleSpeed[0].y = -g_SPEED;

   g_cyclePos[1].x   = 250 - ( g_cycle[1][0]->GetWidth( ) / 2 );
   g_cyclePos[1].y   = 0;
   g_cycleSpeed[1].x = 0;
   g_cycleSpeed[1].y = g_SPEED;

   g_trailLength[0]     = g_trailLength[1]     = 2;
   g_cycleTrail[0][0].x = g_cycleTrail[0][1].x = 250;
   g_cycleTrail[0][0].y = g_cycleTrail[0][1].y = 400;
   g_cycleTrail[1][0].x = g_cycleTrail[1][1].x = 250;
   g_cycleTrail[1][0].y = g_cycleTrail[1][1].y = 0;

   g_gameOver = FALSE;

   EnableMenuItem( GetMenu( g_game->GetWindow( ) ),
                   ( UINT ) ( ULONGLONG ) MAKEINTRESOURCEW( IDM_GAME_NEW ), MF_GRAYED );
}

void UpdateCycles( )
{
   for ( int i = 0; i < 2; i++ )
   {
      g_cyclePos[i].x = g_cyclePos[i].x + g_cycleSpeed[i].x;
      g_cyclePos[i].y = g_cyclePos[i].y + g_cycleSpeed[i].y;

      g_cycleTrail[i][g_trailLength[i] - 1].x = g_cyclePos[i].x + ( g_cycle[i][0]->GetWidth( ) / 2 );
      g_cycleTrail[i][g_trailLength[i] - 1].y = g_cyclePos[i].y + ( g_cycle[i][0]->GetHeight( ) / 2 );

      if ( g_cyclePos[i].x < 0 || g_cyclePos[i].x >( 500 - g_cycle[i][0]->GetWidth( ) ) ||
           g_cyclePos[i].y < 0 || g_cyclePos[i].y >( 400 - g_cycle[i][0]->GetHeight( ) ) )
      {
         EndGame( 1 - i );
         return;
      }

      RECT tempTrail = { };

      if ( g_trailLength[i] > 2 )
      {
         for ( int j = 0; j < g_trailLength[i] - 2; j++ )
         {
            tempTrail.left   = min( g_cycleTrail[i][j].x, g_cycleTrail[i][j + 1].x ) - 1;
            tempTrail.right  = max( g_cycleTrail[i][j].x, g_cycleTrail[i][j + 1].x ) + 1;
            tempTrail.top    = min( g_cycleTrail[i][j].y, g_cycleTrail[i][j + 1].y ) - 1;
            tempTrail.bottom = max( g_cycleTrail[i][j].y, g_cycleTrail[i][j + 1].y ) + 1;

            if ( PtInRect( &tempTrail, g_cycleTrail[i][g_trailLength[i] - 1] ) != 0 )
            {
               EndGame( 1 - i );
               return;
            }
         }
      }

      for ( int j = 0; j <= g_trailLength[1 - i] - 2; j++ )
      {
         tempTrail.left   = min( g_cycleTrail[1 - i][j].x, g_cycleTrail[1 - i][j + 1].x ) - 3;
         tempTrail.right  = max( g_cycleTrail[1 - i][j].x, g_cycleTrail[1 - i][j + 1].x ) + 3;
         tempTrail.top    = min( g_cycleTrail[1 - i][j].y, g_cycleTrail[1 - i][j + 1].y ) - 3;
         tempTrail.bottom = max( g_cycleTrail[1 - i][j].y, g_cycleTrail[1 - i][j + 1].y ) + 3;

         if ( PtInRect( &tempTrail, g_cycleTrail[i][g_trailLength[i] - 1] ) != 0 )
         {
            EndGame( 1 - i );
            return;
         }
      }
   }
}

void SteerCycle( LONG cycle, LONG direction )
{
   POINT oldSpeed = { };

   oldSpeed.x = g_cycleSpeed[cycle].x;
   oldSpeed.y = g_cycleSpeed[cycle].y;

   switch ( direction )
   {
   case 0:
      if ( 0 == g_cycleSpeed[cycle].y )
      {
         g_cycleSpeed[cycle].x = 0;
         g_cycleSpeed[cycle].y = -g_SPEED;
      }
      break;

   case 1:
      if ( 0 == g_cycleSpeed[cycle].x )
      {
         g_cycleSpeed[cycle].x = g_SPEED;
         g_cycleSpeed[cycle].y = 0;
      }
      break;

   case 2:
      if ( 0 == g_cycleSpeed[cycle].y )
      {
         g_cycleSpeed[cycle].x = 0;
         g_cycleSpeed[cycle].y = g_SPEED;
      }
      break;

   case 3:
      if ( 0 == g_cycleSpeed[cycle].x )
      {
         g_cycleSpeed[cycle].x = -g_SPEED;
         g_cycleSpeed[cycle].y = 0;
      }
      break;
   }

   if ( ( g_cycleSpeed[cycle].x != oldSpeed.x ) || ( g_cycleSpeed[cycle].y != oldSpeed.y ) )
   {
      g_trailLength[cycle]++;

      g_cycleTrail[cycle][g_trailLength[cycle] - 1].x = g_cyclePos[cycle].x + ( g_cycle[cycle][0]->GetWidth( ) / 2 );
      g_cycleTrail[cycle][g_trailLength[cycle] - 1].y = g_cyclePos[cycle].y + ( g_cycle[cycle][0]->GetHeight( ) / 2 );
   }
}

void EndGame( LONG cycle )
{
   g_gameOver = TRUE;

   EnableMenuItem( GetMenu( g_game->GetWindow( ) ),
                   ( UINT ) ( ULONGLONG ) MAKEINTRESOURCEW( IDM_GAME_NEW ), MF_ENABLED );

   if ( 0 == cycle )
   {
      MessageBoxW( g_game->GetWindow( ), L"Blue wins!", L"Light Cycles", MB_OK );
   }
   else
   {
      MessageBoxW( g_game->GetWindow( ), L"Orange wins!", L"Light Cycles", MB_OK );
   }
}
