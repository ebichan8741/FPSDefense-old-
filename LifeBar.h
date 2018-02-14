//=============================================================================
//
// ライフバー [ LifeBar.h ]
// Author : TAKUYA EBIHARA
//
//=============================================================================
#ifndef LIFEBAR_H
#define LIFEBAR_H

#include "scene3D.h"

//=============================================================================
// クラス宣言
//=============================================================================
class CLifeBar :public CScene3D
{
public:
    CLifeBar(int nPriolity);

    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);

    static CLifeBar* Create(D3DXVECTOR3 pos);

    static HRESULT Load(void);
    static void Unload(void);

    void SetPosition(D3DXVECTOR3 pos) { m_vPos = pos; }
    void SetHP(int hp) { m_HP = hp; }
    void SetMaxHP(int maxHp) { m_MaxHP = maxHp; }

private:
    int m_ID;
    int m_HP;
    int m_MaxHP;
    static LPDIRECT3DTEXTURE9 m_pTexture;   //テクスチャポインタ

};


#endif
