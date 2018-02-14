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
#include "core.h"
#include "sceneModel.h"
#include "ModelManager.h"
#include "HitDome.h"
#include "LifeBar.h"
#include "LifeBarFrame.h"

//=================================================================================================
// �萔��`
//=================================================================================================

//=================================================================================================
// �\���̐錾
//=================================================================================================

//=================================================================================================
// �O���[�o���ϐ�
//=================================================================================================

//*************************************************************************************************
// �R���X�g���N�^
//*************************************************************************************************
CCore::CCore(int nPriolity) :CSceneModel(nPriolity)
{
}

//*************************************************************************************************
// ����������
//*************************************************************************************************
HRESULT CCore::Init(void)
{
    //m_vPos = D3DXVECTOR3(0.0f,10.0f,100.0f);
    m_Rot = 0.0f;
    m_Hit = false;

    // ���苅�̍쐬
#ifdef _DEBUG
    m_pHitDome = CHitDome::Create(m_vPos, m_vSize);
#endif

    CSceneModel::Init();
    return S_OK;
}

//*************************************************************************************************
// �I������
//*************************************************************************************************
void CCore::Uninit(void)
{
    CSceneModel::Uninit();
}

//*************************************************************************************************
// �X�V����
//*************************************************************************************************
void CCore::Update(void)
{
    m_Rot += 0.01f;

    // ���C�t�o�[�̐ݒ�
    m_pLifeBar->SetPosition(m_vPos);
    m_pLifeBar->SetHP(m_Life);
    m_pLifeBar->SetMaxHP(m_MaxLife);
    //m_pLifeBar->Update();
    m_pLifeBarFrame->SetPosition(m_vPos);
    m_pLifeBarFrame->SetHP(m_Life);
    //m_pLifeBarFrame->Update();

    // �R�A�̔j��
    if (m_Life <= 0) {
        //CSmoke::Create(m_vPos);
        CManager::GetCamera()->Swing(0.05f);
        Release();
#ifdef _DEBUG
        m_pHitDome->Release();
#endif
        return;
    }

#ifdef _DEBUG
    m_pHitDome->SetDome(m_vPos, m_vSize);
#endif

    // �����蔻��
    if (m_Hit) {
#ifdef _DEBUG
        m_pHitDome->HitToDome();
#endif
        m_Hit = false;
    }

    CSceneModel::Update();

}

//*************************************************************************************************
// �`�揈��
//*************************************************************************************************
void CCore::Draw(void)
{
    LPDIRECT3DDEVICE9 pDevice;
    D3DMATERIAL9 matDef;
    D3DXMATRIX mtxWorld;
    D3DXMATRIX mtxTrans, mtxScale, mtxRot;

    //�f�o�C�X�̎擾
    pDevice = CManager::GetRenderer()->GetDevice();

    //���[���h���W�ϊ��s��̏�����
    D3DXMatrixIdentity(&mtxWorld);

    //D3DXMatrixScaling(&mtxScale, 2.0f, 2.0f, 2.0f);

    //���s�ړ�
    D3DXMatrixTranslation(&mtxTrans, m_vPos.x, m_vPos.y, m_vPos.z);
    D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot, sinf(m_Rot * 2), cosf(m_Rot));

    //�s��̏�Z�i�o�͐�,����,�E�Ӂj
    //D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);
    D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
    D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

    //���[���h���W�ϊ��s��̓K�p
    pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

    CSceneModel::Draw();
}

//*************************************************************************************************
// �C���X�^���X����
//*************************************************************************************************
CCore *CCore::Create(D3DXVECTOR3 pos, int hp)
{
    CCore *core;
    core = new CCore(PRIOLITY_1);
    core->m_Life = core->m_MaxLife = hp;
    core->m_vPos = pos;
    core->m_pLifeBarFrame = CLifeBarFrame::Create(pos);
    core->m_pLifeBar = CLifeBar::Create(pos);
    core->Init();
    core->m_ModelInfo = CManager::GetModelManager()->getResource(CManager::GetModelManager()->getModelName(MODEL_CORE));
    core->m_Type = SCENE_TYPE_CORE;

    return core;
}

//*************************************************************************************************
// �_���[�W����
//*************************************************************************************************
void CCore::Hit(int damage)
{
    m_Life -= damage;
    m_Hit = true;
}
