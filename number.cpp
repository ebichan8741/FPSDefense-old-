//=============================================================================
//
// 数字設定処理 [number.cpp]
// Author : TAKUYA EBIHARA
//
//=============================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "number.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUMBER_TEXTURENAME "data/TEXTURE/number002.png"

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CNumber::m_pTexture = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CNumber::CNumber()
{
    m_vTexPos = D3DXVECTOR2(0.0f,0.0f);
    m_vTexSize = D3DXVECTOR2(0.1f,0.5f);
}

//=============================================================================
// デストラクタ
//=============================================================================
CNumber::~CNumber()
{

}


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CNumber::Init(void)
{
    //デバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // 頂点情報を設定
    if(FAILED(MakeVertexNumber(pDevice)))
    {
        //エラー時
        MessageBox(NULL,"頂点情報の設定に失敗しました","終了メッセージ",MB_OK);
    }

    return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CNumber::Uninit(void)
{
    // 頂点バッファの破棄
    if( m_pVtxBuff != NULL )
    {
        m_pVtxBuff -> Release();
        m_pVtxBuff = NULL;
    }

    // テクスチャの破棄
    if( m_pTexture != NULL)
    {
        m_pTexture -> Release();
        m_pTexture = NULL;
    }
}

//=============================================================================
// 更新処理
//=============================================================================
void CNumber::Update(void)
{
    VERTEX_2D* pVtx; //仮想アドレスを取得するためのポインタ

    //仮想アドレスを取得する
    m_pVtxBuff->Lock(0,0,(void**)&pVtx,0);

    //数字の設定
    //CNumber::SetNumber(nNum);

    //頂点の設定
    CNumber::SetVertex(pVtx,0);

    m_pVtxBuff->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void CNumber::Draw(void)
{
    LPDIRECT3DDEVICE9 pDevice;
    pDevice = CManager::GetRenderer()->GetDevice();


    // 頂点バッファをデータストリームに設定
    pDevice->SetStreamSource(0,         //ストリーム番号(パイプ番号)
        m_pVtxBuff,                     //ストリームの元になる頂点バッファ
        0,                              //オフセット(バイト)
        sizeof(VERTEX_2D));             //一個の頂点データのサイズ(ストライド量)

    // 頂点フォーマットの設定
    pDevice->SetFVF(FVF_VERTEX_2D);

    // テクスチャの設定
    pDevice->SetTexture(0,m_pTexture);

    //プリミティブの描画
    pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,    //プリミティブの種類
        0,                                        //オフセット(頂点数)
        NUM_POLYGON);                            //プリミティブの数
}

//=============================================================================
// 頂点情報の設定
//=============================================================================
HRESULT CNumber::MakeVertexNumber(LPDIRECT3DDEVICE9 pDevice)
{
    VERTEX_2D* pVtx; //仮想アドレスを取得するためのポインタ

    // 頂点バッファの生成
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,                //作成したい頂点バッファのサイズ
        D3DUSAGE_WRITEONLY,                            //使用方法
        FVF_VERTEX_2D,                                 //
        D3DPOOL_MANAGED,                               //メモリ管理方法(デバイスにお任せ)
        &m_pVtxBuff,
        NULL)))
    {
        return E_FAIL;
    }

    //仮想アドレスを取得する
    m_pVtxBuff->Lock(0,0,(void**)&pVtx,0);

    //頂点座標の設定(２D座標・右回り)
    pVtx[0].pos = D3DXVECTOR3(m_vPos.x,m_vPos.y,0.0f);
    pVtx[1].pos = D3DXVECTOR3(m_vPos.x + m_vSize.x,m_vPos.y,0.0f);
    pVtx[2].pos = D3DXVECTOR3(m_vPos.x,m_vPos.y + m_vSize.y,0.0f);
    pVtx[3].pos = D3DXVECTOR3(m_vPos.x + m_vSize.x,m_vPos.y + m_vSize.y,0.0f);

    //テクスチャの頂点座標
    pVtx[0].tex = D3DXVECTOR2(m_vTexPos.x,m_vTexPos.y);
    pVtx[1].tex = D3DXVECTOR2(m_vTexPos.x + m_vTexSize.x,m_vTexPos.y);
    pVtx[2].tex = D3DXVECTOR2(m_vTexPos.x,m_vTexPos.y + m_vTexSize.y);
    pVtx[3].tex = D3DXVECTOR2(m_vTexPos.x + m_vTexSize.x,m_vTexPos.y + m_vTexSize.y);

    //rhwの設定(必ず1.0f)
    pVtx[0].rhw = 1.0f;
    pVtx[1].rhw = 1.0f;
    pVtx[2].rhw = 1.0f;
    pVtx[3].rhw = 1.0f;

    //頂点カラーの設定(0～255の整数値)
    pVtx[0].color = D3DCOLOR_RGBA(255,255,255,255);
    pVtx[1].color = D3DCOLOR_RGBA(255,255,255,255);
    pVtx[2].color = D3DCOLOR_RGBA(255,255,255,255);
    pVtx[3].color = D3DCOLOR_RGBA(255,255,255,255);

    m_pVtxBuff->Unlock();

    return 0;
}

