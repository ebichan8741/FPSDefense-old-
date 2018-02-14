//=================================================================================================
//
// レンダリング処理 [renderer.cpp]
// Author : TAKUYA EBIHARA
//
//=================================================================================================
#include <stdio.h>
#include <string>
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "GameMode.h"
#include "camera.h"
#include "light.h"
#include "scene.h"
#include "scene2D.h"
#include "scene3D.h"
#include "sceneModel.h"
#include "player.h"

//=================================================================================================
// 定数定義
//=================================================================================================

//=================================================================================================
// グローバル変数
//=================================================================================================


//*************************************************************************************************
// コンストラクタ
//*************************************************************************************************
CRenderer::CRenderer()
{

}

//*************************************************************************************************
// デストラクタ
//*************************************************************************************************
CRenderer::~CRenderer()
{

}

//*************************************************************************************************
// 初期化処理
//*************************************************************************************************
HRESULT CRenderer::Init(HWND hWnd, bool bWindow)
{
    D3DPRESENT_PARAMETERS d3dpp;
    D3DDISPLAYMODE d3ddm;

    // Direct3Dオブジェクトの作成
    m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if(m_pD3D == NULL)
    {
        return E_FAIL;
    }

    // 現在のディスプレイモードを取得
    if(FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
    {
        return E_FAIL;
    }

    // デバイスのプレゼンテーションパラメータの設定
    ZeroMemory(&d3dpp, sizeof(d3dpp));                              // ワークをゼロクリア
    d3dpp.BackBufferCount             = 1;                          // バックバッファの数
    d3dpp.BackBufferWidth             = SCREEN_WIDTH;               // ゲーム画面サイズ(幅)
    d3dpp.BackBufferHeight             = SCREEN_HEIGHT;             // ゲーム画面サイズ(高さ)
    d3dpp.BackBufferFormat             = d3ddm.Format;              // カラーモードの指定
    d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD;       // 映像信号に同期してフリップする
    d3dpp.EnableAutoDepthStencil     = TRUE;                        // デプスバッファ（Ｚバッファ）とステンシルバッファを作成
    d3dpp.AutoDepthStencilFormat     = D3DFMT_D16;                  // デプスバッファとして16bitを使う
    d3dpp.Windowed                     = bWindow;                   // ウィンドウモード
    d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;     // リフレッシュレート
    d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT; // インターバル

    // デバイスの生成
    // ディスプレイアダプタを表すためのデバイスを作成
    // 描画と頂点処理をハードウェアで行なう
    if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
                                    D3DDEVTYPE_HAL, 
                                    hWnd, 
                                    D3DCREATE_HARDWARE_VERTEXPROCESSING, 
                                    &d3dpp, &m_pD3DDevice)))
    {
        // 上記の設定が失敗したら
        // 描画をハードウェアで行い、頂点処理はCPUで行なう
        if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
                                        D3DDEVTYPE_HAL, 
                                        hWnd, 
                                        D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
                                        &d3dpp, &m_pD3DDevice)))
        {
            // 上記の設定が失敗したら
            // 描画と頂点処理をCPUで行なう
            if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
                                            D3DDEVTYPE_REF, hWnd, 
                                            D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
                                            &d3dpp, &m_pD3DDevice)))
            {
                // 生成失敗
                return E_FAIL;
            }
        }
    }

    D3DXCOLOR color(1.0f, 1.0f, 1.0, 1.0f); // フォグの色
    FLOAT StartPos = 0;     //フォグ開始位置
    FLOAT EndPos = 3000;    //フォグ終了位置

    // レンダーステートの設定
    m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);              // カリングを行わない
	m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);			// ワイヤーフレーム切替
    m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);                      // Zバッファを使用
    m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);             // αブレンドを行う
    m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);        // αソースカラーの指定
    m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);    // αデスティネーションカラーの指定
    m_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);                    // フォグを有効にする
    m_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, color);                    // フォグの色を設定
    m_pD3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);       // 頂点フォグ
    m_pD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);        // ピクセルフォグ
    m_pD3DDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&StartPos));     // フォグの開始位置
    m_pD3DDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&EndPos));         // フォグの終了位置

    // サンプラーステートの設定
    m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);    // テクスチャＵ値の繰り返し設定
    m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);    // テクスチャＶ値の繰り返し設定
    m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);     // テクスチャ拡大時の補間設定
    m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);     // テクスチャ縮小時の補間設定
    m_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);     // ミップマップ作成時の設定
    m_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);// 異方性フィルタリングの設定
    m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 3);              // 異方性フィルタリングの次数

    // テクスチャステージステートの設定
    m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);  // アルファブレンディング処理(初期値はD3DTOP_SELECTARG1)
    m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);  // 最初のアルファ引数(初期値はD3DTA_TEXTURE、テクスチャがない場合はD3DTA_DIFFUSE)
    m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);  // ２番目のアルファ引数(初期値はD3DTA_CURRENT)

    return S_OK;
}

//*************************************************************************************************
// 終了処理
//*************************************************************************************************
void CRenderer::Uninit(void)
{
    // デバイスの破棄
    if(m_pD3DDevice != NULL)
    {
        m_pD3DDevice->Release();
        m_pD3DDevice = NULL;
    }

    // Direct3Dオブジェクトの破棄
    if(m_pD3D != NULL)
    {
        m_pD3D->Release();
        m_pD3D = NULL;
    }
}

//*************************************************************************************************
// 更新処理
//*************************************************************************************************
void CRenderer::Update(void)
{

}

//*************************************************************************************************
// 描画処理
//*************************************************************************************************
void CRenderer::DrawBegin(void)
{
    // バックバッファ＆Ｚバッファのクリア
    m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(127, 127, 127, 255), 1.0f, 0);

    // Direct3Dによる描画の開始
    m_pD3DDevice->BeginScene();
}

//*************************************************************************************************
// 描画処理
//*************************************************************************************************
void CRenderer::DrawEnd(void)
{
    // Direct3Dによる描画の終了
    m_pD3DDevice->EndScene();

    // バックバッファとフロントバッファの入れ替え
    m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//*************************************************************************************************
// デバイス取得
//*************************************************************************************************
LPDIRECT3DDEVICE9 CRenderer::GetDevice(void)
{
    return m_pD3DDevice;
}
