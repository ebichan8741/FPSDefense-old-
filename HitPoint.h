//=============================================================================
//
// �q�b�g�|�C���g [ HitPoint.h ]
// Author : TAKUYA EBIHARA
//
//=============================================================================
#ifndef HITPOINT_H
#define HITPOINT_H



//=============================================================================
// �N���X�錾
//=============================================================================
class CHitPoint :public CScene2D
{
public:
	CHitPoint(int nPriolity);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CHitPoint* Create(void);
};


#endif
