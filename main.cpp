#include <windows.h>
#include <SFML/Graphics.hpp>

#include <sstream>
#include <fstream>
#include <string>
#include<vector>

#include "buttonRectImage.h"
#include "buttonRectDrawn.h"
#include "dragDropRect.h"
#include "linLeg.h"
#include "Path.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
HDC hdc;
int mseX=0, mseY=0;
int w_szX = 800, w_szY = 600;
int w_posX = 50, w_posY = 50;

const int& button::r_mseX = mseX;
const int& button::r_mseY = mseY;
const int& dragDrop::r_mseX = mseX;
const int& dragDrop::r_mseY = mseY;

sf::Sprite buttSprite1;
sf::Image buttImg1;
buttonRectImage newButt('m', 50, 100, NULL);
buttonRectImage savedButt('m', 50, 170, NULL);
buttonRectImage testButt('m', 50, 240, NULL);
buttonRectImage saveGameButt('m', 50, 310, NULL);


sf::Sprite wh_chSprite;
sf::Image wh_chImg;
sf::Sprite wh_kSprite;
sf::Image wh_kingImg;

sf::Sprite bk_chSprite;
sf::Image bk_chImg;
sf::Sprite bk_kSprite;
sf::Image bk_kingImg;

dragDropRect chObj(200, 200, NULL);


const int Nanchors = 32;
std::pair<int, int> anchorArr1[Nanchors];
char checkerPos[32];

int Nmoves = 5;
int moveIdxList[5];

sf::Sprite boardSprite;
sf::Image boardImg;
int ch_bdPosX = 150;
int ch_bdPosY = 50;
int ch_bdOffX = 44;
int ch_bdOffY = 43;
float sqrSzX = 51.75f;
float sqrSzY = 50.f;
int chSz = 40;


bool turn = true;
bool jumpMade = false, canJump = false;
bool reset = false;
bool testMode = false;
bool gameOn = false;
bool gameOver = false;
bool pause = false;
bool takeScreenshot = false;


bool LoadImages(void);
void INITbuttons(void);
void INITboard(void);
bool INITcheckerPositions_NEW(void);
bool INITcheckerPositions_SAVED(const char* fname);
void INITdragDrops(void);
void INITtext(void);
bool saveGameToFile(const char* fname);
void drawCheckers(sf::RenderWindow& rApp);

void gameLogic(void);
void gameDraw(sf::RenderWindow& rApp);
void gameHitDown(void);
void gameHitUp(void);
void gameCapture_SplitAniLogic(void);
void gameKingMeAniLogic(void);
bool menuHitDown(void);
void menuHitUp(void);

int hitAnchor(bool Turn);
bool fillMoveList(int idx);
bool didJump( int hm_0, int hm_f, int& jumpIdx );


Path wh_dealPath( 12, 50, 3, 400.0f, -40.0f );
Path bk_dealPath( 12, 50, 3, 400.0f, 620.0f );
void ALLOC_paths(void);
void INITpathLegs(void);

sf::String msg;
sf::String msg_Turn;
sf::String msg_Whites;
sf::String msg_Blacks;
sf::String msg_WELCOME;
sf::String msg_GAMEOVER;
sf::String msg_WHITEWINS;
sf::String msg_BLACKWINS;
sf::String* p_FXmsg = &msg_WELCOME;
int textFXdelay = 30;
bool textFX = true;
bool textExpand( sf::String& r_text, float scale_i, float scale_f, int Nframes, bool reset = false );
bool textFade( sf::String& r_text, int alpha_i, int alpha_f, int Nframes, bool reset = false );


bool animateCapt = false;
char colorCapt;
float wh_captX = 150.0f, wh_captY = 550.0f;
float bk_captX = 150.0f, bk_captY = 5.0f;
float capt_dx = 50.0f;
int Nwh_capt = 0, Nbk_capt = 0;
Path aniPath( 1, 0, 0, 100.0f, 100.0f );
linLeg* p_aniPathLeg = NULL;

Path kingMePath( 1, 0, 0, 100.0f, 100.0f );
linLeg* p_kingMePathLeg = NULL;


bool justKinged = false;
bool animateSplit = false;
bool animateKing = false;

template<class T>
float to_sf_string(sf::String& strArg, T x)
{
    std::stringstream ss;
    ss << x;
    std::string tempStr;
    ss >> tempStr;
    strArg.SetText( tempStr.c_str() );
    return strArg.GetRect().Right;
}


