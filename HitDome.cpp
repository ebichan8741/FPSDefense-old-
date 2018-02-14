//================================================================================================
//
// ���b�V���h�[�� [meshdome.cpp]
// Author : TAKUYA EBIHARA
//
//================================================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "hitdome.h"
#include "camera.h"
#include "input.h"
#include "GameMode.h"

//=================================================================================================
// �萔��`
//=================================================================================================
#define HIT_SPHERE_MAX_U    (8)        //�t�����̕�����
#define HIT_SPHERE_MAX_V    (8)        //�u�����̕�����

//=================================================================================================
// �O���[�o���ϐ�
//=================================================================================================

//*************************************************************************************************
// �R���X�g���N�^
//*************************************************************************************************
CHitDome::CHitDome(int nPriolity):CScene(nPriolity)
{
    m_pTexture = NULL;
    m_Hit = NULL;
    m_Color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f);
}

//*************************************************************************************************
// ����������
//*************************************************************************************************
HRESULT CHitDome::Init(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    m_Len = sqrt((m_vSize.x * m_vSize.x) + (m_vSize.y * m_vSize.y) + (m_vSize.z * m_vSize.z)) / 2.0f;

    //���_���̐ݒ�
    if(FAILED(MakeVertex(pDevice)))
    {
        return E_FAIL;
    }

    //���_�C���f�b�N�X�̐ݒ�
    if(FAILED(MakeIndex(pDevice)))
    {
        return E_FAIL;
    }

    return 0;
}

//*************************************************************************************************
// �I������
//*************************************************************************************************
void CHitDome::Uninit(void)
{
    //���_�o�b�t�@�[�̉��
    if( m_pVtxBuff != NULL )
    {
        m_pVtxBuff -> Release();
        m_pVtxBuff = NULL;
    }
    //�C���f�b�N�X�o�b�t�@�[�̉��
    if( m_pIdxBuff != NULL )
    {
        m_pIdxBuff -> Release();
        m_pIdxBuff = NULL;
    }
}

//*************************************************************************************************
// �X�V����
//*************************************************************************************************
void CHitDome::Update(void)
{
    if(m_Hit){
        m_Color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f);
        m_Hit = false;
    }
    else{
        m_Color = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.5f);
    }

    VERTEX_3D* pVtx;
    m_pVtxBuff->Lock(0,0,(void**)&pVtx,0);    //���b�N
    //���_���W
    for(int i = 0;i < HIT_SPHERE_MAX_V + 1;i++)
    {
        for(int j = 0;j < (HIT_SPHERE_MAX_U + 1);j++)
        {
            pVtx[i * (HIT_SPHERE_MAX_U + 1) + j].color = m_Color;
        }
    }
    m_pVtxBuff->Unlock();    //�A�����b�N
}

//*************************************************************************************************
// �`�揈��
//*************************************************************************************************
void CHitDome::Draw(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
    D3DXMATRIX mtxWorld,mtxRotY,mtxTrans;
    D3DXVECTOR3 vCameraPos = CManager::GetCamera()->GetCameraPos();
    int nNumVtx,nNumFace;

    pDevice->SetStreamSource(
        0,                          //�X�g���[���ԍ�(�p�C�v�ԍ�)
        m_pVtxBuff,                 //�X�g���[���̌��ɂȂ钸�_�o�b�t�@
        0,                          //�I�t�Z�b�g(�o�C�g)
        sizeof(VERTEX_3D));         //��̒��_�f�[�^�̃T�C�Y(�X�g���C�h��)

    //�f�o�C�X�ɃC���f�b�N�X�o�b�t�@�̐ݒ�
    pDevice->SetIndices(m_pIdxBuff);

    //���_�t�H�[�}�b�g�̐ݒ�
    pDevice -> SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0,m_pTexture);

    //���C�g�̃I�t
    pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);

    //���[���h���W�ϊ��s��̏�����
    D3DXMatrixIdentity(&mtxWorld);

    //���s�ړ�
    D3DXMatrixTranslation(&mtxTrans,m_vPos.x,m_vPos.y,m_vPos.z);
    D3DXMatrixMultiply(&mtxWorld,&mtxWorld,&mtxTrans);

    pDevice->SetTransform(D3DTS_WORLD,&mtxWorld);

    //���_���ƃv���~�e�B�u���̌v�Z
    nNumVtx = (HIT_SPHERE_MAX_U + 1) * (HIT_SPHERE_MAX_V + 1);
    nNumFace = (HIT_SPHERE_MAX_U + 1) * HIT_SPHERE_MAX_V * 2 + (HIT_SPHERE_MAX_V - 1) * 4;

    //�C���f�b�N�X�o�b�t�@�ɂ��`��
    pDevice->DrawIndexedPrimitive(
        D3DPT_TRIANGLESTRIP,        //�`��`��
        0,                          //�ŏ��̒��_�C���f�b�N�X�܂ł̃I�t�Z�b�g(���_�o�b�t�@�̃I�t�Z�b�g�@��{0)
        0,                          //�ŏ��̒��_�C���f�b�N�X(�K�v�Ȃ�)
        nNumVtx,                    //���_��
        0,                          //�X�^�[�g�C���f�b�N�X(�C���f�b�N�X�o�b�t�@�̃I�t�Z�b�g�@���f���̐������炷)
        nNumFace);                  //�v���~�e�B�u��
    
    //���C�g�̃I��
    pDevice->SetRenderState(D3DRS_LIGHTING,TRUE);
}

