//=================================================================================================
//
// 3Dポリゴン表示プログラム [scene3D.cpp]
// Author : TAKUYA EBIHARA
//
//=================================================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "scene3D.h"

//=================================================================================================
// 定数定義
//=================================================================================================

//=================================================================================================
// 構造体宣言
//=================================================================================================
//テクスチャ情報構造体
typedef struct
{
	char *pFilename;	// ファイル名
} TEXPARAM_3D;

//=================================================================================================
// グローバル変数
//=================================================================================================
//テクスチャパス
TEXPARAM_3D g_aTexParam[TEXTURE_3D_MAX] = 
{
    "data/TEXTURE/ball.png"
    "data/TEXTURE/lifebar_frame.png",
    "data/TEXTURE/lifebar_BG.png",
    "data/TEXTURE/lifebar_Green.png",
    "data/TEXTURE/lifebar_Red.png"

};

//*************************************************************************************************
// コンストラクタ
//*************************************************************************************************
CScene3D::CScene3D(int nPriolity):CScene(nPriolity)
{
    m_bLoadTexture = false;
}

//*************************************************************************************************
// デストラクタ
//*************************************************************************************************
CScene3D::~CScene3D()
{
    m_bLoadTexture = false;
}

//*************************************************************************************************
// 初期化処理
//*************************************************************************************************
HRESULT CScene3D::Init(void)
{
    LPDIRECT3DDEVICE9 pDevice;

     //デバイスの取得
    pDevice = CManager::GetRenderer()->GetDevice();

    //頂点情報の設定
    if(FAILED(MakeVertex(pDevice)))
    {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CScene3D::Init2(void)
{
    LPDIRECT3DDEVICE9 pDevice;

    //デバイスの取得
    pDevice = CManager::GetRenderer()->GetDevice();

    //頂点情報の設定
    if (FAILED(MakeVertex2(pDevice)))
    {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CScene3D::InitLine(void)
{
    LPDIRECT3DDEVICE9 pDevice;

    //デバイスの取得
    pDevice = CManager::GetRenderer()->GetDevice();

    //頂点情報の設定
    if (FAILED(MakeVertexLine(pDevice)))
    {
        return E_FAIL;
    }

    return S_OK;
}

//*************************************************************************************************
// 終了処理
//*************************************************************************************************
void CScene3D::Uninit(void)
{
    //テクスチャの破棄
    if(m_pTexture != NULL && m_bLoadTexture == true)
    {
        m_pTexture -> Release();
        m_pTexture = NULL;
    }

    //頂点バッファ解放
	if(m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//*************************************************************************************************
// 更新処理
//*************************************************************************************************
void CScene3D::Update(void)
{

}

//*************************************************************************************************
// 描画処理
//*************************************************************************************************
void CScene3D::Draw(void)
{
    LPDIRECT3DDEVICE9 pDevice;

     //デバイスの取得
    pDevice = CManager::GetRenderer()->GetDevice();

	//ストリームの設定
	pDevice->SetStreamSource(0,				//ストリーム番号(パイプ番号)
		m_pVtxBuff,				            //ストリームの元になる頂点バッファ
		0,									//オフセット(バイト)
		sizeof(VERTEX_3D));					//一個の頂点データのサイズ(ストライド量)

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0,m_pTexture);

	//プリミティブの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
		0,										//オフセット(頂点数)
		NUM_POLYGON);							//プリミティブの数
}

void CScene3D::DrawLine(void)
{
    LPDIRECT3DDEVICE9 pDevice;

    //デバイスの取得
    pDevice = CManager::GetRenderer()->GetDevice();

    //ストリームの設定
    pDevice->SetStreamSource(0,				//ストリーム番号(パイプ番号)
        m_pVtxBuff,				            //ストリームの元になる頂点バッファ
        0,									//オフセット(バイト)
        sizeof(VERTEX_3D));					//一個の頂点データのサイズ(ストライド量)

    //頂点フォーマットの設定
    pDevice->SetFVF(FVF_VERTEX_3D);

    //テクスチャの設定
    pDevice->SetTexture(0, NULL);

    //プリミティブの描画
    pDevice->DrawPrimitive(D3DPT_LINELIST,	//プリミティブの種類
        0,									//オフセット(頂点数)
        1);							        //プリミティブの数
}

//*************************************************************************************************
// テクスチャの読み込み
//*************************************************************************************************
void CScene3D::LoadTexture(TEXTURE_LABEL_3D label)
{
    //デバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    //NULLチェック
    if(m_pTexture == NULL)
    {
        //テクスチャの読み込み
        if(FAILED(D3DXCreateTextureFromFile(pDevice,g_aTexParam[label].pFilename,&m_pTexture)))
        {
            //エラー時
            MessageBox(NULL,"テクスチャの読み込みに失敗しました","終了メッセージ",MB_OK);
        }

        m_bLoadTexture = true;
    }
}

//=============================================================================
// テクスチャの取得処理
//=============================================================================
void CScene3D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
    if(pTexture != NULL)
    {
        m_pTexture = pTexture;
    }
}

//*************************************************************************************************
// 頂点情報の設定【中心基準】
//*************************************************************************************************
HRESULT CScene3D::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_3D* pVtx;

	//頂点バッファ
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,		//作成したい頂点バッファのサイズ
		D3DUSAGE_WRITEONLY,							//使用方法
		FVF_VERTEX_3D,								//
		D3DPOOL_MANAGED,							//メモリ管理方法(デバイスにお任せ)
		&m_pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	//仮想アドレスを取得する
	m_pVtxBuff->Lock(0,0,(void**)&pVtx,0);

	//頂点座標の設定
    pVtx[0].pos = D3DXVECTOR3(0.0f - (m_vSize.x * 0.5f),0.0f + (m_vSize.y * 0.5f),0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f + (m_vSize.x * 0.5f),0.0f + (m_vSize.y * 0.5f),0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f - (m_vSize.x * 0.5f),0.0f - (m_vSize.y * 0.5f),0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f + (m_vSize.x * 0.5f),0.0f - (m_vSize.y * 0.5f),0.0f);

    //法線の設定
    pVtx[0].normal = D3DXVECTOR3(0.0f,0.0f,-1.0f);
    pVtx[1].normal = D3DXVECTOR3(0.0f,0.0f,-1.0f);
    pVtx[2].normal = D3DXVECTOR3(0.0f,0.0f,-1.0f);
    pVtx[3].normal = D3DXVECTOR3(0.0f,0.0f,-1.0f);

	//テクスチャの頂点座標
	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

	//頂点カラーの設定(0～255の整数値)
	pVtx[0].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[1].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[2].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[3].color = D3DCOLOR_RGBA(255,255,255,255);

	m_pVtxBuff->Unlock();		//ロック解除

	return 0;
}

//*************************************************************************************************
// 頂点情報の設定【左上基準】
//*************************************************************************************************
HRESULT CScene3D::MakeVertex2(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_3D* pVtx;

	//頂点バッファ
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,		//作成したい頂点バッファのサイズ
		D3DUSAGE_WRITEONLY,							//使用方法
		FVF_VERTEX_3D,								//
		D3DPOOL_MANAGED,							//メモリ管理方法(デバイスにお任せ)
		&m_pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	//仮想アドレスを取得する
	m_pVtxBuff->Lock(0,0,(void**)&pVtx,0);

	//頂点座標の設定
    pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f + m_vSize.x,0.0f,0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f,0.0f - m_vSize.y,0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f + m_vSize.x,0.0f - m_vSize.y,0.0f);

    //法線の設定
    pVtx[0].normal = D3DXVECTOR3(0.0f,0.0f,-1.0f);
    pVtx[1].normal = D3DXVECTOR3(0.0f,0.0f,-1.0f);
    pVtx[2].normal = D3DXVECTOR3(0.0f,0.0f,-1.0f);
    pVtx[3].normal = D3DXVECTOR3(0.0f,0.0f,-1.0f);

	//テクスチャの頂点座標
	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

	//頂点カラーの設定(0～255の整数値)
	pVtx[0].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[1].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[2].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[3].color = D3DCOLOR_RGBA(255,255,255,255);

	m_pVtxBuff->Unlock();		//ロック解除

	return 0;
}

//*************************************************************************************************
// 頂点情報の設定【ライン】
//*************************************************************************************************
HRESULT CScene3D::MakeVertexLine(LPDIRECT3DDEVICE9 pDevice)
{
    VERTEX_3D* pVtx;

    //頂点バッファ
    if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 2,		//作成したい頂点バッファのサイズ
        D3DUSAGE_WRITEONLY,							//使用方法
        FVF_VERTEX_3D,								//
        D3DPOOL_MANAGED,							//メモリ管理方法(デバイスにお任せ)
        &m_pVtxBuff,
        NULL)))
    {
        return E_FAIL;
    }

    //仮想アドレスを取得する
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    //頂点座標の設定
    pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    //法線の設定
    pVtx[0].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
    pVtx[1].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

    //テクスチャの頂点座標
    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
    pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);

    //頂点カラーの設定(0～255の整数値)
    pVtx[0].color = D3DCOLOR_RGBA(255, 255, 255, 255);
    pVtx[1].color = D3DCOLOR_RGBA(255, 255, 255, 255);

    m_pVtxBuff->Unlock();		//ロック解除

    return 0;
}

