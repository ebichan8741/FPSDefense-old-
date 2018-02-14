//================================================================================================
//
// メッシュドーム [meshdome.cpp]
// Author : TAKUYA EBIHARA
//
//================================================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "hitdome.h"
#include "camera.h"
#include "input.h"
#include "GameMode.h"

//=================================================================================================
// 定数定義
//=================================================================================================
#define HIT_SPHERE_MAX_U    (8)        //Ｕ方向の分割数
#define HIT_SPHERE_MAX_V    (8)        //Ｖ方向の分割数

//=================================================================================================
// グローバル変数
//=================================================================================================

//*************************************************************************************************
// コンストラクタ
//*************************************************************************************************
CHitDome::CHitDome(int nPriolity):CScene(nPriolity)
{
    m_pTexture = NULL;
    m_Hit = NULL;
    m_Color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f);
}

//*************************************************************************************************
// 初期化処理
//*************************************************************************************************
HRESULT CHitDome::Init(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    m_Len = sqrt((m_vSize.x * m_vSize.x) + (m_vSize.y * m_vSize.y) + (m_vSize.z * m_vSize.z)) / 2.0f;

    //頂点情報の設定
    if(FAILED(MakeVertex(pDevice)))
    {
        return E_FAIL;
    }

    //頂点インデックスの設定
    if(FAILED(MakeIndex(pDevice)))
    {
        return E_FAIL;
    }

    return 0;
}

//*************************************************************************************************
// 終了処理
//*************************************************************************************************
void CHitDome::Uninit(void)
{
    //頂点バッファーの解放
    if( m_pVtxBuff != NULL )
    {
        m_pVtxBuff -> Release();
        m_pVtxBuff = NULL;
    }
    //インデックスバッファーの解放
    if( m_pIdxBuff != NULL )
    {
        m_pIdxBuff -> Release();
        m_pIdxBuff = NULL;
    }
}

//*************************************************************************************************
// 更新処理
//*************************************************************************************************
void CHitDome::Update(void)
{
    if(m_Hit){
        m_Color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f);
        m_Hit = false;
    }
    else{
        m_Color = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.5f);
    }

    VERTEX_3D* pVtx;
    m_pVtxBuff->Lock(0,0,(void**)&pVtx,0);    //ロック
    //頂点座標
    for(int i = 0;i < HIT_SPHERE_MAX_V + 1;i++)
    {
        for(int j = 0;j < (HIT_SPHERE_MAX_U + 1);j++)
        {
            pVtx[i * (HIT_SPHERE_MAX_U + 1) + j].color = m_Color;
        }
    }
    m_pVtxBuff->Unlock();    //アンロック
}

//*************************************************************************************************
// 描画処理
//*************************************************************************************************
void CHitDome::Draw(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
    D3DXMATRIX mtxWorld,mtxRotY,mtxTrans;
    D3DXVECTOR3 vCameraPos = CManager::GetCamera()->GetCameraPos();
    int nNumVtx,nNumFace;

    pDevice->SetStreamSource(
        0,                          //ストリーム番号(パイプ番号)
        m_pVtxBuff,                 //ストリームの元になる頂点バッファ
        0,                          //オフセット(バイト)
        sizeof(VERTEX_3D));         //一個の頂点データのサイズ(ストライド量)

    //デバイスにインデックスバッファの設定
    pDevice->SetIndices(m_pIdxBuff);

    //頂点フォーマットの設定
    pDevice -> SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0,m_pTexture);

    //ライトのオフ
    pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);

    //ワールド座標変換行列の初期化
    D3DXMatrixIdentity(&mtxWorld);

    //平行移動
    D3DXMatrixTranslation(&mtxTrans,m_vPos.x,m_vPos.y,m_vPos.z);
    D3DXMatrixMultiply(&mtxWorld,&mtxWorld,&mtxTrans);

    pDevice->SetTransform(D3DTS_WORLD,&mtxWorld);

    //頂点数とプリミティブ数の計算
    nNumVtx = (HIT_SPHERE_MAX_U + 1) * (HIT_SPHERE_MAX_V + 1);
    nNumFace = (HIT_SPHERE_MAX_U + 1) * HIT_SPHERE_MAX_V * 2 + (HIT_SPHERE_MAX_V - 1) * 4;

    //インデックスバッファによる描画
    pDevice->DrawIndexedPrimitive(
        D3DPT_TRIANGLESTRIP,        //描画形式
        0,                          //最初の頂点インデックスまでのオフセット(頂点バッファのオフセット　基本0)
        0,                          //最小の頂点インデックス(必要ない)
        nNumVtx,                    //頂点数
        0,                          //スタートインデックス(インデックスバッファのオフセット　モデルの数分ずらす)
        nNumFace);                  //プリミティブ数
    
    //ライトのオン
    pDevice->SetRenderState(D3DRS_LIGHTING,TRUE);
}

