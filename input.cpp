//=============================================================================
//
// 入力処理 [input.cpp]
// Author : AKIRA TANAKA
//
//=============================================================================
#include "main.h"
#include "input.h"
#include <stdio.h>

//=================================================================================================
// マクロ定義
//=================================================================================================
#define    COUNT_WAIT_REPEAT    (20)    // リピート待ち時間

//=================================================================================================
// 静的変数
//=================================================================================================
LPDIRECTINPUT8    CInput::m_pDInput = NULL;    // DirectInputオブジェクト

//*************************************************************************************************
// CInputコンストラスタ
//*************************************************************************************************
CInput::CInput()
{
    m_pDIDevice = NULL;
}

//*************************************************************************************************
// CInputデストラスタ
//*************************************************************************************************
CInput::~CInput()
{
}

//*************************************************************************************************
// 入力処理の初期化処理
//*************************************************************************************************
HRESULT CInput::Init(HINSTANCE hInst, HWND hWnd)
{
    HRESULT hr = S_FALSE;

    // DirectInputオブジェクトの作成
    if(!m_pDInput)
    {
        hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
                                    IID_IDirectInput8, (void**)&m_pDInput, NULL);
    }

    return hr;
}

//*************************************************************************************************
// 入力処理の更新処理
//*************************************************************************************************
#if 0    // ---> 純粋仮想関数化
HRESULT CInput::Update(void)
{
    return S_OK;
}
#endif

//*************************************************************************************************
// 入力処理の終了処理
//*************************************************************************************************
void CInput::Uninit(void)
{
    if(m_pDIDevice)
    {// デバイスオブジェクトの開放
        m_pDIDevice->Unacquire();

        m_pDIDevice->Release();
        m_pDIDevice = NULL;
    }

    if(m_pDInput)
    {// DirectInputオブジェクトの開放
        m_pDInput->Release();
        m_pDInput = NULL;
    }
}

//*************************************************************************************************
// CInputKeyboardコンストラスタ
//*************************************************************************************************
CInputKeyboard::CInputKeyboard()
{
    // 各ワークのクリア
    ZeroMemory(m_aKeyState, sizeof m_aKeyState);
    ZeroMemory(m_aKeyStateTrigger, sizeof m_aKeyStateTrigger);
    ZeroMemory(m_aKeyStateRelease, sizeof m_aKeyStateRelease);
    ZeroMemory(m_aKeyStateRepeat, sizeof m_aKeyStateRepeat);
    ZeroMemory(m_aKeyStateRepeatCnt, sizeof m_aKeyStateRepeatCnt);
}

//*************************************************************************************************
// CInputKeyboardデストラスタ
//*************************************************************************************************
CInputKeyboard::~CInputKeyboard()
{
}

