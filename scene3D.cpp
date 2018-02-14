//=================================================================================================
//
// 3D�|���S���\���v���O���� [scene3D.cpp]
// Author : TAKUYA EBIHARA
//
//=================================================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "scene3D.h"

//=================================================================================================
// �萔��`
//=================================================================================================

//=================================================================================================
// �\���̐錾
//=================================================================================================
//�e�N�X�`�����\����
typedef struct
{
	char *pFilename;	// �t�@�C����
} TEXPARAM_3D;

//=================================================================================================
// �O���[�o���ϐ�
//=================================================================================================
//�e�N�X�`���p�X
TEXPARAM_3D g_aTexParam[TEXTURE_3D_MAX] = 
{
    "data/TEXTURE/ball.png"
    "data/TEXTURE/lifebar_frame.png",
    "data/TEXTURE/lifebar_BG.png",
    "data/TEXTURE/lifebar_Green.png",
    "data/TEXTURE/lifebar_Red.png"

};

//*************************************************************************************************
// �R���X�g���N�^
//*************************************************************************************************
CScene3D::CScene3D(int nPriolity):CScene(nPriolity)
{
    m_bLoadTexture = false;
}

//*************************************************************************************************
// �f�X�g���N�^
//*************************************************************************************************
CScene3D::~CScene3D()
{
    m_bLoadTexture = false;
}