//*************************************************************************************************
// 頂点情報の設定
//*************************************************************************************************
HRESULT CHitDome::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{
    VERTEX_3D* pVtx;
    int nNumVtx;
    D3DXVECTOR3 pos[(HIT_SPHERE_MAX_U + 1) * (HIT_SPHERE_MAX_V + 1)];

    //頂点数の計算
    nNumVtx = (HIT_SPHERE_MAX_U + 1) * (HIT_SPHERE_MAX_V + 1);

    //頂点バッファ
    if(FAILED(pDevice->CreateVertexBuffer(
        sizeof(VERTEX_3D) * nNumVtx,        //作成したい頂点バッファのサイズ
        D3DUSAGE_WRITEONLY,                 //使用方法
        FVF_VERTEX_3D,                      //
        D3DPOOL_MANAGED,                    //メモリ管理方法(デバイスにお任せ)
        &m_pVtxBuff,
        NULL)))
    {
        return E_FAIL;
    }

    //頂点情報の設定///////////////////////////////////////////////////////////////////////////////
    m_pVtxBuff->Lock(0,0,(void**)&pVtx,0);    //ロック

    //頂点座標
    for(int i = 0;i < HIT_SPHERE_MAX_V + 1;i++)
    {
        for(int j = 0;j < (HIT_SPHERE_MAX_U + 1);j++)
        {
            //頂点座標計算
            pVtx[i * (HIT_SPHERE_MAX_U + 1) + j].pos = D3DXVECTOR3(
                m_vSize.x * sinf(D3DX_PI * i / HIT_SPHERE_MAX_V) * cosf(D3DX_PI * j / (HIT_SPHERE_MAX_U / 2) * -1), //X
                m_vSize.y * cosf(D3DX_PI * i / HIT_SPHERE_MAX_V),                                               //Y
                m_vSize.z * sinf(D3DX_PI * i / HIT_SPHERE_MAX_V) * sinf(D3DX_PI * j / (HIT_SPHERE_MAX_U / 2) * -1));//Z

#ifdef _DEBUG
            //デバック用
            pos[i * (HIT_SPHERE_MAX_U + 1) + j] = D3DXVECTOR3(
                m_vSize.x * sinf(D3DX_PI * i / HIT_SPHERE_MAX_V) * cosf(D3DX_PI * j / (HIT_SPHERE_MAX_U / 2)),  //X
                m_vSize.x * cosf(D3DX_PI * i / HIT_SPHERE_MAX_V),                                           //Y
                m_vSize.x * sinf(D3DX_PI * i / HIT_SPHERE_MAX_V) * sinf(D3DX_PI * j / (HIT_SPHERE_MAX_U / 2))); //Z
#endif
            pVtx[i * (HIT_SPHERE_MAX_U + 1) + j].normal = D3DXVECTOR3(0.0f,0.0f,-1.0f);
            pVtx[i * (HIT_SPHERE_MAX_U + 1) + j].color = D3DCOLOR_RGBA(255, 0, 0, 100);
            pVtx[i * (HIT_SPHERE_MAX_U + 1) + j].tex = D3DXVECTOR2(j * 1.0f / HIT_SPHERE_MAX_U,i * 1.0f / HIT_SPHERE_MAX_V);
        }
    }

    m_pVtxBuff->Unlock();    //アンロック

    return 0;
}

