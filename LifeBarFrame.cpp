//=================================================================================================
//
// ライフバーのフレーム [ LifeBarFrame.cpp ]
// Author : TAKUYA EBIHARA
//
//=================================================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "GameMode.h"
#include "scene3D.h"
#include "enemy.h"
#include "LifeBarFrame.h"
#include "camera.h"

//=================================================================================================
// 定数定義
//=================================================================================================
#define TEXTURE_LIFEBARFRAME   "data/TEXTURE/lifebar_frame.png"

//=================================================================================================
// 構造体宣言
//=================================================================================================

//=================================================================================================
// グローバル変数
//=================================================================================================
LPDIRECT3DTEXTURE9 CLifeBarFrame::m_pTexture = NULL;

//*************************************************************************************************
// コンストラクタ
//*************************************************************************************************
CLifeBarFrame::CLifeBarFrame(int nPriolity) :CScene3D(nPriolity)
{
}

//*************************************************************************************************
// 初期化処理
//*************************************************************************************************
HRESULT CLifeBarFrame::Init(void)
{

    CScene3D::Init2();
    return S_OK;
}

//*************************************************************************************************
// 終了処理
//*************************************************************************************************
void CLifeBarFrame::Uninit(void)
{
    CScene3D::Uninit();
}

//*************************************************************************************************
// 更新処理
//*************************************************************************************************
void CLifeBarFrame::Update(void)
{
    //int nowHp = CGameMode::GetEnemy(m_ID)->GetLife();
    //m_vPos = CGameMode::GetEnemy(m_ID)->GetPosition();
    m_vPos.y += 7.0f;

    if (m_HP <= 0) Release();

    CScene3D::Update();
}

//*************************************************************************************************
// 描画処理
//*************************************************************************************************
void CLifeBarFrame::Draw(void)
{
    LPDIRECT3DDEVICE9 pDevice;
    D3DXMATRIX mtxWorld;   //ワールド座標変換行列
    D3DXMATRIX mtxTrans, mtxScale, mtxInv;

    //デバイスの取得
    pDevice = CManager::GetRenderer()->GetDevice();

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

    //ライトのオン
    pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//*************************************************************************************************
// インスタンス生成
//*************************************************************************************************
CLifeBarFrame *CLifeBarFrame::Create(D3DXVECTOR3 pos)
{
    CLifeBarFrame *lifebarFrame;
    lifebarFrame = new CLifeBarFrame(PRIOLITY_1);

    lifebarFrame->m_vPos = pos;
    lifebarFrame->Init();

    lifebarFrame->BindTexture(m_pTexture);

    return lifebarFrame;
}

//=============================================================================
// テクスチャの読み込み処理
//=============================================================================
HRESULT CLifeBarFrame::Load(void)
{
    //デバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    if (m_pTexture == NULL) {
        //テクスチャの読み込み
        if (FAILED(D3DXCreateTextureFromFile(pDevice, TEXTURE_LIFEBARFRAME, &m_pTexture))) {
            //エラー時
            MessageBox(NULL, "テクスチャの読み込みに失敗しました", "終了メッセージ", MB_OK);
        }
    }
    return S_OK;
}

//=============================================================================
// テクスチャの解放処理
//=============================================================================
void CLifeBarFrame::Unload(void)
{
    //テクスチャの破棄
    if (m_pTexture != NULL) {
        m_pTexture->Release();
        m_pTexture = NULL;
    }
}