//*************************************************************************************************
// ����������
//*************************************************************************************************
HRESULT CScene3D::Init(void)
{
    LPDIRECT3DDEVICE9 pDevice;

     //�f�o�C�X�̎擾
    pDevice = CManager::GetRenderer()->GetDevice();

    //���_���̐ݒ�
    if(FAILED(MakeVertex(pDevice)))
    {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CScene3D::Init2(void)
{
    LPDIRECT3DDEVICE9 pDevice;

    //�f�o�C�X�̎擾
    pDevice = CManager::GetRenderer()->GetDevice();

    //���_���̐ݒ�
    if (FAILED(MakeVertex2(pDevice)))
    {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CScene3D::InitLine(void)
{
    LPDIRECT3DDEVICE9 pDevice;

    //�f�o�C�X�̎擾
    pDevice = CManager::GetRenderer()->GetDevice();

    //���_���̐ݒ�
    if (FAILED(MakeVertexLine(pDevice)))
    {
        return E_FAIL;
    }

    return S_OK;
}

//*************************************************************************************************
// �I������
//*************************************************************************************************
void CScene3D::Uninit(void)
{
    //�e�N�X�`���̔j��
    if(m_pTexture != NULL && m_bLoadTexture == true)
    {
        m_pTexture -> Release();
        m_pTexture = NULL;
    }

    //���_�o�b�t�@���
	if(m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//*************************************************************************************************
// �X�V����
//*************************************************************************************************
void CScene3D::Update(void)
{

}

//*************************************************************************************************
// �`�揈��
//*************************************************************************************************
void CScene3D::Draw(void)
{
    LPDIRECT3DDEVICE9 pDevice;

     //�f�o�C�X�̎擾
    pDevice = CManager::GetRenderer()->GetDevice();

	//�X�g���[���̐ݒ�
	pDevice->SetStreamSource(0,				//�X�g���[���ԍ�(�p�C�v�ԍ�)
		m_pVtxBuff,				            //�X�g���[���̌��ɂȂ钸�_�o�b�t�@
		0,									//�I�t�Z�b�g(�o�C�g)
		sizeof(VERTEX_3D));					//��̒��_�f�[�^�̃T�C�Y(�X�g���C�h��)

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0,m_pTexture);

	//�v���~�e�B�u�̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
		0,										//�I�t�Z�b�g(���_��)
		NUM_POLYGON);							//�v���~�e�B�u�̐�
}

void CScene3D::DrawLine(void)
{
    LPDIRECT3DDEVICE9 pDevice;

    //�f�o�C�X�̎擾
    pDevice = CManager::GetRenderer()->GetDevice();

    //�X�g���[���̐ݒ�
    pDevice->SetStreamSource(0,				//�X�g���[���ԍ�(�p�C�v�ԍ�)
        m_pVtxBuff,				            //�X�g���[���̌��ɂȂ钸�_�o�b�t�@
        0,									//�I�t�Z�b�g(�o�C�g)
        sizeof(VERTEX_3D));					//��̒��_�f�[�^�̃T�C�Y(�X�g���C�h��)

    //���_�t�H�[�}�b�g�̐ݒ�
    pDevice->SetFVF(FVF_VERTEX_3D);

    //�e�N�X�`���̐ݒ�
    pDevice->SetTexture(0, NULL);

    //�v���~�e�B�u�̕`��
    pDevice->DrawPrimitive(D3DPT_LINELIST,	//�v���~�e�B�u�̎��
        0,									//�I�t�Z�b�g(���_��)
        1);							        //�v���~�e�B�u�̐�
}

//*************************************************************************************************
// �e�N�X�`���̓ǂݍ���
//*************************************************************************************************
void CScene3D::LoadTexture(TEXTURE_LABEL_3D label)
{
    //�f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    //NULL�`�F�b�N
    if(m_pTexture == NULL)
    {
        //�e�N�X�`���̓ǂݍ���
        if(FAILED(D3DXCreateTextureFromFile(pDevice,g_aTexParam[label].pFilename,&m_pTexture)))
        {
            //�G���[��
            MessageBox(NULL,"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���","�I�����b�Z�[�W",MB_OK);
        }

        m_bLoadTexture = true;
    }
}

//=============================================================================
// �e�N�X�`���̎擾����
//=============================================================================
void CScene3D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
    if(pTexture != NULL)
    {
        m_pTexture = pTexture;
    }
}

//*************************************************************************************************
// ���_���̐ݒ�y���S��z
//*************************************************************************************************
HRESULT CScene3D::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_3D* pVtx;

	//���_�o�b�t�@
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,		//�쐬���������_�o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,							//�g�p���@
		FVF_VERTEX_3D,								//
		D3DPOOL_MANAGED,							//�������Ǘ����@(�f�o�C�X�ɂ��C��)
		&m_pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	//���z�A�h���X���擾����
	m_pVtxBuff->Lock(0,0,(void**)&pVtx,0);

	//���_���W�̐ݒ�
    pVtx[0].pos = D3DXVECTOR3(0.0f - (m_vSize.x * 0.5f),0.0f + (m_vSize.y * 0.5f),0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f + (m_vSize.x * 0.5f),0.0f + (m_vSize.y * 0.5f),0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f - (m_vSize.x * 0.5f),0.0f - (m_vSize.y * 0.5f),0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f + (m_vSize.x * 0.5f),0.0f - (m_vSize.y * 0.5f),0.0f);

    //�@���̐ݒ�
    pVtx[0].normal = D3DXVECTOR3(0.0f,0.0f,-1.0f);
    pVtx[1].normal = D3DXVECTOR3(0.0f,0.0f,-1.0f);
    pVtx[2].normal = D3DXVECTOR3(0.0f,0.0f,-1.0f);
    pVtx[3].normal = D3DXVECTOR3(0.0f,0.0f,-1.0f);

	//�e�N�X�`���̒��_���W
	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

	//���_�J���[�̐ݒ�(0�`255�̐����l)
	pVtx[0].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[1].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[2].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[3].color = D3DCOLOR_RGBA(255,255,255,255);

	m_pVtxBuff->Unlock();		//���b�N����

	return 0;
}

//*************************************************************************************************
// ���_���̐ݒ�y�����z
//*************************************************************************************************
HRESULT CScene3D::MakeVertex2(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_3D* pVtx;

	//���_�o�b�t�@
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,		//�쐬���������_�o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,							//�g�p���@
		FVF_VERTEX_3D,								//
		D3DPOOL_MANAGED,							//�������Ǘ����@(�f�o�C�X�ɂ��C��)
		&m_pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	//���z�A�h���X���擾����
	m_pVtxBuff->Lock(0,0,(void**)&pVtx,0);

	//���_���W�̐ݒ�
    pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f + m_vSize.x,0.0f,0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f,0.0f - m_vSize.y,0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f + m_vSize.x,0.0f - m_vSize.y,0.0f);

    //�@���̐ݒ�
    pVtx[0].normal = D3DXVECTOR3(0.0f,0.0f,-1.0f);
    pVtx[1].normal = D3DXVECTOR3(0.0f,0.0f,-1.0f);
    pVtx[2].normal = D3DXVECTOR3(0.0f,0.0f,-1.0f);
    pVtx[3].normal = D3DXVECTOR3(0.0f,0.0f,-1.0f);

	//�e�N�X�`���̒��_���W
	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

	//���_�J���[�̐ݒ�(0�`255�̐����l)
	pVtx[0].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[1].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[2].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[3].color = D3DCOLOR_RGBA(255,255,255,255);

	m_pVtxBuff->Unlock();		//���b�N����

	return 0;
}

//*************************************************************************************************
// ���_���̐ݒ�y���C���z
//*************************************************************************************************
HRESULT CScene3D::MakeVertexLine(LPDIRECT3DDEVICE9 pDevice)
{
    VERTEX_3D* pVtx;

    //���_�o�b�t�@
    if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 2,		//�쐬���������_�o�b�t�@�̃T�C�Y
        D3DUSAGE_WRITEONLY,							//�g�p���@
        FVF_VERTEX_3D,								//
        D3DPOOL_MANAGED,							//�������Ǘ����@(�f�o�C�X�ɂ��C��)
        &m_pVtxBuff,
        NULL)))
    {
        return E_FAIL;
    }

    //���z�A�h���X���擾����
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    //���_���W�̐ݒ�
    pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    //�@���̐ݒ�
    pVtx[0].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
    pVtx[1].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

    //�e�N�X�`���̒��_���W
    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
    pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);

    //���_�J���[�̐ݒ�(0�`255�̐����l)
    pVtx[0].color = D3DCOLOR_RGBA(255, 255, 255, 255);
    pVtx[1].color = D3DCOLOR_RGBA(255, 255, 255, 255);

    m_pVtxBuff->Unlock();		//���b�N����

    return 0;
}