//*************************************************************************************************
// ���_���̐ݒ�
//*************************************************************************************************
HRESULT CHitDome::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{
    VERTEX_3D* pVtx;
    int nNumVtx;
    D3DXVECTOR3 pos[(HIT_SPHERE_MAX_U + 1) * (HIT_SPHERE_MAX_V + 1)];

    //���_���̌v�Z
    nNumVtx = (HIT_SPHERE_MAX_U + 1) * (HIT_SPHERE_MAX_V + 1);

    //���_�o�b�t�@
    if(FAILED(pDevice->CreateVertexBuffer(
        sizeof(VERTEX_3D) * nNumVtx,        //�쐬���������_�o�b�t�@�̃T�C�Y
        D3DUSAGE_WRITEONLY,                 //�g�p���@
        FVF_VERTEX_3D,                      //
        D3DPOOL_MANAGED,                    //�������Ǘ����@(�f�o�C�X�ɂ��C��)
        &m_pVtxBuff,
        NULL)))
    {
        return E_FAIL;
    }

    //���_���̐ݒ�///////////////////////////////////////////////////////////////////////////////
    m_pVtxBuff->Lock(0,0,(void**)&pVtx,0);    //���b�N

    //���_���W
    for(int i = 0;i < HIT_SPHERE_MAX_V + 1;i++)
    {
        for(int j = 0;j < (HIT_SPHERE_MAX_U + 1);j++)
        {
            //���_���W�v�Z
            pVtx[i * (HIT_SPHERE_MAX_U + 1) + j].pos = D3DXVECTOR3(
                m_vSize.x * sinf(D3DX_PI * i / HIT_SPHERE_MAX_V) * cosf(D3DX_PI * j / (HIT_SPHERE_MAX_U / 2) * -1), //X
                m_vSize.y * cosf(D3DX_PI * i / HIT_SPHERE_MAX_V),                                               //Y
                m_vSize.z * sinf(D3DX_PI * i / HIT_SPHERE_MAX_V) * sinf(D3DX_PI * j / (HIT_SPHERE_MAX_U / 2) * -1));//Z

#ifdef _DEBUG
            //�f�o�b�N�p
            pos[i * (HIT_SPHERE_MAX_U + 1) + j] = D3DXVECTOR3(
                m_vSize.x * sinf(D3DX_PI * i / HIT_SPHERE_MAX_V) * cosf(D3DX_PI * j / (HIT_SPHERE_MAX_U / 2)),  //X
                m_vSize.x * cosf(D3DX_PI * i / HIT_SPHERE_MAX_V),                                           //Y
                m_vSize.x * sinf(D3DX_PI * i / HIT_SPHERE_MAX_V) * sinf(D3DX_PI * j / (HIT_SPHERE_MAX_U / 2))); //Z
#endif
            pVtx[i * (HIT_SPHERE_MAX_U + 1) + j].normal = D3DXVECTOR3(0.0f,0.0f,-1.0f);
            pVtx[i * (HIT_SPHERE_MAX_U + 1) + j].color = D3DCOLOR_RGBA(255, 0, 0, 100);
            pVtx[i * (HIT_SPHERE_MAX_U + 1) + j].tex = D3DXVECTOR2(j * 1.0f / HIT_SPHERE_MAX_U,i * 1.0f / HIT_SPHERE_MAX_V);
        }
    }

    m_pVtxBuff->Unlock();    //�A�����b�N

    return 0;
}

