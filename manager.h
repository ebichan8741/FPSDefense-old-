//=============================================================================
//
// マネージャー処理 [maneger.h]
// Author : TAKUYA EBIHARA
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

//前方宣言
class CRenderer;
CRenderer *GetRenderer(void);
class CInputKeyboard;
CInputKeyboard *GetInputKeyboard(void);
class CInputMouse;
CInputMouse *GetInputMouse(void);
class CCamera;
CCamera *GetCamera(void);
class CMode;
void SetMode(CMode *nextMode);
class CFade;
class CSound;
CSound *GetSound(void);
class CTextureManager;
CTextureManager *GetTextureManager(void);
class CModelManager;
CModelManager *GetModelManager(void);


typedef enum {
	MODE_TITLE,
	MODE_GAME,
	MODE_RESULT,
	MODE_MAX
}MODE_TYPE;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CManager
{
public:
    CManager();
    ~CManager();
    HRESULT Init(HINSTANCE hInstance,HWND hWnd,BOOL bWindow);
    void Uninit(void);
    void Update(void);
    void Draw(void);

    static CRenderer *GetRenderer(void);
    static CInputKeyboard *GetInputKeyboard(void);
    static CInputMouse *GetInputMouse(void);
    static CCamera *GetCamera(void);
    static CSound *GetSound(void);
	static void SetMode(CMode *nextMode);
	static MODE_TYPE GetMode(void);
	static MODE_TYPE mode;
    static CTextureManager *GetTextureManager(void);
    static CModelManager *GetModelManager(void);


private:
    static CRenderer *m_pRenderer;
    static CInputKeyboard *m_pInputKeyboard;
    static CInputMouse *m_pInputMouse;
    static CCamera *m_pCamera;
	static CMode *m_Mode;
	static CMode *m_nextMode;
	static CFade *m_Fade;
    static CSound *m_Sound;
    static CTextureManager *m_TextureManager;
    static CModelManager *m_ModelManager;

protected:

};


#endif