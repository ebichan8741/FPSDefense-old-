//=================================================================================================
//
// �}�l�[�W���[���� [maneger.cpp]
// Author : TAKUYA EBIHARA
//
//=================================================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "camera.h"
#include "light.h"
#include "input.h"
#include "Mode.h"
#include "TitleMode.h"
#include "GameMode.h"
#include "Fade.h"
#include "sound.h"
#include "TextureManager.h"
#include "ModelManager.h"


//=================================================================================================
// �萔��`
//=================================================================================================

//=================================================================================================
// �ÓI�����o�ϐ�
//=================================================================================================
CRenderer *CManager::m_pRenderer = 0;
CInputKeyboard *CManager::m_pInputKeyboard = 0;
CInputMouse *CManager::m_pInputMouse = 0;
CCamera *CManager::m_pCamera = 0;
CMode *CManager::m_Mode = new CTitleMode;
CMode *CManager::m_nextMode = m_Mode;
CFade *CManager::m_Fade = 0;
CSound *CManager::m_Sound = 0;
MODE_TYPE CManager::mode = MODE_GAME;
CTextureManager *CManager::m_TextureManager = 0;
CModelManager *CManager::m_ModelManager = 0;


//=================================================================================================
// �O���[�o���ϐ�
//=================================================================================================
CLight *g_pLight = NULL;

//*************************************************************************************************
// �R���X�g���N�^
//*************************************************************************************************
CManager::CManager()
{
}

//*************************************************************************************************
// �f�X�g���N�^
//*************************************************************************************************
CManager::~CManager()
{
}

//*************************************************************************************************
// ����������
//*************************************************************************************************
HRESULT CManager::Init(HINSTANCE hInstance,HWND hWnd,BOOL bWindow)
{
    //�����_���[�̐���
    m_pRenderer = new CRenderer;

    //�����_���[������
    if(FAILED(m_pRenderer->Init(hWnd,true)))
    {
        return -1;
    }

#ifdef _DEBUG
	CDebug::Init();
#endif

    //�J�����̃C���X�^���X�����E����������
    m_pCamera = new CCamera;
    m_pCamera->Init();

    //���C�g�̃C���X�^���X�����E����������
    g_pLight = new CLight;
    g_pLight->SetLight();

    //�L�[�{�[�h�̃C���X�^���X����
    m_pInputKeyboard = new CInputKeyboard;
    m_pInputKeyboard->Init(hInstance,hWnd);

    //�}�E�X�̃C���X�^���X����
    m_pInputMouse = new CInputMouse;
    m_pInputMouse->Init(hInstance,hWnd);

	//�t�F�[�h�̃C���X�^���X����
	m_Fade = new CFade;
	m_Fade->Init();

    // �T�E���h�̐���
    m_Sound = new CSound;
    m_Sound->Init(hWnd);

    m_TextureManager = new CTextureManager;
    m_ModelManager = new CModelManager;

	m_Mode->Init();
    return S_OK;
}

//*************************************************************************************************
// �I������
//*************************************************************************************************
void CManager::Uninit(void)
{
#ifdef _DEBUG
	CDebug::Uninit();
#endif

	// ���[�h�̔j��
	if (m_Mode != NULL)
	{
		m_Mode->Uninit();
		m_Mode = NULL;
	}

	// �t�F�[�h�̔j��
	if (m_Fade != NULL)
	{
		m_Fade->Uninit();
		m_Fade = NULL;
	}

    // �T�E���h�̔j��
    if (m_Sound != NULL)
    {
        m_Sound->Uninit();
        m_Sound = NULL;
    }

    //�����_���[�̔j��
    if(m_pRenderer != NULL)
    {
        m_pRenderer->Uninit();
        m_pRenderer = NULL;
    }

    //�L�[�{�[�h�̔j��
    if(m_pInputKeyboard != NULL)
    {
        m_pInputKeyboard->Uninit();
        m_pInputKeyboard = NULL;
    }

    //�}�E�X�̔j��
    if(m_pInputMouse != NULL)
    {
        m_pInputMouse->Uninit();
        m_pInputMouse = NULL;
    }

    //�J�����̔j��
    if(m_pCamera != NULL)
    {
        m_pCamera->Uninit();
        m_pCamera = NULL;
    }
}

