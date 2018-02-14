//================================================================================================
//
// ���b�V���t�B�[���h [meshfield.cpp]
// Author : TAKUYA EBIHARA
//
//================================================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "meshfield2.h"

//=================================================================================================
// �萔��`
//=================================================================================================
#define FIELD2_TEXTURENAME "data/TEXTURE/water000.jpg"

#define MAX_FIELD_BLOCK_X	(20)			//X�����̃u���b�N�̐�
#define MAX_FIELD_BLOCK_Z	(20)			//Z�����̃u���b�N�̐�
#define FIELD_WIDTH			(11.0f)			//�u���b�N�̉���
#define FIELD_HEIGHT		(11.0f)			//�u���b�N�̏c��
#define FIELD_POS_X			(-(FIELD_WIDTH * MAX_FIELD_BLOCK_X) / 2)	//�t�B�[���h�̊J�n���WX
#define FIELD_POS_Z			((FIELD_HEIGHT * MAX_FIELD_BLOCK_Z) / 2)	//�t�B�[���h�̊J�n���WZ

//=================================================================================================
// �O���[�o���ϐ�
//=================================================================================================

//*************************************************************************************************
// �R���X�g���N�^
//*************************************************************************************************
CMeshField2::CMeshField2(int nPriolity):CScene(nPriolity)
{
}

//*************************************************************************************************
// ����������
//*************************************************************************************************
HRESULT CMeshField2::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

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
	if(FAILED(D3DXCreateTextureFromFile(pDevice,FIELD2_TEXTURENAME,&m_pTexture)))
	{
		//�G���[��
		MessageBox(NULL,"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���","�I�����b�Z�[�W",MB_OK);
	}

	return 0;
}

//*************************************************************************************************
// �I������
//*************************************************************************************************
void CMeshField2::Uninit(void)
{
	if( m_pTexture != NULL )
	{
		m_pTexture -> Release();		//Direct3D�f�o�C�X�̉��
		m_pTexture = NULL;
	}

	if( m_pVtxBuff != NULL )
	{
		m_pVtxBuff -> Release();			//Direct3D�C���^�[�t�F�[�X�̉��
		m_pVtxBuff = NULL;
	}

	if( m_pIdxBuff != NULL )
	{
		m_pIdxBuff -> Release();			//Direct3D�C���^�[�t�F�[�X�̉��
		m_pIdxBuff = NULL;
	}
}

//*************************************************************************************************
// �X�V����
//*************************************************************************************************
void CMeshField2::Update(void)
{

}

//*************************************************************************************************
// �`�揈��
//*************************************************************************************************
void CMeshField2::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX mtxWorld;

	pDevice->SetStreamSource(
		0,						//�X�g���[���ԍ�(�p�C�v�ԍ�)
		m_pVtxBuff,		//�X�g���[���̌��ɂȂ钸�_�o�b�t�@
		0,						//�I�t�Z�b�g(�o�C�g)
		sizeof(VERTEX_3D));		//��̒��_�f�[�^�̃T�C�Y(�X�g���C�h��)

	//�f�o�C�X�ɃC���f�b�N�X�o�b�t�@�̐ݒ�
	pDevice->SetIndices(m_pIdxBuff);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice -> SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0,m_pTexture);

	//���[���h���W�ϊ��s��
	D3DXMatrixIdentity(&mtxWorld);			//���[���h���W�ϊ��s���������

	pDevice->SetTransform(D3DTS_WORLD,&mtxWorld);		//���[���h���W�ϊ��s����ړ��ł���悤�ɂ���(?)

	//�C���f�b�N�X�o�b�t�@�ɂ��`��
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,		//�`��`��(�g���C�A���O���X�g���b�v)
		0,							//�ŏ��̒��_�C���f�b�N�X�܂ł̃I�t�Z�b�g
		0,							//�ŏ��̒��_�C���f�b�N�X
		(MAX_FIELD_BLOCK_X + 1) * (MAX_FIELD_BLOCK_Z + 1),//���_��
		0,							//�X�^�[�g�C���f�b�N�X
		NUM_POLYGON * MAX_FIELD_BLOCK_X * MAX_FIELD_BLOCK_Z + (MAX_FIELD_BLOCK_Z - 1) * 4) ;						//�v���~�e�B�u��
}

