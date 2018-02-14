//=================================================================================================
//
// 煙エフェクト [Smoke.cpp]
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
#include "Smoke.h"
#include "GameMode.h"

//=================================================================================================
// 定数定義
//=================================================================================================
#define SMOKE_TEXTURE "data/TEXTURE/smoke.png"

//=================================================================================================
// 構造体宣言
//=================================================================================================

//=================================================================================================
// 静的メンバ変数
//=================================================================================================
LPDIRECT3DTEXTURE9 CSmoke::m_pTexture = NULL;

//*************************************************************************************************
// コンストラクタ
//*************************************************************************************************
CSmoke::CSmoke(int nPriolity) :CScene3D(nPriolity)
{
}

//*************************************************************************************************
// 初期化処理
//*************************************************************************************************
HRESULT CSmoke::Init(void)
{
    //メンバ初期化
    m_vSize = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
    m_TexPos = D3DXVECTOR2(0.0f, 0.0f);
    m_TexSize = D3DXVECTOR2(1.0f, 1.0f);
    m_Velocity = D3DXVECTOR3(0.0f,0.0f,0.0f);
    m_Frame = 0;
    CScene3D::Init();
    return S_OK;
}

//*************************************************************************************************
// 終了処理
//*************************************************************************************************
void CSmoke::Uninit(void)
{
    CScene3D::Uninit();
}

//*************************************************************************************************
// 更新処理
//*************************************************************************************************
void CSmoke::Update(void)
{
    // 変数宣言 ------------------------------------------------------------------------------------
    VERTEX_3D* pVtx; //仮想アドレスを取得するためのポインタ
    //---------------------------------------------------------------------------------------------

    //使用フラグがfalseの時は処理しない
    if (!m_Use) {
        Release();
        return;
    }

    m_vPos += m_Velocity;
    m_Velocity *= 0.8f;

    m_Rot += m_RotSpeed;
    m_RotSpeed *= 0.8f;

    m_Frame++;
    if (m_Frame > 120) {
        m_Use = false;
    }

    CScene3D::Update();
}

//*************************************************************************************************
// 描画処理
//*************************************************************************************************
void CSmoke::Draw(void)
{
    LPDIRECT3DDEVICE9 pDevice;
    D3DXMATRIX mtxWorld;
    D3DXMATRIX mtxTrans, mtxScale, mtxInv,mtxRot;

    //デバイスの取得
    pDevice = CManager::GetRenderer()->GetDevice();

    // 加算合成
    pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

    //ライトのオフ
    pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

    //ワールド座標変換行列の初期化
    D3DXMatrixIdentity(&mtxWorld);

    //拡大・縮小
    D3DXMatrixScaling(&mtxScale, 20.0f, 20.0f, 20.0f);
    D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

    // 回転
    D3DXMatrixRotationZ(&mtxRot, m_Rot);
    D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

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
CSmoke *CSmoke::Create(D3DXVECTOR3 pos)
{
    CSmoke *smoke;
    int PI = D3DX_PI * 100;
    smoke = new CSmoke(PRIOLITY_1);

    smoke->m_vPos = pos;
    smoke->m_Use = true;

    smoke->Init();

    smoke->m_Velocity.x = ((rand() % 3) - 1) * 0.1f;
    smoke->m_Velocity.y = ((rand() % 3) - 1) * 0.1f;
    smoke->m_Velocity.z = ((rand() % 3) - 1) * 0.1f;
    smoke->m_Rot = (rand() % (PI * 2)) - PI;
    smoke->m_Rot * 0.01f;
    smoke->m_RotSpeed = (rand() % 2) - 0.5f;
    smoke->BindTexture(m_pTexture);

    return smoke;
}


//=============================================================================
// テクスチャの読み込み処理
//=============================================================================
HRESULT CSmoke::Load(void)
{
    //デバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    if (m_pTexture == NULL)
    {
        //テクスチャの読み込み
        if (FAILED(D3DXCreateTextureFromFile(pDevice, SMOKE_TEXTURE, &m_pTexture)))
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
void CSmoke::Unload(void)
{
    //テクスチャの破棄
    if (m_pTexture != NULL)
    {
        m_pTexture->Release();
        m_pTexture = NULL;
    }
}
