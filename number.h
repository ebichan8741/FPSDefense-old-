//=============================================================================
//
// 数字設定処理 [number.h]
// Author : TAKUYA EBIHARA
//
//=============================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CNumber
{
public:
    CNumber();
    ~CNumber();
    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);

    static CNumber *Create(D3DXVECTOR2 pos,D3DXVECTOR2 size);
    static HRESULT Load(void);
    static void Unload(void);

    void SetNumber(int nNumber, float y);
    void SetNumber(float nNumber, float y);

private:
    static LPDIRECT3DTEXTURE9 m_pTexture;   //共有テクスチャポインタ
    LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;     //頂点バッファ
    D3DXVECTOR2 m_vPos;             //頂点座標
    D3DXVECTOR2 m_vSize;            //大きさ
    D3DXVECTOR2 m_vTexPos;          //テクスチャ座標
    D3DXVECTOR2 m_vTexSize;         //テクスチャの範囲

    HRESULT MakeVertexNumber(LPDIRECT3DDEVICE9 pDevice);
    void CNumber::SetVertex(VERTEX_2D* pVtx,int i);
};

#endif