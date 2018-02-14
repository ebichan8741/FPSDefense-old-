//=============================================================================
//
// �����_�����O���� [renderer.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CRenderer
{
public:
    CRenderer();
    ~CRenderer();

    HRESULT Init(HWND hWnd,bool bWindow);
    void Uninit(void);
    void Update(void);
    void DrawBegin(void);
    void DrawEnd(void);
    LPDIRECT3DDEVICE9 GetDevice(void);

private:
    HRESULT InitPolygon(void);
    void UninitPolygon(void);
    void UpdatePolygon(void);
    void DrawPolygon(void);

    D3DXVECTOR3 m_posPolygon;
    LPDIRECT3D9 m_pD3D;
    LPDIRECT3DDEVICE9 m_pD3DDevice;

    VERTEX_2D g_aVertex[NUM_VERTEX];            //���_���i�[�p�z��
};

#endif

