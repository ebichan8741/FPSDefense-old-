//================================================================================================
//
// メッシュフィールド [meshfield.cpp]
// Author : TAKUYA EBIHARA
//
//================================================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "meshfield.h"

//=================================================================================================
// 定数定義
//=================================================================================================
#define FIELD_TEXTURENAME "data/TEXTURE/field004.jpg"


//*************************************************************************************************
// コンストラクタ
//*************************************************************************************************
CMeshField::CMeshField(int nPriolity):CScene(nPriolity)
{
	m_CheckPoint[0] = D3DXVECTOR3(-100.0f, 0.0f, -130.0f);
	m_CheckPoint[1] = D3DXVECTOR3(-100.0f, 0.0f, 130.0f);
	m_CheckPoint[2] = D3DXVECTOR3(100.0f, 0.0f, 130.0f);
	m_CheckPoint[3] = D3DXVECTOR3(100.0f, 0.0f, -130.0f);
}

//*************************************************************************************************
// 初期化処理
//*************************************************************************************************
HRESULT CMeshField::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    LoadField();

	//頂点情報の設定
	if(FAILED(MakeVertex(pDevice)))
	{
		return E_FAIL;
	}

	//頂点インデックスの設定
	if(FAILED(MakeIndex(pDevice)))
	{
		return E_FAIL;
	}

	//ハードディスクからテクスチャを読み込む
	if(FAILED(D3DXCreateTextureFromFile(pDevice,FIELD_TEXTURENAME,&m_pTexture)))
	{
		//エラー時
		MessageBox(NULL,"テクスチャの読み込みに失敗しました","終了メッセージ",MB_OK);
	}

	return 0;
}

//*************************************************************************************************
// 終了処理
//*************************************************************************************************
void CMeshField::Uninit(void)
{
	if( m_pTexture != NULL ){
		m_pTexture -> Release();		//Direct3Dデバイスの解放
		m_pTexture = NULL;
	}

	if( m_pVtxBuff != NULL ){
		m_pVtxBuff -> Release();			//Direct3Dインターフェースの解放
		m_pVtxBuff = NULL;
	}

	if( m_pIdxBuff != NULL ){
		m_pIdxBuff -> Release();			//Direct3Dインターフェースの解放
		m_pIdxBuff = NULL;
	}
}

//*************************************************************************************************
// 更新処理
//*************************************************************************************************
void CMeshField::Update(void)
{
}

//*************************************************************************************************
// 描画処理
//*************************************************************************************************
void CMeshField::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX mtxWorld;

	pDevice->SetStreamSource(
		0,						//ストリーム番号(パイプ番号)
		m_pVtxBuff,		//ストリームの元になる頂点バッファ
		0,						//オフセット(バイト)
		sizeof(VERTEX_3D));		//一個の頂点データのサイズ(ストライド量)

	//デバイスにインデックスバッファの設定
	pDevice->SetIndices(m_pIdxBuff);

	//頂点フォーマットの設定
	pDevice -> SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0,m_pTexture);

	//ワールド座標変換行列
	D3DXMatrixIdentity(&mtxWorld);			//ワールド座標変換行列を初期化

	pDevice->SetTransform(D3DTS_WORLD,&mtxWorld);		//ワールド座標変換行列を移動できるようにする(?)

	//インデックスバッファによる描画
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,		//描画形式(トライアングルストリップ)
		0,							//最初の頂点インデックスまでのオフセット
		0,							//最小の頂点インデックス
		(MAX_FIELD_BLOCK_X + 1) * (MAX_FIELD_BLOCK_Z + 1),//頂点数
		0,							//スタートインデックス
		NUM_POLYGON * MAX_FIELD_BLOCK_X * MAX_FIELD_BLOCK_Z + (MAX_FIELD_BLOCK_Z - 1) * 4) ;						//プリミティブ数
}

