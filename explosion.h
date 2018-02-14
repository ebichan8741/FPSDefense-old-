//=============================================================================
//
// 爆発エフェクト [explosion.h]
// Author : TAKUYA EBIHARA
//
//=============================================================================
#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "scene.h"
#include "scene3D.h"

//=============================================================================
// 構造体宣言
//=============================================================================

//=============================================================================
// クラス宣言
//=============================================================================
class CExplosion :public CScene3D
{
public:
    CExplosion(int nPriolity);
    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);

    static CExplosion *Create(D3DXVECTOR3 pos);
    static HRESULT Load(void);
    static void Unload(void);

private:
    static LPDIRECT3DTEXTURE9 m_pTexture;   //テクスチャポインタ
    int m_Life;
    bool m_Use;
    D3DXVECTOR2 m_TexPos,m_TexSize;
    int m_AnimCnt;
};

#endif
