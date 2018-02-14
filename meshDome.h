//=================================================================================================
//
// メッシュドーム [meshdome.h]
// Author : TAKUYA EBIHARA
//
//=================================================================================================
#ifndef _MESHDOME_H_
#define _MESHDOME_H_

//=================================================================================================
// クラス宣言
//=================================================================================================
class CMeshDome:public CScene
{
public:
    CMeshDome(int nPriolity);
    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    HRESULT MakeVertex(LPDIRECT3DDEVICE9 pDevice);
    HRESULT MakeIndex(LPDIRECT3DDEVICE9 pDevice);

    static CMeshDome *Create(void);

private:
    LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;
};
#endif