//*************************************************************************************************
// 頂点インデックスの設定
//*************************************************************************************************
HRESULT CHitDome::MakeIndex(LPDIRECT3DDEVICE9 pDevice)
{
    WORD* pIdx;
    WORD wIdx[(HIT_SPHERE_MAX_U + 1) * 2 * HIT_SPHERE_MAX_V + (HIT_SPHERE_MAX_V - 1) * 2];
    int nNumIdx;

    //インデックス数の計算
    nNumIdx = (HIT_SPHERE_MAX_U + 1) * 2 * HIT_SPHERE_MAX_V + (HIT_SPHERE_MAX_V - 1) * 2;

    //インデックスバッファ
    if(FAILED(pDevice->CreateIndexBuffer(
        sizeof(WORD) * nNumIdx,             //インデックス数
        D3DUSAGE_WRITEONLY,                 //使用用途フラグ
        D3DFMT_INDEX16,                     //インデックスデータのフォーマット
        D3DPOOL_MANAGED,                    //メモリの管理方法（お任せ）
        &m_pIdxBuff,                //インデックスバッファインターフェースポインタのアドレス
        NULL)));

    //頂点インデックスの設定////////////////////////////////////////////////////////////////////////
    m_pIdxBuff->Lock(0,0,(void**)&pIdx,0);    //ロック

    //N字型に生成
    //縦のメッシュ生成
    for(int height = 0;height < HIT_SPHERE_MAX_V;height++)
    {
        //横のメッシュ生成
        for(int width = 0;width < HIT_SPHERE_MAX_U + 1;width++)
        {
            //縮退計算
            if(width == 0 && height != 0)
            {
                pIdx[height * ((HIT_SPHERE_MAX_U + 1) * 2 + 2) - 2] = (height - 1) * (HIT_SPHERE_MAX_U + 1) + HIT_SPHERE_MAX_U;
                pIdx[height * ((HIT_SPHERE_MAX_U + 1) * 2 + 2) - 1] = height * (HIT_SPHERE_MAX_U + 1) + (HIT_SPHERE_MAX_U + 1);
            
                //デバック用
                wIdx[height * ((HIT_SPHERE_MAX_U + 1) * 2 + 2) - 2] = (height - 1) * (HIT_SPHERE_MAX_U + 1) + HIT_SPHERE_MAX_U;
                wIdx[height * ((HIT_SPHERE_MAX_U + 1) * 2 + 2) - 1] = height * (HIT_SPHERE_MAX_U + 1) + (HIT_SPHERE_MAX_U + 1);
            }

            pIdx[(width * 2) + (height * ((HIT_SPHERE_MAX_U + 1) * 2 + 2))] = height * (HIT_SPHERE_MAX_U + 1) + width + (HIT_SPHERE_MAX_U + 1);
            pIdx[(width * 2 + 1) + (height * ((HIT_SPHERE_MAX_U + 1) * 2 + 2))] = height * (HIT_SPHERE_MAX_U + 1) + width;

            //デバック用
            wIdx[(width * 2) + (height * ((HIT_SPHERE_MAX_U + 1) * 2 + 2))] = height * (HIT_SPHERE_MAX_U + 1) + width + (HIT_SPHERE_MAX_U + 1);
            wIdx[(width * 2 + 1) + (height * ((HIT_SPHERE_MAX_U + 1) * 2 + 2))] = height * (HIT_SPHERE_MAX_U + 1) + width;
        }
    }

    m_pIdxBuff->Unlock();    //アンロック
    return 0;
}

//*************************************************************************************************
// インスタンス生成
//*************************************************************************************************
CHitDome *CHitDome::Create(D3DXVECTOR3 pos,D3DXVECTOR3 size)
{
    CHitDome *hitdome;

    hitdome = new CHitDome(PRIOLITY_1);

    hitdome->m_vPos = pos;
    hitdome->m_vSize = size * 1.5f;

    hitdome->Init();

    return hitdome;
}

//*************************************************************************************************
// 座標とサイズの設定
//*************************************************************************************************
void CHitDome::SetDome(D3DXVECTOR3 pos,D3DXVECTOR3 size)
{
    m_vPos = pos;
    m_vSize = size;
}

//*************************************************************************************************
// ヒット中
//*************************************************************************************************
void CHitDome::HitToDome(void)
{
    m_Hit = true;
}