//*************************************************************************************************
// 頂点座標の設定【ライン】
//*************************************************************************************************
void CScene3D::SetLinePos(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
    VERTEX_3D* pVtx;
    //仮想アドレスを取得する
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    //頂点座標の設定
    pVtx[0].pos = pos1;
    pVtx[1].pos = pos2;

    m_pVtxBuff->Unlock();		//ロック解除
}

//*************************************************************************************************
// 頂点カラーの設定【ライン】
//*************************************************************************************************
void CScene3D::SetLineColor(D3DCOLOR color1, D3DCOLOR color2)
{
    VERTEX_3D* pVtx;
    //仮想アドレスを取得する
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    //頂点カラーの設定(0～255の整数値)
    pVtx[0].color = color1;
    pVtx[1].color = color2;

    m_pVtxBuff->Unlock();		//ロック解除
}

void CScene3D::SetPosition(D3DXVECTOR3 pos)
{
    m_vPos = pos;
}



//*************************************************************************************************
// 大きさ変更
//*************************************************************************************************
void CScene3D::UpdateSize(void)
{
    VERTEX_3D* pVtx;
    //仮想アドレスを取得する
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    //頂点座標の設定

    pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    pVtx[1].pos = D3DXVECTOR3(0.0f + m_vSize.x, 0.0f, 0.0f);
    pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f - m_vSize.y, 0.0f);
    pVtx[3].pos = D3DXVECTOR3(0.0f + m_vSize.x, 0.0f - m_vSize.y, 0.0f);

    m_pVtxBuff->Unlock();		//ロック解除
}

void CScene3D::AnimationTexture(D3DXVECTOR2 texpos, D3DXVECTOR2 texsize) 
{
    VERTEX_3D* pVtx;
    //仮想アドレスを取得する
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    //テクスチャの頂点座標
    pVtx[0].tex = D3DXVECTOR2(texpos.x, texpos.y);
    pVtx[1].tex = D3DXVECTOR2(texpos.x + texsize.x, texpos.y);
    pVtx[2].tex = D3DXVECTOR2(texpos.x, texpos.y + texsize.y);
    pVtx[3].tex = D3DXVECTOR2(texpos.x + texsize.x, texpos.y + texsize.y);

    m_pVtxBuff->Unlock();		//ロック解除
}
