//=============================================================================
//
// �f�o�b�O�\���v���O���� [Debug.h]
// Author : TAKUYA EBIHARA
//
//=============================================================================
#ifndef DEBUG_H
#define DEBUG_H

//=============================================================================
// �萔��`
//=============================================================================
#define TEXT_MAX    (1024)

//=============================================================================
// �N���X�錾
//=============================================================================
class CDebug
{
public:
    static char m_Text[TEXT_MAX];
    static void ClearText(void);
    static void AddText(char *text);
	static void Init(void);
	static void Uninit(void);
    static void Draw(void);

private:
    static LPD3DXFONT m_pFont;         // �t�H���g�ւ̃|�C���^
};

#endif