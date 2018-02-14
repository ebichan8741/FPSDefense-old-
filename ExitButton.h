//=============================================================================
//
// 終了ボタン [ ExitButton.h ]
// Author : TAKUYA EBIHARA
//
//=============================================================================
#ifndef EXITBUTTON_H
#define EXITBUTTON_H

//=============================================================================
// クラス宣言
//=============================================================================
class CExitButton :public CScene2D
{
public:
    CExitButton(int nPriolity);

    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);

    static CExitButton* Create(void);

private:
    D3DXCOLOR m_Color;
    float m_AlphaRate;
};


#endif
