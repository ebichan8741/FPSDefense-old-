//=================================================================================================
//
// ���G�t�F�N�g [Smoke.cpp]
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
#include "Smoke.h"
#include "GameMode.h"

//=================================================================================================
// �萔��`
//=================================================================================================
#define SMOKE_TEXTURE "data/TEXTURE/smoke.png"

//=================================================================================================
// �\���̐錾
//=================================================================================================

//=================================================================================================
// �ÓI�����o�ϐ�
//=================================================================================================
LPDIRECT3DTEXTURE9 CSmoke::m_pTexture = NULL;

//*************************************************************************************************
// �R���X�g���N�^
//*************************************************************************************************
CSmoke::CSmoke(int nPriolity) :CScene3D(nPriolity)
{
}

//*************************************************************************************************
// ����������
//*************************************************************************************************
HRESULT CSmoke::Init(void)
{
    //�����o������
    m_vSize = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
    m_TexPos = D3DXVECTOR2(0.0f, 0.0f);
    m_TexSize = D3DXVECTOR2(1.0f, 1.0f);
    m_Velocity = D3DXVECTOR3(0.0f,0.0f,0.0f);
    m_Frame = 0;
    CScene3D::Init();
    return S_OK;
}

//*************************************************************************************************
// �I������
//*************************************************************************************************
void CSmoke::Uninit(void)
{
    CScene3D::Uninit();
}

//*************************************************************************************************
// �X�V����
//*************************************************************************************************
void CSmoke::Update(void)
{
    // �ϐ��錾 ------------------------------------------------------------------------------------
    VERTEX_3D* pVtx; //���z�A�h���X���擾���邽�߂̃|�C���^
    //---------------------------------------------------------------------------------------------

    //�g�p�t���O��false�̎��͏������Ȃ�
    if (!m_Use) {
        Release();
        return;
    }

    m_vPos += m_Velocity;
    m_Velocity *= 0.8f;

    m_Rot += m_RotSpeed;
    m_RotSpeed *= 0.8f;

    m_Frame++;
    if (m_Frame > 120) {
        m_Use = false;
    }

    CScene3D::Update();
}

//*************************************************************************************************
// �`�揈��
//*************************************************************************************************
void CSmoke::Draw(void)
{
    LPDIRECT3DDEVICE9 pDevice;
    D3DXMATRIX mtxWorld;
    D3DXMATRIX mtxTrans, mtxScale, mtxInv,mtxRot;

    //�f�o�C�X�̎擾
    pDevice = CManager::GetRenderer()->GetDevice();

    // ���Z����
    pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

    //���C�g�̃I�t
    pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

    //���[���h���W�ϊ��s��̏�����
    D3DXMatrixIdentity(&mtxWorld);

    //�g��E�k��
    D3DXMatrixScaling(&mtxScale, 20.0f, 20.0f, 20.0f);
    D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

    // ��]
    D3DXMatrixRotationZ(&mtxRot, m_Rot);
    D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

    //�t�s��̎擾
    mtxInv = CManager::GetCamera()->GetViewInverse();
    mtxInv._41 = 0.0f;
    mtxInv._42 = 0.0f;
    mtxInv._43 = 0.0f;
    D3DXMatrixMultiply(&mtxWorld, &mtxInv, &mtxWorld);    //�s��̏�Z�i�o�͐�,����,�E�Ӂj

    //���s�ړ�
    D3DXMatrixTranslation(&mtxTrans, m_vPos.x, m_vPos.y, m_vPos.z);
    D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

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
CSmoke *CSmoke::Create(D3DXVECTOR3 pos)
{
    CSmoke *smoke;
    int PI = D3DX_PI * 100;
    smoke = new CSmoke(PRIOLITY_1);

    smoke->m_vPos = pos;
    smoke->m_Use = true;

    smoke->Init();

    smoke->m_Velocity.x = ((rand() % 3) - 1) * 0.1f;
    smoke->m_Velocity.y = ((rand() % 3) - 1) * 0.1f;
    smoke->m_Velocity.z = ((rand() % 3) - 1) * 0.1f;
    smoke->m_Rot = (rand() % (PI * 2)) - PI;
    smoke->m_Rot * 0.01f;
    smoke->m_RotSpeed = (rand() % 2) - 0.5f;
    smoke->BindTexture(m_pTexture);

    return smoke;
}


//=============================================================================
// �e�N�X�`���̓ǂݍ��ݏ���
//=============================================================================
HRESULT CSmoke::Load(void)
{
    //�f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    if (m_pTexture == NULL)
    {
        //�e�N�X�`���̓ǂݍ���
        if (FAILED(D3DXCreateTextureFromFile(pDevice, SMOKE_TEXTURE, &m_pTexture)))
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
void CSmoke::Unload(void)
{
    //�e�N�X�`���̔j��
    if (m_pTexture != NULL)
    {
        m_pTexture->Release();
        m_pTexture = NULL;
    }
}
