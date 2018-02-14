//================================================================================================
//
// メッシュフィールド [meshfield.cpp]
// Author : TAKUYA EBIHARA
//
//================================================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "meshfield2.h"

//=================================================================================================
// 定数定義
//=================================================================================================
#define FIELD2_TEXTURENAME "data/TEXTURE/water000.jpg"

#define MAX_FIELD_BLOCK_X	(20)			//X方向のブロックの数
#define MAX_FIELD_BLOCK_Z	(20)			//Z方向のブロックの数
#define FIELD_WIDTH			(11.0f)			//ブロックの横幅
#define FIELD_HEIGHT		(11.0f)			//ブロックの縦幅
#define FIELD_POS_X			(-(FIELD_WIDTH * MAX_FIELD_BLOCK_X) / 2)	//フィールドの開始座標X
#define FIELD_POS_Z			((FIELD_HEIGHT * MAX_FIELD_BLOCK_Z) / 2)	//フィールドの開始座標Z

//=================================================================================================
// グローバル変数
//=================================================================================================

//*************************************************************************************************
// コンストラクタ
//*************************************************************************************************
CMeshField2::CMeshField2(int nPriolity):CScene(nPriolity)
{
}

//*************************************************************************************************
// 初期化処理
//*************************************************************************************************
HRESULT CMeshField2::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

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
	if(FAILED(D3DXCreateTextureFromFile(pDevice,FIELD2_TEXTURENAME,&m_pTexture)))
	{
		//エラー時
		MessageBox(NULL,"テクスチャの読み込みに失敗しました","終了メッセージ",MB_OK);
	}

	return 0;
}

//*************************************************************************************************
// 終了処理
//*************************************************************************************************
void CMeshField2::Uninit(void)
{
	if( m_pTexture != NULL )
	{
		m_pTexture -> Release();		//Direct3Dデバイスの解放
		m_pTexture = NULL;
	}

	if( m_pVtxBuff != NULL )
	{
		m_pVtxBuff -> Release();			//Direct3Dインターフェースの解放
		m_pVtxBuff = NULL;
	}

	if( m_pIdxBuff != NULL )
	{
		m_pIdxBuff -> Release();			//Direct3Dインターフェースの解放
		m_pIdxBuff = NULL;
	}
}

//*************************************************************************************************
// 更新処理
//*************************************************************************************************
void CMeshField2::Update(void)
{

}

//*************************************************************************************************
// 描画処理
//*************************************************************************************************
void CMeshField2::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX mtxWorld;

	pDevice->SetStreamSource(
		0,						//ストリーム番号(パイプ番号)
		m_pVtxBuff,		//ストリームの元になる頂点バッファ
		0,						//オフセット(バイト)
		sizeof(VERTEX_3D));		//一個の頂点データのサイズ(ストライド量)

	//デバイスにインデックスバッファの設定
	pDevice->SetIndices(m_pIdxBuff);

	//頂点フォーマットの設定
	pDevice -> SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0,m_pTexture);

	//ワールド座標変換行列
	D3DXMatrixIdentity(&mtxWorld);			//ワールド座標変換行列を初期化

	pDevice->SetTransform(D3DTS_WORLD,&mtxWorld);		//ワールド座標変換行列を移動できるようにする(?)

	//インデックスバッファによる描画
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,		//描画形式(トライアングルストリップ)
		0,							//最初の頂点インデックスまでのオフセット
		0,							//最小の頂点インデックス
		(MAX_FIELD_BLOCK_X + 1) * (MAX_FIELD_BLOCK_Z + 1),//頂点数
		0,							//スタートインデックス
		NUM_POLYGON * MAX_FIELD_BLOCK_X * MAX_FIELD_BLOCK_Z + (MAX_FIELD_BLOCK_Z - 1) * 4) ;						//プリミティブ数
}

