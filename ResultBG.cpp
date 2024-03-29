//=================================================================================================
//
// リザルト背景 [ResultBG.cpp]
// Author : TAKUYA EBIHARA
//
//=================================================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"
#include "ResultBG.h"
#include "TextureManager.h"

//=================================================================================================
// 定数定義
//=================================================================================================

//=================================================================================================
// 構造体宣言
//=================================================================================================

//=================================================================================================
// グローバル変数
//=================================================================================================

//*************************************************************************************************
// コンストラクタ
//*************************************************************************************************
CResultBG::CResultBG(int nPriolity) :CScene2D(nPriolity)
{
}

//*************************************************************************************************
// 初期化処理
//*************************************************************************************************
HRESULT CResultBG::Init(void)
{
	m_vPos = D3DXVECTOR3(0.0f + SCREEN_WIDTH * 0.5f, 0.0f + SCREEN_HEIGHT * 0.5f, 0.0f);
	m_vSize = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	CScene2D::Init();
	return S_OK;
}

//*************************************************************************************************
// 終了処理
//*************************************************************************************************
void CResultBG::Uninit(void)
{
	CScene2D::Uninit();
}

//*************************************************************************************************
// 更新処理
//*************************************************************************************************
void CResultBG::Update(void)
{

}

//*************************************************************************************************
// 描画処理
//*************************************************************************************************
void CResultBG::Draw(void)
{
	CScene2D::Draw();
}

//*************************************************************************************************
// インスタンス生成
//*************************************************************************************************
CResultBG *CResultBG::Create(void)
{
	CResultBG *resultBG;
	resultBG = new CResultBG(PRIOLITY_0);
	resultBG->Init();

	//resultBG->LoadTexture(TEXTURE_RESULT);
    resultBG->m_pTexture = CManager::GetTextureManager()->getResource(CManager::GetTextureManager()->getTextureName(TEXTURE_RESULT));

	return resultBG;
}
