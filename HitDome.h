//=================================================================================================
//
// 当たり判定ドーム [hitdome.h]
// Author : TAKUYA EBIHARA
//
//=================================================================================================
#ifndef _HITDOME_H_
#define _HITDOME_H_

//=================================================================================================
// クラス宣言
//=================================================================================================
class CHitDome:public CScene
{
public:
    CHitDome(int nPriolity);
    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    HRESULT MakeVertex(LPDIRECT3DDEVICE9 pDevice);
    HRESULT MakeIndex(LPDIRECT3DDEVICE9 pDevice);

    static CHitDome *Create(D3DXVECTOR3 pos,D3DXVECTOR3 size);
    void SetDome(D3DXVECTOR3 pos,D3DXVECTOR3 size);
    void HitToDome(void);

private:
    LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;
    bool m_Hit;
    D3DXCOLOR m_Color;
    float m_Len;
};
#endif