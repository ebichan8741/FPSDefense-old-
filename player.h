//=============================================================================
//
// プレイヤー [player.h]
// Author : TAKUYA EBIHARA
//
//=============================================================================
#ifndef PLAYER_H
#define PLAYER_H

#include "sceneModel.h"

//=============================================================================
// 構造体宣言
//============================================================================= 

//=============================================================================
// クラス宣言
//=============================================================================
class CPlayer:public CSceneModel
{
public:
    CPlayer(int nPriolity);
    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);

    static CPlayer *Create(D3DXVECTOR3 pos);
	void Hit(int damage);
	void Reload(void);

	int GetLife(void);
    int GetBullet(void);

private:
    D3DXVECTOR3 m_vAngle;
    D3DXVECTOR3 m_Offset;
	bool m_Hit;
	bool m_Reload;
	int m_Life;
	int m_BulletNum;
	int m_BulletRate;
	int m_ReloadCnt;
};

#endif