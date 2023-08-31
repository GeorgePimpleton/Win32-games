#include "Light Cycles.hpp"

HRESULT GameInitialize(HINSTANCE inst)
{
   g_game = std::make_unique<GameEngine>(inst, L"Light Cycles", L"Light Cycles",
                                         IDI_ICON, IDI_ICON_SM, 500, 400);

   if ( nullptr == g_game )
   {
      return E_FAIL;
   }

   g_game->SetFrameRate(30);

   g_game->InitJoystick( );

   return S_OK;
}

void GameStart(HWND wnd)
{
   // create and load the background and light cycle bitmaps
   HINSTANCE inst = GetModuleHandleW(nullptr);

   g_background      = std::make_unique<Bitmap>(IDB_BACKGROUND, inst);
   g_cycle[ 0 ][ 0 ] = std::make_unique<Bitmap>(IDB_CYCLEBLUE_0, inst);
   g_cycle[ 0 ][ 1 ] = std::make_unique<Bitmap>(IDB_CYCLEBLUE_90, inst);
   g_cycle[ 0 ][ 2 ] = std::make_unique<Bitmap>(IDB_CYCLEBLUE_180, inst);
   g_cycle[ 0 ][ 3 ] = std::make_unique<Bitmap>(IDB_CYCLEBLUE_270, inst);
   g_cycle[ 1 ][ 0 ] = std::make_unique<Bitmap>(IDB_CYCLEORANGE_0, inst);
   g_cycle[ 1 ][ 1 ] = std::make_unique<Bitmap>(IDB_CYCLEORANGE_90, inst);
   g_cycle[ 1 ][ 2 ] = std::make_unique<Bitmap>(IDB_CYCLEORANGE_180, inst);
   g_cycle[ 1 ][ 3 ] = std::make_unique<Bitmap>(IDB_CYCLEORANGE_270, inst);

   // start a new game
   NewGame( );
}

void GameEnd( )
{ }

void GameActivate(HWND hwnd)
{
   g_game->CaptureJoystick( );
}

void GameDeactivate(HWND hwnd)
{
   g_game->ReleaseJoystick( );
}

void GamePaint(HDC dc)
{
   g_background->Draw(dc, 0, 0);

   // draw the light cycle trails
   for ( int i { }; i < 2; i++ )
   {
      // create a blue/orange pen depending on which trail is being drawn
      HPEN hPen { CreatePen(PS_SOLID, 5, (i == 0) ? RGB(0, 0, 255) : RGB(255, 146, 73)) };
      SelectObject(dc, hPen);

      // start at the first point in the trail
      MoveToEx(dc, g_cycleTrail[ i ][ 0 ].x, g_cycleTrail[ i ][ 0 ].y, nullptr);

      // draw a line to each of the remaining points
      for ( int j { 1 }; j < g_trailLength[ i ]; j++ )
      {
         LineTo(dc, g_cycleTrail[ i ][ j ].x, g_cycleTrail[ i ][ j ].y);
      }

      // clean up the pen
      DeleteObject(hPen);
   }

   // determine the directions of the light cycles
   int iDirection[ 2 ] = { 0, 0 };

   for ( int i = 0; i < 2; i++ )
   {
      if ( g_cycleSpeed[ i ].y < 0 )
      {
         iDirection[ i ] = 0;
      }
      else if ( g_cycleSpeed[ i ].x > 0 )
      {
         iDirection[ i ] = 1;
      }
      else if ( g_cycleSpeed[ i ].y > 0 )
      {
         iDirection[ i ] = 2;
      }
      else if ( g_cycleSpeed[ i ].x < 0 )
      {
         iDirection[ i ] = 3;
      }
   }

   // draw the light cycles
   g_cycle[ 0 ][ iDirection[ 0 ] ]->Draw(dc, g_cyclePos[ 0 ].x, g_cyclePos[ 0 ].y, TRUE);
   g_cycle[ 1 ][ iDirection[ 1 ] ]->Draw(dc, g_cyclePos[ 1 ].x, g_cyclePos[ 1 ].y, TRUE);
}

void GameCycle( )
{
   if ( !g_gameOver )
   {
      // move the light cycles
      UpdateCycles( );

      InvalidateRect(g_game->GetWindow( ), nullptr, FALSE);
   }
}

