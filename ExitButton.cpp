//=================================================================================================
//
// 終了ボタン [ ExitButton.cpp ]
// Author : TAKUYA EBIHARA
//
//=================================================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"
#include "ExitButton.h"
#include "TextureManager.h"

//=================================================================================================
// 定数定義
//=================================================================================================
#define EXITBUTTON_POS_X	        (640.0f)	// X座標
#define EXITBUTTON_POS_Y	        (700.0f)	// Y座標
#define EXITBUTTON_SIZE_X	    (350.0f)	    // 幅
#define EXITBUTTON_SIZE_Y	    (70.0f)		    // 高さ
#define EXITBUTTON_ALPHA_RATE    (0.01f)       // 点滅の速さ


//*************************************************************************************************
// コンストラクタ
//*************************************************************************************************
CExitButton::CExitButton(int nPriolity) :CScene2D(nPriolity)
{
}

//*************************************************************************************************
// 初期化処理
//*************************************************************************************************
HRESULT CExitButton::Init(void)
{
    m_vPos = D3DXVECTOR3(EXITBUTTON_POS_X, EXITBUTTON_POS_Y, 0.0f);
    m_vSize = D3DXVECTOR3(EXITBUTTON_SIZE_X, EXITBUTTON_SIZE_Y, 0.0f);
    m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    m_AlphaRate = -EXITBUTTON_ALPHA_RATE;
    CScene2D::Init();
    return S_OK;
}

//*************************************************************************************************
// 終了処理
//*************************************************************************************************
void CExitButton::Uninit(void)
{
    CScene2D::Uninit();
}

//*************************************************************************************************
// 更新処理
//*************************************************************************************************
void CExitButton::Update(void)
{
    m_Color.a += m_AlphaRate;

    if (m_Color.a <= 0.0f) { m_AlphaRate *= -1; }
    else if (m_Color.a >= 1.0f) { m_AlphaRate *= -1; }

    SetColor(m_Color);
}

//*************************************************************************************************
// 描画処理
//*************************************************************************************************
void CExitButton::Draw(void)
{
    CScene2D::Draw();
}

//*************************************************************************************************
// インスタンス生成
//*************************************************************************************************
CExitButton *CExitButton::Create(void)
{
    CExitButton *exit;
    exit = new CExitButton(PRIOLITY_2);
    exit->Init();

    //exit->LoadTexture(TEXTURE_TITLESTART);
    exit->m_pTexture = CManager::GetTextureManager()->getResource(CManager::GetTextureManager()->getTextureName(TEXTURE_TITLESTART));

    return exit;
}
