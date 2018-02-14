//=============================================================================
//
// エネミー [enemy.h]
// Author : TAKUYA EBIHARA
//
//=============================================================================
#ifndef ENEMY_H
#define ENEMY_H

#include "sceneModel.h"
#include "EnemyManager.h"

class CHitDome;
//=============================================================================
// 構造体宣言/ 列挙型
//=============================================================================
#define ENEMY_MOVE_SPEED	(1.0f)		// エネミーの移動速度

//=============================================================================
// クラス宣言
//=============================================================================
class CEnemy:public CSceneModel
{
public:
    CEnemy(int nPriolity);
    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);

    static CEnemy *Create(D3DXVECTOR3 pos,int m_ID);
    void Hit(int damage);
	bool CheckEnemySite(PRIOLITY priolity, SCENE_TYPE scene);
	static int GetEnemyNum(void);
	void DecEnemyNum(void);
    int GetLife(void);
    int GetMaxLife(void);
	void Path(void);
	void Direction(void);

	static int m_EnemyCnt;
	int m_Life;

protected:
    D3DXVECTOR3 m_vAngle;
	D3DXVECTOR3 m_vDir;
	D3DXVECTOR3 m_vUp;
	D3DXVECTOR3 m_vSiteDir;
	D3DXVECTOR3 m_vTargetDir;
	D3DXMATRIX m_mtxRot;
    CHitDome *m_pHitDome;
    bool m_Hit;

	int m_ID;
    int m_MaxLife;
	float m_Dot;
	float m_Radian;
	float m_PathDistance;
	float m_Distance;
    float m_Distance_CP;
	ENEMY_STATE m_EnemyState;
	ENEMY_TYPE m_EnemyType;
	int m_BulletCnt;
	int m_CheckPointCnt;
    ENEMYDATA m_Data;
    int m_nodeIndex;
    D3DXVECTOR3 m_nextPos;
    D3DXVECTOR3 m_Dir;

};

#endif