int APIENTRY WinMain(HINSTANCE hinst,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	HWND hwnd;
	MSG  lpmsg;
	memset(&lpmsg,0,sizeof(lpmsg));
	WNDCLASSEX wc = {0};
	static char szAppName[] = "Shell";

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpszClassName = szAppName;
	wc.hInstance = hinst;
	wc.lpfnWndProc = WndProc;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL,IDI_APPLICATION);
	wc.lpszMenuName = 0;
	wc.hbrBackground =  reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.style = CS_GLOBALCLASS;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	if( !RegisterClassEx (&wc) )
		return 0;

    
	if( !LoadImages() )
		return 0;

	
	hwnd = CreateWindow(szAppName, "Dereks checkers game", WS_SYSMENU | WS_VISIBLE | WS_MINIMIZEBOX,
		w_posX, w_posY, w_szX + 6, w_szY + 32, NULL, NULL, hinst, NULL);

	
	DWORD Style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS;
	HWND  View1 = CreateWindow( "STATIC", NULL, Style, 0,  0, w_szX, w_szY, hwnd, NULL, hinst, NULL);

	
	sf::RenderWindow App(View1);
	float frPeriod = 1.0f/30.0f;
	sf::Clock frClock; frClock.Reset();

	
	button::pWndw = &App;
    dragDrop::pWndw = &App;
    Path::pWndw = &App;
	INITboard();
    ALLOC_paths();
	INITbuttons();
	INITtext();
	INITdragDrops();
	wh_dealPath.inUse = false;
	bk_dealPath.inUse = false;

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	App.UseVerticalSync(true);

	while( lpmsg.message != WM_QUIT )
	{
		if( PeekMessage( &lpmsg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &lpmsg );
			DispatchMessage( &lpmsg );
		}
        else
		{
		    
		    if( !pause && frClock.GetElapsedTime() > frPeriod )
            {
                frClock.Reset();
                gameLogic();
            }

			
			App.Clear( sf::Color(0, 50, 100) );

			gameDraw( App );

			App.Display();
		}
	}
	UnregisterClass( szAppName, wc.hInstance );

	return(lpmsg.wParam);
 	
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    char ch = 'b';

	switch(msg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_LBUTTONDOWN:
            if( !menuHitDown() )
                PostQuitMessage(0);

            gameHitDown();

			break;
		case WM_LBUTTONUP:
            menuHitUp();
            gameHitUp();

			break;
		case WM_MOUSEMOVE:
            mseX = LOWORD (lParam);
			mseY = HIWORD (lParam);

			
            newButt.mseOver();
            savedButt.mseOver();
            testButt.mseOver();

            if( gameOn )
                saveGameButt.mseOver();

            
            chObj.drag();

			break;
        case WM_CHAR:
            ch = (TCHAR)wParam;

			ch = tolower(ch);
			switch( ch )
			{
				case VK_ESCAPE:
				
					PostQuitMessage( 0 );
					break;
                case 'p':
					pause = !pause;
					break;
                case 's':
					takeScreenshot = true;
					break;

                default:
					break;
			}
		default:
			return( DefWindowProc(hwnd, msg, wParam, lParam) );
			break;
	}
	return(0);
}

bool LoadImages(void)
{
    
    if( !buttImg1.LoadFromFile( "images/buttonsImageAlt.png" ) )
        return false;
	buttImg1.CreateMaskFromColor( sf::Color(0,0,0) );
	buttSprite1.SetImage( buttImg1 );

	
	if( !wh_chImg.LoadFromFile( "images/white_checker.png" ) )
        return false;
	wh_chImg.CreateMaskFromColor( sf::Color(0,0,0) );
	wh_chSprite.SetImage( wh_chImg );

	if( !wh_kingImg.LoadFromFile( "images/white_checker_King.png" ) )
        return false;
	wh_kingImg.CreateMaskFromColor( sf::Color(0,0,0) );
	wh_kSprite.SetImage( wh_kingImg );


	if( !bk_chImg.LoadFromFile( "images/black_checker.png" ) )
        return false;
	bk_chImg.CreateMaskFromColor( sf::Color(0,0,0) );
	bk_chSprite.SetImage( bk_chImg );

	if( !bk_kingImg.LoadFromFile( "images/black_checker_King.png" ) )
        return false;
	bk_kingImg.CreateMaskFromColor( sf::Color(0,0,0) );
	bk_kSprite.SetImage( bk_kingImg );

	
	if( !boardImg.LoadFromFile( "images/checker_board.jpg" ) )
        return false;
	boardSprite.SetImage( boardImg );
	boardSprite.SetPosition( static_cast<float>(ch_bdPosX), static_cast<float>(ch_bdPosY) );

	return true;
}

void INITbuttons(void)
{
    newButt.assign_Sprite( &buttSprite1 );
    newButt.INITlabel( "start new game", 12, sf::Color(0,255,0), 'b' );

    savedButt.assign_Sprite( &buttSprite1 );
    savedButt.INITlabel( "load saved game", 12, sf::Color(0,255,0), 'b' );

    testButt.assign_Sprite( &buttSprite1 );
    testButt.INITlabel( "load test game", 12, sf::Color(0,255,0), 'b' );

    saveGameButt.assign_Sprite( &buttSprite1 );
    saveGameButt.INITlabel( "save this game", 12, sf::Color(0,255,0), 'b' );

    return;
}