//=============================================================================
// 頂点情報更新処理
//=============================================================================
void CNumber::SetVertex(VERTEX_2D* pVtx,int i)
{
    //頂点座標の設定(２D座標・右回り)
    pVtx[i * NUM_VERTEX].pos     = D3DXVECTOR3(m_vPos.x,m_vPos.y,0.0f);
    pVtx[i * NUM_VERTEX + 1].pos = D3DXVECTOR3(m_vPos.x + m_vSize.x,m_vPos.y,0.0f);
    pVtx[i * NUM_VERTEX + 2].pos = D3DXVECTOR3(m_vPos.x,m_vPos.y + m_vSize.y,0.0f);
    pVtx[i * NUM_VERTEX + 3].pos = D3DXVECTOR3(m_vPos.x + m_vSize.x,m_vPos.y + m_vSize.y,0.0f);

    //テクスチャの頂点座標
    pVtx[i * NUM_VERTEX + 0].tex = D3DXVECTOR2(m_vTexPos.x,              m_vTexPos.y);
    pVtx[i * NUM_VERTEX + 1].tex = D3DXVECTOR2(m_vTexPos.x + m_vTexSize.x,m_vTexPos.y);
    pVtx[i * NUM_VERTEX + 2].tex = D3DXVECTOR2(m_vTexPos.x,              m_vTexPos.y + m_vTexSize.y);
    pVtx[i * NUM_VERTEX + 3].tex = D3DXVECTOR2(m_vTexPos.x + m_vTexSize.x,m_vTexPos.y + m_vTexSize.y);

    //rhwの設定(必ず1.0f)
    pVtx[i * NUM_VERTEX + 0].rhw = 1.0f;
    pVtx[i * NUM_VERTEX + 1].rhw = 1.0f;
    pVtx[i * NUM_VERTEX + 2].rhw = 1.0f;
    pVtx[i * NUM_VERTEX + 3].rhw = 1.0f;

    //頂点カラーの設定(0～255の整数値)
    pVtx[i * NUM_VERTEX + 0].color = D3DCOLOR_RGBA(255,255,255,255);
    pVtx[i * NUM_VERTEX + 1].color = D3DCOLOR_RGBA(255,255,255,255);
    pVtx[i * NUM_VERTEX + 2].color = D3DCOLOR_RGBA(255,255,255,255);
    pVtx[i * NUM_VERTEX + 3].color = D3DCOLOR_RGBA(255,255,255,255);
}

//=============================================================================
// 数字生成
//=============================================================================
CNumber *CNumber::Create(D3DXVECTOR2 pos,D3DXVECTOR2 size)
{
    CNumber *pNumber;

    //メモリの動的確保
    pNumber = new CNumber;
    pNumber->Init();

    pNumber->m_vPos = pos;
    pNumber->m_vSize = size;

    return pNumber;
}

//=============================================================================
// テクスチャの読み込み
//=============================================================================
HRESULT CNumber::Load(void)
{
    //デバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    if(m_pTexture == NULL)
    {
        //テクスチャの読み込み
        if(FAILED(D3DXCreateTextureFromFile(pDevice,NUMBER_TEXTURENAME,&m_pTexture)))
        {
            //エラー時
            MessageBox(NULL,"テクスチャの読み込みに失敗しました","終了メッセージ",MB_OK);
        }
    }
    return S_OK;
}


//=============================================================================
// テクスチャポインタの破棄
//=============================================================================
void CNumber::Unload(void)
{
    //テクスチャの破棄
    if(m_pTexture != NULL)
    {
        m_pTexture -> Release();
        m_pTexture = NULL;
    }
}

//=============================================================================
// 値設定
//=============================================================================
void CNumber::SetNumber(int nNumber,float y)
{
    m_vTexPos.x = nNumber * 0.1f;
	m_vTexPos.y = y;
}

void CNumber::SetNumber(float fNumber, float y)
{
    m_vTexPos.x = fNumber * 0.1f;
    m_vTexPos.y = y;
}