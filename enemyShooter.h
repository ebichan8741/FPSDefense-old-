//=============================================================================
//
// 遠距離型エネミー [enemyShooter.h]
// Author : TAKUYA EBIHARA
//
//=============================================================================
#ifndef ENEMYSHOOTER_H
#define ENEMYSHOOTER_H

#include "enemy.h"

class CLifeBar;
class CLifeBarFrame;
//=============================================================================
// 構造体宣言/ 列挙型
//=============================================================================

//=============================================================================
// クラス宣言
//=============================================================================
class CEnemyShooter :public CEnemy
{
public:
    CEnemyShooter(int nPriolity);
    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);

    static CEnemyShooter *Create(D3DXVECTOR3 pos,int hp, int m_ID);

private:
    CLifeBar *m_pLifeBar;
    CLifeBarFrame *m_pLifeBarFrame;
};

#endif
