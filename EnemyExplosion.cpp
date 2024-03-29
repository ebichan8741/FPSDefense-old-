//=================================================================================================
//
// 敵爆発エフェクト [EnemyExplosion.cpp]
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
#include "EnemyExplosion.h"
#include "GameMode.h"

//=================================================================================================
// 定数定義
//=================================================================================================
#define ENEMYEXPLOSION_TEXTURE "data/TEXTURE/explosion.png"

//=================================================================================================
// 構造体宣言
//=================================================================================================

//=================================================================================================
// 静的メンバ変数
//=================================================================================================
LPDIRECT3DTEXTURE9 CEnemyExplosion::m_pTexture = NULL;

//*************************************************************************************************
// コンストラクタ
//*************************************************************************************************
CEnemyExplosion::CEnemyExplosion(int nPriolity) :CScene3D(nPriolity)
{
}

//*************************************************************************************************
// 初期化処理
//*************************************************************************************************
HRESULT CEnemyExplosion::Init(void)
{
    //メンバ初期化
    m_vSize = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
    m_TexPos = D3DXVECTOR2(0.0f, 0.0f);
    m_TexSize = D3DXVECTOR2(1.0f / 8, 0.5f);

    CScene3D::Init();
    return S_OK;
}

//*************************************************************************************************
// 終了処理
//*************************************************************************************************
void CEnemyExplosion::Uninit(void)
{
    CScene3D::Uninit();
}

//*************************************************************************************************
// 更新処理
//*************************************************************************************************
void CEnemyExplosion::Update(void)
{
    // 変数宣言 ------------------------------------------------------------------------------------
    VERTEX_3D* pVtx; //仮想アドレスを取得するためのポインタ
    //---------------------------------------------------------------------------------------------

    //使用フラグがfalseの時は処理しない
    if (!m_Use){
        Release();
        return;
    }

    //テクスチャアニメーション
    if (m_AnimCnt >= 1){
        m_TexPos.x += m_TexSize.x;
        if (m_TexPos.x >= 1.0f){
            m_TexPos.x = 0.0f;
            m_TexPos.y += m_TexSize.y;
            if (m_TexPos.y >= 1.0f){
                m_TexPos.y = 0.0f;
                Release();
            }
        }
        m_AnimCnt = 0;
    }
    m_AnimCnt++;

    // 位置更新
    //SetPosition();
    // UV値更新
    AnimationTexture(m_TexPos, m_TexSize);

    CScene3D::Update();
}

//*************************************************************************************************
// 描画処理
//*************************************************************************************************
void CEnemyExplosion::Draw(void)
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
    D3DXMatrixScaling(&mtxScale,30.0f,30.0f,30.0f);
    D3DXMatrixMultiply(&mtxWorld,&mtxWorld,&mtxScale);

    //逆行列の取得
    mtxInv = CManager::GetCamera()->GetViewInverse();
    mtxInv._41 = 0.0f;
    mtxInv._42 = 0.0f;
    mtxInv._43 = 0.0f;
    D3DXMatrixMultiply(&mtxWorld, &mtxInv, &mtxWorld);    //行列の乗算（出力先,左辺,右辺）

    //平行移動
    D3DXMatrixTranslation(&mtxTrans, m_vPos.x, m_vPos.y, m_vPos.z);
    D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

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
CEnemyExplosion *CEnemyExplosion::Create(D3DXVECTOR3 pos)
{
    CEnemyExplosion *explosion;
    explosion = new CEnemyExplosion(PRIOLITY_1);

    explosion->m_vPos = pos;
    explosion->m_Use = true;

    explosion->Init();

    explosion->BindTexture(m_pTexture);

    return explosion;
}


//=============================================================================
// テクスチャの読み込み処理
//=============================================================================
HRESULT CEnemyExplosion::Load(void)
{
    //デバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    if (m_pTexture == NULL)
    {
        //テクスチャの読み込み
        if (FAILED(D3DXCreateTextureFromFile(pDevice, ENEMYEXPLOSION_TEXTURE, &m_pTexture)))
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
void CEnemyExplosion::Unload(void)
{
    //テクスチャの破棄
    if (m_pTexture != NULL)
    {
        m_pTexture->Release();
        m_pTexture = NULL;
    }
}
