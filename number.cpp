//=============================================================================
//
// �����ݒ菈�� [number.cpp]
// Author : TAKUYA EBIHARA
//
//=============================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "number.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUMBER_TEXTURENAME "data/TEXTURE/number002.png"

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 CNumber::m_pTexture = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CNumber::CNumber()
{
    m_vTexPos = D3DXVECTOR2(0.0f,0.0f);
    m_vTexSize = D3DXVECTOR2(0.1f,0.5f);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CNumber::~CNumber()
{

}


//=============================================================================
// ����������
//=============================================================================
HRESULT CNumber::Init(void)
{
    //�f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // ���_����ݒ�
    if(FAILED(MakeVertexNumber(pDevice)))
    {
        //�G���[��
        MessageBox(NULL,"���_���̐ݒ�Ɏ��s���܂���","�I�����b�Z�[�W",MB_OK);
    }

    return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CNumber::Uninit(void)
{
    // ���_�o�b�t�@�̔j��
    if( m_pVtxBuff != NULL )
    {
        m_pVtxBuff -> Release();
        m_pVtxBuff = NULL;
    }

    // �e�N�X�`���̔j��
    if( m_pTexture != NULL)
    {
        m_pTexture -> Release();
        m_pTexture = NULL;
    }
}

//=============================================================================
// �X�V����
//=============================================================================
void CNumber::Update(void)
{
    VERTEX_2D* pVtx; //���z�A�h���X���擾���邽�߂̃|�C���^

    //���z�A�h���X���擾����
    m_pVtxBuff->Lock(0,0,(void**)&pVtx,0);

    //�����̐ݒ�
    //CNumber::SetNumber(nNum);

    //���_�̐ݒ�
    CNumber::SetVertex(pVtx,0);

    m_pVtxBuff->Unlock();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CNumber::Draw(void)
{
    LPDIRECT3DDEVICE9 pDevice;
    pDevice = CManager::GetRenderer()->GetDevice();


    // ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
    pDevice->SetStreamSource(0,         //�X�g���[���ԍ�(�p�C�v�ԍ�)
        m_pVtxBuff,                     //�X�g���[���̌��ɂȂ钸�_�o�b�t�@
        0,                              //�I�t�Z�b�g(�o�C�g)
        sizeof(VERTEX_2D));             //��̒��_�f�[�^�̃T�C�Y(�X�g���C�h��)

    // ���_�t�H�[�}�b�g�̐ݒ�
    pDevice->SetFVF(FVF_VERTEX_2D);

    // �e�N�X�`���̐ݒ�
    pDevice->SetTexture(0,m_pTexture);

    //�v���~�e�B�u�̕`��
    pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,    //�v���~�e�B�u�̎��
        0,                                        //�I�t�Z�b�g(���_��)
        NUM_POLYGON);                            //�v���~�e�B�u�̐�
}

//=============================================================================
// ���_���̐ݒ�
//=============================================================================
HRESULT CNumber::MakeVertexNumber(LPDIRECT3DDEVICE9 pDevice)
{
    VERTEX_2D* pVtx; //���z�A�h���X���擾���邽�߂̃|�C���^

    // ���_�o�b�t�@�̐���
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,                //�쐬���������_�o�b�t�@�̃T�C�Y
        D3DUSAGE_WRITEONLY,                            //�g�p���@
        FVF_VERTEX_2D,                                 //
        D3DPOOL_MANAGED,                               //�������Ǘ����@(�f�o�C�X�ɂ��C��)
        &m_pVtxBuff,
        NULL)))
    {
        return E_FAIL;
    }

    //���z�A�h���X���擾����
    m_pVtxBuff->Lock(0,0,(void**)&pVtx,0);

    //���_���W�̐ݒ�(�QD���W�E�E���)
    pVtx[0].pos = D3DXVECTOR3(m_vPos.x,m_vPos.y,0.0f);
    pVtx[1].pos = D3DXVECTOR3(m_vPos.x + m_vSize.x,m_vPos.y,0.0f);
    pVtx[2].pos = D3DXVECTOR3(m_vPos.x,m_vPos.y + m_vSize.y,0.0f);
    pVtx[3].pos = D3DXVECTOR3(m_vPos.x + m_vSize.x,m_vPos.y + m_vSize.y,0.0f);

    //�e�N�X�`���̒��_���W
    pVtx[0].tex = D3DXVECTOR2(m_vTexPos.x,m_vTexPos.y);
    pVtx[1].tex = D3DXVECTOR2(m_vTexPos.x + m_vTexSize.x,m_vTexPos.y);
    pVtx[2].tex = D3DXVECTOR2(m_vTexPos.x,m_vTexPos.y + m_vTexSize.y);
    pVtx[3].tex = D3DXVECTOR2(m_vTexPos.x + m_vTexSize.x,m_vTexPos.y + m_vTexSize.y);

    //rhw�̐ݒ�(�K��1.0f)
    pVtx[0].rhw = 1.0f;
    pVtx[1].rhw = 1.0f;
    pVtx[2].rhw = 1.0f;
    pVtx[3].rhw = 1.0f;

    //���_�J���[�̐ݒ�(0�`255�̐����l)
    pVtx[0].color = D3DCOLOR_RGBA(255,255,255,255);
    pVtx[1].color = D3DCOLOR_RGBA(255,255,255,255);
    pVtx[2].color = D3DCOLOR_RGBA(255,255,255,255);
    pVtx[3].color = D3DCOLOR_RGBA(255,255,255,255);

    m_pVtxBuff->Unlock();

    return 0;
}

