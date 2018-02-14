//================================================================================================
//
// ���b�V���V�����_�[ [meshcylinder.cpp]
// Author : TAKUYA EBIHARA
//
//================================================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "meshcylinder.h"
#include "camera.h"

//=================================================================================================
// �萔��`
//=================================================================================================
#define CYLINDER_TEXTURENAME "data/TEXTURE/cylinder.jpg"
#define CYLINDER_MAX_U      (12)    //�t�����̕�����
#define CYLINDER_HEIGHT     (150.0f) //�V�����_�[�̍���
#define CYLINDER_RADIUS     (500.0f) //�V�����_�[�̔��a

//=================================================================================================
// �O���[�o���ϐ�
//=================================================================================================
D3DXVECTOR3 g_cylinderPos;

//*************************************************************************************************
// �R���X�g���N�^
//*************************************************************************************************
CMeshCylinder::CMeshCylinder(int nPriolity) :CScene(nPriolity)
{
}

//*************************************************************************************************
// �|���S������������
//*************************************************************************************************
HRESULT CMeshCylinder::Init(void)
{
    //---------------------------------------------------------------------------------------------
    // �ϐ��錾�J�n
    //---------------------------------------------------------------------------------------------
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
    //---------------------------------------------------------------------------------------------
    // �ϐ��錾�I��
    //---------------------------------------------------------------------------------------------

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
    if(FAILED(D3DXCreateTextureFromFile(pDevice,CYLINDER_TEXTURENAME,&m_pTexture)))
    {
        //�G���[��
        MessageBox(NULL,"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���","�I�����b�Z�[�W",MB_OK);
    }

    return 0;
}

//*************************************************************************************************
// �I������
//*************************************************************************************************
void CMeshCylinder::Uninit(void)
{
    if( m_pTexture != NULL )
    {
        m_pTexture -> Release();        //Direct3D�f�o�C�X�̉��
        m_pTexture = NULL;
    }

    if( m_pVtxBuff != NULL )
    {
        m_pVtxBuff -> Release();            //Direct3D�C���^�[�t�F�[�X�̉��
        m_pVtxBuff = NULL;
    }

    if( m_pIdxBuff != NULL )
    {
        m_pIdxBuff -> Release();            //Direct3D�C���^�[�t�F�[�X�̉��
        m_pIdxBuff = NULL;
    }
}

//*************************************************************************************************
// �X�V����
//*************************************************************************************************
void CMeshCylinder::Update(void)
{

}

//*************************************************************************************************
// �`�揈��
//*************************************************************************************************
void CMeshCylinder::Draw(void)
{
    //---------------------------------------------------------------------------------------------
    // �ϐ��錾�J�n
    //---------------------------------------------------------------------------------------------
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
    D3DXMATRIX mtxWorld;
    int nNumVtx,nNumFace;
    //---------------------------------------------------------------------------------------------
    // �ϐ��錾�I��
    //---------------------------------------------------------------------------------------------

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
    D3DXMatrixIdentity(&mtxWorld);            //���[���h���W�ϊ��s���������

    pDevice->SetTransform(D3DTS_WORLD,&mtxWorld);        //���[���h���W�ϊ��s����ړ��ł���悤�ɂ���(?)

    //���_���ƃv���~�e�B�u���̌v�Z
    nNumVtx = (CYLINDER_MAX_U + 1) * 2;
    nNumFace = (CYLINDER_MAX_U + 1) * 2;

    //�C���f�b�N�X�o�b�t�@�ɂ��`��
    pDevice->DrawIndexedPrimitive(
        D3DPT_TRIANGLESTRIP,        //�`��`��
        0,                          //�ŏ��̒��_�C���f�b�N�X�܂ł̃I�t�Z�b�g
        0,                          //�ŏ��̒��_�C���f�b�N�X
        nNumVtx,                    //���_��
        0,                          //�X�^�[�g�C���f�b�N�X
        nNumFace);                  //�v���~�e�B�u��

    //���C�g�̃I��
    pDevice->SetRenderState(D3DRS_LIGHTING,TRUE);
}

