//=================================================================================================
//
// マネージャー処理 [maneger.cpp]
// Author : TAKUYA EBIHARA
//
//=================================================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "camera.h"
#include "light.h"
#include "input.h"
#include "Mode.h"
#include "TitleMode.h"
#include "GameMode.h"
#include "Fade.h"
#include "sound.h"
#include "TextureManager.h"
#include "ModelManager.h"


//=================================================================================================
// 定数定義
//=================================================================================================

//=================================================================================================
// 静的メンバ変数
//=================================================================================================
CRenderer *CManager::m_pRenderer = 0;
CInputKeyboard *CManager::m_pInputKeyboard = 0;
CInputMouse *CManager::m_pInputMouse = 0;
CCamera *CManager::m_pCamera = 0;
CMode *CManager::m_Mode = new CTitleMode;
CMode *CManager::m_nextMode = m_Mode;
CFade *CManager::m_Fade = 0;
CSound *CManager::m_Sound = 0;
MODE_TYPE CManager::mode = MODE_GAME;
CTextureManager *CManager::m_TextureManager = 0;
CModelManager *CManager::m_ModelManager = 0;


//=================================================================================================
// グローバル変数
//=================================================================================================
CLight *g_pLight = NULL;

//*************************************************************************************************
// コンストラクタ
//*************************************************************************************************
CManager::CManager()
{
}

//*************************************************************************************************
// デストラクタ
//*************************************************************************************************
CManager::~CManager()
{
}

//*************************************************************************************************
// 初期化処理
//*************************************************************************************************
HRESULT CManager::Init(HINSTANCE hInstance,HWND hWnd,BOOL bWindow)
{
    //レンダラーの生成
    m_pRenderer = new CRenderer;

    //レンダラー初期化
    if(FAILED(m_pRenderer->Init(hWnd,true)))
    {
        return -1;
    }

#ifdef _DEBUG
	CDebug::Init();
#endif

    //カメラのインスタンス生成・初期化処理
    m_pCamera = new CCamera;
    m_pCamera->Init();

    //ライトのインスタンス生成・初期化処理
    g_pLight = new CLight;
    g_pLight->SetLight();

    //キーボードのインスタンス生成
    m_pInputKeyboard = new CInputKeyboard;
    m_pInputKeyboard->Init(hInstance,hWnd);

    //マウスのインスタンス生成
    m_pInputMouse = new CInputMouse;
    m_pInputMouse->Init(hInstance,hWnd);

	//フェードのインスタンス生成
	m_Fade = new CFade;
	m_Fade->Init();

    // サウンドの生成
    m_Sound = new CSound;
    m_Sound->Init(hWnd);

    m_TextureManager = new CTextureManager;
    m_ModelManager = new CModelManager;

	m_Mode->Init();
    return S_OK;
}

//*************************************************************************************************
// 終了処理
//*************************************************************************************************
void CManager::Uninit(void)
{
#ifdef _DEBUG
	CDebug::Uninit();
#endif

	// モードの破棄
	if (m_Mode != NULL)
	{
		m_Mode->Uninit();
		m_Mode = NULL;
	}

	// フェードの破棄
	if (m_Fade != NULL)
	{
		m_Fade->Uninit();
		m_Fade = NULL;
	}

    // サウンドの破棄
    if (m_Sound != NULL)
    {
        m_Sound->Uninit();
        m_Sound = NULL;
    }

    //レンダラーの破棄
    if(m_pRenderer != NULL)
    {
        m_pRenderer->Uninit();
        m_pRenderer = NULL;
    }

    //キーボードの破棄
    if(m_pInputKeyboard != NULL)
    {
        m_pInputKeyboard->Uninit();
        m_pInputKeyboard = NULL;
    }

    //マウスの破棄
    if(m_pInputMouse != NULL)
    {
        m_pInputMouse->Uninit();
        m_pInputMouse = NULL;
    }

    //カメラの破棄
    if(m_pCamera != NULL)
    {
        m_pCamera->Uninit();
        m_pCamera = NULL;
    }
}

//*************************************************************************************************
// 更新処理
//*************************************************************************************************
void CManager::Update(void)
{
#ifdef _DEBUG
    char text[32];
    CDebug::ClearText();
    sprintf(text,"FPS:%d\n",GetFPSCount());
    CDebug::AddText(text);
#endif

    m_pRenderer->Update();
    m_pInputKeyboard->Update();
    m_pInputMouse->Update();
    m_pCamera->Update();

	//オブジェクトの更新
	CScene::UpdateAll();

	m_Mode->Update();

	if (m_Fade->GetFade() != FADE_NONE)
	{
		m_Fade->Update();
	}
}

//*************************************************************************************************
// 描画処理
//*************************************************************************************************
void CManager::Draw(void)
{
    m_pRenderer->DrawBegin();

    //カメラの設定
    m_pCamera->SetCamera();

    // オブジェクトの描画処理
    CScene::DrawAll();

	m_Mode->Draw();

	//次モードの予約がされている場合次モードに移行
	if (m_Mode != m_nextMode)
	{
		if (m_Mode != NULL)
		{
			m_Mode->Uninit();
			delete m_Mode;
		}

		if (m_nextMode != NULL)
		{
			m_nextMode->Init();
		}

		m_Mode = m_nextMode;
	}

	if (m_Fade->GetFade() != FADE_NONE)
	{
		m_Fade->Draw();
	}

    //デバッグ表示
#ifdef _DEBUG
    CDebug::Draw();
#endif

    m_pRenderer->DrawEnd();
}

//*************************************************************************************************
// レンダラーのポインタ取得
//*************************************************************************************************
CRenderer *CManager::GetRenderer(void)
{
    return CManager::m_pRenderer;
}

//*************************************************************************************************
// キーボードのポインタ取得
//*************************************************************************************************
CInputKeyboard *CManager::GetInputKeyboard(void)
{
    return CManager::m_pInputKeyboard;
}

//*************************************************************************************************
// マウスのポインタ取得
//*************************************************************************************************
CInputMouse *CManager::GetInputMouse(void)
{
    return CManager::m_pInputMouse;
}

//*************************************************************************************************
// カメラのポインタ取得
//*************************************************************************************************
CCamera *CManager::GetCamera(void)
{
    return CManager::m_pCamera;
}

//*************************************************************************************************
// サウンドのポインタ取得
//*************************************************************************************************
CSound *CManager::GetSound(void)
{
    return CManager::m_Sound;
}

//*************************************************************************************************
// 次モードの予約
//*************************************************************************************************
void CManager::SetMode(CMode *nextMode)
{
	m_nextMode = nextMode;
}

//void CManager::SetModeType(MODE_TYPE type)
//{
//	m_type = type;
//}

//*************************************************************************************************
// 現在のモードの取得
//*************************************************************************************************
MODE_TYPE CManager::GetMode(void)
{
	return mode;
}

//*************************************************************************************************
// テクスチャマネージャーの取得
//*************************************************************************************************
CTextureManager *CManager::GetTextureManager(void)
{
    return m_TextureManager;
}

//*************************************************************************************************
// モデルマネージャーの取得
//*************************************************************************************************
CModelManager *CManager::GetModelManager(void)
{
    return m_ModelManager;
}

