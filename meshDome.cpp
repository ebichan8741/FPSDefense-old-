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
#include "meshdome.h"
#include "camera.h"
#include "GameMode.h"

//=================================================================================================
// �萔��`
//=================================================================================================
#define SPHERE_TEXTURENAME "data/TEXTURE/skydome001.png"
#define SPHERE_MAX_U    (100)        //�t�����̕�����
#define SPHERE_MAX_V    (100)        //�u�����̕�����
#define SPHERE_RADIUS   (2000.0f)   //���̂̔��a
#define SPHERE_POS_Y     (-50.0f)      //�h�[���̂x���W

//=================================================================================================
// �O���[�o���ϐ�
//=================================================================================================
float g_sphereRot;

//*************************************************************************************************
// �R���X�g���N�^
//*************************************************************************************************
CMeshDome::CMeshDome(int nPriolity):CScene(nPriolity)
{
}

//*************************************************************************************************
// ����������
//*************************************************************************************************
HRESULT CMeshDome::Init(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    g_sphereRot = 0.0f;

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

    //�n�[�h�f�B�X�N����e�N�X�`����ǂݍ���
    if(FAILED(D3DXCreateTextureFromFile(pDevice,SPHERE_TEXTURENAME,&m_pTexture)))
    {
        //�G���[��
        MessageBox(NULL,"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���","�I�����b�Z�[�W",MB_OK);
    }

    return 0;
}

//*************************************************************************************************
// �I������
//*************************************************************************************************
void CMeshDome::Uninit(void)
{
    //�e�N�X�`���C���^�[�t�F�[�X�̉��
    if( m_pTexture != NULL )
    {
        m_pTexture -> Release();
        m_pTexture = NULL;
    }
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
void CMeshDome::Update(void)
{
    g_sphereRot += 0.005f;
}

//*************************************************************************************************
// �`�揈��
//*************************************************************************************************
void CMeshDome::Draw(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
    D3DXMATRIX mtxWorld,mtxRotY,mtxTrans;
    D3DXVECTOR3 vCameraPos = CManager::GetCamera()->GetCameraPos();
    int nNumVtx,nNumFace;

    pDevice->SetStreamSource(
        0,                        //�X�g���[���ԍ�(�p�C�v�ԍ�)
        m_pVtxBuff,       //�X�g���[���̌��ɂȂ钸�_�o�b�t�@
        0,                        //�I�t�Z�b�g(�o�C�g)
        sizeof(VERTEX_3D));       //��̒��_�f�[�^�̃T�C�Y(�X�g���C�h��)

    //�f�o�C�X�ɃC���f�b�N�X�o�b�t�@�̐ݒ�
    pDevice->SetIndices(m_pIdxBuff);

    //���_�t�H�[�}�b�g�̐ݒ�
    pDevice -> SetFVF(FVF_VERTEX_3D);

    //�e�N�X�`���̐ݒ�
    pDevice->SetTexture(0,m_pTexture);

    //���C�g�̃I�t
    pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);

    //���[���h���W�ϊ��s��
    D3DXMatrixIdentity(&mtxWorld);

    //��]
    D3DXMatrixRotationYawPitchRoll(&mtxRotY,D3DXToRadian(g_sphereRot),0.0f,0.0f);	//��](�o�͐�,���W�A���p�xY,���W�A���p�xZ,���W�A���p�xX)
    D3DXMatrixMultiply(&mtxWorld,&mtxWorld,&mtxRotY);
    //�J�����̈ʒu�ɕ��s�ړ�
    D3DXMatrixTranslation(&mtxTrans,vCameraPos.x,SPHERE_POS_Y,vCameraPos.z);
    D3DXMatrixMultiply(&mtxWorld,&mtxWorld,&mtxTrans);

    pDevice->SetTransform(D3DTS_WORLD,&mtxWorld);

    //���_���ƃv���~�e�B�u���̌v�Z
    nNumVtx = (SPHERE_MAX_U + 1) * (SPHERE_MAX_V + 1);
    nNumFace = (SPHERE_MAX_U + 1) * SPHERE_MAX_V * 2 + (SPHERE_MAX_V - 1) * 4;

    //�C���f�b�N�X�o�b�t�@�ɂ��`��
    pDevice->DrawIndexedPrimitive(
        D3DPT_TRIANGLESTRIP,        //�`��`��
        0,                          //�ŏ��̒��_�C���f�b�N�X�܂ł̃I�t�Z�b�g(���_�o�b�t�@�̃I�t�Z�b�g�@��{0)
        0,                          //�ŏ��̒��_�C���f�b�N�X(�K�v�Ȃ�)
        nNumVtx,                    //���_��
        0,                          //�X�^�[�g�C���f�b�N�X(�C���f�b�N�X�o�b�t�@�̃I�t�Z�b�g�@���f���̐������炷)
        nNumFace);              //�v���~�e�B�u��

    //���C�g�̃I��
    pDevice->SetRenderState(D3DRS_LIGHTING,TRUE);
}

