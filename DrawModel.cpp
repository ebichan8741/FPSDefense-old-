#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "ModelManager.h"
#include "DrawModel.h"
#include "camera.h"
#include "input.h"
#include "debug.h"

CDrawModel::CDrawModel(int nPriolity) :CSceneModel(nPriolity)
{
    m_modelParam = 0;
    m_fRadius = 3.0f;
    m_vAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_vScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
}

HRESULT CDrawModel::Init(void)
{
    //m_vPos = D3DXVECTOR3(0.0f,0.0f,0.0f);
    m_vSize = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
    m_bSelect = false;

    CSceneModel::Init();

    return S_OK;
}

void CDrawModel::Uninit(void)
{

    CSceneModel::Uninit();
}

void CDrawModel::Update(void)
{

    //デバッグ表示
#ifdef _DEBUG
    //char text[1024];
    //for (int i = 0; i < CManager::GetModelManager()->getModelNum(); i++) {
    //    sprintf(text, " %s  \n",
    //        CManager::GetModelManager()->getModelName((MODEL_PARAM)i));
    //}
    //CDebug::AddText(text);
#endif

    CSceneModel::Update();
}

void CDrawModel::Draw(void)
{
    LPDIRECT3DDEVICE9 pDevice;
    D3DXMATRIX mtxWorld, mtxTrans, mtxRot, mtxScale;

    //デバイスの取得
    pDevice = CManager::GetRenderer()->GetDevice();

    D3DXMatrixIdentity(&mtxWorld);

    // 拡縮・回転・平行移動
    D3DXMatrixScaling(&mtxScale, m_vScale.x, m_vScale.y, m_vScale.z);
    D3DXMatrixRotationYawPitchRoll(&mtxRot, m_vAngle.y, m_vAngle.x, m_vAngle.z);
    D3DXMatrixTranslation(&mtxTrans, m_vPos.x, m_vPos.y, m_vPos.z);
    D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);
    D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
    D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

    //ワールド座標変換行列の適用
    pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

    CSceneModel::Draw();

}

CDrawModel *CDrawModel::Create( std::string modelParam, D3DXVECTOR3 pos, D3DXVECTOR3 angle,D3DXVECTOR3 scale)
{
    CDrawModel *model;
    model = new CDrawModel(PRIOLITY_2);

    model->m_vPos = pos;
    //model->m_modelParam = modelParam;
    model->m_ModelInfo = CManager::GetModelManager()->getResource(modelParam);
    model->m_vAngle = angle;
    model->m_vScale = scale;
    model->Init();

    return model;
}

//CDrawModel* CDrawModel::GetSelectModelPointer(void)
//{
//    
//}
