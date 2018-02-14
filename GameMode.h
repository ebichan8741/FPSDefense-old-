#pragma *once
//=============================================================================
//
// ゲームモード管理 [GameMode.h]
// Author : TAKUYA EBIHARA
//
//=============================================================================
#ifndef _GAMEMODE_H_
#define _GAMEMODE_H_

#include <vector>
#include "Mode.h"

#define NUMBER_DIGIT	(5)		// 弾数の桁数
#define ENEMY_NUM		(2)		// エネミーの数
#define HP_DIGIT		(3)		// HPの桁数
#define SCORE_DIGIT     (6)     // スコアの桁数

//=============================================================================
//前方宣言
//=============================================================================
class CMeshField;
CMeshField *GetMeshField(void);
class CPlayer;
CPlayer *GetPlayer(void);
class CEnemy;
CEnemy *GetEnemy(int ID);
class CEnemyShooter;
class CNumber;
CNumber *GetBulletNumber(int ID);
CNumber *GetHPNumber(int ID);
CNumber *GetScore(int ID);
class CMap;
class Dijkstra;
Dijkstra *GetDijkstra(void);

//=============================================================================
// クラス定義
//=============================================================================
class CGameMode :public CMode
{
public:
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CMeshField *GetMeshField(void);
	static CPlayer *GetPlayer(void);
	static CEnemy *GetEnemy(int ID);
	static CNumber *GetBulletNumber(int ID);
	static CNumber *GetHPNumber(int ID);
    static CNumber *GetScore(int ID);
    static Dijkstra *GetDijkstra(void);

	static bool m_flag;
	static int m_EnemyCnt;

protected:
	static CPlayer *m_pPlayer;
	static CMeshField *m_pMeshField;
	static std::vector<CEnemy*> m_pEnemy;
	static CNumber *m_pBulletNumber[NUMBER_DIGIT];
	static CNumber *m_pHPNumber[HP_DIGIT];
    static CNumber *m_pScore[SCORE_DIGIT];
    static CMap *m_pMap;
    static Dijkstra *m_pDijkstra;

};

#endif