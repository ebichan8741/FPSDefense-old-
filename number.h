//=============================================================================
//
// �����ݒ菈�� [number.h]
// Author : TAKUYA EBIHARA
//
//=============================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

//*****************************************************************************
// �N���X��`
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
    static LPDIRECT3DTEXTURE9 m_pTexture;   //���L�e�N�X�`���|�C���^
    LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;     //���_�o�b�t�@
    D3DXVECTOR2 m_vPos;             //���_���W
    D3DXVECTOR2 m_vSize;            //�傫��
    D3DXVECTOR2 m_vTexPos;          //�e�N�X�`�����W
    D3DXVECTOR2 m_vTexSize;         //�e�N�X�`���͈̔�

    HRESULT MakeVertexNumber(LPDIRECT3DDEVICE9 pDevice);
    void CNumber::SetVertex(VERTEX_2D* pVtx,int i);
};

#endif