void INITboard(void)
{
    
    
    

    sqrSzX += 0.7f;
    sqrSzY += 0.7f;

    
    int x0 = ch_bdPosX + ch_bdOffX + 26;
    int y0 = ch_bdPosY + ch_bdOffY + 25;
    for(int i=0; i<Nanchors; ++i)
    {
        if( (i/4)%2 )
            anchorArr1[i].first = x0 + (int)( sqrSzX*( 2*(i%4) ) );
        else
            anchorArr1[i].first =  x0 + (int)( sqrSzX*( 1 + 2*(i%4) ) );

        anchorArr1[i].second = y0 + (int)( sqrSzY*(i/4) );
    }

    return;
}

bool INITcheckerPositions_NEW(void)
{
    int i=0;

    
    for(i=0; i<=11; ++i)
        checkerPos[i] = 'w';

    for(i=12; i<=19; ++i)
        checkerPos[i] = 'n';

    for(i=20; i<=31; ++i)
        checkerPos[i] = 'b';

    turn = true;
    Nwh_capt = Nbk_capt = 0;

    return true;
}

bool INITcheckerPositions_SAVED( const char* fname )
{
    int i=0;

    
    for(i=0; i<32; ++i)
        checkerPos[i] = 'n';

   std::ifstream infile( fname );
   int Nwh_reg=0, Nwh_k=0;
   int Nbk_reg=0, Nbk_k=0;
   int Turn=0;
   int idx=0;

	if(infile)
	{
	    
		infile >> Nwh_reg >> Nwh_k >> Nbk_reg >> Nbk_k >> Turn;
		for(i=0; i<Nwh_reg; ++i)
		{
		    infile >> idx;
		    checkerPos[idx] = 'w';
		}
		for(i=0; i<Nwh_k; ++i)
		{
		    infile >> idx;
		    checkerPos[idx] = 'W';
		}

		
		for(i=0; i<Nbk_reg; ++i)
		{
		    infile >> idx;
		    checkerPos[idx] = 'b';
		}
		for(i=0; i<Nbk_k; ++i)
		{
		    infile >> idx;
		    checkerPos[idx] = 'B';
		}

		Nwh_capt = 12 - Nwh_reg - 2*Nwh_k;
		Nbk_capt = 12 - Nbk_reg - 2*Nbk_k;
		if( Turn )
            turn = true;
        else
            turn = false;

		infile.close();
		return true;
	}

    return false;;
}

bool saveGameToFile(const char* fname)
{
    int i=0;
    int Nwh_reg=0, Nwh_k=0;
    int Nbk_reg=0, Nbk_k=0;

    for(i=0; i<32; ++i)
        switch( checkerPos[i] )
        {
            case 'w': ++Nwh_reg; break;
            case 'W': ++Nwh_k; break;
            case 'b': ++Nbk_reg; break;
            case 'B': ++Nbk_k; break;
            default: break;
        }

    std::ofstream fout( fname );
    if( fout )
	{
	    
		fout << Nwh_reg << " " << Nwh_k << " " << Nbk_reg << " " << Nbk_k << " ";
		if( turn )
            fout << 1 << '\n';
        else
            fout << 0 << '\n';

		for(i=0; i<32; ++i)
            if( checkerPos[i] == 'w' )
                fout << i << " ";
		fout << '\n';

		for(i=0; i<32; ++i)
            if( checkerPos[i] == 'W' )
                fout << i << " ";
		fout << '\n';

		for(i=0; i<32; ++i)
            if( checkerPos[i] == 'b' )
                fout << i << " ";
		fout << '\n';

		for(i=0; i<32; ++i)
            if( checkerPos[i] == 'B' )
                fout << i << " ";
		fout << '\n';

		fout.close();
		return true;
	}

    return false;
}

void INITdragDrops(void)
{
    
    chObj.set_anchors(anchorArr1, Nanchors, 0);
    chObj.assign_Size( &wh_chSprite );

    chSz = chObj.szX;

    return;
}

