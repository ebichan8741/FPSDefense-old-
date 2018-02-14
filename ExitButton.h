//=============================================================================
//
// �I���{�^�� [ ExitButton.h ]
// Author : TAKUYA EBIHARA
//
//=============================================================================
#ifndef EXITBUTTON_H
#define EXITBUTTON_H

//=============================================================================
// �N���X�錾
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
