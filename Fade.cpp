//================================================================================================
//
// フェード処理 [Fade.cpp]
// Author : TAKUYA EBIHARA
//
//================================================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "Fade.h"

//=================================================================================================
// 定数定義
//=================================================================================================
#define FADE_TEXTURE	"data/TEXTURE/fade.jpg"
#define FADE_RATE		(0.03f)		//フェード係数

//=================================================================================================
// 静的メンバ変数
//=================================================================================================
FADE CFade::m_fade = FADE_NONE;
CMode *CFade::m_nextMode = 0;

//*************************************************************************************************
// 初期化処理
//*************************************************************************************************
HRESULT CFade::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	m_colorFade = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	//頂点の作成
	if (FAILED(MakeVertex(pDevice)))
	{
		return E_FAIL;
	}

	//ハードディスクからテクスチャを読み込む
	if (FAILED(D3DXCreateTextureFromFile(pDevice, FADE_TEXTURE, &m_pTexture)))
	{
		//エラー時
		MessageBox(NULL, "テクスチャの読み込みに失敗しました", "終了メッセージ", MB_OK);
	}

	return 0;
}

//*************************************************************************************************
// 終了処理
//*************************************************************************************************
void CFade::Uninit(void)
{
	if (m_pTexture != NULL)
	{	
		m_pTexture->Release();
		m_pTexture = NULL;
	}	
		
	if (m_pVtxBuff != NULL)
	{	
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//*************************************************************************************************
// 更新処理
//*************************************************************************************************
void CFade::Update(void)
{
	//フェードなし
	if (m_fade == FADE_NONE)
	{
		return;
	}

	//フェードイン
	if (m_fade == FADE_IN)
	{
		m_colorFade.a -= FADE_RATE;
		if (m_colorFade.a <= 0.0f)
		{
			m_colorFade.a = 0.0f;
			m_fade = FADE_NONE;
		}
	}

	//フェードアウト
	else if (m_fade == FADE_OUT)
	{
		m_colorFade.a += FADE_RATE;
		if (m_colorFade.a >= 1.0f)
		{
			m_colorFade.a = 1.0f;
			m_fade = FADE_IN;
			CManager::SetMode(m_nextMode);
		}
	}

	SetColorFade(m_colorFade);
}

//*************************************************************************************************
// 描画処理
//*************************************************************************************************
void CFade::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//ストリームの設定
	pDevice->SetStreamSource(0,		//ストリーム番号(パイプ番号)
		m_pVtxBuff,				    //ストリームの元になる頂点バッファ
		0,							//オフセット(バイト)
		sizeof(VERTEX_2D));			//一個の頂点データのサイズ(ストライド量)

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	//プリミティブの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
		0,										//オフセット(頂点数)
		NUM_POLYGON);							//プリミティブの数
}


//*************************************************************************************************
// 頂点の作成(フェード)
//*************************************************************************************************
HRESULT CFade::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D* pVtx; //仮想アドレスを取得するためのポインタ

	// 頂点バッファ
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,				//作成したい頂点バッファのサイズ
		D3DUSAGE_WRITEONLY,							//使用方法
		FVF_VERTEX_2D,								//
		D3DPOOL_MANAGED,							//メモリ管理方法(デバイスにお任せ)
		&m_pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	// 仮想アドレスを取得する
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f + SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f + SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f + SCREEN_WIDTH, 0.0f + SCREEN_HEIGHT, 0.0f);

	//rhwの設定(必ず1.0f)
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定(0.0fの整数値)
	pVtx[0].color = m_colorFade;
	pVtx[1].color = m_colorFade;
	pVtx[2].color = m_colorFade;
	pVtx[3].color = m_colorFade;

	//テクスチャの頂点座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	m_pVtxBuff->Unlock();		//ロック解除

	return 0;
}

//*************************************************************************************************
// フェード色の設定
//*************************************************************************************************
void CFade::SetColorFade(D3DXCOLOR color)
{
	VERTEX_2D* pVtx = NULL;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定(0.0fの整数値)
	pVtx[0].color = color;
	pVtx[1].color = color;
	pVtx[2].color = color;
	pVtx[3].color = color;

	m_pVtxBuff->Unlock();
}

//*************************************************************************************************
// フェードの予約
//*************************************************************************************************
void CFade::SetFade(FADE fade,CMode *nextMode)
{
	m_fade = fade;
	m_nextMode = nextMode;
}

//*************************************************************************************************
// 
//*************************************************************************************************
FADE CFade::GetFade(void)
{
	return m_fade;
}
