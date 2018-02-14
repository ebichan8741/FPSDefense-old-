//=============================================================================
//
// ライフバーのフレーム [ LifeBarFrame.h ]
// Author : TAKUYA EBIHARA
//
//=============================================================================
#ifndef LIFEBARFRAME_H
#define LIFEBARFRAME_H

#include "scene3D.h"

#define LIFEBARFRAME_SIZE_X	(12.0f)		// 幅
#define LIFEBARFRAME_SIZE_Y	(2.0f)		// 高さ

//=============================================================================
// クラス宣言
//=============================================================================
class CLifeBarFrame :public CScene3D
{
public:
    CLifeBarFrame(int nPriolity);

    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);

    static CLifeBarFrame* Create(D3DXVECTOR3 pos);

    static HRESULT Load(void);
    static void Unload(void);

    void SetPosition(D3DXVECTOR3 pos) { m_vPos = pos; }
    void SetHP(int hp) { m_HP = hp; }

private:
    int m_ID;
    int m_HP;
    static LPDIRECT3DTEXTURE9 m_pTexture;   //テクスチャポインタ

};


#endif