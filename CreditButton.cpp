//=================================================================================================
//
// クレジットボタン [ CreditButton.cpp ]
// Author : TAKUYA EBIHARA
//
//=================================================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"
#include "CreditButton.h"
#include "TextureManager.h"

//=================================================================================================
// 定数定義
//=================================================================================================
#define CREDITBUTTON_POS_X	        (640.0f)	// X座標
#define CREDITBUTTON_POS_Y	        (700.0f)	// Y座標
#define CREDITBUTTON_SIZE_X	    (350.0f)	    // 幅
#define CREDITBUTTON_SIZE_Y	    (70.0f)		    // 高さ
#define CREDITBUTTON_ALPHA_RATE    (0.01f)       // 点滅の速さ


//*************************************************************************************************
// コンストラクタ
//*************************************************************************************************
CCreditButton::CCreditButton(int nPriolity) :CScene2D(nPriolity)
{
}

//*************************************************************************************************
// 初期化処理
//*************************************************************************************************
HRESULT CCreditButton::Init(void)
{
    m_vPos = D3DXVECTOR3(CREDITBUTTON_POS_X, CREDITBUTTON_POS_Y, 0.0f);
    m_vSize = D3DXVECTOR3(CREDITBUTTON_SIZE_X, CREDITBUTTON_SIZE_Y, 0.0f);
    m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    m_AlphaRate = -CREDITBUTTON_ALPHA_RATE;
    CScene2D::Init();
    return S_OK;
}

//*************************************************************************************************
// 終了処理
//*************************************************************************************************
void CCreditButton::Uninit(void)
{
    CScene2D::Uninit();
}

//*************************************************************************************************
// 更新処理
//*************************************************************************************************
void CCreditButton::Update(void)
{
    m_Color.a += m_AlphaRate;

    if (m_Color.a <= 0.0f) { m_AlphaRate *= -1; }
    else if (m_Color.a >= 1.0f) { m_AlphaRate *= -1; }

    SetColor(m_Color);
}

//*************************************************************************************************
// 描画処理
//*************************************************************************************************
void CCreditButton::Draw(void)
{
    CScene2D::Draw();
}

//*************************************************************************************************
// インスタンス生成
//*************************************************************************************************
CCreditButton *CCreditButton::Create(void)
{
    CCreditButton *credit;
    credit = new CCreditButton(PRIOLITY_2);
    credit->Init();

    //credit->LoadTexture(TEXTURE_TITLESTART);
    credit->m_pTexture = CManager::GetTextureManager()->getResource(CManager::GetTextureManager()->getTextureName(TEXTURE_TITLESTART));

    return credit;
}
