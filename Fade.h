#pragma once
//=============================================================================
//
// �t�F�[�h���� [Fade.h]
// Author : TAKUYA EBIHARA
//
//=============================================================================
#ifndef FADE_H
#define FADE_H

#include "Mode.h"

//=============================================================================
// �񋓌^
//=============================================================================
//�t�F�[�h���[�h
typedef enum
{
	FADE_NONE = 0,
	FADE_IN,		//�t�F�[�h�C��
	FADE_OUT,		//�t�F�[�h�A�E�g
	FADE_MAX
}FADE;

//=============================================================================
// �N���X�錾
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
	LPDIRECT3DTEXTURE9 m_pTexture;	//�e�N�X�`���C���^�[�t�F�[�X�̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	D3DXCOLOR m_colorFade;
	static FADE m_fade;
	static CMode *m_nextMode;

	HRESULT MakeVertex(LPDIRECT3DDEVICE9 pDevice);
	void SetColorFade(D3DXCOLOR color);
};

#endif