void GameMenu(WPARAM wParam)
{
   switch ( LOWORD(wParam) )
   {
   case IDM_GAME_NEW:
      // starts a new game
      if ( g_gameOver )
      {
         NewGame( );
      }
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
   if ( !g_gameOver )
   {
      // steer the blue light cycle in response to arrow key presses
      if ( GetAsyncKeyState(VK_UP) < 0 )
      {
         SteerCycle(0, 0);
      }
      else if ( GetAsyncKeyState(VK_RIGHT) < 0 )
      {
         SteerCycle(0, 1);
      }
      else if ( GetAsyncKeyState(VK_DOWN) < 0 )
      {
         SteerCycle(0, 2);
      }
      else if ( GetAsyncKeyState(VK_LEFT) < 0 )
      {
         SteerCycle(0, 3);
      }
   }
   // removed this so pressing enter after the message box displays
   // doesn't start a new game ::sad::
   //else if (GetAsyncKeyState(VK_RETURN) < 0)
   //{
   //   NewGame();
   //}
}

void MouseButtonDown(LONG x, LONG y, BOOL left)
{ }

void MouseButtonUp(LONG x, LONG y, BOOL left)
{ }

void MouseMove(LONG x, LONG y)
{ }

void HandleJoystick(JOYSTATE joyState)
{
   if ( !g_gameOver )
   {
      // steer the orange light cycle in response to joystick moves
      if ( joyState & JOY_UP )
      {
         SteerCycle(1, 0);
      }
      else if ( joyState & JOY_RIGHT )
      {
         SteerCycle(1, 1);
      }
      else if ( joyState & JOY_DOWN )
      {
         SteerCycle(1, 2);
      }
      else if ( joyState & JOY_LEFT )
      {
         SteerCycle(1, 3);
      }
   }
   else if ( joyState & JOY_FIRE1 )
   {
      NewGame( );
   }
}

// specific game functions
void NewGame( )
{
   // set the initial blue light cycle position and speed
   g_cyclePos[ 0 ].x   = 250 - (g_cycle[ 0 ][ 0 ]-> GetWidth( ) / 2);
   g_cyclePos[ 0 ].y   = 400 - g_cycle[ 0 ][ 0 ]-> GetHeight( );
   g_cycleSpeed[ 0 ].x = 0;
   g_cycleSpeed[ 0 ].y = -g_SPEED;

   // set the initial orange light cycle position and speed
   g_cyclePos[ 1 ].x   = 250 - (g_cycle[ 1 ][ 0 ]-> GetWidth( ) / 2);
   g_cyclePos[ 1 ].y   = 0;
   g_cycleSpeed[ 1 ].x = 0;
   g_cycleSpeed[ 1 ].y = g_SPEED;

   // set the light cycle trail lengths and initial points
   g_trailLength[ 0 ]       = g_trailLength[ 1 ] = 2;
   g_cycleTrail[ 0 ][ 0 ].x = g_cycleTrail[ 0 ][ 1 ].x = 250;
   g_cycleTrail[ 0 ][ 0 ].y = g_cycleTrail[ 0 ][ 1 ].y = 400;
   g_cycleTrail[ 1 ][ 0 ].x = g_cycleTrail[ 1 ][ 1 ].x = 250;
   g_cycleTrail[ 1 ][ 0 ].y = g_cycleTrail[ 1 ][ 1 ].y = 0;

   // start the game
   g_gameOver = FALSE;
}

void UpdateCycles( )
{
   for ( int i = 0; i < 2; i++ )
   {
      // update the light cycle position based on its speed
      g_cyclePos[ i ].x = g_cyclePos[ i ].x + g_cycleSpeed[ i ].x;
      g_cyclePos[ i ].y = g_cyclePos[ i ].y + g_cycleSpeed[ i ].y;

      // update the light cycle trail based on its new position
      g_cycleTrail[ i ][ g_trailLength[ i ] - 1 ].x = g_cyclePos[ i ].x + (g_cycle[ i ][ 0 ]->GetWidth( ) / 2);
      g_cycleTrail[ i ][ g_trailLength[ i ] - 1 ].y = g_cyclePos[ i ].y + (g_cycle[ i ][ 0 ]->GetHeight( ) / 2);

      // see if the light cycle ran into the edge of the screen
      if ( g_cyclePos[ i ].x < 0 || g_cyclePos[ i ].x >(500 - g_cycle[ i ][ 0 ]->GetWidth( )) ||
           g_cyclePos[ i ].y < 0 || g_cyclePos[ i ].y >(400 - g_cycle[ i ][ 0 ]->GetHeight( )) )
      {
         // the game is over
         EndGame(1 - i);
         return;
      }

      // see if the light cycle collided with its own trail
      RECT tempTrail;

      if ( g_trailLength[ i ] > 2 ) // Must have steered at least once
      {
         for ( int j = 0; j < g_trailLength[ i ] - 2; j++ )
         {
            tempTrail.left   = min(g_cycleTrail[ i ][ j ].x, g_cycleTrail[ i ][ j + 1 ].x) - 1;
            tempTrail.right  = max(g_cycleTrail[ i ][ j ].x, g_cycleTrail[ i ][ j + 1 ].x) + 1;
            tempTrail.top    = min(g_cycleTrail[ i ][ j ].y, g_cycleTrail[ i ][ j + 1 ].y) - 1;
            tempTrail.bottom = max(g_cycleTrail[ i ][ j ].y, g_cycleTrail[ i ][ j + 1 ].y) + 1;

            if ( PtInRect(&tempTrail, g_cycleTrail[ i ][ g_trailLength[ i ] - 1 ]) != 0 )
            {
               // the game is over
               EndGame(1 - i);
               return;
            }
         }
      }

      // see if the light cycle collided with the other cycle's trail
      for ( int j = 0; j <= g_trailLength[ 1 - i ] - 2; j++ )
      {
         tempTrail.left   = min(g_cycleTrail[ 1 - i ][ j ].x, g_cycleTrail[ 1 - i ][ j + 1 ].x) - 3;
         tempTrail.right  = max(g_cycleTrail[ 1 - i ][ j ].x, g_cycleTrail[ 1 - i ][ j + 1 ].x) + 3;
         tempTrail.top    = min(g_cycleTrail[ 1 - i ][ j ].y, g_cycleTrail[ 1 - i ][ j + 1 ].y) - 3;
         tempTrail.bottom = max(g_cycleTrail[ 1 - i ][ j ].y, g_cycleTrail[ 1 - i ][ j + 1 ].y) + 3;

         if ( PtInRect(&tempTrail, g_cycleTrail[ i ][ g_trailLength[ i ] - 1 ]) != 0 )
         {
            // the game is over
            EndGame(1 - i);
            return;
         }
      }
   }
}

void SteerCycle(int cycle, int direction)
{
   // remember the old light cycle speed
   POINT oldSpeed;

   oldSpeed.x = g_cycleSpeed[ cycle ].x;
   oldSpeed.y = g_cycleSpeed[ cycle ].y;

   // change the speed of the light cycle to steer it
   switch ( direction )
   {
   case 0: // up (0 degrees)
      if ( 0 == g_cycleSpeed[ cycle ].y )
      {
         g_cycleSpeed[ cycle ].x = 0;
         g_cycleSpeed[ cycle ].y = -g_SPEED;
      }
      break;

   case 1: // right (90 degrees)
      if ( 0 == g_cycleSpeed[ cycle ].x )
      {
         g_cycleSpeed[ cycle ].x = g_SPEED;
         g_cycleSpeed[ cycle ].y = 0;
      }
      break;

   case 2: // down (180 degrees)
      if ( 0 == g_cycleSpeed[ cycle ].y )
      {
         g_cycleSpeed[ cycle ].x = 0;
         g_cycleSpeed[ cycle ].y = g_SPEED;
      }
      break;

   case 3: // left (270 degrees)
      if ( 0 == g_cycleSpeed[ cycle ].x )
      {
         g_cycleSpeed[ cycle ].x = -g_SPEED;
         g_cycleSpeed[ cycle ].y = 0;
      }
      break;
   }

   // if the speed changed, move to a new point in the light cycle trail
   if ( (g_cycleSpeed[ cycle ].x != oldSpeed.x) || (g_cycleSpeed[ cycle ].y != oldSpeed.y) )
   {
      // increment the number of trail points
      g_trailLength[ cycle ]++;

      // set the initial position of the new trail point
      g_cycleTrail[ cycle ][ g_trailLength[ cycle ] - 1 ].x = g_cyclePos[ cycle ].x + (g_cycle[ cycle ][ 0 ]->GetWidth( ) / 2);
      g_cycleTrail[ cycle ][ g_trailLength[ cycle ] - 1 ].y = g_cyclePos[ cycle ].y + (g_cycle[ cycle ][ 0 ]->GetHeight( ) / 2);
   }
}

void EndGame(int cycle)
{
   // set the game over flag
   g_gameOver = TRUE;

   // display a message about the winner
   if ( 0 == cycle )
   {
      MessageBoxW(g_game->GetWindow( ), L"Blue wins!", L"Light Cycles", MB_OK);
   }
   else
   {
      MessageBox(g_game->GetWindow( ), L"Orange wins!", L"Light Cycles", MB_OK);
   }
}