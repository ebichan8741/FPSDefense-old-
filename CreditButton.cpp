//=================================================================================================
//
// �N���W�b�g�{�^�� [ CreditButton.cpp ]
// Author : TAKUYA EBIHARA
//
//=================================================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"
#include "CreditButton.h"
#include "TextureManager.h"

//=================================================================================================
// �萔��`
//=================================================================================================
#define CREDITBUTTON_POS_X	        (640.0f)	// X���W
#define CREDITBUTTON_POS_Y	        (700.0f)	// Y���W
#define CREDITBUTTON_SIZE_X	    (350.0f)	    // ��
#define CREDITBUTTON_SIZE_Y	    (70.0f)		    // ����
#define CREDITBUTTON_ALPHA_RATE    (0.01f)       // �_�ł̑���


//*************************************************************************************************
// �R���X�g���N�^
//*************************************************************************************************
CCreditButton::CCreditButton(int nPriolity) :CScene2D(nPriolity)
{
}

//*************************************************************************************************
// ����������
//*************************************************************************************************
HRESULT CCreditButton::Init(void)
{
    m_vPos = D3DXVECTOR3(CREDITBUTTON_POS_X, CREDITBUTTON_POS_Y, 0.0f);
    m_vSize = D3DXVECTOR3(CREDITBUTTON_SIZE_X, CREDITBUTTON_SIZE_Y, 0.0f);
    m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    m_AlphaRate = -CREDITBUTTON_ALPHA_RATE;
    CScene2D::Init();
    return S_OK;
}

//*************************************************************************************************
// �I������
//*************************************************************************************************
void CCreditButton::Uninit(void)
{
    CScene2D::Uninit();
}

//*************************************************************************************************
// �X�V����
//*************************************************************************************************
void CCreditButton::Update(void)
{
    m_Color.a += m_AlphaRate;

    if (m_Color.a <= 0.0f) { m_AlphaRate *= -1; }
    else if (m_Color.a >= 1.0f) { m_AlphaRate *= -1; }

    SetColor(m_Color);
}

//*************************************************************************************************
// �`�揈��
//*************************************************************************************************
void CCreditButton::Draw(void)
{
    CScene2D::Draw();
}

//*************************************************************************************************
// �C���X�^���X����
//*************************************************************************************************
CCreditButton *CCreditButton::Create(void)
{
    CCreditButton *credit;
    credit = new CCreditButton(PRIOLITY_2);
    credit->Init();

    //credit->LoadTexture(TEXTURE_TITLESTART);
    credit->m_pTexture = CManager::GetTextureManager()->getResource(CManager::GetTextureManager()->getTextureName(TEXTURE_TITLESTART));

    return credit;
}
