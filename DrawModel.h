#ifndef DRAWMODEL_H
#define DRAWMODEL_H

#include "sceneModel.h"
#include "ModelManager.h"

class CSphere;

class CDrawModel :public CSceneModel
{
public:
    CDrawModel(int nPriolity);
    ~CDrawModel();

    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);

    static CDrawModel *Create(std::string modelParam, D3DXVECTOR3 pos, D3DXVECTOR3 angle, D3DXVECTOR3 scale);
    int GetModelParam(void) { return m_modelParam; }
    bool GetModelSelect(void) { return m_bSelect; }
    CDrawModel* GetSelectModelPointer(void);
    void SetSelectFlag(bool b) { m_bSelect = b; }

    void SetPos(D3DXVECTOR3 pos) { m_vPos = pos; }
    void SetAngle(D3DXVECTOR3 angle) { m_vAngle = angle; }
    void SetScale(D3DXVECTOR3 scale) { m_vScale = scale; }
    D3DXVECTOR3 GetPos(void) { return m_vPos; }
    D3DXVECTOR3 GetAngle(void) { return m_vAngle; }
    D3DXVECTOR3 GetScale(void) { return m_vScale; }

private:
    int m_modelParam;
    D3DXVECTOR3 m_vPosition;
    D3DXVECTOR3 m_vAngle;
    D3DXVECTOR3 m_vScale;
    float m_fRadius;
    bool m_bSelect;
    CSphere *m_pSphere;
    CDrawModel *m_pDrawModel;
};

#endif
