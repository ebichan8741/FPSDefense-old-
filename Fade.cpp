//================================================================================================
//
// �t�F�[�h���� [Fade.cpp]
// Author : TAKUYA EBIHARA
//
//================================================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "Fade.h"

//=================================================================================================
// �萔��`
//=================================================================================================
#define FADE_TEXTURE	"data/TEXTURE/fade.jpg"
#define FADE_RATE		(0.03f)		//�t�F�[�h�W��

//=================================================================================================
// �ÓI�����o�ϐ�
//=================================================================================================
FADE CFade::m_fade = FADE_NONE;
CMode *CFade::m_nextMode = 0;

//*************************************************************************************************
// ����������
//*************************************************************************************************
HRESULT CFade::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	m_colorFade = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	//���_�̍쐬
	if (FAILED(MakeVertex(pDevice)))
	{
		return E_FAIL;
	}

	//�n�[�h�f�B�X�N����e�N�X�`����ǂݍ���
	if (FAILED(D3DXCreateTextureFromFile(pDevice, FADE_TEXTURE, &m_pTexture)))
	{
		//�G���[��
		MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", "�I�����b�Z�[�W", MB_OK);
	}

	return 0;
}

//*************************************************************************************************
// �I������
//*************************************************************************************************
void CFade::Uninit(void)
{
	if (m_pTexture != NULL)
	{	
		m_pTexture->Release();
		m_pTexture = NULL;
	}	
		
	if (m_pVtxBuff != NULL)
	{	
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//*************************************************************************************************
// �X�V����
//*************************************************************************************************
void CFade::Update(void)
{
	//�t�F�[�h�Ȃ�
	if (m_fade == FADE_NONE)
	{
		return;
	}

	//�t�F�[�h�C��
	if (m_fade == FADE_IN)
	{
		m_colorFade.a -= FADE_RATE;
		if (m_colorFade.a <= 0.0f)
		{
			m_colorFade.a = 0.0f;
			m_fade = FADE_NONE;
		}
	}

	//�t�F�[�h�A�E�g
	else if (m_fade == FADE_OUT)
	{
		m_colorFade.a += FADE_RATE;
		if (m_colorFade.a >= 1.0f)
		{
			m_colorFade.a = 1.0f;
			m_fade = FADE_IN;
			CManager::SetMode(m_nextMode);
		}
	}

	SetColorFade(m_colorFade);
}

//*************************************************************************************************
// �`�揈��
//*************************************************************************************************
void CFade::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�X�g���[���̐ݒ�
	pDevice->SetStreamSource(0,		//�X�g���[���ԍ�(�p�C�v�ԍ�)
		m_pVtxBuff,				    //�X�g���[���̌��ɂȂ钸�_�o�b�t�@
		0,							//�I�t�Z�b�g(�o�C�g)
		sizeof(VERTEX_2D));			//��̒��_�f�[�^�̃T�C�Y(�X�g���C�h��)

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	//�v���~�e�B�u�̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
		0,										//�I�t�Z�b�g(���_��)
		NUM_POLYGON);							//�v���~�e�B�u�̐�
}


//*************************************************************************************************
// ���_�̍쐬(�t�F�[�h)
//*************************************************************************************************
HRESULT CFade::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D* pVtx; //���z�A�h���X���擾���邽�߂̃|�C���^

	// ���_�o�b�t�@
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,				//�쐬���������_�o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,							//�g�p���@
		FVF_VERTEX_2D,								//
		D3DPOOL_MANAGED,							//�������Ǘ����@(�f�o�C�X�ɂ��C��)
		&m_pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	// ���z�A�h���X���擾����
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f + SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f + SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f + SCREEN_WIDTH, 0.0f + SCREEN_HEIGHT, 0.0f);

	//rhw�̐ݒ�(�K��1.0f)
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�(0.0f�̐����l)
	pVtx[0].color = m_colorFade;
	pVtx[1].color = m_colorFade;
	pVtx[2].color = m_colorFade;
	pVtx[3].color = m_colorFade;

	//�e�N�X�`���̒��_���W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	m_pVtxBuff->Unlock();		//���b�N����

	return 0;
}

//*************************************************************************************************
// �t�F�[�h�F�̐ݒ�
//*************************************************************************************************
void CFade::SetColorFade(D3DXCOLOR color)
{
	VERTEX_2D* pVtx = NULL;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_�J���[�̐ݒ�(0.0f�̐����l)
	pVtx[0].color = color;
	pVtx[1].color = color;
	pVtx[2].color = color;
	pVtx[3].color = color;

	m_pVtxBuff->Unlock();
}

//*************************************************************************************************
// �t�F�[�h�̗\��
//*************************************************************************************************
void CFade::SetFade(FADE fade,CMode *nextMode)
{
	m_fade = fade;
	m_nextMode = nextMode;
}

//*************************************************************************************************
// 
//*************************************************************************************************
FADE CFade::GetFade(void)
{
	return m_fade;
}
