//=============================================================================
//
// 2Dポリゴン表示プログラム [main.h]
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
#include <XAudio2.h>					//ミュージック用ヘッダ
#include <map>
#include <string>
#include <list>

#include <Math.h>

#pragma comment(lib,"d3d9.lib")			//ライブラリのリンク
#pragma comment(lib,"d3dx9.lib")		//ライブラリのリンク
#pragma comment(lib,"dxguid.lib")		//ライブラリのリンク
#pragma comment(lib,"winmm.lib")        //システム時刻取得に必要
#pragma comment(lib,"dinput8.lib")

#ifdef _DEBUG
#include "Debug.h"
#endif
//=============================================================================
// 定数定義
//=============================================================================
#define SCREEN_WIDTH	(1280)			//ウィンドウの幅
#define SCREEN_HEIGHT	(960)			//ウィンドウの高さ
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)		//頂点フォーマット

#define CLASS_NAME		"サンプル"		//ウィンドウクラスの名前
#define WINDOW_NAME		"ウィンドウ"	//ウィンドウの名前
#define NUM_VERTEX (4)		//頂点数
#define NUM_POLYGON (2)		//ポリゴン数

#ifdef _DEBUG
int GetFPSCount(void);
#endif

//=============================================================================
// 構造体宣言
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;
	float rhw;
	D3DCOLOR color;
   	D3DXVECTOR2 tex;		//テクスチャＵＶ値
}VERTEX_2D;

typedef struct
{
	D3DXVECTOR3 pos;		//ポジション
	D3DXVECTOR3 normal;		//法線
	D3DCOLOR color;			//カラー
	D3DXVECTOR2 tex;		//テクスチャＵＶ値
}VERTEX_3D;

LPDIRECT3DDEVICE9 GetDevice(void);	//D3Dデバイスの取得


#endif