void INITtext(void)
{
    msg.SetText( "Animating checkers" );
    msg.SetPosition( 300.0f, 30.0f );
    msg.SetFont( sf::Font::GetDefaultFont() );
	msg.SetColor( sf::Color(255,255,255) );
	msg.SetSize( 12.0 );

    msg_Whites.SetText( "WHITES" );
    msg_Whites.SetPosition( 700.0f, 280.0f );
    msg_Whites.SetFont( sf::Font::GetDefaultFont() );
	msg_Whites.SetColor( sf::Color(255,255,255) );
	msg_Whites.SetSize( 14.0 );

    msg_Blacks.SetText( "BLACKS" );
    msg_Blacks.SetPosition( 700.0f, 280.0f );
    msg_Blacks.SetFont( sf::Font::GetDefaultFont() );
	msg_Blacks.SetColor( sf::Color(255,255,255) );
	msg_Blacks.SetSize( 14.0 );

	msg_Turn.SetText( "turn" );
	msg_Turn.SetPosition( 720.0f, 295.0f );
    msg_Turn.SetFont( sf::Font::GetDefaultFont() );
	msg_Turn.SetColor( sf::Color(255,255,255) );
	msg_Turn.SetSize( 12.0 );

	msg_WELCOME.SetText( "WELCOME" );
	msg_WELCOME.SetFont( sf::Font::GetDefaultFont() );
	msg_WELCOME.SetColor( sf::Color(0,100,200) );
	msg_WELCOME.SetPosition( 320.0f, 255.0f );

	msg_GAMEOVER.SetText( "GAME OVER" );
	msg_GAMEOVER.SetFont( sf::Font::GetDefaultFont() );
	msg_GAMEOVER.SetColor( sf::Color(0,100,200) );
	msg_GAMEOVER.SetPosition( 320.0f, 255.0f );

	msg_WHITEWINS.SetText( "WHITE WINS!" );
	msg_WHITEWINS.SetFont( sf::Font::GetDefaultFont() );
	msg_WHITEWINS.SetColor( sf::Color(0,100,200) );
	msg_WHITEWINS.SetPosition( 320.0f, 255.0f );

	msg_BLACKWINS.SetText( "BLACK WINS!" );
	msg_BLACKWINS.SetFont( sf::Font::GetDefaultFont() );
	msg_BLACKWINS.SetColor( sf::Color(0,100,200) );
	msg_BLACKWINS.SetPosition( 320.0f, 255.0f );
}

void ALLOC_paths(void)
{
    int i=0;


    
    
    for(i=0; i<wh_dealPath.nLegs; ++i)
    {
        wh_dealPath.ppLeg[i] = new linLeg;
        wh_dealPath.ppSprite[i] = &wh_chSprite;
    }

    for(i=0; i<bk_dealPath.nLegs; ++i)
    {
        bk_dealPath.ppLeg[i] = new linLeg;
        bk_dealPath.ppSprite[i] = &bk_chSprite;
    }

    
    p_aniPathLeg = new linLeg;
    *aniPath.ppLeg = p_aniPathLeg;

    p_kingMePathLeg = new linLeg;
    *kingMePath.ppLeg = p_kingMePathLeg;

    return;
}


void INITpathLegs(void)
{
    int i=0;
    int Nwh_ch=0, Nbk_ch=0;
    int Nwh_k=0, Nbk_k=0;
    float hSz = chSz/2.0f;

    for(i=0; i<32; ++i)
    {
        switch( checkerPos[i] )
        {
            case 'w':
            wh_dealPath.ppLeg[ Nwh_ch ]->INIT( 400.0f-hSz, -20.0f-hSz, static_cast<float>(anchorArr1[i].first)-hSz, static_cast<float>(anchorArr1[i].second)-hSz, 30.0f );
            wh_dealPath.ppSprite[ Nwh_ch++ ] = &wh_chSprite; break;
            case 'W':
            wh_dealPath.ppLeg[ Nwh_ch ]->INIT( 400.0f-hSz, -20.0f-hSz, static_cast<float>(anchorArr1[i].first)-hSz, static_cast<float>(anchorArr1[i].second)-hSz, 30.0f );
            wh_dealPath.ppSprite[ Nwh_ch++ ] = &wh_kSprite;
            ++Nwh_k; break;
            case 'b':
            bk_dealPath.ppLeg[ Nbk_ch ]->INIT( 400.0f-hSz, 620.0f-hSz, static_cast<float>(anchorArr1[i].first)-hSz, static_cast<float>(anchorArr1[i].second)-hSz, 30.0f );
            bk_dealPath.ppSprite[ Nbk_ch++ ] = &bk_chSprite; break;
            case 'B':
            bk_dealPath.ppLeg[ Nbk_ch ]->INIT( 400.0f-hSz, 620.0f-hSz, static_cast<float>(anchorArr1[i].first)-hSz, static_cast<float>(anchorArr1[i].second)-hSz, 30.0f );
            bk_dealPath.ppSprite[ Nbk_ch++ ] = &bk_kSprite;
            ++Nbk_k; break;
            default: break;
        }
    }

    wh_dealPath.nLegs_inUse = 12 - Nwh_k;
    bk_dealPath.nLegs_inUse = 12 - Nbk_k;

    for(i = Nwh_ch; i < wh_dealPath.nLegs_inUse; ++i )
    {
        wh_dealPath.ppLeg[i]->INIT( 820.0f, wh_captY, wh_captX + (i-Nwh_ch)*capt_dx, wh_captY, 30.0f );
        wh_dealPath.ppSprite[i] = &wh_chSprite;
    }

    for(i = Nbk_ch; i < bk_dealPath.nLegs_inUse; ++i )
    {
        bk_dealPath.ppLeg[i]->INIT( 820.0f, bk_captY, bk_captX + (i-Nbk_ch)*capt_dx, bk_captY, 30.0f );
        bk_dealPath.ppSprite[i] = &bk_chSprite;
    }

    return;
}


