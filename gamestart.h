//=============================================================================
//
// ゲームスタートボタン [ gamestart.h ]
// Author : TAKUYA EBIHARA
//
//=============================================================================
#ifndef GAMESTART_H
#define GAMESTART_H



//=============================================================================
// クラス宣言
//=============================================================================
class CGameStart :public CScene2D
{
public:
    CGameStart(int nPriolity);

    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);

    static CGameStart* Create(void);

private:
    D3DXCOLOR m_Color;
    float m_AlphaRate;
};


#endif