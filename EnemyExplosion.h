//=============================================================================
//
// �G�����G�t�F�N�g [EnemyExplosion.h]
// Author : TAKUYA EBIHARA
//
//=============================================================================
#ifndef ENEMYEXPLOSION_H
#define ENEMYEXPLOSION_H

#include "scene.h"
#include "scene3D.h"

//=============================================================================
// �\���̐錾
//=============================================================================

//=============================================================================
// �N���X�錾
//=============================================================================
class CEnemyExplosion :public CScene3D
{
public:
    CEnemyExplosion(int nPriolity);
    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);

    static CEnemyExplosion *Create(D3DXVECTOR3 pos);
    static HRESULT Load(void);
    static void Unload(void);

private:
    static LPDIRECT3DTEXTURE9 m_pTexture;   //�e�N�X�`���|�C���^
    int m_Life;
    bool m_Use;
    D3DXVECTOR3 m_Rot;
    D3DXVECTOR2 m_TexPos, m_TexSize;
    int m_AnimCnt;
};

#endif