bool fillMoveList(int hm)
{
    Nmoves = 0;
    
    bool isKing = checkerPos[hm] == 'W' || checkerPos[hm] == 'B';
    char opCol = turn ? 'b' : 'w';
    char opColK = turn ? 'B' : 'W';

    if( turn || isKing )
    {

        if( hm < 28 && !jumpMade )
        {
            if( hm%8 == 3 || hm%8 == 4 )
            {
                if( checkerPos[hm + 4] == 'n')
                    moveIdxList[ Nmoves++ ] = hm + 4;
            }
            else if( (hm/4)%2 )
            {
                if( checkerPos[hm + 3] == 'n')
                    moveIdxList[ Nmoves++ ] = hm + 3;
                if( checkerPos[hm + 4] == 'n')
                    moveIdxList[ Nmoves++ ] = hm + 4;
            }
            else
            {
                if( checkerPos[hm + 4] == 'n')
                    moveIdxList[ Nmoves++ ] = hm + 4;
                if( checkerPos[hm + 5] == 'n')
                    moveIdxList[ Nmoves++ ] = hm + 5;
            }
        }
        if( hm < 24 )
        {
            if( hm%8 == 3 )
            {
                if( (checkerPos[hm + 4]==opCol || checkerPos[hm + 4]==opColK) && checkerPos[hm + 7] == 'n' )
                    moveIdxList[ Nmoves++ ] = hm + 7;
            }
            else if( hm%8 == 4 )
            {
                if( (checkerPos[hm + 4]==opCol || checkerPos[hm + 4]==opColK) && checkerPos[hm + 9] == 'n' )
                    moveIdxList[ Nmoves++ ] = hm + 9;
            }
            else if( (hm/4)%2 )
            {
                if( (checkerPos[hm + 3]==opCol || checkerPos[hm + 3]==opColK) && checkerPos[hm + 7] == 'n' )
                    moveIdxList[ Nmoves++ ] = hm + 7;
                if( (checkerPos[hm + 4]==opCol || checkerPos[hm + 4]==opColK) && checkerPos[hm + 9] == 'n' )
                    moveIdxList[ Nmoves++ ] = hm + 9;
            }
            else
            {
                if( (checkerPos[hm + 4]==opCol || checkerPos[hm + 4]==opColK) && checkerPos[hm + 7] == 'n' )
                    moveIdxList[ Nmoves++ ] = hm + 7;
                if( (checkerPos[hm + 5]==opCol || checkerPos[hm + 5]==opColK) && checkerPos[hm + 9] == 'n' )
                    moveIdxList[ Nmoves++ ] = hm + 9;
            }
        }
    }
    if( !turn || isKing )
    {
   
        if( hm >3 && !jumpMade )
        {
            if( hm%8 == 3 || hm%8 == 4 )
            {
                if( checkerPos[hm - 4] == 'n')
                    moveIdxList[ Nmoves++ ] = hm - 4;
            }
            else if( (hm/4)%2 )
            {
                if( checkerPos[hm - 4] == 'n')
                    moveIdxList[ Nmoves++ ] = hm - 4;
                if( checkerPos[hm - 5] == 'n')
                    moveIdxList[ Nmoves++ ] = hm - 5;
            }
            else
            {
                if( checkerPos[hm - 3] == 'n')
                    moveIdxList[ Nmoves++ ] = hm - 3;
                if( checkerPos[hm - 4] == 'n')
                    moveIdxList[ Nmoves++ ] = hm - 4;
            }
        }
        if( hm > 7 )
        {
            if( hm%8 == 3 )
            {
                if( (checkerPos[hm - 4]==opCol || checkerPos[hm - 4]==opColK) && checkerPos[hm - 9] == 'n' )
                    moveIdxList[ Nmoves++ ] = hm - 9;
            }
            else if( hm%8 == 4 )
            {
                if( (checkerPos[hm - 4]==opCol || checkerPos[hm - 4]==opColK) && checkerPos[hm - 7] == 'n' )
                    moveIdxList[ Nmoves++ ] = hm - 7;
            }
            else if( (hm/4)%2 )
            {
                if( (checkerPos[hm - 4]==opCol || checkerPos[hm - 4]==opColK) && checkerPos[hm - 7] == 'n' )
                    moveIdxList[ Nmoves++ ] = hm - 7;
                if( (checkerPos[hm - 5]==opCol || checkerPos[hm - 5]==opColK) && checkerPos[hm - 9] == 'n' )
                    moveIdxList[ Nmoves++ ] = hm - 9;
            }
            else
            {
                if( (checkerPos[hm - 3]==opCol || checkerPos[hm - 3]==opColK) && checkerPos[hm - 7] == 'n' )
                    moveIdxList[ Nmoves++ ] = hm - 7;
                if( (checkerPos[hm - 4]==opCol || checkerPos[hm - 4]==opColK) && checkerPos[hm - 9] == 'n' )
                    moveIdxList[ Nmoves++ ] = hm - 9;
            }
        }
    }

    
    for(int i=0; i<Nmoves; ++i)
    {
        
        if( (moveIdxList[i] - hm > 5) || (moveIdxList[i] - hm < -5) )
            return true;
    }

    return false;
}

