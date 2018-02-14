//=============================================================================
//
// 3Dモデル表示プログラム [sceneModel.h]
// Author : TAKUYA EBIHARA
//
//=============================================================================
#ifndef SCENEMODEL_H
#define SCENEMODEL_H

#define MAX_MATERIAL (17)

#include "scene.h"

//=============================================================================
// 構造体宣言
//=============================================================================

// モデル情報構造体
typedef struct {
    LPD3DXMESH pMesh;
    LPD3DXBUFFER pBuff;
    DWORD NumMat;
    LPDIRECT3DTEXTURE9 pMatTex[MAX_MATERIAL];
    D3DXMATERIAL* pMat;
}MODEL_INFO;
//=============================================================================
// クラス宣言
//=============================================================================
class CSceneModel:public CScene
{
private:

protected:
    D3DXVECTOR3 m_vAngle;
    MODEL_INFO m_ModelInfo;

public:
    CSceneModel(int nPriolity);
    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    //void LoadModel(MODEL_LABEL label);

};

#endif