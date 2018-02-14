//=================================================================================================
//
// ���b�V���t�B�[���h [meshfield.h]
// Author : TAKUYA EBIHARA
//
//=================================================================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "scene.h"

#define MAX_FIELD_BLOCK_X	(40)			//X�����̃u���b�N�̐�
#define MAX_FIELD_BLOCK_Z	(50)			//Z�����̃u���b�N�̐�
#define FIELD_WIDTH			(15.0f)			//�u���b�N�̉���
#define FIELD_HEIGHT		(15.0f)			//�u���b�N�̏c��
#define FIELD_POS_X			(-(FIELD_WIDTH * MAX_FIELD_BLOCK_X) / 2)	//�t�B�[���h�̊J�n���WX
#define FIELD_POS_Z			((FIELD_HEIGHT * MAX_FIELD_BLOCK_Z) / 2)	//�t�B�[���h�̊J�n���WZ
#define CHECK_POINT_CNT		(4)				// �`�F�b�N�|�C���g�̐�

//=================================================================================================
// �N���X�錾
//=================================================================================================
class CMeshField:public CScene
{
public:
    CMeshField(int nPriolity);
    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    HRESULT MakeVertex(LPDIRECT3DDEVICE9 pDevice);
    HRESULT MakeIndex(LPDIRECT3DDEVICE9 pDevice);

    static CMeshField *Create(void);
    float GetHeight(D3DXVECTOR3 pos);

    void LoadField(void);

	D3DXVECTOR3 m_CheckPoint[CHECK_POINT_CNT];

private:
    LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;
    float m_Field[(MAX_FIELD_BLOCK_Z + 1)][(MAX_FIELD_BLOCK_X + 1)];
};
#endif