//=================================================================================================
//
// �v���C���[ [player.cpp]
// Author : TAKUYA EBIHARA
//
//=================================================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "GameMode.h"
#include "camera.h"
#include "sceneModel.h"
#include "player.h"
#include "enemy.h"
#include "enemyShooter.h"
#include "bullet.h"
#include "meshField.h"
#include "input.h"
#include "collision.h"
#include "HitDome.h"
#include "Debug.h"
#include "Core.h"

//=================================================================================================
// �萔��`
//=================================================================================================
#define ENEMY_LIFE			(5)			// �G�l�~�[�̗̑�
#define ENEMY_SITE_ANGLE	(30.0f)     // �G�l�~�[�̎���p�x
#define ENEMY_SITE_LENGTH	(15000.0f)	// �G�l�~�[�̎��싗��
#define _EPS				(1E-11)		// ���������_�덷

//=================================================================================================
// �\���̐錾
//=================================================================================================

//=================================================================================================
// �O���[�o���ϐ�
//=================================================================================================
int CEnemy::m_EnemyCnt = 0;

//*************************************************************************************************
// �R���X�g���N�^
//*************************************************************************************************
CEnemy::CEnemy(int nPriolity):CSceneModel(nPriolity)
{

}

//*************************************************************************************************
// ����������
//*************************************************************************************************
HRESULT CEnemy::Init(void)
{
    //�����o������
    m_vSize = D3DXVECTOR3(10.0f,5.0f,10.0f);
    m_vAngle = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_vDir = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	m_vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    m_LengthSq = m_vSize.x * m_vSize.x + m_vSize.y * m_vSize.y + m_vSize.z * m_vSize.z;
    m_Hit = false;
	m_BulletCnt = 0;
	m_CheckPointCnt = 0;
	m_EnemyState = ENEMY_STATE_WALK;
	m_Distance = 0.0f;

#ifdef _DEBUG
    m_pHitDome = CHitDome::Create(m_vPos,m_vSize);
#endif
    return S_OK;
}

//*************************************************************************************************
// �I������
//*************************************************************************************************
void CEnemy::Uninit(void)
{
    CSceneModel::Uninit();
}

//*************************************************************************************************
// �X�V����
//*************************************************************************************************
void CEnemy::Update(void)
{
    CSceneModel::Update();

    //�f�o�b�O�\��
#ifdef _DEBUG
    char text[256];
    sprintf(text, "EnemyPos  X : %f  Y : %f  Z : %f  EnemyLife  : %d  EnemyAngle : %5f  EnemyDistance : %5f  EnemyState : %d\n", m_vPos.x, m_vPos.y, m_vPos.z,m_Life, m_vAngle.y,m_Distance,m_EnemyState);
    CDebug::AddText(text);
#endif
}

//*************************************************************************************************
// �`�揈��
//*************************************************************************************************
void CEnemy::Draw(void)
{
    LPDIRECT3DDEVICE9 pDevice;
    D3DMATERIAL9 matDef;
    D3DXMATRIX mtxWorld;
    D3DXMATRIX mtxTrans,mtxScale,mtxRotY;
    D3DXMATRIX mtxRot = CManager::GetCamera()->GetCameraRotMat();

    //�f�o�C�X�̎擾
    pDevice = CManager::GetRenderer()->GetDevice();

    //���[���h���W�ϊ��s��̏�����
    D3DXMatrixIdentity(&mtxWorld);

    D3DXMatrixScaling(&mtxScale,2.0f,2.0f,2.0f);

    //���s�ړ�
    D3DXMatrixTranslation(&mtxTrans,m_vPos.x,m_vPos.y,m_vPos.z);

    //�s��̏�Z�i�o�͐�,����,�E�Ӂj
    D3DXMatrixMultiply(&mtxWorld,&mtxWorld,&mtxScale);
    D3DXMatrixMultiply(&mtxWorld,&mtxWorld,&m_mtxRot);
    D3DXMatrixMultiply(&mtxWorld,&mtxWorld,&mtxTrans);

    //���[���h���W�ϊ��s��̓K�p
    pDevice->SetTransform(D3DTS_WORLD,&mtxWorld);

    CSceneModel::Draw();
}

//*************************************************************************************************
// �C���X�^���X����
//*************************************************************************************************
CEnemy *CEnemy::Create(D3DXVECTOR3 pos,int ID)
{
    CEnemy *enemy;
    enemy = new CEnemy(PRIOLITY_1);

    enemy->m_vPos = pos;
	enemy->m_ID = ID;

    enemy->Init();

    //enemy->LoadModel(MODEL_ENEMY0);
    enemy->m_Type = SCENE_TYPE_ENEMY;

    return enemy;
}

//*************************************************************************************************
// �_���[�W����
//*************************************************************************************************
void CEnemy::Hit(int damage)
{
    m_Life -= damage;
    m_Hit = true;
}

