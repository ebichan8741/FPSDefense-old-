//=================================================================================================
//
// 2D�|���S���\���v���O���� [main.cpp]
// Author : TAKUYA EBIHARA
//
//=================================================================================================
#include "main.h"
#include "manager.h"
#include <stdio.h>
#include <new>
#include <memory>
#include <cstdlib>

using namespace std;

#include <crtdbg.h>

#define _CRTDBG_MAP_ALLOC

#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)


//=================================================================================================
// �v���g�^�C�v�錾
//=================================================================================================
LRESULT CALLBACK WndProc (HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM IParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);			//�I���֐�
void Update(void);			//�A�b�v�f�[�g�֐�
void Draw(void);			//�\���֐�



//=================================================================================================
// �O���[�o���ϐ�
//=================================================================================================
LPDIRECT3D9 g_pD3D = NULL;								//Direct3D�C���^�[�t�F�[�X  //LP=�|�C���^�^
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;					//Direct3D�f�o�C�X�ւ̃|�C���^
int g_nFPSCount = 0;


//*************************************************************************************************
// ���C������
//*************************************************************************************************
int APIENTRY WinMain(HINSTANCE hInstance , HINSTANCE hPrevInstance , LPSTR lpCmdLine , int nCmdShow)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	WNDCLASSEX wcex;

	wcex.cbSize				=sizeof(WNDCLASSEX);				//�E�B���h�E�̃������T�C�Y���w��
	wcex.style				=CS_CLASSDC;						//�E�B���h�E����̃X�^�C���������Ă�
	wcex.lpfnWndProc		=WndProc;							//�E�B���h�E�v���V�[�W���̃A�h���X�u�֐����v��ݒ�
	wcex.cbClsExtra			=0;									//�ʏ�͎g�p���Ȃ��̂�"0"���w��
	wcex.cbWndExtra			=0;									//�ʏ�͎g�p���Ȃ��̂�"0"���w��
	wcex.hInstance			=hInstance;							//WinMain�̃p�����[�^�̃C���X�^���X�n���h��
	wcex.hIcon				=NULL;								//�g�p����A�C�R���̐ݒ�
	wcex.hCursor			=LoadCursor(NULL, IDC_ARROW);		//�J�[�\����ύX���邱�Ƃ��ł���
	wcex.hbrBackground		=(HBRUSH) (COLOR_WINDOW + 1);		//�}�E�X�J�[�\�����w��)�F
	wcex.lpszMenuName		=NULL;								//���j���[�i�t�@�C���Ƃ��j
	wcex.lpszClassName		=CLASS_NAME;						//�E�B���h�E�̖��O
	wcex.hIconSm			=NULL;								//�g�����ꂽ����

	//�E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	HWND hWnd;
	MSG msg;
	RECT cr = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	RECT dr;

	GetWindowRect(GetDesktopWindow(), &dr);

	int dw = dr.right-dr.left;		//�f�X�N�g�b�v�̕������߂�
	int dh = dr.bottom-dr.top;		//�f�X�N�g�b�v�̍��������߂�


	DWORD style = WS_OVERLAPPEDWINDOW &~ (WS_MAXIMIZEBOX | WS_THICKFRAME);	//�E�B���h�E�X�^�C��

	AdjustWindowRect(&cr, style, FALSE);	//�X�^�C���⍂������ύX�������

	int ww = cr.right-cr.left;		//�E�B���h�E�̕��u�t���[�������킹���v
	int wh = cr.bottom-cr.top;		//�E�B���h�E�̍����u�t���[�������킹���v
	
	int wx = ww > dw ? 0: (dw-ww)/2;	//�E�B���h�E�̍��E�̐^�񒆂����߂�
	int wy = wh > dh ? 0: (dh-wh)/2;	//�E�B���h�E�̏㉺�̐^�񒆂����߂�

	//�E�B���h�E���쐬
	hWnd = CreateWindowEx(0,		//�g���E�B���h�E�X�^�C��
		CLASS_NAME,					//�E�B���h�E�N���X�̖��O
		WINDOW_NAME,				//�E�B���h�E�̖��O
		style,						//�E�B���h�E�̃X�^�C��					����
		wx,							//�E�B���h�E�̍��W(X���W�P�ʃs�N�Z��)
		wy,							//�E�B���h�E�̍��W(Y���W�P�ʃs�N�Z��)
		ww,							//�E�B���h�E�̕��u�t���[�������킹���v
		wh,							//�E�B���h�E�̍����u�t���[�������킹���v
		NULL,						//�e�E�B���h�E�̃n���h��
		NULL,						//���j���[�n���h���܂��͎q�E�B���h�EID
		hInstance,					//�C���X�^���X�n���h��
		NULL);						//�E�B���h�E�쐬�f�[�^(NULL)

    CManager *pManager = NULL;
    pManager = new CManager;

	//�E�B���h�E�̕\��
	if(FAILED(pManager->Init(hInstance, hWnd, true)))
	{
		MessageBox(NULL,"�������Ɏ��s���܂����B","",MB_OK);
		return E_FAIL;
	}

    //�ϐ��錾
    DWORD dwFrameCount;
	DWORD dwCurrentTime;
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;

    // ����\��ݒ�
    timeBeginPeriod(1);

    //�e�J�E���^�[�̏�����
	dwCurrentTime = 0;
	dwFrameCount = 0;
	dwExecLastTime = dwFPSLastTime = timeGetTime();	//�V�X�e���������~���b�P�ʂŎ擾

	ShowWindow(hWnd, nCmdShow);	//�\����Ԃ̐ݒ�
	UpdateWindow(hWnd);			//�E�B���h�E�̃A�b�v�f�[�g(�X�V)

    _CrtDumpMemoryLeaks();

	// ���C�� ���b�Z�[�W ���[�v:
    while(1)
	{
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE) != 0)
		{
			//Windows�̏���
			if(msg.message == WM_QUIT)
			{
				break;
			}

			else
			{
				TranslateMessage(&msg);				//���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�֕ϊ�
				DispatchMessage(&msg);				//�E�B���h�E�v���V�[�W���փ��b�Z�[�W�𑗏o
			}
		}

		//FPS�J�E���g
		else
		{
			dwCurrentTime = timeGetTime();			//���݂̎��Ԃ��擾

			//0.5�b���ƂɎ��s
			if((dwCurrentTime - dwFPSLastTime) >= 500)
			{
				g_nFPSCount = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);	//�b�ԉ��t���[���o�߂�����
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;					//�t���[���J�E���^�����Z�b�g
			};

			if((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				dwExecLastTime = dwCurrentTime;		//������������
				//DirectX�̏���
				pManager->Update();
				pManager->Draw();
				dwFrameCount++;
			}
		}
	}

	//�E�B���h�E�̏I������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

    //�I������
    if(pManager != NULL)
    {
        pManager->Uninit();
        pManager = NULL;
    }

    // ����\��߂�
    timeEndPeriod(1);

	return (int)msg.wParam;
}

