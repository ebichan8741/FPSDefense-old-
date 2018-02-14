//=================================================================================================
//
// �Q�[���X�^�[�g�{�^�� [ gamestart.cpp ]
// Author : TAKUYA EBIHARA
//
//=================================================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"
#include "gamestart.h"
#include "TextureManager.h"

//=================================================================================================
// �萔��`
//=================================================================================================
#define STARTBUTTON_POS_X	        (640.0f)	// X���W
#define STARTBUTTON_POS_Y	        (700.0f)	// Y���W
#define STARTBUTTON_SIZE_X	    (350.0f)	    // ��
#define STARTBUTTON_SIZE_Y	    (70.0f)		    // ����
#define STARTBUTTON_ALPHA_RATE    (0.01f)       // �_�ł̑���


//*************************************************************************************************
// �R���X�g���N�^
//*************************************************************************************************
CGameStart::CGameStart(int nPriolity) :CScene2D(nPriolity)
{
}

//*************************************************************************************************
// ����������
//*************************************************************************************************
HRESULT CGameStart::Init(void)
{
    m_vPos = D3DXVECTOR3(STARTBUTTON_POS_X, STARTBUTTON_POS_Y, 0.0f);
    m_vSize = D3DXVECTOR3(STARTBUTTON_SIZE_X, STARTBUTTON_SIZE_Y, 0.0f);
    m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    m_AlphaRate = -STARTBUTTON_ALPHA_RATE;
    CScene2D::Init();
    return S_OK;
}

//*************************************************************************************************
// �I������
//*************************************************************************************************
void CGameStart::Uninit(void)
{
    CScene2D::Uninit();
}

//*************************************************************************************************
// �X�V����
//*************************************************************************************************
void CGameStart::Update(void)
{
    m_Color.a += m_AlphaRate;

    if (m_Color.a <= 0.0f) { m_AlphaRate *= -1; }
    else if (m_Color.a >= 1.0f) { m_AlphaRate *= -1; }

    SetColor(m_Color);
}

//*************************************************************************************************
// �`�揈��
//*************************************************************************************************
void CGameStart::Draw(void)
{
    CScene2D::Draw();
}

//*************************************************************************************************
// �C���X�^���X����
//*************************************************************************************************
CGameStart *CGameStart::Create(void)
{
    CGameStart *gameStart;
    gameStart = new CGameStart(PRIOLITY_2);
    gameStart->Init();

    //gameStart->LoadTexture(TEXTURE_TITLESTART);
    gameStart->m_pTexture = CManager::GetTextureManager()->getResource(CManager::GetTextureManager()->getTextureName(TEXTURE_TITLESTART));

    return gameStart;
}
