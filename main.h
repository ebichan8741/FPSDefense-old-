//=============================================================================
//
// 2D�|���S���\���v���O���� [main.h]
// Author : TAKUYA EBIHARA
//
//=============================================================================
#ifndef MAIN_H
#define MAIN_H

#include <windows.h>
#include <stdlib.h>
#include <iostream>
#include <d3dx9.h>
#define DIRECTINPUT_VERSION (0x0800)
#include <dinput.h>
#include <XAudio2.h>					//�~���[�W�b�N�p�w�b�_
#include <map>
#include <string>
#include <list>

#include <Math.h>

#pragma comment(lib,"d3d9.lib")			//���C�u�����̃����N
#pragma comment(lib,"d3dx9.lib")		//���C�u�����̃����N
#pragma comment(lib,"dxguid.lib")		//���C�u�����̃����N
#pragma comment(lib,"winmm.lib")        //�V�X�e�������擾�ɕK�v
#pragma comment(lib,"dinput8.lib")

#ifdef _DEBUG
#include "Debug.h"
#endif
//=============================================================================
// �萔��`
//=============================================================================
#define SCREEN_WIDTH	(1280)			//�E�B���h�E�̕�
#define SCREEN_HEIGHT	(960)			//�E�B���h�E�̍���
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)		//���_�t�H�[�}�b�g

#define CLASS_NAME		"�T���v��"		//�E�B���h�E�N���X�̖��O
#define WINDOW_NAME		"�E�B���h�E"	//�E�B���h�E�̖��O
#define NUM_VERTEX (4)		//���_��
#define NUM_POLYGON (2)		//�|���S����

#ifdef _DEBUG
int GetFPSCount(void);
#endif

//=============================================================================
// �\���̐錾
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;
	float rhw;
	D3DCOLOR color;
   	D3DXVECTOR2 tex;		//�e�N�X�`���t�u�l
}VERTEX_2D;

typedef struct
{
	D3DXVECTOR3 pos;		//�|�W�V����
	D3DXVECTOR3 normal;		//�@��
	D3DCOLOR color;			//�J���[
	D3DXVECTOR2 tex;		//�e�N�X�`���t�u�l
}VERTEX_3D;

LPDIRECT3DDEVICE9 GetDevice(void);	//D3D�f�o�C�X�̎擾


#endif