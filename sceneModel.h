//=============================================================================
//
// 3D���f���\���v���O���� [sceneModel.h]
// Author : TAKUYA EBIHARA
//
//=============================================================================
#ifndef SCENEMODEL_H
#define SCENEMODEL_H

#define MAX_MATERIAL (17)

#include "scene.h"

//=============================================================================
// �\���̐錾
//=============================================================================

// ���f�����\����
typedef struct {
    LPD3DXMESH pMesh;
    LPD3DXBUFFER pBuff;
    DWORD NumMat;
    LPDIRECT3DTEXTURE9 pMatTex[MAX_MATERIAL];
    D3DXMATERIAL* pMat;
}MODEL_INFO;
//=============================================================================
// �N���X�錾
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