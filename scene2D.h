//=============================================================================
//
// 2Dポリゴン表示プログラム [scene2D.h]
// Author : TAKUYA EBIHARA
//
//=============================================================================
#ifndef SCENE2D_H
#define SCENE2D_H

#include "scene.h"

//=============================================================================
// クラス宣言
//=============================================================================
class CScene2D:public CScene
{
public:
    CScene2D(int nPriolity);
    ~CScene2D();
    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    HRESULT MakeVertex(LPDIRECT3DDEVICE9 pDevice);
    //void LoadTexture(TEXTURE_LABEL label);
    void SetColor(D3DXCOLOR color);

private:
    bool m_bLoadTexture;

protected:
    D3DXVECTOR2 m_TexPos, m_TexSize;
};

#endif