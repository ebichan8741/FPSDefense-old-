//================================================================================================
//
// マップ生成 [Map.cpp]
// Author : TAKUYA EBIHARA
//
//================================================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "Map.h"
#include "DrawModel.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

//=================================================================================================
// 定数定義
//=================================================================================================


//*************************************************************************************************
// コンストラクタ
//*************************************************************************************************
CMap::CMap(int nPriolity) :CScene(nPriolity)
{

}

//*************************************************************************************************
// 初期化処理
//*************************************************************************************************
HRESULT CMap::Init(void)
{

    return S_OK;
}

//*************************************************************************************************
// 終了処理
//*************************************************************************************************
void CMap::Uninit(void)
{

}

//*************************************************************************************************
// 更新処理
//*************************************************************************************************
void CMap::Update(void)
{
}

//*************************************************************************************************
// 描画処理
//*************************************************************************************************
void CMap::Draw(void)
{

}

//*************************************************************************************************
// 頂点情報の設定
//*************************************************************************************************
HRESULT CMap::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{
    return S_OK;
}


//*************************************************************************************************
// インスタンス生成
//*************************************************************************************************
CMap *CMap::Create(string filename)
{
    CMap *map;

    map = new CMap(PRIOLITY_0);
    map->Init();
    map->LoadMap(filename);

    return map;
}

//*************************************************************************************************
// フィールドデータ読み込み
//*************************************************************************************************
void CMap::LoadMap(string filename)
{
    ifstream ifs(filename);
    string str;
    CSVDATA csvdata;
    list<CDrawModel*> model;

    // エラー処理
    if (!ifs) {
        cout << "指定されたファイルが見つかりません！" << filename << endl;
        return;
    }

    while (getline(ifs, str)) {
        string token;
        stringstream ss, ss2,ss3,ss4,ss5,ss6,ss7,ss8,ss9;
        istringstream stream(str);
        // ファイルパス
        getline(stream, token, ',');
        csvdata.filename = token;
        getline(stream, token, ',');
        // 移動量
        getline(stream, token, ',');
        ss << token;
        ss >> csvdata.pos.x;
        getline(stream, token, ',');
        ss2 << token;
        ss2 >> csvdata.pos.y;
        getline(stream, token, ',');
        ss3 << token;
        ss3 >> csvdata.pos.z;
        // 回転量
        getline(stream, token, ',');
        ss4 << token;
        ss4 >> csvdata.rot.x;
        getline(stream, token, ',');
        ss5 << token;
        ss5 >> csvdata.rot.y;
        getline(stream, token, ',');
        ss6 << token;
        ss6 >> csvdata.rot.z;
        // 拡大量
        getline(stream, token, ',');
        ss7 << token;
        ss7 >> csvdata.scl.x;
        getline(stream, token, ',');
        ss8 << token;
        ss8 >> csvdata.scl.y;
        getline(stream, token, ',');
        ss9 << token;
        ss9 >> csvdata.scl.z;
        m_DrawModelList.push_back(CDrawModel::Create(csvdata.filename, csvdata.pos, csvdata.rot, csvdata.scl));
    }
    return;
}