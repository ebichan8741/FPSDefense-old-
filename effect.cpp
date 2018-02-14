//=================================================================================================
//
// �v���C���[ [player.cpp]
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
#include "effect.h"
#include "collision.h"
#include "GameMode.h"

//=================================================================================================
// �萔��`
//=================================================================================================
#define EFFECT_TEXTURE "data/TEXTURE/effect000.jpg"
#define EFFECT_LIFE	(120)
#define EFFECT_MOVE	(3.0f)

//=================================================================================================
// �\���̐錾
//=================================================================================================

//=================================================================================================
// �ÓI�����o�ϐ�
//=================================================================================================
LPDIRECT3DTEXTURE9 CEffect::m_pTexture = NULL;

//*************************************************************************************************
// �R���X�g���N�^
//*************************************************************************************************
CEffect::CEffect(int nPriolity) :CScene3D(nPriolity)
{
}

//*************************************************************************************************
// ����������
//*************************************************************************************************
HRESULT CEffect::Init(void)
{
	//�����o������
	m_vSize = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	m_Life = EFFECT_LIFE;
	m_Move = EFFECT_MOVE;
	m_LengthSq = m_vSize.x * m_vSize.x + m_vSize.y * m_vSize.y;
	CScene3D::Init();
	return S_OK;
}

//*************************************************************************************************
// �I������
//*************************************************************************************************
void CEffect::Uninit(void)
{
	CScene3D::Uninit();
}

//*************************************************************************************************
// �X�V����
//*************************************************************************************************
void CEffect::Update(void)
{
	//---------------------------------------------------------------------------------------------
	// �ϐ��錾�J�n
	//---------------------------------------------------------------------------------------------
	VERTEX_3D* pVtx; //���z�A�h���X���擾���邽�߂̃|�C���^
	D3DXVECTOR3 vMove;
	//---------------------------------------------------------------------------------------------
	// �ϐ��錾�I��
	//---------------------------------------------------------------------------------------------

	//�g�p�t���O��false�̎��͏������Ȃ�
	if (!m_Use)
	{
		Release();
		return;
	}

	//�ړ��ʌv�Z
	vMove = m_Dir * m_Move;
	m_vPos += vMove;

	//�������Z
	m_Life--;
	if (m_Life <= 0)
	{
		m_Use = false;
		Release();
	}

	CScene3D::Update();
}

//*************************************************************************************************
// �`�揈��
//*************************************************************************************************
void CEffect::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX mtxWorld;
	D3DXMATRIX mtxTrans, mtxScale, mtxInv;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	// ���Z����
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//���C�g�̃I�t
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//���[���h���W�ϊ��s��̏�����
	D3DXMatrixIdentity(&mtxWorld);

	//�g��E�k��
	//D3DXMatrixScaling(&mtxScale,1.0f,1.0f,1.0f);
	//D3DXMatrixMultiply(&mtxWorld,&mtxWorld,&mtxScale);

	//���s�ړ�
	D3DXMatrixTranslation(&mtxTrans, m_vPos.x, m_vPos.y, m_vPos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	//�t�s��̎擾
	mtxInv = CManager::GetCamera()->GetViewInverse();
	mtxInv._41 = 0.0f;
	mtxInv._42 = 0.0f;
	mtxInv._43 = 0.0f;

	D3DXMatrixMultiply(&mtxWorld, &mtxInv, &mtxWorld);    //�s��̏�Z�i�o�͐�,����,�E�Ӂj

	//���[���h���W�ϊ��s��̓K�p
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	CScene3D::Draw();

	// ��Z����
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//���C�g�̃I��
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//*************************************************************************************************
// �C���X�^���X����
//*************************************************************************************************
CEffect *CEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 dir)
{
	CEffect *effect;
	effect = new CEffect(PRIOLITY_1);
	effect->Init();

	effect->m_vPos = pos;
	D3DXVec3Normalize(&dir, &dir);
	effect->m_Dir = dir;
	effect->m_Use = true;

	effect->BindTexture(m_pTexture);

	return effect;
}


//=============================================================================
// �e�N�X�`���̓ǂݍ��ݏ���
//=============================================================================
HRESULT CEffect::Load(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	if (m_pTexture == NULL)
	{
		//�e�N�X�`���̓ǂݍ���
		if (FAILED(D3DXCreateTextureFromFile(pDevice, EFFECT_TEXTURE, &m_pTexture)))
		{
			//�G���[��
			MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", "�I�����b�Z�[�W", MB_OK);
		}
	}
	return S_OK;
}

//=============================================================================
// �e�N�X�`���̉������
//=============================================================================
void CEffect::Unload(void)
{
	//�e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}
