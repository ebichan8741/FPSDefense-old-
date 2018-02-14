//=================================================================================================
//
// 2Dポリゴン表示プログラム [main.cpp]
// Author : TAKUYA EBIHARA
//
//=================================================================================================
#include "main.h"
#include "manager.h"
#include <stdio.h>
#include <new>
#include <memory>
#include <cstdlib>

using namespace std;

#include <crtdbg.h>

#define _CRTDBG_MAP_ALLOC

#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)


//=================================================================================================
// プロトタイプ宣言
//=================================================================================================
LRESULT CALLBACK WndProc (HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM IParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);			//終了関数
void Update(void);			//アップデート関数
void Draw(void);			//表示関数



//=================================================================================================
// グローバル変数
//=================================================================================================
LPDIRECT3D9 g_pD3D = NULL;								//Direct3Dインターフェース  //LP=ポインタ型
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;					//Direct3Dデバイスへのポインタ
int g_nFPSCount = 0;


//*************************************************************************************************
// メイン処理
//*************************************************************************************************
int APIENTRY WinMain(HINSTANCE hInstance , HINSTANCE hPrevInstance , LPSTR lpCmdLine , int nCmdShow)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	WNDCLASSEX wcex;

	wcex.cbSize				=sizeof(WNDCLASSEX);				//ウィンドウのメモリサイズを指定
	wcex.style				=CS_CLASSDC;						//ウィンドウするのスタイルをせってい
	wcex.lpfnWndProc		=WndProc;							//ウィンドウプロシージャのアドレス「関数名」を設定
	wcex.cbClsExtra			=0;									//通常は使用しないので"0"を指定
	wcex.cbWndExtra			=0;									//通常は使用しないので"0"を指定
	wcex.hInstance			=hInstance;							//WinMainのパラメータのインスタンスハンドル
	wcex.hIcon				=NULL;								//使用するアイコンの設定
	wcex.hCursor			=LoadCursor(NULL, IDC_ARROW);		//カーソルを変更することができる
	wcex.hbrBackground		=(HBRUSH) (COLOR_WINDOW + 1);		//マウスカーソルを指定)色
	wcex.lpszMenuName		=NULL;								//メニュー（ファイルとか）
	wcex.lpszClassName		=CLASS_NAME;						//ウィンドウの名前
	wcex.hIconSm			=NULL;								//拡張された部分

	//ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	HWND hWnd;
	MSG msg;
	RECT cr = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	RECT dr;

	GetWindowRect(GetDesktopWindow(), &dr);

	int dw = dr.right-dr.left;		//デスクトップの幅を求める
	int dh = dr.bottom-dr.top;		//デスクトップの高さを求める


	DWORD style = WS_OVERLAPPEDWINDOW &~ (WS_MAXIMIZEBOX | WS_THICKFRAME);	//ウィンドウスタイル

	AdjustWindowRect(&cr, style, FALSE);	//スタイルや高さ幅を変更するもの

	int ww = cr.right-cr.left;		//ウィンドウの幅「フレームも合わせた」
	int wh = cr.bottom-cr.top;		//ウィンドウの高さ「フレームも合わせた」
	
	int wx = ww > dw ? 0: (dw-ww)/2;	//ウィンドウの左右の真ん中を求める
	int wy = wh > dh ? 0: (dh-wh)/2;	//ウィンドウの上下の真ん中を求める

	//ウィンドウを作成
	hWnd = CreateWindowEx(0,		//拡張ウィンドウスタイル
		CLASS_NAME,					//ウィンドウクラスの名前
		WINDOW_NAME,				//ウィンドウの名前
		style,						//ウィンドウのスタイル					◎◎
		wx,							//ウィンドウの座標(X座標単位ピクセル)
		wy,							//ウィンドウの座標(Y座標単位ピクセル)
		ww,							//ウィンドウの幅「フレームも合わせた」
		wh,							//ウィンドウの高さ「フレームも合わせた」
		NULL,						//親ウィンドウのハンドル
		NULL,						//メニューハンドルまたは子ウィンドウID
		hInstance,					//インスタンスハンドル
		NULL);						//ウィンドウ作成データ(NULL)

    CManager *pManager = NULL;
    pManager = new CManager;

	//ウィンドウの表示
	if(FAILED(pManager->Init(hInstance, hWnd, true)))
	{
		MessageBox(NULL,"初期化に失敗しました。","",MB_OK);
		return E_FAIL;
	}

    //変数宣言
    DWORD dwFrameCount;
	DWORD dwCurrentTime;
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;

    // 分解能を設定
    timeBeginPeriod(1);

    //各カウンターの初期化
	dwCurrentTime = 0;
	dwFrameCount = 0;
	dwExecLastTime = dwFPSLastTime = timeGetTime();	//システム時刻をミリ秒単位で取得

	ShowWindow(hWnd, nCmdShow);	//表示状態の設定
	UpdateWindow(hWnd);			//ウィンドウのアップデート(更新)

    _CrtDumpMemoryLeaks();

	// メイン メッセージ ループ:
    while(1)
	{
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE) != 0)
		{
			//Windowsの処理
			if(msg.message == WM_QUIT)
			{
				break;
			}

			else
			{
				TranslateMessage(&msg);				//仮想キーメッセージを文字メッセージへ変換
				DispatchMessage(&msg);				//ウィンドウプロシージャへメッセージを送出
			}
		}

		//FPSカウント
		else
		{
			dwCurrentTime = timeGetTime();			//現在の時間を取得

			//0.5秒ごとに実行
			if((dwCurrentTime - dwFPSLastTime) >= 500)
			{
				g_nFPSCount = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);	//秒間何フレーム経過したか
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;					//フレームカウンタをリセット
			};

			if((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				dwExecLastTime = dwCurrentTime;		//処理した時間
				//DirectXの処理
				pManager->Update();
				pManager->Draw();
				dwFrameCount++;
			}
		}
	}

	//ウィンドウの終了処理
	UnregisterClass(CLASS_NAME, wcex.hInstance);

    //終了処理
    if(pManager != NULL)
    {
        pManager->Uninit();
        pManager = NULL;
    }

    // 分解能を戻す
    timeEndPeriod(1);

	return (int)msg.wParam;
}

//*************************************************************************************************
// ウィンドウプロシージャ
//*************************************************************************************************
LRESULT CALLBACK WndProc (HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM IParam)
{
	static HWND hWndPaint;			//ペイントの変数を宣言
	static HDC  hDC;				//ウィンドウハンドルを宣言
	PAINTSTRUCT static ps;			//ペイント変数を宣言

	switch(uMsg)
	{
	case WM_DESTROY:				//ウィンドウ破壊命令
		PostQuitMessage(0);			//
		break;

	case WM_KEYDOWN:								//キーが入力されたら↓
		switch(wParam)								//wParamが↓
	{						
		case VK_ESCAPE:								//入力されたキーがエスケープだったら↓
			UINT nID = MessageBox(NULL,"終了しますか？","終了メッセージ",MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2);			//メッセージボックスを表示する。
			if(nID == IDYES)
			{
				DestroyWindow(hWnd);	//終了する
			}
		break;
	}

	default:
			break;
	}

	return DefWindowProc(hWnd,
		uMsg, wParam, IParam);

}

#ifdef _DEBUG
//=============================================================================
// FPS取得
//=============================================================================
int GetFPSCount(void)
{
    return g_nFPSCount;
}
#endif