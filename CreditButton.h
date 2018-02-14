//=============================================================================
//
// �N���W�b�g�{�^�� [ CreditButton.h ]
// Author : TAKUYA EBIHARA
//
//=============================================================================
#ifndef CREDITBUTTON_H
#define CREDITBUTTON_H

//=============================================================================
// �N���X�錾
//=============================================================================
class CCreditButton :public CScene2D
{
public:
    CCreditButton(int nPriolity);

    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);

    static CCreditButton* Create(void);

private:
    D3DXCOLOR m_Color;
    float m_AlphaRate;
};


#endif