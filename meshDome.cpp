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
#include "meshdome.h"
#include "camera.h"
#include "GameMode.h"

//=================================================================================================
// 定数定義
//=================================================================================================
#define SPHERE_TEXTURENAME "data/TEXTURE/skydome001.png"
#define SPHERE_MAX_U    (100)        //Ｕ方向の分割数
#define SPHERE_MAX_V    (100)        //Ｖ方向の分割数
#define SPHERE_RADIUS   (2000.0f)   //球体の半径
#define SPHERE_POS_Y     (-50.0f)      //ドームのＹ座標

//=================================================================================================
// グローバル変数
//=================================================================================================
float g_sphereRot;

//*************************************************************************************************
// コンストラクタ
//*************************************************************************************************
CMeshDome::CMeshDome(int nPriolity):CScene(nPriolity)
{
}

//*************************************************************************************************
// 初期化処理
//*************************************************************************************************
HRESULT CMeshDome::Init(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    g_sphereRot = 0.0f;

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

    //ハードディスクからテクスチャを読み込む
    if(FAILED(D3DXCreateTextureFromFile(pDevice,SPHERE_TEXTURENAME,&m_pTexture)))
    {
        //エラー時
        MessageBox(NULL,"テクスチャの読み込みに失敗しました","終了メッセージ",MB_OK);
    }

    return 0;
}

//*************************************************************************************************
// 終了処理
//*************************************************************************************************
void CMeshDome::Uninit(void)
{
    //テクスチャインターフェースの解放
    if( m_pTexture != NULL )
    {
        m_pTexture -> Release();
        m_pTexture = NULL;
    }
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
void CMeshDome::Update(void)
{
    g_sphereRot += 0.005f;
}

//*************************************************************************************************
// 描画処理
//*************************************************************************************************
void CMeshDome::Draw(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
    D3DXMATRIX mtxWorld,mtxRotY,mtxTrans;
    D3DXVECTOR3 vCameraPos = CManager::GetCamera()->GetCameraPos();
    int nNumVtx,nNumFace;

    pDevice->SetStreamSource(
        0,                        //ストリーム番号(パイプ番号)
        m_pVtxBuff,       //ストリームの元になる頂点バッファ
        0,                        //オフセット(バイト)
        sizeof(VERTEX_3D));       //一個の頂点データのサイズ(ストライド量)

    //デバイスにインデックスバッファの設定
    pDevice->SetIndices(m_pIdxBuff);

    //頂点フォーマットの設定
    pDevice -> SetFVF(FVF_VERTEX_3D);

    //テクスチャの設定
    pDevice->SetTexture(0,m_pTexture);

    //ライトのオフ
    pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);

    //ワールド座標変換行列
    D3DXMatrixIdentity(&mtxWorld);

    //回転
    D3DXMatrixRotationYawPitchRoll(&mtxRotY,D3DXToRadian(g_sphereRot),0.0f,0.0f);	//回転(出力先,ラジアン角度Y,ラジアン角度Z,ラジアン角度X)
    D3DXMatrixMultiply(&mtxWorld,&mtxWorld,&mtxRotY);
    //カメラの位置に平行移動
    D3DXMatrixTranslation(&mtxTrans,vCameraPos.x,SPHERE_POS_Y,vCameraPos.z);
    D3DXMatrixMultiply(&mtxWorld,&mtxWorld,&mtxTrans);

    pDevice->SetTransform(D3DTS_WORLD,&mtxWorld);

    //頂点数とプリミティブ数の計算
    nNumVtx = (SPHERE_MAX_U + 1) * (SPHERE_MAX_V + 1);
    nNumFace = (SPHERE_MAX_U + 1) * SPHERE_MAX_V * 2 + (SPHERE_MAX_V - 1) * 4;

    //インデックスバッファによる描画
    pDevice->DrawIndexedPrimitive(
        D3DPT_TRIANGLESTRIP,        //描画形式
        0,                          //最初の頂点インデックスまでのオフセット(頂点バッファのオフセット　基本0)
        0,                          //最小の頂点インデックス(必要ない)
        nNumVtx,                    //頂点数
        0,                          //スタートインデックス(インデックスバッファのオフセット　モデルの数分ずらす)
        nNumFace);              //プリミティブ数

    //ライトのオン
    pDevice->SetRenderState(D3DRS_LIGHTING,TRUE);
}

