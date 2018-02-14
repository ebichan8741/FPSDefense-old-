//=================================================================================================
//
// メッシュフィールド [meshfield.h]
// Author : TAKUYA EBIHARA
//
//=================================================================================================
#ifndef _MESHFIELD2_H_
#define _MESHFIELD2_H_

#include "scene.h"

//=================================================================================================
// クラス宣言
//=================================================================================================
class CMeshField2:public CScene
{
public:
    CMeshField2(int nPriolity);
    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    HRESULT MakeVertex(LPDIRECT3DDEVICE9 pDevice);
    HRESULT MakeIndex(LPDIRECT3DDEVICE9 pDevice);

    static CMeshField2 *Create(void);
    float GetHeight(D3DXVECTOR3 pos);

private:
    LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;

};
#endif