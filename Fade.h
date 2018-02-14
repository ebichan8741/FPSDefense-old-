#pragma once
//=============================================================================
//
// フェード処理 [Fade.h]
// Author : TAKUYA EBIHARA
//
//=============================================================================
#ifndef FADE_H
#define FADE_H

#include "Mode.h"

//=============================================================================
// 列挙型
//=============================================================================
//フェードモード
typedef enum
{
	FADE_NONE = 0,
	FADE_IN,		//フェードイン
	FADE_OUT,		//フェードアウト
	FADE_MAX
}FADE;

//=============================================================================
// クラス宣言
//=============================================================================
class CFade
{
public:
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void SetFade(FADE fade,CMode *nextMode);
	static FADE GetFade(void);

private:
	LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャインターフェースのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	D3DXCOLOR m_colorFade;
	static FADE m_fade;
	static CMode *m_nextMode;

	HRESULT MakeVertex(LPDIRECT3DDEVICE9 pDevice);
	void SetColorFade(D3DXCOLOR color);
};

#endif