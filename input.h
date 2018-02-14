//=============================================================================
//
// 入力処理 [input.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define NUM_KEY_MAX        (256)    // キー最大数

#define CLICK_LEFT          (0)     //左クリック
#define CLICK_RIGHT         (1)     //右クリック
#define CLICK_CENTER        (2)     //中央ボタン

//=============================================================================
// 入力クラス
//=============================================================================
class CInput
{
public:
    CInput();
    virtual ~CInput();

    virtual HRESULT Init(HINSTANCE hInst, HWND hWnd);
    virtual void Uninit(void);
    virtual void Update(void) = 0;        // ---> 純粋仮想関数化

protected:
    static LPDIRECTINPUT8 m_pDInput;    // DirectInputオブジェクト
    LPDIRECTINPUTDEVICE8 m_pDIDevice;    // Deviceオブジェクト(入力に必要)
};

//=============================================================================
// キーボード入力クラス
//=============================================================================
class CInputKeyboard : public CInput
{
public:
    CInputKeyboard();
    ~CInputKeyboard();

    HRESULT Init(HINSTANCE hInst, HWND hWnd);
    void Uninit(void);
    void Update(void);

    BOOL GetKeyPress(int nKey);
    BOOL GetKeyTrigger(int nKey);
    BOOL GetKeyRelease(int nKey);
    BOOL GetKeyRepeat(int nKey);
    void FlushKeyTrigger(int nKey);

private:
    BYTE    m_aKeyState[NUM_KEY_MAX];               // キーボードの状態を受け取るワーク
    BYTE    m_aKeyStateTrigger[NUM_KEY_MAX];        // トリガーワーク
    BYTE    m_aKeyStateRelease[NUM_KEY_MAX];        // リリースワーク
    BYTE    m_aKeyStateRepeat[NUM_KEY_MAX];         // リピートワーク
    int     m_aKeyStateRepeatCnt[NUM_KEY_MAX];      // リピートカウンタ
};


//=============================================================================
// マウス入力クラス
//=============================================================================
class CInputMouse : public CInput
{
public:
    CInputMouse();
    ~CInputMouse();

    HRESULT Init(HINSTANCE hInst, HWND hWnd);
    void Uninit(void);
    void Update(void);
    void GetMouseState(void);

	DIMOUSESTATE2 m_pMouseState;

	void SetMousePos(D3DXVECTOR3 vPos);
	D3DXVECTOR3 GetMousePos(void);
    bool GetMousePress(int nClick);
    bool GetMouseTrigger(int nClick);

private:
    RECT    moveRect;     // 画面上で動ける範囲
    int     x;            // X座標
    int     y;            // Y座標
    bool    lButton;      // 左ボタン
    bool    rButton;      // 右ボタン
    bool    cButton;      // 真ん中ボタン
    int     moveAdd;      // 移動量
	D3DXVECTOR3	m_vMousePos;
};

#endif
