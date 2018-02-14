//=============================================================================
//
// オブジェクト管理 [scene.h]
// Author : TAKUYA EBIHARA
//
//=============================================================================
#ifndef SCENE_H
#define SCENE_H

//=============================================================================
// 定数定義
//=============================================================================
#define MAX_SCENE       (256)

//=============================================================================
// 列挙型
//=============================================================================
//描画優先度
typedef enum
{
    PRIOLITY_0 = 0,
    PRIOLITY_1,
    PRIOLITY_2,
    PRIOLITY_3,
    PRIOLITY_4,
    PRIOLITY_MAX
}PRIOLITY;

//オブジェクトタイプ
typedef enum
{
    SCENE_TYPE_PLAYER = 0,
    SCENE_TYPE_BULLET,
    SCENE_TYPE_ENEMY,
    SCENE_TYPE_CORE,
    SCENE_TYPE_MAX
}SCENE_TYPE;

//=============================================================================
// クラス宣言
//=============================================================================
class CScene
{
public:
    virtual HRESULT Init(void) = 0;
    virtual void Uninit(void) = 0;
    virtual void Update(void) = 0;
    virtual void Draw(void) = 0;
    CScene(int nPriolity);
    static void UpdateAll(void);
    static void DrawAll(void);
    static void ReleaseAll(void);
    void Release(void);

    D3DXVECTOR3 GetPosition(void);
    D3DXVECTOR3 GetSize(void);
    float GetLength(void);
    CScene* GetNext(void);
    SCENE_TYPE GetType(void);
    static CScene *GetList(int nPriority);

protected:
    D3DXVECTOR3 m_vPos;
    D3DXVECTOR3 m_vSize;
    float m_LengthSq;
    LPDIRECT3DTEXTURE9 m_pTexture;
    LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;

    static CScene *m_Top[PRIOLITY_MAX];
    CScene *m_Next;
    bool m_Delete;

    SCENE_TYPE m_Type;

private:

};

#endif