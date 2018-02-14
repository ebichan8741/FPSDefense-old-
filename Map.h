//=================================================================================================
//
// マップ生成 [Map.h]
// Author : TAKUYA EBIHARA
//
//=================================================================================================
#ifndef _MAP_H_
#define _MAP_H_

#include "scene.h"
class CDrawModel;
//=================================================================================================
// クラス宣言
//=================================================================================================
class CMap :public CScene
{
public:
    CMap(int nPriolity);
    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    HRESULT MakeVertex(LPDIRECT3DDEVICE9 pDevice);
    void LoadMap(std::string filename);
    static CMap *Create(std::string filename);

private:
    // 入出力データ
    struct CSVDATA
    {
        std::string filename;
        D3DXVECTOR3 pos;
        D3DXVECTOR3 rot;
        D3DXVECTOR3 scl;
    };
    std::list<CDrawModel*> m_DrawModelList;
};
#endif
