//=================================================================================================
//
// メッシュフィールド [meshfield.h]
// Author : TAKUYA EBIHARA
//
//=================================================================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "scene.h"

#define MAX_FIELD_BLOCK_X	(40)			//X方向のブロックの数
#define MAX_FIELD_BLOCK_Z	(50)			//Z方向のブロックの数
#define FIELD_WIDTH			(15.0f)			//ブロックの横幅
#define FIELD_HEIGHT		(15.0f)			//ブロックの縦幅
#define FIELD_POS_X			(-(FIELD_WIDTH * MAX_FIELD_BLOCK_X) / 2)	//フィールドの開始座標X
#define FIELD_POS_Z			((FIELD_HEIGHT * MAX_FIELD_BLOCK_Z) / 2)	//フィールドの開始座標Z
#define CHECK_POINT_CNT		(4)				// チェックポイントの数

//=================================================================================================
// クラス宣言
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