//*************************************************************************************************
// ���_���̐ݒ�
//*************************************************************************************************
HRESULT CMeshDome::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{
    VERTEX_3D* pVtx;
    int nNumVtx;
    D3DXVECTOR3 pos[(SPHERE_MAX_U + 1) * (SPHERE_MAX_V + 1)];

    //���_���̌v�Z
    nNumVtx = (SPHERE_MAX_U + 1) * (SPHERE_MAX_V + 1);

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
    for(int i = 0;i < SPHERE_MAX_V + 1;i++)
    {
        for(int j = 0;j < (SPHERE_MAX_U + 1);j++)
        {
            //���_���W�v�Z
            pVtx[i * (SPHERE_MAX_U + 1) + j].pos = D3DXVECTOR3(
                SPHERE_RADIUS * sinf(D3DX_PI * i / SPHERE_MAX_V) * cosf(D3DX_PI * j / (SPHERE_MAX_U / 2) * -1), //X
                SPHERE_RADIUS * cosf(D3DX_PI * i / SPHERE_MAX_V),                                               //Y
                SPHERE_RADIUS * sinf(D3DX_PI * i / SPHERE_MAX_V) * sinf(D3DX_PI * j / (SPHERE_MAX_U / 2) * -1));//Z

#ifdef _DEBUG
            //�f�o�b�N�p
            pos[i * (SPHERE_MAX_U + 1) + j] = D3DXVECTOR3(
                SPHERE_RADIUS * sinf(D3DX_PI * i / SPHERE_MAX_V) * cosf(D3DX_PI * j / (SPHERE_MAX_U / 2)),  //X
                SPHERE_RADIUS * cosf(D3DX_PI * i / SPHERE_MAX_V),                                           //Y
                SPHERE_RADIUS * sinf(D3DX_PI * i / SPHERE_MAX_V) * sinf(D3DX_PI * j / (SPHERE_MAX_U / 2))); //Z
#endif
            pVtx[i * (SPHERE_MAX_U + 1) + j].normal = D3DXVECTOR3(0.0f,0.0f,-1.0f);
            pVtx[i * (SPHERE_MAX_U + 1) + j].color = D3DCOLOR_RGBA(255, 255, 255, 255);
            pVtx[i * (SPHERE_MAX_U + 1) + j].tex = D3DXVECTOR2(j * 1.0f / SPHERE_MAX_U,i * 1.0f / SPHERE_MAX_V);
        }
    }

    m_pVtxBuff->Unlock();    //�A�����b�N

    return 0;
}

//*************************************************************************************************
// ���_�C���f�b�N�X�̐ݒ�
//*************************************************************************************************
HRESULT CMeshDome::MakeIndex(LPDIRECT3DDEVICE9 pDevice)
{
    WORD* pIdx;
    WORD wIdx[(SPHERE_MAX_U + 1) * 2 * SPHERE_MAX_V + (SPHERE_MAX_V - 1) * 2];
    int nNumIdx;

    //�C���f�b�N�X���̌v�Z
    nNumIdx = (SPHERE_MAX_U + 1) * 2 * SPHERE_MAX_V + (SPHERE_MAX_V - 1) * 2;

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
    for(int height = 0;height < SPHERE_MAX_V;height++)
    {
        //���̃��b�V������
        for(int width = 0;width < SPHERE_MAX_U + 1;width++)
        {
            //�k�ތv�Z
            if(width == 0 && height != 0)
            {
                pIdx[height * ((SPHERE_MAX_U + 1) * 2 + 2) - 2] = (height - 1) * (SPHERE_MAX_U + 1) + SPHERE_MAX_U;
                pIdx[height * ((SPHERE_MAX_U + 1) * 2 + 2) - 1] = height * (SPHERE_MAX_U + 1) + (SPHERE_MAX_U + 1);
            
                //�f�o�b�N�p
                wIdx[height * ((SPHERE_MAX_U + 1) * 2 + 2) - 2] = (height - 1) * (SPHERE_MAX_U + 1) + SPHERE_MAX_U;
                wIdx[height * ((SPHERE_MAX_U + 1) * 2 + 2) - 1] = height * (SPHERE_MAX_U + 1) + (SPHERE_MAX_U + 1);
            }

            pIdx[(width * 2) + (height * ((SPHERE_MAX_U + 1) * 2 + 2))] = height * (SPHERE_MAX_U + 1) + width + (SPHERE_MAX_U + 1);
            pIdx[(width * 2 + 1) + (height * ((SPHERE_MAX_U + 1) * 2 + 2))] = height * (SPHERE_MAX_U + 1) + width;

            //�f�o�b�N�p
            wIdx[(width * 2) + (height * ((SPHERE_MAX_U + 1) * 2 + 2))] = height * (SPHERE_MAX_U + 1) + width + (SPHERE_MAX_U + 1);
            wIdx[(width * 2 + 1) + (height * ((SPHERE_MAX_U + 1) * 2 + 2))] = height * (SPHERE_MAX_U + 1) + width;
        }
    }

    m_pIdxBuff->Unlock();    //�A�����b�N
    return 0;
}

//*************************************************************************************************
// �C���X�^���X����
//*************************************************************************************************
CMeshDome *CMeshDome::Create(void)
{
    CMeshDome *meshdome;

    meshdome = new CMeshDome(PRIOLITY_0);

    meshdome->Init();

    return meshdome;
}