#ifndef ENEMYMANAGER_H
#define ENEMYMANAGER_H

#include "GameMode.h"


//=============================================================================
// 構造体宣言/ 列挙型
//=============================================================================
typedef enum
{
    ENEMY_STATE_WALK = 0,		//進行状態
    ENEMY_STATE_ATTACK,			//攻撃状態
    ENEMY_STATE_MAX
}ENEMY_STATE;

typedef enum
{
    ENEMY_TYPE_SHOOTER = 0,     // シューター
    ENEMY_TYPE_FIGHTER,         // ファイター
    ENEMY_TYPE_BOOMER           // ボマー
}ENEMY_TYPE;

struct ENEMYDATA {
    int type;           //敵種類
    int stype;          //弾種類
    int m_pattern;      //移動パターン
    int s_pattern;      //発射パターン
    int in_time;        //出現時間
    int stop_time;      //停止時間
    int shot_time;      //弾発射時間
    int out_time;       //帰還時間
    D3DXVECTOR3 pos;    // 座標
    float speed;        //弾スピード
    int hp;             //HP
    int item;           //アイテム
};


//=============================================================================
// クラス宣言
//=============================================================================
class CEnemyManager:public CGameMode
{
public:
	static void EnemyLoad(void);
    static int GetMaxEnemy(void);

private:
    static int m_MaxEnemy;
};

#endif 