bool didJump( int hm_0, int hm_f, int& jumpIdx )
{
    
    if( (hm_f - hm_0 >= -5) && (hm_f - hm_0 <= 5) ) return false;



    if( hm_f - hm_0 > 5 )
    {
        if( (hm_0/4)%2 )
        {
            if( hm_f - hm_0 == 7 )
                jumpIdx = hm_0 + 3;
            else
                jumpIdx = hm_0 + 4;
        }
        else
        {
            if( hm_f - hm_0 == 7 )
                jumpIdx = hm_0 + 4;
            else
                jumpIdx = hm_0 + 5;
        }
        return true;
    }

    if( hm_f - hm_0 < -5 )
    {
        if( (hm_0/4)%2 )
        {
            if( hm_0 - hm_f == 7 )
                jumpIdx = hm_f + 3;
            else
                jumpIdx = hm_f + 4;
        }
        else
        {
            if( hm_0 - hm_f == 7 )
                jumpIdx = hm_f + 4;
            else
                jumpIdx = hm_f + 5;
        }
        return true;
    }
    return false;
}


int hitAnchor(bool Turn)
{
    int hSz = chSz/2;
    int idx = 0;

    for( idx=0; idx<Nanchors; ++idx)
    {
        if( mseX < anchorArr1[idx].first - hSz ) continue;
        if( mseX > anchorArr1[idx].first + hSz ) continue;
        if( mseY < anchorArr1[idx].second - hSz ) continue;
        if( mseY > anchorArr1[idx].second + hSz ) continue;
        break;
    }
    if( idx == Nanchors ) return -1;
    if( Turn && (checkerPos[idx] == 'w' || checkerPos[idx] == 'W') ) return idx;
    if( !Turn && (checkerPos[idx] == 'b' || checkerPos[idx] == 'B') ) return idx;
    return -1;
}

void drawCheckers(sf::RenderWindow& rApp)
{
    
    float hSz = chSz/2;
    int i=0;

    if( reset )
    {
        wh_dealPath.draw();
        bk_dealPath.draw();
        rApp.Draw( msg );
    }

    if( gameOn )
    {
        for(i=0; i<Nanchors; ++i)
            if( i != chObj.homeIdx )
            {
                switch( checkerPos[i] )
                {
                    case 'w':
                    wh_chSprite.SetPosition( static_cast<float>(anchorArr1[i].first) - hSz, static_cast<float>(anchorArr1[i].second) - hSz );
                    rApp.Draw( wh_chSprite ); break;
                    case 'W':
                    wh_kSprite.SetPosition( static_cast<float>(anchorArr1[i].first) - hSz, static_cast<float>(anchorArr1[i].second) - hSz );
                    rApp.Draw( wh_kSprite ); break;
                    case 'b':
                    bk_chSprite.SetPosition( static_cast<float>(anchorArr1[i].first) - hSz, static_cast<float>(anchorArr1[i].second) - hSz );
                    rApp.Draw( bk_chSprite ); break;
                    case 'B':
                    bk_kSprite.SetPosition( static_cast<float>(anchorArr1[i].first) - hSz, static_cast<float>(anchorArr1[i].second) - hSz );
                    rApp.Draw( bk_kSprite ); break;
                    default: break;
                }
            }

        chObj.draw();

        
        if( !gameOver )
        {
            rApp.Draw( msg_Turn );
            if( turn )
                rApp.Draw( msg_Whites );
            else
                rApp.Draw( msg_Blacks );
        }

        
        for(i=0; i<Nwh_capt; ++i)
        {
            wh_chSprite.SetPosition( wh_captX + capt_dx*(float)i, wh_captY );
            rApp.Draw( wh_chSprite );
        }
        for(i=0; i<Nbk_capt; ++i)
        {
            bk_chSprite.SetPosition( bk_captX + capt_dx*(float)i, bk_captY );
            rApp.Draw( bk_chSprite );
        }

        
        if( aniPath.inUse ) aniPath.draw();
        if( kingMePath.inUse ) kingMePath.draw();
    }

    return;
}

