//=============================================================================
//
// �G�t�F�N�g [effect.h]
// Author : TAKUYA EBIHARA
//
//=============================================================================
#ifndef EFFECT_H
#define EFFECT_H

#include "scene.h"
#include "scene3D.h"

//=============================================================================
// �\���̐錾
//=============================================================================

//=============================================================================
// �N���X�錾
//=============================================================================
class CEffect :public CScene3D
{
public:
	CEffect(int nPriolity);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CEffect *Create(D3DXVECTOR3 pos, D3DXVECTOR3 dir);
	static HRESULT Load(void);
	static void Unload(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;   //�e�N�X�`���|�C���^

	D3DXVECTOR3 m_Dir;
	float m_Move;
	int m_Life;
	int m_Damage;
	bool m_Hit;
	bool m_Use;
};

#endif
