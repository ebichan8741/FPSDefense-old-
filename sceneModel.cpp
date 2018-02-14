//=================================================================================================
//
// 3Dモデル表示プログラム [sceneModel.cpp]
// Author : TAKUYA EBIHARA
//
//=================================================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "sceneModel.h"
#include "ModelManager.h"

//=================================================================================================
// 定数定義
//=================================================================================================


//=================================================================================================
// 構造体宣言
//=================================================================================================
//モデル情報構造体
typedef struct
{
	char *pFilename;	// ファイル名
} MODELPARAM;

//=================================================================================================
// グローバル変数
//=================================================================================================
MODELPARAM g_aModelParam[MODEL_MAX] = {
    "data/MODEL/AK_47.x",
    "data/MODEL/enemy001.x",
    "data/MODEL/core000.x"
};

CSceneModel::CSceneModel(int nPriolity):CScene(nPriolity)
{

}

//*************************************************************************************************
// 初期化処理
//*************************************************************************************************
HRESULT CSceneModel::Init(void)
{
     LPDIRECT3DDEVICE9 pDevice;

     //デバイスの取得
    pDevice = CManager::GetRenderer()->GetDevice();

    return S_OK;
}

//*************************************************************************************************
// 終了処理
//*************************************************************************************************
void CSceneModel::Uninit(void)
{
	// メッシュ情報の解放
	if (m_ModelInfo.pMesh != NULL)
	{
        m_ModelInfo.pMesh->Release();
        m_ModelInfo.pMesh = NULL;
	}

    // 頂点バッファ解放
    if(m_ModelInfo.pBuff != NULL )
    {
        m_ModelInfo.pBuff -> Release();		//マテリアルバッファーの解放
        m_ModelInfo.pBuff = NULL;
    }

	// テクスチャの解放
	for (int i = 0; i < (int)m_ModelInfo.NumMat; i++)
	{
        if (m_ModelInfo.pMatTex[i] != NULL)
		{
            m_ModelInfo.pMatTex[i]->Release();
            m_ModelInfo.pMatTex[i] = NULL;
		}
	}

	// マテリアル情報の解放
	if (m_ModelInfo.pMat != NULL)
	{
        m_ModelInfo.pMat = NULL;
	}
}

//*************************************************************************************************
// 更新処理
//*************************************************************************************************
void CSceneModel::Update(void)
{

}

//*************************************************************************************************
// 描画処理
//*************************************************************************************************
void CSceneModel::Draw(void)
{
    LPDIRECT3DDEVICE9 pDevice;
    D3DMATERIAL9 matDef;
    D3DXMATRIX mtxWorld;
    D3DXMATRIX mtxTrans;
    D3DXMATRIX mtxRotY;

    //デバイスの取得
    pDevice = CManager::GetRenderer()->GetDevice();

    //頂点フォーマットの設定
    pDevice->SetFVF(FVF_VERTEX_3D);

    //m_ModelInfo->pMat = (D3DXMATERIAL*)m_ModelInfo->pBuff->GetBufferPointer();

    pDevice->GetMaterial(&matDef);		//マテリアル情報設定(現在の情報再設定)

    for (int i = 0; i < (int)m_ModelInfo.NumMat; i++)
    {
        //マテリアルの設定
        pDevice->SetMaterial(&m_ModelInfo.pMat[i].MatD3D);
        //テクスチャの設定
        if (m_ModelInfo.pMat[i].pTextureFilename != NULL) {     // テクスチャが存在する
            pDevice->SetTexture(0, m_ModelInfo.pMatTex[i]);
        }
        else {                                                  // テクスチャが存在しない
            pDevice->SetTexture(0, NULL);
        }
        //メッシュの描画
        m_ModelInfo.pMesh->DrawSubset(i);
    }

    pDevice->SetMaterial(&matDef);		//マテリアル情報設定(現在の情報再設定)
}
