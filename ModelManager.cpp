//=============================================================================
//
// ���f���}�l�[�W���[ [ModelManager.cpp]
// Author : TAKUYA EBIHARA
//
//=============================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "ModelManager.h"

using namespace std;

typedef map<string, MODEL_INFO> ModelTable;

CModelManager::CModelManager()
{

}

//*************************************************************************************************
// ���f�������[�h����
// ����  �F key     ���f���̃p�X�y�сA�����L�[
// �߂�l�F true    ���� / false �s����
//*************************************************************************************************
bool CModelManager::Load(string key)
{
    LPDIRECT3DDEVICE9 pDevice;

    //�f�o�C�X�̎擾
    pDevice = CManager::GetRenderer()->GetDevice();

    // ���f��������
    ModelTable::const_iterator it = m_ModelInfo.find(key);
    if (it == m_ModelInfo.end()) {            // ������Ȃ������ꍇ
        HRESULT hr;
        MODEL_INFO modelInfo;

        hr = D3DXLoadMeshFromX(key.c_str(), D3DXMESH_MANAGED, pDevice, NULL, &modelInfo.pBuff, NULL, &modelInfo.NumMat, &modelInfo.pMesh);
        if (FAILED(hr)) {
            MessageBox(NULL, key.c_str(), "LOAD ERROR!", MB_OK);
            return false;
        }

        //�}�e���A�����擾
        modelInfo.pMat = (D3DXMATERIAL*)modelInfo.pBuff->GetBufferPointer();

        //�e�N�X�`�������邩�H
        if (modelInfo.pMat[0].pTextureFilename != NULL)
        {
            for (int i = 0; i < (int)modelInfo.NumMat; i++)
            {
                //�n�[�h�f�B�X�N����e�N�X�`����ǂݍ���
                if (FAILED(D3DXCreateTextureFromFile(pDevice, modelInfo.pMat[i].pTextureFilename, &modelInfo.pMatTex[i])))
                {
                    //�G���[��
                    MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", "�I�����b�Z�[�W", MB_OK);
                }
            }
        }

        // �}�b�v�֑}������
        m_ModelInfo.insert(ModelTable::value_type(key, modelInfo));
    }

    return true;
}

//*************************************************************************************************
// @brie �w�肵�����f�������炤
// �}�b�v���Ɏw�肵�����f��������΁Akey���g���ă��f�������[�h����B
// ���[�h�Ɏ��s�����NULL�|�C���^���Ԃ�B
// ����   �F key �e�N�X�`���̃p�X�y�сA�����L�[
// �߂�l �F ��������ƃe�N�X�`���̃|�C���^�B���s�����NULL�B
//*************************************************************************************************
MODEL_INFO CModelManager::getResource(string key)
{
    LPDIRECT3DDEVICE9 pDevice;

    //�f�o�C�X�̎擾
    pDevice = CManager::GetRenderer()->GetDevice();

    // �e�N�X�`��������
    ModelTable::const_iterator it = m_ModelInfo.find(key);
    if (it != m_ModelInfo.end()) {       // ���������ꍇ
                                      // �e�N�X�`����Ԃ�
        return it->second;
    }
    else {                            // ������Ȃ������ꍇ
                                      // ����������������L�[�Ƃ���
                                      // �V�����}�b�v�Ƀe�N�X�`����ǉ�����
        HRESULT hr;
        MODEL_INFO modelInfo;

        hr = D3DXLoadMeshFromX(key.c_str(), D3DXMESH_MANAGED, pDevice, NULL, &modelInfo.pBuff, NULL, &modelInfo.NumMat, &modelInfo.pMesh);
        if (FAILED(hr)) {
            // �����ł��Ȃ������ꍇ��NULL�|�C���^��Ԃ�
            MessageBox(NULL, key.c_str(), "LOAD ERROR!", MB_OK);
            return modelInfo;
        }

        //�}�e���A�����擾
        modelInfo.pMat = (D3DXMATERIAL*)modelInfo.pBuff->GetBufferPointer();

        //�e�N�X�`�������邩�H
        if (modelInfo.pMat[0].pTextureFilename != NULL)
        {
            for (int i = 0; i < (int)modelInfo.NumMat; i++)
            {
                //�n�[�h�f�B�X�N����e�N�X�`����ǂݍ���
                if (FAILED(D3DXCreateTextureFromFile(pDevice, modelInfo.pMat[i].pTextureFilename, &modelInfo.pMatTex[i])))
                {
                    //�G���[��
                    MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", "�I�����b�Z�[�W", MB_OK);
                }
            }
        }

        // �}�b�v�֑}������
        m_ModelInfo.insert(ModelTable::value_type(key, modelInfo));

        return modelInfo;
    }
}