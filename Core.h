//=============================================================================
//
// コア [Core.h]
// Author : TAKUYA EBIHARA
//
//=============================================================================
#ifndef Core_H
#define Core_H

#include "sceneModel.h"

class CHitDome;
class CLifeBar;
class CLifeBarFrame;
//=============================================================================
// 構造体宣言/ 列挙型
//=============================================================================

//=============================================================================
// クラス宣言
//=============================================================================
class CCore :public CSceneModel
{
public:
    CCore(int nPriolity);
    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);

    static CCore *Create(D3DXVECTOR3 pos, int hp);

    void Hit(int damage);

private:
    int m_Life;
    int m_MaxLife;
    bool m_Use;
    bool m_Hit;
    float m_Rot;
    CHitDome *m_pHitDome;
    CLifeBar *m_pLifeBar;
    CLifeBarFrame *m_pLifeBarFrame;
};

#endif