//*************************************************************************************************
// ���_���̐ݒ�
//*************************************************************************************************
HRESULT CMeshCylinder::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{
    //---------------------------------------------------------------------------------------------
    // �ϐ��錾�J�n
    //---------------------------------------------------------------------------------------------
    VERTEX_3D* pVtx;
    D3DXVECTOR3 pos[(CYLINDER_MAX_U + 1) * 2];
    int nNumVtx;
    //---------------------------------------------------------------------------------------------
    // �ϐ��錾�I��
    //---------------------------------------------------------------------------------------------

    //���_���̌v�Z
    nNumVtx = (CYLINDER_MAX_U + 1) * 2;

    //���_�o�b�t�@
    if(FAILED(pDevice->CreateVertexBuffer(
        sizeof(VERTEX_3D) * nNumVtx,    //�쐬���������_�o�b�t�@�̃T�C�Y
        D3DUSAGE_WRITEONLY,             //�g�p���@
        FVF_VERTEX_3D,                  //�����ݒ�
        D3DPOOL_MANAGED,                //�������Ǘ����@(�f�o�C�X�ɂ��C��)
        &m_pVtxBuff,
        NULL)))
    {
        return E_FAIL;
    }

    //���_���̐ݒ�///////////////////////////////////////////////////////////////////////////////
    m_pVtxBuff->Lock(0,0,(void**)&pVtx,0);    //���b�N

    //���_���W
    for(int i = 0;i < 2;i++)
    {
        for(int j = 0;j < CYLINDER_MAX_U + 1;j++)
        {
            //���_���W�v�Z
            pVtx[i * (CYLINDER_MAX_U + 1) + j].pos = D3DXVECTOR3(
                CYLINDER_RADIUS * cosf(D3DX_PI * j / (CYLINDER_MAX_U / 2) * -1),          //X
                CYLINDER_HEIGHT - i * CYLINDER_HEIGHT,                                    //Y
                CYLINDER_RADIUS * sinf(D3DX_PI * j / (CYLINDER_MAX_U / 2) * -1));         //Z

            //�f�o�b�N�p
            pos[i * (CYLINDER_MAX_U + 1) + j] = D3DXVECTOR3(
                CYLINDER_RADIUS * cosf(D3DX_PI * j / (CYLINDER_MAX_U / 2)),          //X
                CYLINDER_HEIGHT - i * CYLINDER_HEIGHT,                               //Y
                CYLINDER_RADIUS * sinf(D3DX_PI * j / (CYLINDER_MAX_U / 2)));         //Z

            pVtx[i * (CYLINDER_MAX_U + 1) + j].normal = D3DXVECTOR3(0.0f,0.0f,1.0f);
            pVtx[i * (CYLINDER_MAX_U + 1) + j].color = D3DCOLOR_RGBA(255, 255, 255, 255);
            pVtx[i * (CYLINDER_MAX_U + 1) + j].tex = D3DXVECTOR2(j * (1.0 / CYLINDER_MAX_U), i * 1.0f);
        }
    }

    m_pVtxBuff->Unlock();    //�A�����b�N

    return 0;
}

//*************************************************************************************************
// ���_�C���f�b�N�X�̐ݒ�
//*************************************************************************************************
HRESULT CMeshCylinder::MakeIndex(LPDIRECT3DDEVICE9 pDevice)
{
    //---------------------------------------------------------------------------------------------
    // �ϐ��錾�J�n
    //---------------------------------------------------------------------------------------------
    WORD* pIdx;
    WORD wIdx[(CYLINDER_MAX_U + 1) * 2];
    int nNumIdx;
    //---------------------------------------------------------------------------------------------
    // �ϐ��錾�I��
    //---------------------------------------------------------------------------------------------

    //�C���f�b�N�X���̌v�Z
    nNumIdx = (CYLINDER_MAX_U + 1) * 2;

    //�C���f�b�N�X�o�b�t�@
    if(FAILED(pDevice->CreateIndexBuffer(
        sizeof(WORD) * nNumIdx,             //�C���f�b�N�X��
        D3DUSAGE_WRITEONLY,                 //�g�p�p�r�t���O
        D3DFMT_INDEX16,                     //�C���f�b�N�X�f�[�^�̃t�H�[�}�b�g
        D3DPOOL_MANAGED,                    //�������̊Ǘ����@�i���C���j
        &m_pIdxBuff,              //�C���f�b�N�X�o�b�t�@�|�C���^�̃A�h���X
        NULL)));

    //���_�C���f�b�N�X�̐ݒ�////////////////////////////////////////////////////////////////////////
    m_pIdxBuff->Lock(0,0,(void**)&pIdx,0);    //���b�N

    //N���^�ɐ���
    //���̃��b�V������
    for(int i = 0;i < CYLINDER_MAX_U + 1;i++)
    {
        pIdx[i * 2] = i + (CYLINDER_MAX_U + 1);
        pIdx[i * 2 + 1] = i;
            
        //�f�o�b�N�p
        wIdx[i * 2] = i + (CYLINDER_MAX_U + 1);
        wIdx[i * 2 + 1] = i;
    }

    m_pIdxBuff->Unlock();    //�A�����b�N
    return 0;
}

//*************************************************************************************************
// �C���X�^���X����
//*************************************************************************************************
CMeshCylinder *CMeshCylinder::Create(void)
{
    CMeshCylinder *meshCylinder;

    meshCylinder = new CMeshCylinder(PRIOLITY_0);

    meshCylinder->Init();

    return meshCylinder;
}
