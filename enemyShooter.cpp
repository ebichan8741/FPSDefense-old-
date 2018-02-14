//=================================================================================================
//
// 遠距離型エネミー [enemyShooter.cpp]
// Author : TAKUYA EBIHARA
//
//=================================================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "GameMode.h"
#include "enemy.h"
#include "enemyShooter.h"
#include "bullet.h"
#include "EnemyExplosion.h"
#include "Smoke.h"
#include "camera.h"
#include "meshField.h"
#include "meshDome.h"
#include "HitDome.h"
#include "score.h"
#include "ModelManager.h"
#include "Dijkstra.h"
#include "LifeBar.h"
#include "LifeBarFrame.h"

//=================================================================================================
// 定数定義
//=================================================================================================
#define ENEMY_BULLET_MOVE       (10.0f)     // 弾速
#define ENEMY_SHOOT_Y           10.0f       // エネミーのｙ座標

//=================================================================================================
// 構造体宣言
//=================================================================================================

//*************************************************************************************************
// コンストラクタ
//*************************************************************************************************
CEnemyShooter::CEnemyShooter(int nPriolity):CEnemy(nPriolity)
{
    m_Distance_CP = 0.0f;
    m_nextPos = D3DXVECTOR3(0.0f, ENEMY_SHOOT_Y, 0.0f);
}

//*************************************************************************************************
// 初期化処理
//*************************************************************************************************
HRESULT CEnemyShooter::Init(void)
{
    CEnemy::Init();
    return S_OK;
}

//*************************************************************************************************
// 終了処理
//*************************************************************************************************
void CEnemyShooter::Uninit(void)
{
    //m_pLifeBar->Uninit();
    //m_pLifeBarFrame->Uninit();
    CEnemy::Uninit();
}

//*************************************************************************************************
// 更新処理
//*************************************************************************************************
void CEnemyShooter::Update(void)
{
    D3DXVECTOR3 dir, pos;

    // ライフバーの設定
    m_pLifeBar->SetPosition(m_vPos);
    m_pLifeBar->SetHP(m_Life);
    m_pLifeBar->SetMaxHP(m_MaxLife);
    //m_pLifeBar->Update();
    m_pLifeBarFrame->SetPosition(m_vPos);
    m_pLifeBarFrame->SetHP(m_Life);
    //m_pLifeBarFrame->Update();

    // エネミー死亡
    if (m_Life <= 0){
        DecEnemyNum();
        CScore::AddScore(100);
        //CSmoke::Create(m_vPos);
        CEnemyExplosion::Create(m_vPos);
        CManager::GetCamera()->Swing(0.05f);
        Release();
#ifdef _DEBUG
        m_pHitDome->Release();
#endif
        return;
    }

    // エネミーのステート管理
    switch (m_EnemyState)
    {
        //! 進行状態 !//
    case ENEMY_STATE_WALK:
        // チェックポイントとの距離を求める
        dir = m_nextPos - m_vPos;
        m_Distance_CP = D3DXVec3Length(&dir);
        // チェックポイントに前進
        m_vPos += m_vDir;
        // チェックポイントとの距離が一定になると次のチェックポイントを目指す
        if (m_Distance_CP <= 10.0f) {
            //pos = dijkstra->GetPos( m_nodeIndex );
            // 次ノードの座標を取得
            m_nextPos = CGameMode::GetDijkstra()->GetPos(m_nodeIndex + 1);
            m_nextPos.y = ENEMY_SHOOT_Y;
            // 次ノードへの方向ベクトルを求める
            m_nodeIndex++;
        }
        m_vDir = m_nextPos - m_vPos;
        D3DXVec3Normalize(&m_vDir, &m_vDir);
        break;

        //! 攻撃状態 !//
    case ENEMY_STATE_ATTACK:

        //弾発射
        if (m_BulletCnt >= 60) {
            //D3DXVec3Subtract(&dir, &CManager::GetCamera()->GetCameraPos(), &m_vPos);
            m_vTargetDir.x += ((rand() % 3) - 1) * 0.1f;
            m_vTargetDir.y += ((rand() % 3) - 1) * 0.1f;
            m_vTargetDir.z += ((rand() % 3) - 1) * 0.1f;
            CBullet::Create(CEnemy::m_vPos, -m_vTargetDir, ENEMY_BULLET_MOVE, m_Type);
            m_BulletCnt = 0;
        }
        break;
    }

    //m_vPos.y = CGameMode::GetMeshField()->GetHeight(m_vPos) + 30.0f;

    // 回転行列作成
    Direction();

    // エネミーの視界内判定
    if (CheckEnemySite(PRIOLITY_1,SCENE_TYPE_PLAYER) || 
        CheckEnemySite(PRIOLITY_1, SCENE_TYPE_CORE)) {
        m_vDir = -m_vTargetDir;
        m_EnemyState = ENEMY_STATE_ATTACK;
    }
    else {
        m_EnemyState = ENEMY_STATE_WALK;
    }

#ifdef _DEBUG
    m_pHitDome->SetDome(m_vPos, m_vSize);
#endif

    // 当たり判定
    if (m_Hit){
#ifdef _DEBUG
        m_pHitDome->HitToDome();
#endif
        m_Hit = false;
    }

    m_BulletCnt++;
    CEnemy::Update();
}

//*************************************************************************************************
// 描画処理
//*************************************************************************************************
void CEnemyShooter::Draw(void)
{
    //m_pLifeBarFrame->Draw();
    //m_pLifeBar->Draw();

    CEnemy::Draw();
}

//*************************************************************************************************
// インスタンス生成
//*************************************************************************************************
CEnemyShooter *CEnemyShooter::Create(D3DXVECTOR3 pos,int hp, int ID)
{
    CEnemyShooter *enemyShooter;
    enemyShooter = new CEnemyShooter(PRIOLITY_1);

    enemyShooter->m_vPos = pos;
    enemyShooter->m_ID = ID;
    enemyShooter->m_Life = enemyShooter->m_MaxLife = hp;
    enemyShooter->m_pLifeBarFrame = CLifeBarFrame::Create(pos);
    enemyShooter->m_pLifeBar = CLifeBar::Create(pos);

    enemyShooter->Init();

    enemyShooter->m_ModelInfo = CManager::GetModelManager()->getResource(CManager::GetModelManager()->getModelName(MODEL_ENEMY0));
    //enemyShooter->LoadModel(MODEL_ENEMY0);
    enemyShooter->m_Type = SCENE_TYPE_ENEMY;

    return enemyShooter;
}