//=================================================================================================
//
// �G�����G�t�F�N�g [EnemyExplosion.cpp]
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
#include "EnemyExplosion.h"
#include "GameMode.h"

//=================================================================================================
// �萔��`
//=================================================================================================
#define ENEMYEXPLOSION_TEXTURE "data/TEXTURE/explosion.png"

//=================================================================================================
// �\���̐錾
//=================================================================================================

//=================================================================================================
// �ÓI�����o�ϐ�
//=================================================================================================
LPDIRECT3DTEXTURE9 CEnemyExplosion::m_pTexture = NULL;

//*************************************************************************************************
// �R���X�g���N�^
//*************************************************************************************************
CEnemyExplosion::CEnemyExplosion(int nPriolity) :CScene3D(nPriolity)
{
}

//*************************************************************************************************
// ����������
//*************************************************************************************************
HRESULT CEnemyExplosion::Init(void)
{
    //�����o������
    m_vSize = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
    m_TexPos = D3DXVECTOR2(0.0f, 0.0f);
    m_TexSize = D3DXVECTOR2(1.0f / 8, 0.5f);

    CScene3D::Init();
    return S_OK;
}

//*************************************************************************************************
// �I������
//*************************************************************************************************
void CEnemyExplosion::Uninit(void)
{
    CScene3D::Uninit();
}

//*************************************************************************************************
// �X�V����
//*************************************************************************************************
void CEnemyExplosion::Update(void)
{
    // �ϐ��錾 ------------------------------------------------------------------------------------
    VERTEX_3D* pVtx; //���z�A�h���X���擾���邽�߂̃|�C���^
    //---------------------------------------------------------------------------------------------

    //�g�p�t���O��false�̎��͏������Ȃ�
    if (!m_Use){
        Release();
        return;
    }

    //�e�N�X�`���A�j���[�V����
    if (m_AnimCnt >= 1){
        m_TexPos.x += m_TexSize.x;
        if (m_TexPos.x >= 1.0f){
            m_TexPos.x = 0.0f;
            m_TexPos.y += m_TexSize.y;
            if (m_TexPos.y >= 1.0f){
                m_TexPos.y = 0.0f;
                Release();
            }
        }
        m_AnimCnt = 0;
    }
    m_AnimCnt++;

    // �ʒu�X�V
    //SetPosition();
    // UV�l�X�V
    AnimationTexture(m_TexPos, m_TexSize);

    CScene3D::Update();
}

//*************************************************************************************************
// �`�揈��
//*************************************************************************************************
void CEnemyExplosion::Draw(void)
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
    D3DXMatrixScaling(&mtxScale,30.0f,30.0f,30.0f);
    D3DXMatrixMultiply(&mtxWorld,&mtxWorld,&mtxScale);

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
CEnemyExplosion *CEnemyExplosion::Create(D3DXVECTOR3 pos)
{
    CEnemyExplosion *explosion;
    explosion = new CEnemyExplosion(PRIOLITY_1);

    explosion->m_vPos = pos;
    explosion->m_Use = true;

    explosion->Init();

    explosion->BindTexture(m_pTexture);

    return explosion;
}


//=============================================================================
// �e�N�X�`���̓ǂݍ��ݏ���
//=============================================================================
HRESULT CEnemyExplosion::Load(void)
{
    //�f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    if (m_pTexture == NULL)
    {
        //�e�N�X�`���̓ǂݍ���
        if (FAILED(D3DXCreateTextureFromFile(pDevice, ENEMYEXPLOSION_TEXTURE, &m_pTexture)))
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
void CEnemyExplosion::Unload(void)
{
    //�e�N�X�`���̔j��
    if (m_pTexture != NULL)
    {
        m_pTexture->Release();
        m_pTexture = NULL;
    }
}