//*************************************************************************************************
// キーボードの初期化処理
//*************************************************************************************************
HRESULT CInputKeyboard::Init(HINSTANCE hInst, HWND hWnd)
{
    HRESULT hr;

    // 入力処理の初期化
    CInput::Init(hInst, hWnd);

    // デバイスオブジェクトを作成
    hr = m_pDInput->CreateDevice(GUID_SysKeyboard, &m_pDIDevice, NULL);
    if(FAILED(hr) || m_pDIDevice == NULL)
    {
        MessageBox(hWnd, "キーボードがねぇ！", "警告！", MB_ICONWARNING);
        return hr;
    }

    // データフォーマットを設定
    hr = m_pDIDevice->SetDataFormat(&c_dfDIKeyboard);
    if(FAILED(hr))
    {
        MessageBox(hWnd, "キーボードのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
        return hr;
    }

    // 協調モードを設定（フォアグラウンド＆非排他モード）
    hr = m_pDIDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
    if(FAILED(hr))
    {
        MessageBox(hWnd, "キーボードの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
        return hr;
    }

    // m_pDIDeviceアクセス権を獲得(入力制御開始)
    m_pDIDevice->Acquire();

    return hr;
}

//*************************************************************************************************
// キーボード入力処理の開放
//*************************************************************************************************
void CInputKeyboard::Uninit(void)
{
    // 入力処理の開放
    CInput::Uninit();
}

//*************************************************************************************************
// キーボードの更新処理
//*************************************************************************************************
void CInputKeyboard::Update(void)
{
    HRESULT hr;
    BYTE aKeyState[NUM_KEY_MAX];

    if(!m_pDIDevice)
    {
        return;
    }

    // デバイスからデータを取得
    hr = m_pDIDevice->GetDeviceState(sizeof(aKeyState), aKeyState);
    if(SUCCEEDED(hr))
    {
        for(int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
        {
            m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
            m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & ~aKeyState[nCntKey];
            m_aKeyStateRepeat[nCntKey] = m_aKeyStateTrigger[nCntKey];

            if(aKeyState[nCntKey])
            {
                m_aKeyStateRepeatCnt[nCntKey]++;
                if(m_aKeyStateRepeatCnt[nCntKey] >= COUNT_WAIT_REPEAT)
                {
                    m_aKeyStateRepeat[nCntKey] = aKeyState[nCntKey];
                }
            }
            else
            {
                m_aKeyStateRepeatCnt[nCntKey] = 0;
                m_aKeyStateRepeat[nCntKey] = 0;
            }

            m_aKeyState[nCntKey] = aKeyState[nCntKey];
        }
    }
    else
    {
        m_pDIDevice->Acquire();
    }
}

//*************************************************************************************************
// キーボードデータ取得(プレス)
//*************************************************************************************************
BOOL CInputKeyboard::GetKeyPress(int nKey)
{
    return (m_aKeyState[nKey] & 0x80) ? TRUE: FALSE;
}

//*************************************************************************************************
// キーボードデータ取得(トリガー)
//*************************************************************************************************
BOOL CInputKeyboard::GetKeyTrigger(int nKey)
{
    return (m_aKeyStateTrigger[nKey] & 0x80) ? TRUE: FALSE;
}

//*************************************************************************************************
// キーボードデータ取得(リリース)
//*************************************************************************************************
BOOL CInputKeyboard::GetKeyRelease(int nKey)
{
    return (m_aKeyStateRelease[nKey] & 0x80) ? TRUE: FALSE;
}

//*************************************************************************************************
// キーボードデータ取得(リピート)
//*************************************************************************************************
BOOL CInputKeyboard::GetKeyRepeat(int nKey)
{
    return (m_aKeyStateRepeat[nKey] & 0x80) ? TRUE: FALSE;
}

//*************************************************************************************************
// キーボードデータ削除(トリガー)
//*************************************************************************************************
void CInputKeyboard::FlushKeyTrigger(int nKey)
{
    m_aKeyStateTrigger[nKey] = 0;
}

//-----------------------------------------------------------------------------
// マウス
//-----------------------------------------------------------------------------
//*************************************************************************************************
// CInputMouseコンストラスタ
//*************************************************************************************************
CInputMouse::CInputMouse()
{
	ZeroMemory(m_pMouseState.rgbButtons, sizeof m_pMouseState.rgbButtons);
    SetRect(&moveRect, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);	    // マウスカーソルの動く範囲
    m_vMousePos.x = moveRect.left;	                        // マウスカーソルのＸ座標を初期化
	m_vMousePos.y = moveRect.top;	                        // マウスカーソルのＹ座標を初期化
	m_vMousePos.z = 0.0;
    lButton = false;	                        // 左ボタンの情報を初期化
    rButton = false;	                        // 右ボタンの情報を初期化
    cButton = false;	                        // 中央ボタンの情報を初期化
    moveAdd = 1;	                            // マウスカーソルの移動量を設定
}

//*************************************************************************************************
// CInputKeyboardデストラスタ
//*************************************************************************************************
CInputMouse::~CInputMouse()
{
}

//*************************************************************************************************
// マウスの初期化処理
//*************************************************************************************************
HRESULT CInputMouse::Init(HINSTANCE hInst, HWND hWnd)
{
    HRESULT hr;

    // 入力処理の初期化
    CInput::Init(hInst, hWnd);

    // デバイスオブジェクトを作成
    hr = m_pDInput->CreateDevice(GUID_SysMouse, &m_pDIDevice, NULL);
    if(FAILED(hr) || m_pDIDevice == NULL)
    {
        MessageBox(hWnd, "マウスがねぇ！", "警告！", MB_ICONWARNING);
        return hr;
    }

    // データフォーマットを設定
    hr = m_pDIDevice->SetDataFormat(&c_dfDIMouse2);
    if(FAILED(hr))
    {
        MessageBox(hWnd, "マウスのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
        return hr;
    }

    // 協調モードを設定（フォアグラウンド＆非排他モード）
    hr = m_pDIDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_EXCLUSIVE));
    if(FAILED(hr))
    {
        MessageBox(hWnd, "マウスの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
        return hr;
    }

    // デバイスの設定
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;	// 相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）
 
	hr = m_pDIDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph);
	if (FAILED(hr)) {
		// デバイスの設定に失敗
		return false;
	}

    // m_pDIDeviceアクセス権を獲得(入力制御開始)
    m_pDIDevice->Acquire();

    return hr;
}

//*************************************************************************************************
// マウス入力処理の開放
//*************************************************************************************************
void CInputMouse::Uninit(void)
{
    // 入力処理の開放
    CInput::Uninit();
}

//*************************************************************************************************
// マウスの更新処理
//*************************************************************************************************
void CInputMouse::Update(void)
{
    HRESULT hr;

    if(!m_pDIDevice)
    {
        return;
    }

	m_pDIDevice->Acquire();

	hr = m_pDIDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &m_pMouseState);
	if (SUCCEEDED(hr))
	{
		// 確認用の処理、ここから -->
		// 取得した情報を元にマウスの情報を更新
		m_vMousePos.x += (m_pMouseState.lX * moveAdd);
		m_vMousePos.y += (m_pMouseState.lY * moveAdd);
		if (m_vMousePos.x < moveRect.left) m_vMousePos.x = moveRect.left;
		if (m_vMousePos.x > moveRect.right) m_vMousePos.x = moveRect.right;
		if (m_vMousePos.y < moveRect.top) m_vMousePos.y = moveRect.top;
		if (m_vMousePos.y > moveRect.bottom) m_vMousePos.y = moveRect.bottom;
		(m_pMouseState.rgbButtons[0] & 0x80) ? lButton = true : lButton = false;
		(m_pMouseState.rgbButtons[1] & 0x80) ? rButton = true : rButton = false;
		(m_pMouseState.rgbButtons[2] & 0x80) ? cButton = true : cButton = false;

		// 値が変わったら表示します
		char buf[128];
		wsprintf(buf, "(%5d, %5d, %5d) %s %s %s\n",
			(int)m_vMousePos.x, (int)m_vMousePos.y, m_pMouseState.lZ,
            GetMousePress(0) ? "Left" : "--",
            GetMousePress(1) ? "Right" : "--",
            GetMousePress(2) ? "Center" : "--");
		OutputDebugString(buf);
		// --> ここまで、確認用の処理
	}
	else if (hr == DIERR_INPUTLOST)
	{
		m_pDIDevice->Acquire();
	}
}


void CInputMouse::GetMouseState(void)
{
#ifdef _DEBUG
 //   char buf[128];
	//wsprintf(buf, "(%5d, %5d, %5d) %s %s %s\n",
	//	m_pMouseState.lX, m_pMouseState.lY, m_pMouseState.lZ,
	//	(m_pMouseState.rgbButtons[0] & 0x80) ? "Left" : "--",
	//	(m_pMouseState.rgbButtons[1] & 0x80) ? "Right" : "--",
	//	(m_pMouseState.rgbButtons[2] & 0x80) ? "Center" : "--");
	//OutputDebugString(buf);
#endif

}

//*************************************************************************************************
// マウスデータ取得(プレス)
//*************************************************************************************************
bool CInputMouse::GetMousePress(int nClick)
{
    return (m_pMouseState.rgbButtons[nClick] & 0x80) ? true : false;
}

//*************************************************************************************************
// マウスデータ取得(トリガー)
//*************************************************************************************************
bool CInputMouse::GetMouseTrigger(int nClick)
{
    // 前に取得した時のマウスの状態
    static bool prevState[sizeof(m_pMouseState.rgbButtons) / sizeof(m_pMouseState.rgbButtons[nClick])];

     //今のマウスの状態 
    bool current = m_pMouseState.rgbButtons[nClick] & 0x80;

     //前の状態がfalseで、今の状態がtrueならば、クリックした瞬間と判定する 
    bool ret = current && !prevState[nClick];

     //今の状態を保存する 
    prevState[nClick] = current;

     //判定結果を返す 
    return ret;
}

//*************************************************************************************************
// マウス座標設定
//*************************************************************************************************
void CInputMouse::SetMousePos(D3DXVECTOR3 vPos)
{
    m_vMousePos = vPos;
}

//*************************************************************************************************
// マウス座標取得
//*************************************************************************************************
D3DXVECTOR3 CInputMouse::GetMousePos(void)
{
    return m_vMousePos;
}