void gameCapture_SplitAniLogic(void)
{
    aniPath.move();

    
    if( animateCapt && !aniPath.inUse )
    {
        animateCapt = false;
        if( colorCapt == 'w' || colorCapt == 'W' )
            ++Nwh_capt;
        else
            ++Nbk_capt;

        
        if( colorCapt == 'W' || colorCapt == 'B' )
        {
            animateSplit = true;
            if( colorCapt == 'W' )
            {
                p_aniPathLeg->INIT( wh_captX + (Nwh_capt-1)*capt_dx, wh_captY, wh_captX + Nwh_capt*capt_dx, wh_captY, 3.0f );
                *aniPath.ppSprite = &wh_chSprite;
            }
            else
            {
                p_aniPathLeg->INIT( bk_captX + (Nbk_capt-1)*capt_dx, bk_captY, bk_captX + Nbk_capt*capt_dx, bk_captY, 3.0f );
                *aniPath.ppSprite = &bk_chSprite;
            }
            aniPath.reset(true, 0);
        }
    }

    
    if( animateSplit && !aniPath.inUse )
    {
        animateSplit = false;
        if( colorCapt == 'W' )
            ++Nwh_capt;
        else
            ++Nbk_capt;
    }
    return;
}

void gameKingMeAniLogic(void)
{
    
    if( justKinged && !aniPath.inUse )
    {
        justKinged = false;
        animateKing = true;
        kingMePath.reset(true, 0);
        if( *kingMePath.ppSprite == &wh_chSprite )
            --Nwh_capt;
        else
            --Nbk_capt;
    }

    kingMePath.move();

    
    if( animateKing && !kingMePath.inUse )
    {
        animateKing = false;
        if( *kingMePath.ppSprite == &wh_chSprite )
            chObj.pSprite = &wh_kSprite;
        else
            chObj.pSprite = &bk_kSprite;

    }
    return;
}

bool menuHitDown(void)
{
    bool init_game = false;

    if( gameOn )
        if( saveGameButt.hit_dn() )
        {
            if( !saveGameToFile( "chPosSAVED.txt" ) )
                return false;
        }
    if( newButt.hit_dn() )
    {
        init_game = true;
        INITcheckerPositions_NEW();
    }
    if( savedButt.hit_dn() )
    {
        init_game = true;
        if( !INITcheckerPositions_SAVED( "chPosSAVED.txt" ) )
            return false;
    }
    if( testButt.hit_dn() )
    {
        init_game = true;
        if( !INITcheckerPositions_SAVED( "chPosTEST.txt" ) )
            return false;
    }

    if( init_game )
    {
        init_game = false;

        INITpathLegs();
        int i=0;
        for( i=0; i<32; ++i)
            if( checkerPos[i] == 'w' ) break;
        chObj.homeIdx = i;  chObj.docked = false; chObj.snap();
        chObj.pSprite = &wh_chSprite;
        
        reset = true;
        gameOn = gameOver = false;
        animateKing = justKinged = false;
        wh_dealPath.reset(true, 0);
        bk_dealPath.reset(true, 0);
        
        
        
        
        
    }

    return true;
}

void menuHitUp(void)
{
    newButt.hit_up();
    savedButt.hit_up();
    testButt.hit_up();
    if( gameOn )
        saveGameButt.hit_up();

    return;
}

void gameHitDown(void)
{
    int idx = -1;
    if( jumpMade && canJump )
    {
        if( chObj.grab() ) return;
        idx = hitAnchor(!turn );
        if( idx != -1 )
        {
            turn = !turn;
            jumpMade = canJump = false;
        }
    }
    else
        idx = hitAnchor(turn);

    
    if( idx != -1 )
    {
        jumpMade = false;
        fillMoveList(idx);
        if( Nmoves > 0 )
        {
            
            chObj.homeIdx = idx;
            chObj.docked = false;
            chObj.snap();
            chObj.grab();
            switch( checkerPos[idx] )
            {
                case 'w': chObj.pSprite = &wh_chSprite; break;
                case 'W': chObj.pSprite = &wh_kSprite; break;
                case 'b': chObj.pSprite = &bk_chSprite; break;
                case 'B': chObj.pSprite = &bk_kSprite; break;
                default: break;
            }
        }
    }

    return;
}

