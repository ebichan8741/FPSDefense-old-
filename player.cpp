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
#include "bullet.h"
#include "meshField.h"
#include "meshField.h"
#include "Fade.h"
#include "ResultMode.h"
#include "input.h"
#include "number.h"
#include "sound.h"
#include "effect.h"
#include "explosion.h"
#include "ModelManager.h"

//=================================================================================================
// �萔��`
//=================================================================================================
#define PLAYER_LIFE		(100)		// �v���C���[�̗̑�
#define BULLET_NUM		(30)		// 1�}�K�W���̒e��
#define BULLET_RATE		(8)			// �e�̘A�ˑ��x
#define RELOAD_TIME		(150)		// �����[�h����
#define PLAYER_BULLET_MOVE      (10.0f)     // �e��

//=================================================================================================
// �\���̐錾
//=================================================================================================

//=================================================================================================
// �O���[�o���ϐ�
//=================================================================================================

//*************************************************************************************************
// �R���X�g���N�^
//*************************************************************************************************
CPlayer::CPlayer(int nPriolity):CSceneModel(nPriolity)
{
}

//*************************************************************************************************
// ����������
//*************************************************************************************************
HRESULT CPlayer::Init(void)
{
    //�����o������
    m_vSize = D3DXVECTOR3(5.0f,5.0f,5.0f);
    m_vAngle = D3DXVECTOR3(0.0f,0.0f,0.0f);
    m_LengthSq = m_vSize.x * m_vSize.x + m_vSize.y * m_vSize.y + m_vSize.z * m_vSize.z;
	m_Hit = false;
	m_Reload = false;
	m_Life = PLAYER_LIFE;
	m_BulletNum = BULLET_NUM;
	m_BulletRate = 0;
	m_ReloadCnt = 0;

    CSceneModel::Init();
    return S_OK;
}

//*************************************************************************************************
// �I������
//*************************************************************************************************
void CPlayer::Uninit(void)
{
    CSceneModel::Uninit();
}

//*************************************************************************************************
// �X�V����
//*************************************************************************************************
void CPlayer::Update(void)
{
    D3DXVECTOR3 Forward = CManager::GetCamera()->GetCameraForward();
    D3DXVECTOR3 Left = CManager::GetCamera()->GetCameraLeft();
    D3DXVECTOR3 Up = CManager::GetCamera()->GetCameraUp();
    D3DXVECTOR3 PosOffset = Forward * 2 + Left *0.25f;

    //FPS���[�h
    if(CManager::GetCamera()->m_bFps == true){
        m_vPos = CManager::GetCamera()->GetCameraPos() + PosOffset;
    }
    else {
        //Y���W���t�B�[���h�ɍ��킹��
        m_vPos.y = CGameMode::GetMeshField()->GetHeight(m_vPos);
    }

    m_vPos += m_Offset;
    m_Offset *= 0.8f;

    //�e����
	if (CManager::GetInputMouse()->GetMouseTrigger(CLICK_LEFT) && m_Reload == false) {
		m_BulletRate = 0;
	}
	if (CManager::GetInputMouse()->GetMousePress(CLICK_LEFT) && m_BulletRate == 0 && m_BulletNum > 0 && m_Reload == false) {
		CBullet::Create(m_vPos - Forward * 2.0f - Left * 0.1f - Up * 0.1f, Forward - Left * 0.01f + Up * 0.01f, PLAYER_BULLET_MOVE, m_Type);
        CExplosion::Create(m_vPos);
		CManager::GetSound()->PlaySound(SOUND_LABEL_SE_SHOT);	// �e���Đ�
		m_BulletNum--;
        // ����
        D3DXVECTOR3 velocity;
        velocity.x = CManager::GetCamera()->GetCameraRotMat()._31;
        velocity.y = CManager::GetCamera()->GetCameraRotMat()._32;
        velocity.z = CManager::GetCamera()->GetCameraRotMat()._33;
        m_Offset -= velocity * 0.2f;
	}

	// R���������e���Ȃ��Ȃ����Ƃ������[�h����
	if (m_Reload == false && m_BulletNum != BULLET_NUM && CManager::GetInputKeyboard()->GetKeyTrigger(DIK_R) || m_BulletNum == 0){
		m_Reload = true;
		CManager::GetSound()->PlaySound(SOUND_LABEL_SE_RELOAD);	// �����[�h���Đ�
	}
	if (m_Reload == true) {
		Reload();
	}

	// �e�̘A�ˑ��x
	m_BulletRate = (m_BulletRate + 1) % BULLET_RATE;

	// �e���\��
	CGameMode::GetBulletNumber(0)->SetNumber(m_BulletNum / 10,0.0f);
	CGameMode::GetBulletNumber(1)->SetNumber(m_BulletNum % 10, 0.0f);
	CGameMode::GetBulletNumber(2)->SetNumber(4, 0.5f);
	CGameMode::GetBulletNumber(3)->SetNumber(BULLET_NUM / 10, 0.0f);
	CGameMode::GetBulletNumber(4)->SetNumber(BULLET_NUM % 10, 0.0f);

	// �g�o�\��
	CGameMode::GetHPNumber(0)->SetNumber(m_Life / 100,0.0f);
	CGameMode::GetHPNumber(1)->SetNumber((m_Life / 10) % 10, 0.0f);
	CGameMode::GetHPNumber(2)->SetNumber(m_Life % 10, 0.0f);

    CSceneModel		::Update();

    //�f�o�b�O�\��
#ifdef _DEBUG
    char text[256];
    sprintf(text,"PlayerPos  X : %f  Y : %f  Z : %f  PlayerLife  %d\n",m_vPos.x,m_vPos.y,m_vPos.z,m_Life);
    CDebug::AddText(text);
#endif
}