//*************************************************************************************************
// 頂点情報の設定
//*************************************************************************************************
HRESULT CMeshField2::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_3D* pVtx;

	//頂点バッファ
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * (MAX_FIELD_BLOCK_X + 1) * (MAX_FIELD_BLOCK_Z + 1),		//作成したい頂点バッファのサイズ
		D3DUSAGE_WRITEONLY,							//使用方法
		FVF_VERTEX_3D,								//
		D3DPOOL_MANAGED,							//メモリ管理方法(デバイスにお任せ)
		&m_pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	//頂点情報の設定///////////////////////////////////////////////////////////////////////////////
	m_pVtxBuff->Lock(0,0,(void**)&pVtx,0);	//ロック

	//頂点座標(奥から順)
	for(int height = 0;height < (MAX_FIELD_BLOCK_Z + 1);height++)
	{
		for(int width = 0;width < (MAX_FIELD_BLOCK_X + 1);width++)
		{
			pVtx[width + height * (MAX_FIELD_BLOCK_X + 1)].pos = D3DXVECTOR3(FIELD_POS_X + FIELD_WIDTH * width,-10.0f,FIELD_POS_Z - FIELD_HEIGHT * height + 15);
			pVtx[width + height * (MAX_FIELD_BLOCK_X + 1)].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[width + height * (MAX_FIELD_BLOCK_X + 1)].color = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[width + height * (MAX_FIELD_BLOCK_X + 1)].tex = D3DXVECTOR2(1.0f * width,1.0f * height);
		}
	}

	m_pVtxBuff->Unlock();	//アンロック

	return 0;
}

//*************************************************************************************************
// 頂点インデックスの設定
//*************************************************************************************************
HRESULT CMeshField2::MakeIndex(LPDIRECT3DDEVICE9 pDevice)
{
	WORD* pIdx;

	//インデックスバッファ
	if(FAILED(pDevice->CreateIndexBuffer(
		sizeof(WORD) * ((MAX_FIELD_BLOCK_X + 1) * 2 * MAX_FIELD_BLOCK_Z + (MAX_FIELD_BLOCK_Z - 1) * 2),	//
		D3DUSAGE_WRITEONLY,							//使用用途フラグ
		D3DFMT_INDEX16,								//インデックスデータのフォーマット
		D3DPOOL_MANAGED,							//メモリの管理方法（お任せ）
		&m_pIdxBuff,							//インデックスバッファインターフェースポインタのアドレス
		NULL)));

	//頂点インデックスの設定////////////////////////////////////////////////////////////////////////
	m_pIdxBuff->Lock(0,0,(void**)&pIdx,0);	//ロック
	//N字型に生成
	//縦のメッシュ生成
	for(int height = 0;height < MAX_FIELD_BLOCK_Z;height++)
	{
		//横のメッシュ生成
		for(int width = 0;width < MAX_FIELD_BLOCK_X + 1;width++)
		{
			//縮退ポリゴンの計算
			if(width == 0 && height != 0)
			{
				pIdx[(height * (MAX_FIELD_BLOCK_X + 2) * 2) - 2] = MAX_FIELD_BLOCK_X + (height - 1) * (MAX_FIELD_BLOCK_X + 1);
				pIdx[(height * (MAX_FIELD_BLOCK_X + 2) * 2) - 1] = (MAX_FIELD_BLOCK_X + 1) * 2 + (height - 1) * (MAX_FIELD_BLOCK_X + 1);
			}
			pIdx[(width * 2) + (height * (MAX_FIELD_BLOCK_X + 2) * 2)] = width + height * (MAX_FIELD_BLOCK_X + 1) + (MAX_FIELD_BLOCK_X + 1);
			pIdx[(width * 2 + 1) + (height * (MAX_FIELD_BLOCK_X + 2) * 2)] = width + height * (MAX_FIELD_BLOCK_X + 1);
		}
	}

	m_pIdxBuff->Unlock();	//アンロック
	return 0;
}

//*************************************************************************************************
// インスタンス生成
//*************************************************************************************************
CMeshField2 *CMeshField2::Create(void)
{
    CMeshField2 *meshfield;

    meshfield = new CMeshField2(PRIOLITY_0);

    meshfield->Init();

    return meshfield;
}
