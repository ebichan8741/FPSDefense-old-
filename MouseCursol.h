//=================================================================================================
//
// マウスカーソル [MouseCursol.h]
// Author : TAKUYA EBIHARA
//
//=================================================================================================
#ifndef MOUSECURSOL_H
#define MOUSECURSOL_H

#include "scene2D.h"

//=============================================================================
// クラス宣言
//=============================================================================
class CMouseCursol:public CScene2D
{
public:
    CMouseCursol(int nPriolity);
    ~CMouseCursol();
    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static CMouseCursol *Create(void);

private:
    D3DXCOLOR m_Color;

};

#endif