//*************************************************************************************************
// �`�揈��
//*************************************************************************************************
void CPlayer::Draw(void)
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

    //D3DXMatrixScaling(&mtxScale,0.25f,0.25f,0.25f);

    //��]
    D3DXMatrixRotationYawPitchRoll(&mtxRotY,D3DX_PI,0.0f,0.0f);

    //���s�ړ�
    D3DXMatrixTranslation(&mtxTrans,m_vPos.x,m_vPos.y,m_vPos.z);

    //�s��̏�Z�i�o�͐�,����,�E�Ӂj
    //D3DXMatrixMultiply(&mtxWorld,&mtxWorld,&mtxScale);
    D3DXMatrixMultiply(&mtxWorld,&mtxWorld,&mtxRotY);
    D3DXMatrixMultiply(&mtxWorld,&mtxWorld,&mtxRot);
    D3DXMatrixMultiply(&mtxWorld,&mtxWorld,&mtxTrans);

    //���[���h���W�ϊ��s��̓K�p
    pDevice->SetTransform(D3DTS_WORLD,&mtxWorld);

    CSceneModel::Draw();
}

//*************************************************************************************************
// �C���X�^���X����
//*************************************************************************************************
CPlayer *CPlayer::Create(D3DXVECTOR3 pos)
{
    CPlayer *player;
    player = new CPlayer(PRIOLITY_1);

	player->m_vPos = pos;

    player->Init();

    player->m_ModelInfo = CManager::GetModelManager()->getResource(CManager::GetModelManager()->getModelName(MODEL_PLAYER));

    //player->LoadModel(MODEL_PLAYER);
    player->m_Type = SCENE_TYPE_PLAYER;

    return player;
}

//*************************************************************************************************
// �_���[�W����
//*************************************************************************************************
void CPlayer::Hit(int damage)
{
	m_Life -= damage;
	m_Hit = true;
}

//*************************************************************************************************
// �����[�h����
//*************************************************************************************************
void CPlayer::Reload(void)
{
	m_ReloadCnt++;

	if (m_ReloadCnt >= RELOAD_TIME) {
		m_BulletNum = BULLET_NUM;
		m_ReloadCnt = 0;
		m_Reload = false;
	}
}

//*************************************************************************************************
// �̗͎擾
//*************************************************************************************************
int CPlayer::GetLife(void) {
	return m_Life;
}

//*************************************************************************************************
// �e���擾
//*************************************************************************************************
int CPlayer::GetBullet(void) {
    return m_BulletNum;
}