//=================================================================================================
//
// メッシュシリンダー [meshcylinder.h]
// Author : TAKUYA EBIHARA
//
//=================================================================================================
#ifndef _MESHCYLINDER_H_
#define _MESHCYLINDER_H_

//=================================================================================================
// クラス宣言
//=================================================================================================
class CMeshCylinder :public CScene
{
public:
    CMeshCylinder(int nPriolity);
    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    HRESULT MakeVertex(LPDIRECT3DDEVICE9 pDevice);
    HRESULT MakeIndex(LPDIRECT3DDEVICE9 pDevice);

    static CMeshCylinder *Create(void);

private:
    LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;
};
#endif