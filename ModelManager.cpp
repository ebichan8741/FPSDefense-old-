//=============================================================================
//
// モデルマネージャー [ModelManager.cpp]
// Author : TAKUYA EBIHARA
//
//=============================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "ModelManager.h"

using namespace std;

typedef map<string, MODEL_INFO> ModelTable;

CModelManager::CModelManager()
{

}

//*************************************************************************************************
// モデルをロードする
// 引数  ： key     モデルのパス及び、検索キー
// 戻り値： true    成功 / false 不成功
//*************************************************************************************************
bool CModelManager::Load(string key)
{
    LPDIRECT3DDEVICE9 pDevice;

    //デバイスの取得
    pDevice = CManager::GetRenderer()->GetDevice();

    // モデルを検索
    ModelTable::const_iterator it = m_ModelInfo.find(key);
    if (it == m_ModelInfo.end()) {            // 見つからなかった場合
        HRESULT hr;
        MODEL_INFO modelInfo;

        hr = D3DXLoadMeshFromX(key.c_str(), D3DXMESH_MANAGED, pDevice, NULL, &modelInfo.pBuff, NULL, &modelInfo.NumMat, &modelInfo.pMesh);
        if (FAILED(hr)) {
            MessageBox(NULL, key.c_str(), "LOAD ERROR!", MB_OK);
            return false;
        }

        //マテリアル情報取得
        modelInfo.pMat = (D3DXMATERIAL*)modelInfo.pBuff->GetBufferPointer();

        //テクスチャがあるか？
        if (modelInfo.pMat[0].pTextureFilename != NULL)
        {
            for (int i = 0; i < (int)modelInfo.NumMat; i++)
            {
                //ハードディスクからテクスチャを読み込む
                if (FAILED(D3DXCreateTextureFromFile(pDevice, modelInfo.pMat[i].pTextureFilename, &modelInfo.pMatTex[i])))
                {
                    //エラー時
                    MessageBox(NULL, "テクスチャの読み込みに失敗しました", "終了メッセージ", MB_OK);
                }
            }
        }

        // マップへ挿入する
        m_ModelInfo.insert(ModelTable::value_type(key, modelInfo));
    }

    return true;
}

//*************************************************************************************************
// @brie 指定したモデルをもらう
// マップ内に指定したモデル無ければ、keyを使ってモデルをロードする。
// ロードに失敗するとNULLポインタが返る。
// 引数   ： key テクスチャのパス及び、検索キー
// 戻り値 ： 成功するとテクスチャのポインタ。失敗するとNULL。
//*************************************************************************************************
MODEL_INFO CModelManager::getResource(string key)
{
    LPDIRECT3DDEVICE9 pDevice;

    //デバイスの取得
    pDevice = CManager::GetRenderer()->GetDevice();

    // テクスチャを検索
    ModelTable::const_iterator it = m_ModelInfo.find(key);
    if (it != m_ModelInfo.end()) {       // 見つかった場合
                                      // テクスチャを返す
        return it->second;
    }
    else {                            // 見つからなかった場合
                                      // 検索した文字列をキーとして
                                      // 新しくマップにテクスチャを追加する
        HRESULT hr;
        MODEL_INFO modelInfo;

        hr = D3DXLoadMeshFromX(key.c_str(), D3DXMESH_MANAGED, pDevice, NULL, &modelInfo.pBuff, NULL, &modelInfo.NumMat, &modelInfo.pMesh);
        if (FAILED(hr)) {
            // 生成できなかった場合はNULLポインタを返す
            MessageBox(NULL, key.c_str(), "LOAD ERROR!", MB_OK);
            return modelInfo;
        }

        //マテリアル情報取得
        modelInfo.pMat = (D3DXMATERIAL*)modelInfo.pBuff->GetBufferPointer();

        //テクスチャがあるか？
        if (modelInfo.pMat[0].pTextureFilename != NULL)
        {
            for (int i = 0; i < (int)modelInfo.NumMat; i++)
            {
                //ハードディスクからテクスチャを読み込む
                if (FAILED(D3DXCreateTextureFromFile(pDevice, modelInfo.pMat[i].pTextureFilename, &modelInfo.pMatTex[i])))
                {
                    //エラー時
                    MessageBox(NULL, "テクスチャの読み込みに失敗しました", "終了メッセージ", MB_OK);
                }
            }
        }

        // マップへ挿入する
        m_ModelInfo.insert(ModelTable::value_type(key, modelInfo));

        return modelInfo;
    }
}