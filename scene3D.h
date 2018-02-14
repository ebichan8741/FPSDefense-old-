//=============================================================================
//
// 3Dポリゴン表示プログラム [scene3D.h]
// Author : TAKUYA EBIHARA
//
//=============================================================================
#ifndef SCENE3D_H
#define SCENE3D_H

#include "scene.h"

//=============================================================================
// 構造体宣言
//=============================================================================
typedef enum
{
    TEXTURE_BULLET = 0,
    TEXTURE_LIFEBAR_FRAME,
    TEXTURE_LIFEBAR_BG,
    TEXTURE_LIFEBAR_GREEN,
    TEXTURE_LIFEBAR_RED,
    TEXTURE_3D_MAX
}TEXTURE_LABEL_3D;


//=============================================================================
// クラス宣言
//=============================================================================
class CScene3D:public CScene
{
private:
    bool m_bLoadTexture;

protected:

    TEXTURE_LABEL_3D m_label;

public:
    CScene3D(int nPriolity);
    ~CScene3D();

    HRESULT Init(void);
    HRESULT Init2(void);
    HRESULT InitLine(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    void DrawLine(void);
    void BindTexture(LPDIRECT3DTEXTURE9 pTexture);
    void LoadTexture(TEXTURE_LABEL_3D label);
    HRESULT MakeVertex(LPDIRECT3DDEVICE9 pDevice);
    HRESULT MakeVertex2(LPDIRECT3DDEVICE9 pDevice);
    HRESULT MakeVertexLine(LPDIRECT3DDEVICE9 pDevice);
    void SetPosition(D3DXVECTOR3 pos);
    void SetLinePos(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);
    void SetLineColor(D3DCOLOR color1,D3DCOLOR color2);
    void UpdateSize(void);
    void AnimationTexture(D3DXVECTOR2 texpos, D3DXVECTOR2 texsize);

};

#endif