//*************************************************************************************************
// 頂点情報の設定
//*************************************************************************************************
HRESULT CMeshField::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_3D*  pVtx;
    D3DXVECTOR3 normal;
    D3DXVECTOR3 vNormalX;
    D3DXVECTOR3 vNormalZ;
    D3DXVECTOR3 vDir,vDirZ;

	//頂点バッファ
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * (MAX_FIELD_BLOCK_X + 1) * (MAX_FIELD_BLOCK_Z + 1),		//作成したい頂点バッファのサイズ
		D3DUSAGE_WRITEONLY,							//使用方法
		FVF_VERTEX_3D,								//
		D3DPOOL_MANAGED,							//メモリ管理方法(デバイスにお任せ)
		&m_pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	//頂点情報の設定///////////////////////////////////////////////////////////////////////////////
	m_pVtxBuff->Lock(0,0,(void**)&pVtx,0);	//ロック

	//頂点座標(奥から順)
	for(int nCntH = 0;nCntH < (MAX_FIELD_BLOCK_Z + 1);nCntH++)
	{
		for(int nCntW = 0;nCntW < (MAX_FIELD_BLOCK_X + 1);nCntW++)
		{
			pVtx[nCntW + (nCntH * (MAX_FIELD_BLOCK_X + 1))].pos = D3DXVECTOR3(FIELD_POS_X + FIELD_WIDTH * nCntW,
                                                                            m_Field[nCntH][nCntW],
                                                                            FIELD_POS_Z - FIELD_HEIGHT * nCntH);
		}
	}

    //法線・カラー・テクスチャ座標
    for(int nCntH = 0;nCntH < (MAX_FIELD_BLOCK_Z + 1);nCntH++)
	{
		for(int nCntW = 0;nCntW < (MAX_FIELD_BLOCK_X + 1);nCntW++)
		{
            if (nCntW != 0) {
                vDir = pVtx[nCntW + (nCntH * (MAX_FIELD_BLOCK_X + 1) + 1)].pos -
                    pVtx[nCntW + (nCntH * (MAX_FIELD_BLOCK_X + 1) - 1)].pos;
            }
            vNormalX.x = -vDir.y;
            vNormalX.y = vDir.x;
            vNormalX.z = 0.0f;

            if (nCntH != 0) {
                vDir = pVtx[nCntW + (nCntH * (MAX_FIELD_BLOCK_X + 1)) - (MAX_FIELD_BLOCK_X + 1)].pos -
                    pVtx[nCntW + (nCntH * (MAX_FIELD_BLOCK_X + 1)) + (MAX_FIELD_BLOCK_X + 1)].pos;
            }
            vNormalZ.x = 0.0f;
            vNormalZ.y = vDir.z;
            vNormalZ.z = -vDir.y;

            normal = vNormalX + vNormalZ;

            if(nCntH == 0 || nCntH == MAX_FIELD_BLOCK_Z)
            {
                normal = D3DXVECTOR3(0.0f,1.0f,0.0f);
            }
            if(nCntW == 0 || nCntW == MAX_FIELD_BLOCK_X)
            {
                normal = D3DXVECTOR3(0.0f,1.0f,0.0f);
            }
            D3DXVec3Normalize (&normal,&normal);

			pVtx[nCntW + (nCntH * (MAX_FIELD_BLOCK_X + 1))].normal  = normal;
			pVtx[nCntW + (nCntH * (MAX_FIELD_BLOCK_X + 1))].color   = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[nCntW + (nCntH * (MAX_FIELD_BLOCK_X + 1))].tex     = D3DXVECTOR2(1.0f * nCntW,1.0f * nCntH);
        }
    }
	m_pVtxBuff->Unlock();	//アンロック

	return 0;
}

//*************************************************************************************************
// 頂点インデックスの設定
//*************************************************************************************************
HRESULT CMeshField::MakeIndex(LPDIRECT3DDEVICE9 pDevice)
{
	WORD* pIdx;

	//インデックスバッファ
	if(FAILED(pDevice->CreateIndexBuffer(
		sizeof(WORD) * ((MAX_FIELD_BLOCK_X + 1) * 2 * MAX_FIELD_BLOCK_Z + (MAX_FIELD_BLOCK_Z - 1) * 2),	//
		D3DUSAGE_WRITEONLY,							//使用用途フラグ
		D3DFMT_INDEX16,								//インデックスデータのフォーマット
		D3DPOOL_MANAGED,							//メモリの管理方法（お任せ）
		&m_pIdxBuff,							//インデックスバッファインターフェースポインタのアドレス
		NULL)));

	//頂点インデックスの設定////////////////////////////////////////////////////////////////////////
	m_pIdxBuff->Lock(0,0,(void**)&pIdx,0);	//ロック
	//N字型に生成
	//縦のメッシュ生成
	for(int height = 0;height < MAX_FIELD_BLOCK_Z;height++)
	{
		//横のメッシュ生成
		for(int width = 0;width < MAX_FIELD_BLOCK_X + 1;width++)
		{
			//縮退ポリゴンの計算
			if(width == 0 && height != 0)
			{
				pIdx[(height * (MAX_FIELD_BLOCK_X + 2) * 2) - 2] = MAX_FIELD_BLOCK_X + (height - 1) * (MAX_FIELD_BLOCK_X + 1);
				pIdx[(height * (MAX_FIELD_BLOCK_X + 2) * 2) - 1] = (MAX_FIELD_BLOCK_X + 1) * 2 + (height - 1) * (MAX_FIELD_BLOCK_X + 1);
			}
			pIdx[(width * 2) + (height * (MAX_FIELD_BLOCK_X + 2) * 2)] = width + height * (MAX_FIELD_BLOCK_X + 1) + (MAX_FIELD_BLOCK_X + 1);
			pIdx[(width * 2 + 1) + (height * (MAX_FIELD_BLOCK_X + 2) * 2)] = width + height * (MAX_FIELD_BLOCK_X + 1);
		}
	}

	m_pIdxBuff->Unlock();	//アンロック
	return 0;
}

