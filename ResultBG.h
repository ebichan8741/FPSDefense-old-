//=============================================================================
//
// リザルト背景 [ ResultBG.h ]
// Author : TAKUYA EBIHARA
//
//=============================================================================
#ifndef RESULTBG_H
#define RESULTBG_H

//=============================================================================
// クラス宣言
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