//*************************************************************************************************
// �X�V����
//*************************************************************************************************
void CManager::Update(void)
{
#ifdef _DEBUG
    char text[32];
    CDebug::ClearText();
    sprintf(text,"FPS:%d\n",GetFPSCount());
    CDebug::AddText(text);
#endif

    m_pRenderer->Update();
    m_pInputKeyboard->Update();
    m_pInputMouse->Update();
    m_pCamera->Update();

	//�I�u�W�F�N�g�̍X�V
	CScene::UpdateAll();

	m_Mode->Update();

	if (m_Fade->GetFade() != FADE_NONE)
	{
		m_Fade->Update();
	}
}

//*************************************************************************************************
// �`�揈��
//*************************************************************************************************
void CManager::Draw(void)
{
    m_pRenderer->DrawBegin();

    //�J�����̐ݒ�
    m_pCamera->SetCamera();

    // �I�u�W�F�N�g�̕`�揈��
    CScene::DrawAll();

	m_Mode->Draw();

	//�����[�h�̗\�񂪂���Ă���ꍇ�����[�h�Ɉڍs
	if (m_Mode != m_nextMode)
	{
		if (m_Mode != NULL)
		{
			m_Mode->Uninit();
			delete m_Mode;
		}

		if (m_nextMode != NULL)
		{
			m_nextMode->Init();
		}

		m_Mode = m_nextMode;
	}

	if (m_Fade->GetFade() != FADE_NONE)
	{
		m_Fade->Draw();
	}

    //�f�o�b�O�\��
#ifdef _DEBUG
    CDebug::Draw();
#endif

    m_pRenderer->DrawEnd();
}

//*************************************************************************************************
// �����_���[�̃|�C���^�擾
//*************************************************************************************************
CRenderer *CManager::GetRenderer(void)
{
    return CManager::m_pRenderer;
}

//*************************************************************************************************
// �L�[�{�[�h�̃|�C���^�擾
//*************************************************************************************************
CInputKeyboard *CManager::GetInputKeyboard(void)
{
    return CManager::m_pInputKeyboard;
}

//*************************************************************************************************
// �}�E�X�̃|�C���^�擾
//*************************************************************************************************
CInputMouse *CManager::GetInputMouse(void)
{
    return CManager::m_pInputMouse;
}

//*************************************************************************************************
// �J�����̃|�C���^�擾
//*************************************************************************************************
CCamera *CManager::GetCamera(void)
{
    return CManager::m_pCamera;
}

//*************************************************************************************************
// �T�E���h�̃|�C���^�擾
//*************************************************************************************************
CSound *CManager::GetSound(void)
{
    return CManager::m_Sound;
}

//*************************************************************************************************
// �����[�h�̗\��
//*************************************************************************************************
void CManager::SetMode(CMode *nextMode)
{
	m_nextMode = nextMode;
}

//void CManager::SetModeType(MODE_TYPE type)
//{
//	m_type = type;
//}

//*************************************************************************************************
// ���݂̃��[�h�̎擾
//*************************************************************************************************
MODE_TYPE CManager::GetMode(void)
{
	return mode;
}

//*************************************************************************************************
// �e�N�X�`���}�l�[�W���[�̎擾
//*************************************************************************************************
CTextureManager *CManager::GetTextureManager(void)
{
    return m_TextureManager;
}

//*************************************************************************************************
// ���f���}�l�[�W���[�̎擾
//*************************************************************************************************
CModelManager *CManager::GetModelManager(void)
{
    return m_ModelManager;
}

