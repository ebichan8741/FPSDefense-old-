//=============================================================================
//
// �X�R�A�ݒ菈�� [score.h]
// Author : TAKUYA EBIHARA
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "scene.h"
#include "number.h"

//*****************************************************************************
// �萔��`
//*****************************************************************************
#define NUM_SCORE   (8)     //�X�R�A�̌���

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CScore:public CScene
{
public:
    CScore(int nPriolity);
    ~CScore();
    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);

    static CScore *Create(void);
    static void AddScore(int nValue);
    int GetScore(void);

private:
    CNumber *m_apNumber[NUM_SCORE];
    static int m_nScore;               //�X�R�A�l
};

#endif