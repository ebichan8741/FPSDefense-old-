//=================================================================================================
//
// ���C�t�o�[ [ LifeBar.cpp ]
// Author : TAKUYA EBIHARA
//
//=================================================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "GameMode.h"
#include "scene3D.h"
#include "enemy.h"
#include "LifeBar.h"
#include "LifeBarFrame.h"
#include "camera.h"

//=================================================================================================
// �萔��`
//=================================================================================================
#define TEXTURE_LIFEBAR_FRAME   "data/TEXTURE/lifebar_Green.png"


//=================================================================================================
// �\���̐錾
//=================================================================================================

//=================================================================================================
// �O���[�o���ϐ�
//=================================================================================================
LPDIRECT3DTEXTURE9 CLifeBar::m_pTexture = NULL;


//*************************************************************************************************
// �R���X�g���N�^
//*************************************************************************************************
CLifeBar::CLifeBar(int nPriolity) :CScene3D(nPriolity)
{
}

//*************************************************************************************************
// ����������
//*************************************************************************************************
HRESULT CLifeBar::Init(void)
{
    m_vSize = D3DXVECTOR3(LIFEBARFRAME_SIZE_X, LIFEBARFRAME_SIZE_Y, 0.0f);

    CScene3D::Init2();
    return S_OK;
}

//*************************************************************************************************
// �I������
//*************************************************************************************************
void CLifeBar::Uninit(void)
{
    CScene3D::Uninit();
}

//*************************************************************************************************
// �X�V����
//*************************************************************************************************
void CLifeBar::Update(void)
{
    //int maxHp = CGameMode::GetEnemy(m_ID)->GetMaxLife();
    //int nowHp = CGameMode::GetEnemy(m_ID)->GetLife();
    int damege = m_MaxHP - m_HP;

    //m_vPos = CGameMode::GetEnemy(m_ID)->GetPosition();
    m_vPos.y += 7.0f;

    m_vSize.x = (float)m_HP / m_MaxHP * LIFEBARFRAME_SIZE_X;

    if (m_HP <= 0) Release();

    UpdateSize();

    CScene3D::Update();
}

//*************************************************************************************************
// �`�揈��
//*************************************************************************************************
void CLifeBar::Draw(void)
{
    LPDIRECT3DDEVICE9 pDevice;
    D3DXMATRIX mtxWorld;   //���[���h���W�ϊ��s��
    D3DXMATRIX mtxTrans, mtxScale, mtxInv;

    //�f�o�C�X�̎擾
    pDevice = CManager::GetRenderer()->GetDevice();

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

    //���C�g�̃I��
    pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//*************************************************************************************************
// �C���X�^���X����
//*************************************************************************************************
CLifeBar *CLifeBar::Create(D3DXVECTOR3 pos)
{
    CLifeBar *lifebar;
    lifebar = new CLifeBar(PRIOLITY_1);

    lifebar->m_vPos = pos;
    lifebar->Init();

    lifebar->BindTexture(m_pTexture);

    return lifebar;
}

//=============================================================================
// �e�N�X�`���̓ǂݍ��ݏ���
//=============================================================================
HRESULT CLifeBar::Load(void)
{
    //�f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    if (m_pTexture == NULL) {
        //�e�N�X�`���̓ǂݍ���
        if (FAILED(D3DXCreateTextureFromFile(pDevice, TEXTURE_LIFEBAR_FRAME, &m_pTexture))) {
            //�G���[��
            MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", "�I�����b�Z�[�W", MB_OK);
        }
    }
    return S_OK;
}

//=============================================================================
// �e�N�X�`���̉������
//=============================================================================
void CLifeBar::Unload(void)
{
    //�e�N�X�`���̔j��
    if (m_pTexture != NULL) {
        m_pTexture->Release();
        m_pTexture = NULL;
    }
}