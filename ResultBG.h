//=============================================================================
//
// ���U���g�w�i [ ResultBG.h ]
// Author : TAKUYA EBIHARA
//
//=============================================================================
#ifndef RESULTBG_H
#define RESULTBG_H

//=============================================================================
// �N���X�錾
//=============================================================================
class CResultBG :public CScene2D
{
public:
	CResultBG(int nPriolity);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CResultBG* Create(void);
};


#endif#pragma once