//*************************************************************************************************
// フィールドのY座標取得
//*************************************************************************************************
float CMeshField::GetHeight(D3DXVECTOR3 pos)
{
    VERTEX_3D*  pVtx;
    D3DXVECTOR3 p0,p1,p2;
    D3DXVECTOR3 v01,v02,v12,v0p,v1p,v2p,vc0,vc1,vc2,vn;

    m_pVtxBuff->Lock(0,0,(void**)&pVtx,0);	//ロック

    for(int nCntH = 0;nCntH < (MAX_FIELD_BLOCK_Z + 1);nCntH++)
	{
		for(int nCntW = 0;nCntW < (MAX_FIELD_BLOCK_X + 1);nCntW++)
		{
            //左の三角形ポリゴン
            //左上頂点
            p0 = pVtx[nCntW + (nCntH * (MAX_FIELD_BLOCK_X + 1))].pos;
            //左下頂点
            p1 = pVtx[nCntW + (nCntH * (MAX_FIELD_BLOCK_X + 1)) + (MAX_FIELD_BLOCK_X + 1)].pos;
            //右下頂点
            p2 = pVtx[nCntW + (nCntH * (MAX_FIELD_BLOCK_X + 1)) + (MAX_FIELD_BLOCK_X + 1) + 1].pos;

            //ベクトル生成
            v01 = p1 - p0;
            v0p = pos - p0;

            //法線計算
            D3DXVec3Cross(&vc0,&v0p,&v01);

            if(vc0.y >= 0.0f)
            {
                v12 = p2 - p1;
                v1p = pos - p1;
                D3DXVec3Cross(&vc1,&v1p,&v12);
                if(vc1.y >= 0.0f)
                {
                    v02 = p2 - p0;
                    v2p = pos - p2;
                    D3DXVec3Cross(&vc2,&v02,&v2p);
                    if(vc2.y >= 0.0f)
                    {
                        v02 = p2 - p0;
                        D3DXVec3Cross(&vn,&v01,&v02);
                        D3DXVec3Normalize(&vn,&vn);

                        pos.y = p0.y - ((pos.x - p0.x) * vn.x + (pos.z - p0.z) * vn.z) / vn.y;

                        return pos.y;
                    }
                }
            }
            //右の三角形ポリゴン
            p0 = pVtx[nCntW + (nCntH * (MAX_FIELD_BLOCK_X + 1))].pos;

            p1 = pVtx[nCntW + (nCntH * (MAX_FIELD_BLOCK_X + 1)) + 1].pos;

            p2 = pVtx[nCntW + (nCntH * (MAX_FIELD_BLOCK_X + 1)) + (MAX_FIELD_BLOCK_X + 1) + 1].pos;

            v01 = p1 - p0;
            v0p = pos - p0;
            D3DXVec3Cross(&vc0,&v01,&v0p);
            if(vc0.y >= 0.0f)
            {
                v12 = p2 - p1;
                v1p = pos - p1;
                D3DXVec3Cross(&vc1,&v12,&v1p);
                if(vc1.y >= 0.0f)
                {
                    v02 = p2 - p0;
                    v2p = pos - p2;
                    D3DXVec3Cross(&vc2,&v2p,&v02);
                    if(vc2.y >= 0.0f)
                {
                        v02 = p2 - p0;
                        D3DXVec3Cross(&vn,&v01,&v02);
                        D3DXVec3Normalize(&vn,&vn);

                        pos.y = p0.y - ((pos.x - p0.x) * vn.x + (pos.z - p0.z) * vn.z) / vn.y;

                        return pos.y;
                    }
                }
            }
        }
    }

    m_pVtxBuff->Unlock();	//アンロック

    return 0;
}

//*************************************************************************************************
// インスタンス生成
//*************************************************************************************************
CMeshField *CMeshField::Create(void)
{
    CMeshField *meshfield;

    meshfield = new CMeshField(PRIOLITY_0);

    meshfield->Init();

    return meshfield;
}

//*************************************************************************************************
// フィールドデータ読み込み
//*************************************************************************************************
void CMeshField::LoadField(void)
{
    FILE *fp;
    char buf[256];
    int c;
    int col = 1;
    int row = 0;

    memset(buf, 0, sizeof(buf));
    fp = fopen("data/CSV/fielddata.csv", "r");

    while (1) {

        while (1) {

            c = fgetc(fp);

            //末尾ならループを抜ける。
            if (c == EOF)
                return ;

            //カンマか改行でなければ、文字としてつなげる
            if (c != ',' && c != '\n')
                strcat(buf, (const char*)&c);
            //カンマか改行ならループ抜ける。
            else
                break;
        }
        //ここに来たということは、1セル分の文字列が出来上がったということ
        m_Field[row][col] = atoi(buf);

        //バッファを初期化
        memset(buf, 0, sizeof(buf));
        //列数を足す
        ++col;

        //もし読み込んだ文字が改行だったら列数を初期化して行数を増やす
        if (c == '\n') {
            col = 1;
            ++row;
        }
    }
    fclose(fp);
}