//*************************************************************************************************
// ���_�C���f�b�N�X�̐ݒ�
//*************************************************************************************************
HRESULT CHitDome::MakeIndex(LPDIRECT3DDEVICE9 pDevice)
{
    WORD* pIdx;
    WORD wIdx[(HIT_SPHERE_MAX_U + 1) * 2 * HIT_SPHERE_MAX_V + (HIT_SPHERE_MAX_V - 1) * 2];
    int nNumIdx;

    //�C���f�b�N�X���̌v�Z
    nNumIdx = (HIT_SPHERE_MAX_U + 1) * 2 * HIT_SPHERE_MAX_V + (HIT_SPHERE_MAX_V - 1) * 2;

    //�C���f�b�N�X�o�b�t�@
    if(FAILED(pDevice->CreateIndexBuffer(
        sizeof(WORD) * nNumIdx,             //�C���f�b�N�X��
        D3DUSAGE_WRITEONLY,                 //�g�p�p�r�t���O
        D3DFMT_INDEX16,                     //�C���f�b�N�X�f�[�^�̃t�H�[�}�b�g
        D3DPOOL_MANAGED,                    //�������̊Ǘ����@�i���C���j
        &m_pIdxBuff,                //�C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�|�C���^�̃A�h���X
        NULL)));

    //���_�C���f�b�N�X�̐ݒ�////////////////////////////////////////////////////////////////////////
    m_pIdxBuff->Lock(0,0,(void**)&pIdx,0);    //���b�N

    //N���^�ɐ���
    //�c�̃��b�V������
    for(int height = 0;height < HIT_SPHERE_MAX_V;height++)
    {
        //���̃��b�V������
        for(int width = 0;width < HIT_SPHERE_MAX_U + 1;width++)
        {
            //�k�ތv�Z
            if(width == 0 && height != 0)
            {
                pIdx[height * ((HIT_SPHERE_MAX_U + 1) * 2 + 2) - 2] = (height - 1) * (HIT_SPHERE_MAX_U + 1) + HIT_SPHERE_MAX_U;
                pIdx[height * ((HIT_SPHERE_MAX_U + 1) * 2 + 2) - 1] = height * (HIT_SPHERE_MAX_U + 1) + (HIT_SPHERE_MAX_U + 1);
            
                //�f�o�b�N�p
                wIdx[height * ((HIT_SPHERE_MAX_U + 1) * 2 + 2) - 2] = (height - 1) * (HIT_SPHERE_MAX_U + 1) + HIT_SPHERE_MAX_U;
                wIdx[height * ((HIT_SPHERE_MAX_U + 1) * 2 + 2) - 1] = height * (HIT_SPHERE_MAX_U + 1) + (HIT_SPHERE_MAX_U + 1);
            }

            pIdx[(width * 2) + (height * ((HIT_SPHERE_MAX_U + 1) * 2 + 2))] = height * (HIT_SPHERE_MAX_U + 1) + width + (HIT_SPHERE_MAX_U + 1);
            pIdx[(width * 2 + 1) + (height * ((HIT_SPHERE_MAX_U + 1) * 2 + 2))] = height * (HIT_SPHERE_MAX_U + 1) + width;

            //�f�o�b�N�p
            wIdx[(width * 2) + (height * ((HIT_SPHERE_MAX_U + 1) * 2 + 2))] = height * (HIT_SPHERE_MAX_U + 1) + width + (HIT_SPHERE_MAX_U + 1);
            wIdx[(width * 2 + 1) + (height * ((HIT_SPHERE_MAX_U + 1) * 2 + 2))] = height * (HIT_SPHERE_MAX_U + 1) + width;
        }
    }

    m_pIdxBuff->Unlock();    //�A�����b�N
    return 0;
}

//*************************************************************************************************
// �C���X�^���X����
//*************************************************************************************************
CHitDome *CHitDome::Create(D3DXVECTOR3 pos,D3DXVECTOR3 size)
{
    CHitDome *hitdome;

    hitdome = new CHitDome(PRIOLITY_1);

    hitdome->m_vPos = pos;
    hitdome->m_vSize = size * 1.5f;

    hitdome->Init();

    return hitdome;
}

//*************************************************************************************************
// ���W�ƃT�C�Y�̐ݒ�
//*************************************************************************************************
void CHitDome::SetDome(D3DXVECTOR3 pos,D3DXVECTOR3 size)
{
    m_vPos = pos;
    m_vSize = size;
}

//*************************************************************************************************
// �q�b�g��
//*************************************************************************************************
void CHitDome::HitToDome(void)
{
    m_Hit = true;
}