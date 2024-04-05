#include "Brainiac.h"

BOOL GameInitialize( HINSTANCE hInstance )
{
   _pGame = new GameEngine( hInstance, TEXT( "Brainiac 2" ),
                            TEXT( "Example Game: Brainiac 2" ), IDI_BRAINIAC, IDI_BRAINIAC_SM, 528, 508 );
   if ( _pGame == NULL )
      return FALSE;

   _pGame->SetFrameRate( 1 );

   _hInstance = hInstance;

   return TRUE;
}

void GameStart( HWND hWindow )
{
   srand( GetTickCount( ) );

   HDC hDC = GetDC( hWindow );
   _pTiles[0] = new Bitmap( hDC, IDB_TILEBLANK, _hInstance );
   _pTiles[1] = new Bitmap( hDC, IDB_TILE1, _hInstance );
   _pTiles[2] = new Bitmap( hDC, IDB_TILE2, _hInstance );
   _pTiles[3] = new Bitmap( hDC, IDB_TILE3, _hInstance );
   _pTiles[4] = new Bitmap( hDC, IDB_TILE4, _hInstance );
   _pTiles[5] = new Bitmap( hDC, IDB_TILE5, _hInstance );
   _pTiles[6] = new Bitmap( hDC, IDB_TILE6, _hInstance );
   _pTiles[7] = new Bitmap( hDC, IDB_TILE7, _hInstance );
   _pTiles[8] = new Bitmap( hDC, IDB_TILE8, _hInstance );

   for ( int i = 0; i < 4; i++ )
      for ( int j = 0; j < 4; j++ )
      {
         _bTileStates[i][j] = FALSE;
         _iTiles[i][j] = 0;
      }

   for ( int i = 0; i < 2; i++ )
      for ( int j = 1; j < 9; j++ )
      {
         int x = rand( ) % 4;
         int y = rand( ) % 4;
         while ( _iTiles[x][y] != 0 )
         {
            x = rand( ) % 4;
            y = rand( ) % 4;
         }
         _iTiles[x][y] = j;
      }

   _ptTile1.x = _ptTile1.y = -1;
   _ptTile2.x = _ptTile2.y = -1;
   _iMatches = _iTries = 0;
}

void GameEnd( )
{
   for ( int i = 0; i < 9; i++ )
      delete _pTiles[i];

   delete _pGame;
}

void GameActivate( HWND hWindow )
{ }

void GameDeactivate( HWND hWindow )
{ }

void GamePaint( HDC hDC )
{
   int iTileWidth = _pTiles[0]->GetWidth( );
   int iTileHeight = _pTiles[0]->GetHeight( );
   for ( int i = 0; i < 4; i++ )
      for ( int j = 0; j < 4; j++ )
         if ( _bTileStates[i][j] || ( ( i == _ptTile1.x ) && ( j == _ptTile1.y ) ) ||
              ( ( i == _ptTile2.x ) && ( j == _ptTile2.y ) ) )
            _pTiles[_iTiles[i][j]]->Draw( hDC, i * iTileWidth, j * iTileHeight,
                                          TRUE );
         else
            _pTiles[0]->Draw( hDC, i * iTileWidth, j * iTileHeight, TRUE );
}

void GameCycle( )
{ }

void HandleKeys( )
{ }

void MouseButtonDown( int x, int y, BOOL bLeft )
{
   int iTileX = x / _pTiles[0]->GetWidth( );
   int iTileY = y / _pTiles[0]->GetHeight( );

   if ( !_bTileStates[iTileX][iTileY] )
   {
      if ( _ptTile1.x == -1 )
      {
         PlaySound( ( PCTSTR ) IDW_SELECT, _hInstance, SND_ASYNC | SND_RESOURCE );

         _ptTile1.x = iTileX;
         _ptTile1.y = iTileY;
      }
      else if ( ( iTileX != _ptTile1.x ) || ( iTileY != _ptTile1.y ) )
      {
         if ( _ptTile2.x == -1 )
         {
            PlaySound( ( PCTSTR ) IDW_SELECT, _hInstance, SND_ASYNC | SND_RESOURCE );

            _iTries++;

            _ptTile2.x = iTileX;
            _ptTile2.y = iTileY;

            if ( _iTiles[_ptTile1.x][_ptTile1.y] == _iTiles[_ptTile2.x][_ptTile2.y] )
            {
               PlaySound( ( PCTSTR ) IDW_MATCH, _hInstance, SND_ASYNC | SND_RESOURCE );

               _bTileStates[_ptTile1.x][_ptTile1.y] = TRUE;
               _bTileStates[_ptTile2.x][_ptTile2.y] = TRUE;

               _ptTile1.x = _ptTile1.y = _ptTile2.x = _ptTile2.y = -1;

               if ( ++_iMatches == 8 )
               {
                  PlaySound( ( PCTSTR ) IDW_WIN, _hInstance, SND_ASYNC | SND_RESOURCE );
                  TCHAR szText[64];
                  wsprintf( szText, TEXT( "You won in %d tries." ), _iTries );
                  MessageBox( _pGame->GetWindow( ), szText, TEXT( "Brainiac" ), MB_OK );
               }
            }
            else
               PlaySound( ( PCTSTR ) IDW_MISMATCH, _hInstance, SND_ASYNC | SND_RESOURCE );
         }
         else
         {
            _ptTile1.x = _ptTile1.y = _ptTile2.x = _ptTile2.y = -1;
         }
      }

      InvalidateRect( _pGame->GetWindow( ), NULL, FALSE );
   }
}

void MouseButtonUp( int x, int y, BOOL bLeft )
{ }

void MouseMove( int x, int y )
{ }

void HandleJoystick( JOYSTATE jsJoystickState )
{ }

BOOL SpriteCollision( Sprite* pSpriteHitter, Sprite* pSpriteHittee )
{
   return FALSE;
}