//*************************************************************************************************
// ���_���W�̐ݒ�y���C���z
//*************************************************************************************************
void CScene3D::SetLinePos(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
    VERTEX_3D* pVtx;
    //���z�A�h���X���擾����
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    //���_���W�̐ݒ�
    pVtx[0].pos = pos1;
    pVtx[1].pos = pos2;

    m_pVtxBuff->Unlock();		//���b�N����
}

//*************************************************************************************************
// ���_�J���[�̐ݒ�y���C���z
//*************************************************************************************************
void CScene3D::SetLineColor(D3DCOLOR color1, D3DCOLOR color2)
{
    VERTEX_3D* pVtx;
    //���z�A�h���X���擾����
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    //���_�J���[�̐ݒ�(0�`255�̐����l)
    pVtx[0].color = color1;
    pVtx[1].color = color2;

    m_pVtxBuff->Unlock();		//���b�N����
}

void CScene3D::SetPosition(D3DXVECTOR3 pos)
{
    m_vPos = pos;
}



//*************************************************************************************************
// �傫���ύX
//*************************************************************************************************
void CScene3D::UpdateSize(void)
{
    VERTEX_3D* pVtx;
    //���z�A�h���X���擾����
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    //���_���W�̐ݒ�

    pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    pVtx[1].pos = D3DXVECTOR3(0.0f + m_vSize.x, 0.0f, 0.0f);
    pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f - m_vSize.y, 0.0f);
    pVtx[3].pos = D3DXVECTOR3(0.0f + m_vSize.x, 0.0f - m_vSize.y, 0.0f);

    m_pVtxBuff->Unlock();		//���b�N����
}

void CScene3D::AnimationTexture(D3DXVECTOR2 texpos, D3DXVECTOR2 texsize) 
{
    VERTEX_3D* pVtx;
    //���z�A�h���X���擾����
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    //�e�N�X�`���̒��_���W
    pVtx[0].tex = D3DXVECTOR2(texpos.x, texpos.y);
    pVtx[1].tex = D3DXVECTOR2(texpos.x + texsize.x, texpos.y);
    pVtx[2].tex = D3DXVECTOR2(texpos.x, texpos.y + texsize.y);
    pVtx[3].tex = D3DXVECTOR2(texpos.x + texsize.x, texpos.y + texsize.y);

    m_pVtxBuff->Unlock();		//���b�N����
}
