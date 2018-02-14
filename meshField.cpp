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
#include "meshfield.h"

//=================================================================================================
// �萔��`
//=================================================================================================
#define FIELD_TEXTURENAME "data/TEXTURE/field004.jpg"


//*************************************************************************************************
// �R���X�g���N�^
//*************************************************************************************************
CMeshField::CMeshField(int nPriolity):CScene(nPriolity)
{
	m_CheckPoint[0] = D3DXVECTOR3(-100.0f, 0.0f, -130.0f);
	m_CheckPoint[1] = D3DXVECTOR3(-100.0f, 0.0f, 130.0f);
	m_CheckPoint[2] = D3DXVECTOR3(100.0f, 0.0f, 130.0f);
	m_CheckPoint[3] = D3DXVECTOR3(100.0f, 0.0f, -130.0f);
}

//*************************************************************************************************
// ����������
//*************************************************************************************************
HRESULT CMeshField::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    LoadField();

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
	if(FAILED(D3DXCreateTextureFromFile(pDevice,FIELD_TEXTURENAME,&m_pTexture)))
	{
		//�G���[��
		MessageBox(NULL,"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���","�I�����b�Z�[�W",MB_OK);
	}

	return 0;
}

//*************************************************************************************************
// �I������
//*************************************************************************************************
void CMeshField::Uninit(void)
{
	if( m_pTexture != NULL ){
		m_pTexture -> Release();		//Direct3D�f�o�C�X�̉��
		m_pTexture = NULL;
	}

	if( m_pVtxBuff != NULL ){
		m_pVtxBuff -> Release();			//Direct3D�C���^�[�t�F�[�X�̉��
		m_pVtxBuff = NULL;
	}

	if( m_pIdxBuff != NULL ){
		m_pIdxBuff -> Release();			//Direct3D�C���^�[�t�F�[�X�̉��
		m_pIdxBuff = NULL;
	}
}

//*************************************************************************************************
// �X�V����
//*************************************************************************************************
void CMeshField::Update(void)
{
}

//*************************************************************************************************
// �`�揈��
//*************************************************************************************************
void CMeshField::Draw(void)
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
HRESULT CMeshField::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_3D*  pVtx;
    D3DXVECTOR3 normal;
    D3DXVECTOR3 vNormalX;
    D3DXVECTOR3 vNormalZ;
    D3DXVECTOR3 vDir,vDirZ;

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
	for(int nCntH = 0;nCntH < (MAX_FIELD_BLOCK_Z + 1);nCntH++)
	{
		for(int nCntW = 0;nCntW < (MAX_FIELD_BLOCK_X + 1);nCntW++)
		{
			pVtx[nCntW + (nCntH * (MAX_FIELD_BLOCK_X + 1))].pos = D3DXVECTOR3(FIELD_POS_X + FIELD_WIDTH * nCntW,
                                                                            m_Field[nCntH][nCntW],
                                                                            FIELD_POS_Z - FIELD_HEIGHT * nCntH);
		}
	}

    //�@���E�J���[�E�e�N�X�`�����W
    for(int nCntH = 0;nCntH < (MAX_FIELD_BLOCK_Z + 1);nCntH++)
	{
		for(int nCntW = 0;nCntW < (MAX_FIELD_BLOCK_X + 1);nCntW++)
		{
            if (nCntW != 0) {
                vDir = pVtx[nCntW + (nCntH * (MAX_FIELD_BLOCK_X + 1) + 1)].pos -
                    pVtx[nCntW + (nCntH * (MAX_FIELD_BLOCK_X + 1) - 1)].pos;
            }
            vNormalX.x = -vDir.y;
            vNormalX.y = vDir.x;
            vNormalX.z = 0.0f;

            if (nCntH != 0) {
                vDir = pVtx[nCntW + (nCntH * (MAX_FIELD_BLOCK_X + 1)) - (MAX_FIELD_BLOCK_X + 1)].pos -
                    pVtx[nCntW + (nCntH * (MAX_FIELD_BLOCK_X + 1)) + (MAX_FIELD_BLOCK_X + 1)].pos;
            }
            vNormalZ.x = 0.0f;
            vNormalZ.y = vDir.z;
            vNormalZ.z = -vDir.y;

            normal = vNormalX + vNormalZ;

            if(nCntH == 0 || nCntH == MAX_FIELD_BLOCK_Z)
            {
                normal = D3DXVECTOR3(0.0f,1.0f,0.0f);
            }
            if(nCntW == 0 || nCntW == MAX_FIELD_BLOCK_X)
            {
                normal = D3DXVECTOR3(0.0f,1.0f,0.0f);
            }
            D3DXVec3Normalize (&normal,&normal);

			pVtx[nCntW + (nCntH * (MAX_FIELD_BLOCK_X + 1))].normal  = normal;
			pVtx[nCntW + (nCntH * (MAX_FIELD_BLOCK_X + 1))].color   = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[nCntW + (nCntH * (MAX_FIELD_BLOCK_X + 1))].tex     = D3DXVECTOR2(1.0f * nCntW,1.0f * nCntH);
        }
    }
	m_pVtxBuff->Unlock();	//�A�����b�N

	return 0;
}