//*************************************************************************************************
// ���_���̐ݒ�
//*************************************************************************************************
HRESULT CMeshField2::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_3D* pVtx;

	//���_�o�b�t�@
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * (MAX_FIELD_BLOCK_X + 1) * (MAX_FIELD_BLOCK_Z + 1),		//�쐬���������_�o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,							//�g�p���@
		FVF_VERTEX_3D,								//
		D3DPOOL_MANAGED,							//�������Ǘ����@(�f�o�C�X�ɂ��C��)
		&m_pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	//���_���̐ݒ�///////////////////////////////////////////////////////////////////////////////
	m_pVtxBuff->Lock(0,0,(void**)&pVtx,0);	//���b�N

	//���_���W(�����珇)
	for(int height = 0;height < (MAX_FIELD_BLOCK_Z + 1);height++)
	{
		for(int width = 0;width < (MAX_FIELD_BLOCK_X + 1);width++)
		{
			pVtx[width + height * (MAX_FIELD_BLOCK_X + 1)].pos = D3DXVECTOR3(FIELD_POS_X + FIELD_WIDTH * width,-10.0f,FIELD_POS_Z - FIELD_HEIGHT * height + 15);
			pVtx[width + height * (MAX_FIELD_BLOCK_X + 1)].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[width + height * (MAX_FIELD_BLOCK_X + 1)].color = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[width + height * (MAX_FIELD_BLOCK_X + 1)].tex = D3DXVECTOR2(1.0f * width,1.0f * height);
		}
	}

	m_pVtxBuff->Unlock();	//�A�����b�N

	return 0;
}

//*************************************************************************************************
// ���_�C���f�b�N�X�̐ݒ�
//*************************************************************************************************
HRESULT CMeshField2::MakeIndex(LPDIRECT3DDEVICE9 pDevice)
{
	WORD* pIdx;

	//�C���f�b�N�X�o�b�t�@
	if(FAILED(pDevice->CreateIndexBuffer(
		sizeof(WORD) * ((MAX_FIELD_BLOCK_X + 1) * 2 * MAX_FIELD_BLOCK_Z + (MAX_FIELD_BLOCK_Z - 1) * 2),	//
		D3DUSAGE_WRITEONLY,							//�g�p�p�r�t���O
		D3DFMT_INDEX16,								//�C���f�b�N�X�f�[�^�̃t�H�[�}�b�g
		D3DPOOL_MANAGED,							//�������̊Ǘ����@�i���C���j
		&m_pIdxBuff,							//�C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�|�C���^�̃A�h���X
		NULL)));

	//���_�C���f�b�N�X�̐ݒ�////////////////////////////////////////////////////////////////////////
	m_pIdxBuff->Lock(0,0,(void**)&pIdx,0);	//���b�N
	//N���^�ɐ���
	//�c�̃��b�V������
	for(int height = 0;height < MAX_FIELD_BLOCK_Z;height++)
	{
		//���̃��b�V������
		for(int width = 0;width < MAX_FIELD_BLOCK_X + 1;width++)
		{
			//�k�ރ|���S���̌v�Z
			if(width == 0 && height != 0)
			{
				pIdx[(height * (MAX_FIELD_BLOCK_X + 2) * 2) - 2] = MAX_FIELD_BLOCK_X + (height - 1) * (MAX_FIELD_BLOCK_X + 1);
				pIdx[(height * (MAX_FIELD_BLOCK_X + 2) * 2) - 1] = (MAX_FIELD_BLOCK_X + 1) * 2 + (height - 1) * (MAX_FIELD_BLOCK_X + 1);
			}
			pIdx[(width * 2) + (height * (MAX_FIELD_BLOCK_X + 2) * 2)] = width + height * (MAX_FIELD_BLOCK_X + 1) + (MAX_FIELD_BLOCK_X + 1);
			pIdx[(width * 2 + 1) + (height * (MAX_FIELD_BLOCK_X + 2) * 2)] = width + height * (MAX_FIELD_BLOCK_X + 1);
		}
	}

	m_pIdxBuff->Unlock();	//�A�����b�N
	return 0;
}

//*************************************************************************************************
// �C���X�^���X����
//*************************************************************************************************
CMeshField2 *CMeshField2::Create(void)
{
    CMeshField2 *meshfield;

    meshfield = new CMeshField2(PRIOLITY_0);

    meshfield->Init();

    return meshfield;
}