void gameHitUp(void)
{
    int hm_0 = chObj.homeIdx;
    chObj.release( moveIdxList, Nmoves );
    chObj.snap();

    int hm_f = chObj.homeIdx;
    if( hm_f != hm_0 )
    {
        
        checkerPos[hm_f] = checkerPos[hm_0];
        checkerPos[hm_0] = 'n';

        float hSz = chSz/2;

        
        justKinged = false;
        float xf = (float)( anchorArr1[hm_f].first ) - hSz;
        float yf = (float)( anchorArr1[hm_f].second ) - hSz;
        if( turn && hm_f > 27 && checkerPos[hm_f] != 'W' )
        {
            checkerPos[hm_f] = 'W';

            *kingMePath.ppSprite = &wh_chSprite;
            p_kingMePathLeg->INIT( wh_captX + (Nwh_capt-1)*capt_dx, wh_captY, xf, yf, 20.0f );
            justKinged = true;
        }
        if( !turn && hm_f < 4 && checkerPos[hm_f] != 'B' )
        {
            checkerPos[hm_f] = 'B';

            *kingMePath.ppSprite = &bk_chSprite;
            p_kingMePathLeg->INIT( bk_captX + (Nbk_capt-1)*capt_dx, bk_captY, xf, yf, 20.0f );
            justKinged = true;
        }

        
        int jumpIdx = 0;
        jumpMade = didJump( hm_0, hm_f, jumpIdx );
        if( jumpMade )
        {
            
            colorCapt = checkerPos[jumpIdx];
            checkerPos[jumpIdx] = 'n';
            animateCapt = true;
            float xi = (float)( anchorArr1[jumpIdx].first ) - hSz;
            float yi = (float)( anchorArr1[jumpIdx].second ) - hSz;

            if( colorCapt == 'w' || colorCapt == 'W' )
                p_aniPathLeg->INIT( xi, yi, wh_captX + Nwh_capt*capt_dx, wh_captY, 40.0f );
            else
                p_aniPathLeg->INIT( xi, yi, bk_captX + Nbk_capt*capt_dx, bk_captY, 40.0f );

            switch( colorCapt )
            {
                case 'w': *aniPath.ppSprite = &wh_chSprite; break;
                case 'W': *aniPath.ppSprite = &wh_kSprite; break;
                case 'b': *aniPath.ppSprite = &bk_chSprite; break;
                case 'B': *aniPath.ppSprite = &bk_kSprite; break;
                default: break;
            }
            aniPath.reset(true, 0);

            
            canJump = false;
            if( !justKinged )
                canJump = fillMoveList(hm_f);
        }
        else
            canJump = false;

        if( !canJump )
            turn = !turn;
    }

    return;
}

void gameLogic(void)
{
    
    if( reset )
    {
        wh_dealPath.move();
        bk_dealPath.move();
        if( !wh_dealPath.inUse && !bk_dealPath.inUse )
        {
            reset = false;
            gameOn = true;
        }
    }

    if( ( animateCapt || animateSplit ) && gameOn )
        gameCapture_SplitAniLogic();

    
    if( (Nwh_capt == 12 || Nbk_capt == 12) && !gameOver )
    {
        gameOver = true;
        textFX = true;
        textFXdelay = 30;

        if( Nwh_capt == 12 )
            p_FXmsg = &msg_BLACKWINS;
        else
            p_FXmsg = &msg_WHITEWINS;

        textExpand( *p_FXmsg, 1.0f, 7.0f, 50, true );
        textFade( *p_FXmsg, 255, 1, 50, true );
    }

    if( !gameOver )
        gameKingMeAniLogic();

    
    if( textFX )
    {
        if( textFXdelay > 1)
            --textFXdelay;
        else
        {
            textFX = textExpand( *p_FXmsg, 1.0f, 7.0f, 50 );
            textFade( *p_FXmsg, 255, 1, 50 );
        }
    }

    return;
}

void gameDraw(sf::RenderWindow& rApp)
{
    newButt.draw();
    savedButt.draw();
    testButt.draw();

    if( gameOn )
        saveGameButt.draw();

    rApp.Draw( boardSprite );
    drawCheckers(rApp);
    if( textFX ) rApp.Draw( *p_FXmsg );

    
    if( takeScreenshot )
    {
        takeScreenshot = false;
        sf::Image Screen = rApp.Capture();
        Screen.SaveToFile("screenshot.jpg");
    }

    return;
}


bool textExpand( sf::String& r_text, float scale_i, float scale_f, int Nframes, bool reset )
{
    static int frCount = 0;
    static float oldRt = r_text.GetRect().Right;
    static float oldBot = r_text.GetRect().Bottom;
    if(reset)
    {
        frCount = 0;
        oldRt = r_text.GetRect().Right;
        oldBot = r_text.GetRect().Bottom;
    }

    if( frCount < Nframes )
        ++frCount;
    else
        return false;

    float scl = scale_i + (scale_f - scale_i)*(float)frCount/(float)Nframes;
    r_text.SetScale( scl, scl );
    float newRt = r_text.GetRect().Right;
    float newBot = r_text.GetRect().Bottom;
    r_text.Move( (oldRt-newRt)/2.0f, (oldBot-newBot)/2.0f );
    oldRt -= (oldRt-newRt)/2.0f;
    oldBot -= (oldBot-newBot)/2.0f;

    return true;
}

bool textFade( sf::String& r_text, int alpha_i, int alpha_f, int Nframes, bool reset )
{
    static int frCount = 0;
    static unsigned int oldRed = r_text.GetColor().r;
    static unsigned int oldGrn = r_text.GetColor().g;
    static unsigned int oldBlu = r_text.GetColor().b;
    if(reset)
    {
        frCount = 0;
        oldRed = r_text.GetColor().r;
        oldGrn = r_text.GetColor().g;
        oldBlu = r_text.GetColor().b;
    }


    if( frCount < Nframes )
        ++frCount;
    else
        return false;

    int alf = ( unsigned int )( (float)alpha_i + (float)(alpha_f - alpha_i)*(float)frCount/(float)Nframes );
    r_text.SetColor( sf::Color(oldRed, oldGrn, oldBlu, alf) );

    return true;
}
