//=============================================================================
//
// エネミー近距離型 [enemyFighter.h]
// Author : TAKUYA EBIHARA
//
//=============================================================================
#ifndef ENEMYFIGHTER_H
#define ENEMYFIGHTER_H

#include "sceneModel.h"
#include "EnemyManager.h"

class CHitDome;
//=============================================================================
// 構造体宣言/ 列挙型
//=============================================================================
typedef enum
{
    ENEMY_STATE_MARCH = 0,		//進行状態
    ENEMY_STATE_ATTACK,			//攻撃状態
    ENEMY_STATE_MAX
}ENEMY_STATE;

typedef enum
{
    ENEMY_TYPE_NORMAL = 0
}ENEMY_TYPE;

//=============================================================================
// クラス宣言
//=============================================================================
class CEnemy :public CSceneModel
{
public:
    CEnemy(int nPriolity);
    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);

    static CEnemy *Create(D3DXVECTOR3 pos, int m_ID, ENEMY_TYPE enemyType);
    void Hit(int damage);
    bool CheckEnemySite(void);
    static int GetEnemyNum(void);
    void DecEnemyNum(void);
    void Path(void);
    void Direction(void);

    static int m_EnemyCnt;
    int m_Life;
private:
    D3DXVECTOR3 m_vAngle;
    D3DXVECTOR3 m_vDir;
    D3DXVECTOR3 m_vUp;
    D3DXVECTOR3 m_vSiteDir;
    D3DXVECTOR3 m_vTargetDir;
    D3DXMATRIX m_mtxRot;
    CHitDome *m_pHitDome;
    bool m_Hit;

    int m_ID;
    float m_Dot;
    float m_Radian;
    float m_PathDistance;
    float m_Distance;
    ENEMY_STATE m_EnemyState;
    ENEMY_TYPE m_EnemyType;
    int m_BulletCnt;
    int m_CheckPointCnt;

};

#endif