//=============================================================================
// ���_���X�V����
//=============================================================================
void CNumber::SetVertex(VERTEX_2D* pVtx,int i)
{
    //���_���W�̐ݒ�(�QD���W�E�E���)
    pVtx[i * NUM_VERTEX].pos     = D3DXVECTOR3(m_vPos.x,m_vPos.y,0.0f);
    pVtx[i * NUM_VERTEX + 1].pos = D3DXVECTOR3(m_vPos.x + m_vSize.x,m_vPos.y,0.0f);
    pVtx[i * NUM_VERTEX + 2].pos = D3DXVECTOR3(m_vPos.x,m_vPos.y + m_vSize.y,0.0f);
    pVtx[i * NUM_VERTEX + 3].pos = D3DXVECTOR3(m_vPos.x + m_vSize.x,m_vPos.y + m_vSize.y,0.0f);

    //�e�N�X�`���̒��_���W
    pVtx[i * NUM_VERTEX + 0].tex = D3DXVECTOR2(m_vTexPos.x,              m_vTexPos.y);
    pVtx[i * NUM_VERTEX + 1].tex = D3DXVECTOR2(m_vTexPos.x + m_vTexSize.x,m_vTexPos.y);
    pVtx[i * NUM_VERTEX + 2].tex = D3DXVECTOR2(m_vTexPos.x,              m_vTexPos.y + m_vTexSize.y);
    pVtx[i * NUM_VERTEX + 3].tex = D3DXVECTOR2(m_vTexPos.x + m_vTexSize.x,m_vTexPos.y + m_vTexSize.y);

    //rhw�̐ݒ�(�K��1.0f)
    pVtx[i * NUM_VERTEX + 0].rhw = 1.0f;
    pVtx[i * NUM_VERTEX + 1].rhw = 1.0f;
    pVtx[i * NUM_VERTEX + 2].rhw = 1.0f;
    pVtx[i * NUM_VERTEX + 3].rhw = 1.0f;

    //���_�J���[�̐ݒ�(0�`255�̐����l)
    pVtx[i * NUM_VERTEX + 0].color = D3DCOLOR_RGBA(255,255,255,255);
    pVtx[i * NUM_VERTEX + 1].color = D3DCOLOR_RGBA(255,255,255,255);
    pVtx[i * NUM_VERTEX + 2].color = D3DCOLOR_RGBA(255,255,255,255);
    pVtx[i * NUM_VERTEX + 3].color = D3DCOLOR_RGBA(255,255,255,255);
}

//=============================================================================
// ��������
//=============================================================================
CNumber *CNumber::Create(D3DXVECTOR2 pos,D3DXVECTOR2 size)
{
    CNumber *pNumber;

    //�������̓��I�m��
    pNumber = new CNumber;
    pNumber->Init();

    pNumber->m_vPos = pos;
    pNumber->m_vSize = size;

    return pNumber;
}

//=============================================================================
// �e�N�X�`���̓ǂݍ���
//=============================================================================
HRESULT CNumber::Load(void)
{
    //�f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    if(m_pTexture == NULL)
    {
        //�e�N�X�`���̓ǂݍ���
        if(FAILED(D3DXCreateTextureFromFile(pDevice,NUMBER_TEXTURENAME,&m_pTexture)))
        {
            //�G���[��
            MessageBox(NULL,"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���","�I�����b�Z�[�W",MB_OK);
        }
    }
    return S_OK;
}


//=============================================================================
// �e�N�X�`���|�C���^�̔j��
//=============================================================================
void CNumber::Unload(void)
{
    //�e�N�X�`���̔j��
    if(m_pTexture != NULL)
    {
        m_pTexture -> Release();
        m_pTexture = NULL;
    }
}

//=============================================================================
// �l�ݒ�
//=============================================================================
void CNumber::SetNumber(int nNumber,float y)
{
    m_vTexPos.x = nNumber * 0.1f;
	m_vTexPos.y = y;
}

void CNumber::SetNumber(float fNumber, float y)
{
    m_vTexPos.x = fNumber * 0.1f;
    m_vTexPos.y = y;
}