//*************************************************************************************************
// �G�l�~�[�̎��������
//*************************************************************************************************
bool CEnemy::CheckEnemySite(PRIOLITY priolity,SCENE_TYPE sceneType)
{
	CScene *scene = CScene::GetList(priolity);

    switch (sceneType) 
    {
    // �v���C���[��������ɂ��邩
    case SCENE_TYPE_PLAYER:
        while (scene != NULL) {
            if (scene->GetType() == sceneType) {
                D3DXVECTOR3 pos;
                CPlayer *player = (CPlayer*)scene;
                pos = player->GetPosition();

                //����x�N�g��
                m_vSiteDir = -m_vDir;
                m_vSiteDir.y = 0.0f;
                //�ڕW�ւ̕����x�N�g��
                m_vTargetDir = m_vPos - pos;
                m_Distance = D3DXVec3LengthSq(&m_vTargetDir);
                m_vTargetDir.y = 0.0f;
                //���K��
                D3DXVec3Normalize(&m_vSiteDir, &m_vSiteDir);
                D3DXVec3Normalize(&m_vTargetDir, &m_vTargetDir);
                //���όv�Z
                m_Dot = m_vSiteDir.x * m_vTargetDir.x + m_vSiteDir.z * m_vTargetDir.z;
                //����x�N�g������̖ڕW�̃��W�A���擾
                m_Radian = acosf(m_Dot);
                //���W�A�����p�x�ɕϊ�
                m_vAngle.y = D3DXToDegree(m_Radian);
                //�����
                if (ENEMY_SITE_ANGLE > m_vAngle.y && ENEMY_SITE_LENGTH > m_Distance) {
                    return true;
                }
                return false;
            }
            scene = scene->GetNext();
        }
        break;
    // �R�A��������ɂ��邩
    case SCENE_TYPE_CORE:
        while (scene != NULL) 
        {
            if (scene->GetType() == sceneType) {
                D3DXVECTOR3 pos;
                CCore *core = (CCore*)scene;
                pos = core->GetPosition();

                //����x�N�g��
                m_vSiteDir = -m_vDir;
                m_vSiteDir.y = 0.0f;
                //�ڕW�ւ̕����x�N�g��
                m_vTargetDir = m_vPos - pos;
                m_Distance = D3DXVec3LengthSq(&m_vTargetDir);
                m_vTargetDir.y = 0.0f;
                //���K��
                D3DXVec3Normalize(&m_vSiteDir, &m_vSiteDir);
                D3DXVec3Normalize(&m_vTargetDir, &m_vTargetDir);
                //���όv�Z
                m_Dot = m_vSiteDir.x * m_vTargetDir.x + m_vSiteDir.z * m_vTargetDir.z;
                //����x�N�g������̖ڕW�̃��W�A���擾
                m_Radian = acosf(m_Dot);
                //���W�A�����p�x�ɕϊ�
                m_vAngle.y = D3DXToDegree(m_Radian);
                //�����
                if (ENEMY_SITE_ANGLE > m_vAngle.y && ENEMY_SITE_LENGTH > m_Distance) {
                    return true;
                }
                return false;
            }
            scene = scene->GetNext();
        }
        break;
    }
	return false;
}

//*************************************************************************************************
// �G�l�~�[�̌o�H�T��
//*************************************************************************************************
void CEnemy::Path(void)
{
	CMeshField *pField = CGameMode::GetMeshField();

	// �`�F�b�N�|�C���g�ւ̕���
	m_vDir = pField->m_CheckPoint[m_CheckPointCnt] - m_vPos;

	// �`�F�b�N�|�C���g����̋���
	m_PathDistance = m_vDir.x * m_vDir.x + m_vDir.z * m_vDir.z;
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	m_vDir.y = 0.0f;

	// �`�F�b�N�|�C���g�ɏ\���߂Â���
	if (m_PathDistance < 1.0f){
		m_CheckPointCnt++;
		// �`�F�b�N�|�C���g����肫����
		if (m_CheckPointCnt == CHECK_POINT_CNT){
			m_CheckPointCnt = 0;
		}
	}
}

//*************************************************************************************************
// �G�l�~�[�̌����v�Z
//*************************************************************************************************
void CEnemy::Direction(void)
{
	D3DXVECTOR3 RotX, RotY, RotZ;

	RotZ = -m_vDir;
	D3DXVec3Normalize(&RotZ, &RotZ);
	D3DXVec3Cross(&RotX, D3DXVec3Normalize(&RotY,&m_vUp), &RotZ);
	D3DXVec3Normalize(&RotX, &RotX);
	//D3DXVec3Cross(&RotY, &RotZ, &RotX);
	D3DXVec3Normalize(&RotY, D3DXVec3Cross(&RotY,&RotZ,&RotX));

	// ��]�s��쐬
	m_mtxRot._11 = RotX.x; m_mtxRot._12 = RotX.y; m_mtxRot._13 = RotX.z; m_mtxRot._14 = 0.0f;
	m_mtxRot._21 = RotY.x; m_mtxRot._22 = RotY.y; m_mtxRot._23 = RotY.z; m_mtxRot._24 = 0.0f;
	m_mtxRot._31 = RotZ.x; m_mtxRot._32 = RotZ.y; m_mtxRot._33 = RotZ.z; m_mtxRot._34 = 0.0f;
	m_mtxRot._41 = 0.0f;   m_mtxRot._42 = 0.0f;   m_mtxRot._43 = 0.0f;   m_mtxRot._44 = 1.0f;
}

int CEnemy::GetEnemyNum(void)
{
	return m_EnemyCnt;
}

void CEnemy::DecEnemyNum(void)
{
	CGameMode::m_EnemyCnt--;
}

int CEnemy::GetLife(void)
{
    return m_Life;
}

int CEnemy::GetMaxLife(void)
{
    return m_MaxLife;
}