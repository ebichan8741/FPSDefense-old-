//=============================================================================
//
// スコア設定処理 [score.h]
// Author : TAKUYA EBIHARA
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "scene.h"
#include "number.h"

//*****************************************************************************
// 定数定義
//*****************************************************************************
#define NUM_SCORE   (8)     //スコアの桁数

//*****************************************************************************
// クラス定義
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
    static int m_nScore;               //スコア値
};

#endif