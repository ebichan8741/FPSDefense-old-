//=================================================================================================
//
// プレイヤー [player.cpp]
// Author : TAKUYA EBIHARA
//
//=================================================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "scene.h"
#include "scene3D.h"
#include "player.h"
#include "enemy.h"
#include "effect.h"
#include "collision.h"
#include "GameMode.h"

//=================================================================================================
// 定数定義
//=================================================================================================
#define EFFECT_TEXTURE "data/TEXTURE/effect000.jpg"
#define EFFECT_LIFE	(120)
#define EFFECT_MOVE	(3.0f)

//=================================================================================================
// 構造体宣言
//=================================================================================================

//=================================================================================================
// 静的メンバ変数
//=================================================================================================
LPDIRECT3DTEXTURE9 CEffect::m_pTexture = NULL;

//*************************************************************************************************
// コンストラクタ
//*************************************************************************************************
CEffect::CEffect(int nPriolity) :CScene3D(nPriolity)
{
}

//*************************************************************************************************
// 初期化処理
//*************************************************************************************************
HRESULT CEffect::Init(void)
{
	//メンバ初期化
	m_vSize = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	m_Life = EFFECT_LIFE;
	m_Move = EFFECT_MOVE;
	m_LengthSq = m_vSize.x * m_vSize.x + m_vSize.y * m_vSize.y;
	CScene3D::Init();
	return S_OK;
}

//*************************************************************************************************
// 終了処理
//*************************************************************************************************
void CEffect::Uninit(void)
{
	CScene3D::Uninit();
}

//*************************************************************************************************
// 更新処理
//*************************************************************************************************
void CEffect::Update(void)
{
	//---------------------------------------------------------------------------------------------
	// 変数宣言開始
	//---------------------------------------------------------------------------------------------
	VERTEX_3D* pVtx; //仮想アドレスを取得するためのポインタ
	D3DXVECTOR3 vMove;
	//---------------------------------------------------------------------------------------------
	// 変数宣言終了
	//---------------------------------------------------------------------------------------------

	//使用フラグがfalseの時は処理しない
	if (!m_Use)
	{
		Release();
		return;
	}

	//移動量計算
	vMove = m_Dir * m_Move;
	m_vPos += vMove;

	//寿命減算
	m_Life--;
	if (m_Life <= 0)
	{
		m_Use = false;
		Release();
	}

	CScene3D::Update();
}

//*************************************************************************************************
// 描画処理
//*************************************************************************************************
void CEffect::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX mtxWorld;
	D3DXMATRIX mtxTrans, mtxScale, mtxInv;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	// 加算合成
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//ライトのオフ
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//ワールド座標変換行列の初期化
	D3DXMatrixIdentity(&mtxWorld);

	//拡大・縮小
	//D3DXMatrixScaling(&mtxScale,1.0f,1.0f,1.0f);
	//D3DXMatrixMultiply(&mtxWorld,&mtxWorld,&mtxScale);

	//平行移動
	D3DXMatrixTranslation(&mtxTrans, m_vPos.x, m_vPos.y, m_vPos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	//逆行列の取得
	mtxInv = CManager::GetCamera()->GetViewInverse();
	mtxInv._41 = 0.0f;
	mtxInv._42 = 0.0f;
	mtxInv._43 = 0.0f;

	D3DXMatrixMultiply(&mtxWorld, &mtxInv, &mtxWorld);    //行列の乗算（出力先,左辺,右辺）

	//ワールド座標変換行列の適用
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	CScene3D::Draw();

	// 乗算合成
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//ライトのオン
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//*************************************************************************************************
// インスタンス生成
//*************************************************************************************************
CEffect *CEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 dir)
{
	CEffect *effect;
	effect = new CEffect(PRIOLITY_1);
	effect->Init();

	effect->m_vPos = pos;
	D3DXVec3Normalize(&dir, &dir);
	effect->m_Dir = dir;
	effect->m_Use = true;

	effect->BindTexture(m_pTexture);

	return effect;
}


//=============================================================================
// テクスチャの読み込み処理
//=============================================================================
HRESULT CEffect::Load(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	if (m_pTexture == NULL)
	{
		//テクスチャの読み込み
		if (FAILED(D3DXCreateTextureFromFile(pDevice, EFFECT_TEXTURE, &m_pTexture)))
		{
			//エラー時
			MessageBox(NULL, "テクスチャの読み込みに失敗しました", "終了メッセージ", MB_OK);
		}
	}
	return S_OK;
}

//=============================================================================
// テクスチャの解放処理
//=============================================================================
void CEffect::Unload(void)
{
	//テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}
