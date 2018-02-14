//=============================================================================
//
// 煙エフェクト [Smoke.h]
// Author : TAKUYA EBIHARA
//
//=============================================================================
#ifndef SMOKE_H
#define SMOKE_H

#include "scene.h"
#include "scene3D.h"

//=============================================================================
// 構造体宣言
//=============================================================================

//=============================================================================
// クラス宣言
//=============================================================================
class CSmoke :public CScene3D
{
public:
    CSmoke(int nPriolity);
    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);

    static CSmoke *Create(D3DXVECTOR3 pos);
    static HRESULT Load(void);
    static void Unload(void);

private:
    static LPDIRECT3DTEXTURE9 m_pTexture;   //テクスチャポインタ
    int m_Life;
    bool m_Use;
    D3DXVECTOR3 m_Velocity;
    D3DXVECTOR2 m_TexPos, m_TexSize;
    float m_Rot;
    float m_RotSpeed;
    int m_Frame;
};

#endif
#pragma once
