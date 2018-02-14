//=============================================================================
//
// モデルマネージャー [ModelManager.h]
// Author : TAKUYA EBIHARA
//
//=============================================================================
#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include "main.h"
#include "sceneModel.h"

//=============================================================================
// 構造体・列挙型
//=============================================================================
// モデル列挙
typedef enum
{
    MODEL_PLAYER = 0,       //プレイヤーモデル
    MODEL_ENEMY0,
    MODEL_CORE,
    MODEL_BLOCK000,
    MODEL_BLOCK001,
    MODEL_BLOCK002,
    MODEL_MAX
}MODEL_PARAM;

//=============================================================================
// クラス宣言
//=============================================================================
class CModelManager
{
public:
    CModelManager();
    ~CModelManager();

    // テクスチャを読み込む
    bool Load(std::string key);

    // テクスチャを取得する
    MODEL_INFO getResource(std::string key);
    char* getModelName(MODEL_PARAM modelParam) { return m_aModelName[modelParam]; }

    bool Release(std::string key);

private:
    std::map<std::string, MODEL_INFO> m_ModelInfo;
    std::map<std::string, MODEL_INFO> m_ModelNull;

    char* m_aModelName[MODEL_MAX] =
    {
        "data/MODEL/AK_47.x",
        "data/MODEL/enemy001.x",
        "data/MODEL/core000.x",
        "data/MODEL/block002.x",
        "data/MODEL/block001.x",
        "data/MODEL/block003.x"
    };
};

#endif