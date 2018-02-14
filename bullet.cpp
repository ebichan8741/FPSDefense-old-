//=================================================================================================
//
// プレイヤー [player.cpp]
// Author : TAKUYA EBIHARA
//
//=================================================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "scene.h"
#include "scene3D.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "explosion.h"
#include "collision.h"
#include "GameMode.h"
#include "Core.h"

//=================================================================================================
// 定数定義
//=================================================================================================
#define BULLET_LIFE     (100)
#define BULLET_MOVE     (2.0f)
#define BULLET_DAMAGE   (1.0f)
#define BULLET_TEXTURE "data/TEXTURE/bullet.png"

//=================================================================================================
// 静的メンバ変数
//=================================================================================================
LPDIRECT3DTEXTURE9 CBullet::m_pTexture = NULL;

//*************************************************************************************************
// コンストラクタ
//*************************************************************************************************
CBullet::CBullet(int nPriolity):CScene3D(nPriolity)
{
}

//*************************************************************************************************
// 初期化処理
//*************************************************************************************************
HRESULT CBullet::Init(void)
{
    //メンバ初期化
    m_vSize     = D3DXVECTOR3(1.0f,1.0f,0.0f);
    m_Life      = BULLET_LIFE;
    m_Move      = BULLET_MOVE;
    m_Damage    = BULLET_DAMAGE;
    m_LengthSq  = m_vSize.x * m_vSize.x + m_vSize.y * m_vSize.y;
    CScene3D::InitLine();
    return S_OK;
}

//*************************************************************************************************
// 終了処理
//*************************************************************************************************
void CBullet::Uninit(void)
{
    CScene3D::Uninit();
}

//*************************************************************************************************
// 更新処理
//*************************************************************************************************
void CBullet::Update(void)
{
    //---------------------------------------------------------------------------------------------
    // 変数宣言開始
    //---------------------------------------------------------------------------------------------
    VERTEX_3D* pVtx; //仮想アドレスを取得するためのポインタ
    D3DXVECTOR3 vMove;
    //---------------------------------------------------------------------------------------------
    // 変数宣言終了
    //---------------------------------------------------------------------------------------------

    //使用フラグがfalseの時は処理しない
    if(!m_Use){
        Release();
        return;
    }

    m_OldPos = m_vPos;

    //移動量計算
    vMove = m_Dir * m_Move;
    m_vPos += vMove;

    //寿命減算
    m_Life--;
    if(m_Life <= 0){
        m_Use = false;
        Release();
    }

    HitBullet();

    CScene3D::Update();
}

//*************************************************************************************************
// 描画処理
//*************************************************************************************************
void CBullet::Draw(void)
{
    LPDIRECT3DDEVICE9 pDevice;
    D3DXMATRIX mtxWorld;   //ワールド座標変換行列
    D3DXMATRIX mtxTrans,mtxScale,mtxInv;

    //デバイスの取得
    pDevice = CManager::GetRenderer()->GetDevice();

    // 加算合成
    pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

    //ライトのオフ
    pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);

    //ワールド座標変換行列の初期化
    D3DXMatrixIdentity(&mtxWorld);

    //ワールド座標変換行列の適用
    pDevice->SetTransform(D3DTS_WORLD,&mtxWorld);

    CScene3D::SetLinePos(m_OldPos,m_vPos);
    CScene3D::SetLineColor(D3DCOLOR_RGBA(255, 191, 63, 191), D3DCOLOR_RGBA(255, 191, 63, 255));
    CScene3D::DrawLine();

    //ライトのオン
    pDevice->SetRenderState(D3DRS_LIGHTING,TRUE);

    // 乗算合成
    pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//*************************************************************************************************
// インスタンス生成
//*************************************************************************************************
CBullet *CBullet::Create(D3DXVECTOR3 pos ,D3DXVECTOR3 dir,float m_vMove,SCENE_TYPE type)
{
    CBullet *bullet;
    bullet = new CBullet(PRIOLITY_1);
    bullet->Init();

    bullet->m_vPos = pos;
    bullet->m_OldPos = bullet->m_vPos;
	D3DXVec3Normalize(&dir,&dir);
    bullet->m_Dir = dir;
    bullet->m_Move = m_vMove;
	bullet->m_Use = true;
	bullet->m_BulletType = type;

    bullet->BindTexture(m_pTexture);
    bullet->m_Type = SCENE_TYPE_BULLET;

    return bullet;
}

//=============================================================================
// テクスチャの読み込み処理
//=============================================================================
HRESULT CBullet::Load(void)
{
    //デバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    if(m_pTexture == NULL){
        //テクスチャの読み込み
        if(FAILED(D3DXCreateTextureFromFile(pDevice,BULLET_TEXTURE,&m_pTexture))){
            //エラー時
            MessageBox(NULL,"テクスチャの読み込みに失敗しました","終了メッセージ",MB_OK);
        }
    }
    return S_OK;
}

//=============================================================================
// テクスチャの解放処理
//=============================================================================
void CBullet::Unload(void)
{
    //テクスチャの破棄
    if(m_pTexture != NULL){
        m_pTexture -> Release();
        m_pTexture = NULL;
    }
}

//=============================================================================
// 弾の当たり判定処理
//=============================================================================
void CBullet::HitBullet(void)
{
    CScene *scene = CScene::GetList(PRIOLITY_1);
    while(scene != NULL){
		switch(m_BulletType){	//プレイヤーの弾
			case SCENE_TYPE_PLAYER:
				if (scene->GetType() == SCENE_TYPE_ENEMY){
					D3DXVECTOR3 enemyPos;
					float enemyLen;
					CEnemy *enemy = (CEnemy*)scene;
					enemyPos = enemy->GetPosition();
					enemyLen = enemy->GetLength();
					//衝突判定
					if (HitSphereToSphere(m_vPos, m_LengthSq, enemyPos, enemyLen)){
						enemy->Hit(m_Damage);
						m_Use = false;
					}
				}
				break;
			//エネミーの弾
			case SCENE_TYPE_ENEMY:
				if (scene->GetType() == SCENE_TYPE_PLAYER){
					D3DXVECTOR3 playerPos;
					float playerLen;
					CPlayer *player = (CPlayer*)scene;
					playerPos = player->GetPosition();
					playerLen = player->GetLength();
					//衝突判定
					if (HitSphereToSphere(m_vPos, m_LengthSq, playerPos, playerLen)){
						player->Hit(m_Damage);
						m_Use = false;
					}
				}

                else if (scene->GetType() == SCENE_TYPE_CORE)
                {
                    D3DXVECTOR3 corePos;
                    float coreLen;
                    CCore *core = (CCore*)scene;
                    corePos = core->GetPosition();
                    coreLen = core->GetLength();
                    //衝突判定
                    if (HitSphereToSphere(m_vPos, m_LengthSq, corePos, coreLen)) {
                        core->Hit(m_Damage);
                        m_Use = false;
                    }
                }
				break;
		}

        scene = scene->GetNext();
    }
}