//*************************************************************************************************
// �E�B���h�E�v���V�[�W��
//*************************************************************************************************
LRESULT CALLBACK WndProc (HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM IParam)
{
	static HWND hWndPaint;			//�y�C���g�̕ϐ���錾
	static HDC  hDC;				//�E�B���h�E�n���h����錾
	PAINTSTRUCT static ps;			//�y�C���g�ϐ���錾

	switch(uMsg)
	{
	case WM_DESTROY:				//�E�B���h�E�j�󖽗�
		PostQuitMessage(0);			//
		break;

	case WM_KEYDOWN:								//�L�[�����͂��ꂽ�火
		switch(wParam)								//wParam����
	{						
		case VK_ESCAPE:								//���͂��ꂽ�L�[���G�X�P�[�v�������火
			UINT nID = MessageBox(NULL,"�I�����܂����H","�I�����b�Z�[�W",MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2);			//���b�Z�[�W�{�b�N�X��\������B
			if(nID == IDYES)
			{
				DestroyWindow(hWnd);	//�I������
			}
		break;
	}

	default:
			break;
	}

	return DefWindowProc(hWnd,
		uMsg, wParam, IParam);

}

#ifdef _DEBUG
//=============================================================================
// FPS�擾
//=============================================================================
int GetFPSCount(void)
{
    return g_nFPSCount;
}
#endif