//*************************************************************************************************
// ���_�C���f�b�N�X�̐ݒ�
//*************************************************************************************************
HRESULT CMeshField::MakeIndex(LPDIRECT3DDEVICE9 pDevice)
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
// �t�B�[���h��Y���W�擾
//*************************************************************************************************
float CMeshField::GetHeight(D3DXVECTOR3 pos)
{
    VERTEX_3D*  pVtx;
    D3DXVECTOR3 p0,p1,p2;
    D3DXVECTOR3 v01,v02,v12,v0p,v1p,v2p,vc0,vc1,vc2,vn;

    m_pVtxBuff->Lock(0,0,(void**)&pVtx,0);	//���b�N

    for(int nCntH = 0;nCntH < (MAX_FIELD_BLOCK_Z + 1);nCntH++)
	{
		for(int nCntW = 0;nCntW < (MAX_FIELD_BLOCK_X + 1);nCntW++)
		{
            //���̎O�p�`�|���S��
            //���㒸�_
            p0 = pVtx[nCntW + (nCntH * (MAX_FIELD_BLOCK_X + 1))].pos;
            //�������_
            p1 = pVtx[nCntW + (nCntH * (MAX_FIELD_BLOCK_X + 1)) + (MAX_FIELD_BLOCK_X + 1)].pos;
            //�E�����_
            p2 = pVtx[nCntW + (nCntH * (MAX_FIELD_BLOCK_X + 1)) + (MAX_FIELD_BLOCK_X + 1) + 1].pos;

            //�x�N�g������
            v01 = p1 - p0;
            v0p = pos - p0;

            //�@���v�Z
            D3DXVec3Cross(&vc0,&v0p,&v01);

            if(vc0.y >= 0.0f)
            {
                v12 = p2 - p1;
                v1p = pos - p1;
                D3DXVec3Cross(&vc1,&v1p,&v12);
                if(vc1.y >= 0.0f)
                {
                    v02 = p2 - p0;
                    v2p = pos - p2;
                    D3DXVec3Cross(&vc2,&v02,&v2p);
                    if(vc2.y >= 0.0f)
                    {
                        v02 = p2 - p0;
                        D3DXVec3Cross(&vn,&v01,&v02);
                        D3DXVec3Normalize(&vn,&vn);

                        pos.y = p0.y - ((pos.x - p0.x) * vn.x + (pos.z - p0.z) * vn.z) / vn.y;

                        return pos.y;
                    }
                }
            }
            //�E�̎O�p�`�|���S��
            p0 = pVtx[nCntW + (nCntH * (MAX_FIELD_BLOCK_X + 1))].pos;

            p1 = pVtx[nCntW + (nCntH * (MAX_FIELD_BLOCK_X + 1)) + 1].pos;

            p2 = pVtx[nCntW + (nCntH * (MAX_FIELD_BLOCK_X + 1)) + (MAX_FIELD_BLOCK_X + 1) + 1].pos;

            v01 = p1 - p0;
            v0p = pos - p0;
            D3DXVec3Cross(&vc0,&v01,&v0p);
            if(vc0.y >= 0.0f)
            {
                v12 = p2 - p1;
                v1p = pos - p1;
                D3DXVec3Cross(&vc1,&v12,&v1p);
                if(vc1.y >= 0.0f)
                {
                    v02 = p2 - p0;
                    v2p = pos - p2;
                    D3DXVec3Cross(&vc2,&v2p,&v02);
                    if(vc2.y >= 0.0f)
                {
                        v02 = p2 - p0;
                        D3DXVec3Cross(&vn,&v01,&v02);
                        D3DXVec3Normalize(&vn,&vn);

                        pos.y = p0.y - ((pos.x - p0.x) * vn.x + (pos.z - p0.z) * vn.z) / vn.y;

                        return pos.y;
                    }
                }
            }
        }
    }

    m_pVtxBuff->Unlock();	//�A�����b�N

    return 0;
}

//*************************************************************************************************
// �C���X�^���X����
//*************************************************************************************************
CMeshField *CMeshField::Create(void)
{
    CMeshField *meshfield;

    meshfield = new CMeshField(PRIOLITY_0);

    meshfield->Init();

    return meshfield;
}

//*************************************************************************************************
// �t�B�[���h�f�[�^�ǂݍ���
//*************************************************************************************************
void CMeshField::LoadField(void)
{
    FILE *fp;
    char buf[256];
    int c;
    int col = 1;
    int row = 0;

    memset(buf, 0, sizeof(buf));
    fp = fopen("data/CSV/fielddata.csv", "r");

    while (1) {

        while (1) {

            c = fgetc(fp);

            //�����Ȃ烋�[�v�𔲂���B
            if (c == EOF)
                return ;

            //�J���}�����s�łȂ���΁A�����Ƃ��ĂȂ���
            if (c != ',' && c != '\n')
                strcat(buf, (const char*)&c);
            //�J���}�����s�Ȃ烋�[�v������B
            else
                break;
        }
        //�����ɗ����Ƃ������Ƃ́A1�Z�����̕����񂪏o���オ�����Ƃ�������
        m_Field[row][col] = atoi(buf);

        //�o�b�t�@��������
        memset(buf, 0, sizeof(buf));
        //�񐔂𑫂�
        ++col;

        //�����ǂݍ��񂾕��������s��������񐔂����������čs���𑝂₷
        if (c == '\n') {
            col = 1;
            ++row;
        }
    }
    fclose(fp);
}