//*************************************************************************************************
// 頂点情報の設定
//*************************************************************************************************
HRESULT CMeshDome::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{
    VERTEX_3D* pVtx;
    int nNumVtx;
    D3DXVECTOR3 pos[(SPHERE_MAX_U + 1) * (SPHERE_MAX_V + 1)];

    //頂点数の計算
    nNumVtx = (SPHERE_MAX_U + 1) * (SPHERE_MAX_V + 1);

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
    for(int i = 0;i < SPHERE_MAX_V + 1;i++)
    {
        for(int j = 0;j < (SPHERE_MAX_U + 1);j++)
        {
            //頂点座標計算
            pVtx[i * (SPHERE_MAX_U + 1) + j].pos = D3DXVECTOR3(
                SPHERE_RADIUS * sinf(D3DX_PI * i / SPHERE_MAX_V) * cosf(D3DX_PI * j / (SPHERE_MAX_U / 2) * -1), //X
                SPHERE_RADIUS * cosf(D3DX_PI * i / SPHERE_MAX_V),                                               //Y
                SPHERE_RADIUS * sinf(D3DX_PI * i / SPHERE_MAX_V) * sinf(D3DX_PI * j / (SPHERE_MAX_U / 2) * -1));//Z

#ifdef _DEBUG
            //デバック用
            pos[i * (SPHERE_MAX_U + 1) + j] = D3DXVECTOR3(
                SPHERE_RADIUS * sinf(D3DX_PI * i / SPHERE_MAX_V) * cosf(D3DX_PI * j / (SPHERE_MAX_U / 2)),  //X
                SPHERE_RADIUS * cosf(D3DX_PI * i / SPHERE_MAX_V),                                           //Y
                SPHERE_RADIUS * sinf(D3DX_PI * i / SPHERE_MAX_V) * sinf(D3DX_PI * j / (SPHERE_MAX_U / 2))); //Z
#endif
            pVtx[i * (SPHERE_MAX_U + 1) + j].normal = D3DXVECTOR3(0.0f,0.0f,-1.0f);
            pVtx[i * (SPHERE_MAX_U + 1) + j].color = D3DCOLOR_RGBA(255, 255, 255, 255);
            pVtx[i * (SPHERE_MAX_U + 1) + j].tex = D3DXVECTOR2(j * 1.0f / SPHERE_MAX_U,i * 1.0f / SPHERE_MAX_V);
        }
    }

    m_pVtxBuff->Unlock();    //アンロック

    return 0;
}

//*************************************************************************************************
// 頂点インデックスの設定
//*************************************************************************************************
HRESULT CMeshDome::MakeIndex(LPDIRECT3DDEVICE9 pDevice)
{
    WORD* pIdx;
    WORD wIdx[(SPHERE_MAX_U + 1) * 2 * SPHERE_MAX_V + (SPHERE_MAX_V - 1) * 2];
    int nNumIdx;

    //インデックス数の計算
    nNumIdx = (SPHERE_MAX_U + 1) * 2 * SPHERE_MAX_V + (SPHERE_MAX_V - 1) * 2;

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
    for(int height = 0;height < SPHERE_MAX_V;height++)
    {
        //横のメッシュ生成
        for(int width = 0;width < SPHERE_MAX_U + 1;width++)
        {
            //縮退計算
            if(width == 0 && height != 0)
            {
                pIdx[height * ((SPHERE_MAX_U + 1) * 2 + 2) - 2] = (height - 1) * (SPHERE_MAX_U + 1) + SPHERE_MAX_U;
                pIdx[height * ((SPHERE_MAX_U + 1) * 2 + 2) - 1] = height * (SPHERE_MAX_U + 1) + (SPHERE_MAX_U + 1);
            
                //デバック用
                wIdx[height * ((SPHERE_MAX_U + 1) * 2 + 2) - 2] = (height - 1) * (SPHERE_MAX_U + 1) + SPHERE_MAX_U;
                wIdx[height * ((SPHERE_MAX_U + 1) * 2 + 2) - 1] = height * (SPHERE_MAX_U + 1) + (SPHERE_MAX_U + 1);
            }

            pIdx[(width * 2) + (height * ((SPHERE_MAX_U + 1) * 2 + 2))] = height * (SPHERE_MAX_U + 1) + width + (SPHERE_MAX_U + 1);
            pIdx[(width * 2 + 1) + (height * ((SPHERE_MAX_U + 1) * 2 + 2))] = height * (SPHERE_MAX_U + 1) + width;

            //デバック用
            wIdx[(width * 2) + (height * ((SPHERE_MAX_U + 1) * 2 + 2))] = height * (SPHERE_MAX_U + 1) + width + (SPHERE_MAX_U + 1);
            wIdx[(width * 2 + 1) + (height * ((SPHERE_MAX_U + 1) * 2 + 2))] = height * (SPHERE_MAX_U + 1) + width;
        }
    }

    m_pIdxBuff->Unlock();    //アンロック
    return 0;
}

//*************************************************************************************************
// インスタンス生成
//*************************************************************************************************
CMeshDome *CMeshDome::Create(void)
{
    CMeshDome *meshdome;

    meshdome = new CMeshDome(PRIOLITY_0);

    meshdome->Init();

    return meshdome;
}