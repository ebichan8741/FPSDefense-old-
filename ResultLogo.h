//=============================================================================
//
// リザルトロゴ [ ResultLogo.h ]
// Author : TAKUYA EBIHARA
//
//=============================================================================
#ifndef RESULTLOGO_H
#define RESULTLOGO_H

//=============================================================================
// クラス宣言
//=============================================================================
class CResultLogo :public CScene2D
{
public:
    CResultLogo(int nPriolity);

    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);

    static CResultLogo* Create(void);
};
#endif#pragma once
