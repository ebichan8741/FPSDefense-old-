//=============================================================================
//
// サウンド処理 [sound.cpp]
// Author : TAKUYA EBIHARA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//=============================================================================
// サウンドファイル構造体宣言
//=============================================================================
typedef enum
{
    SOUND_LABEL_TITLE = 0,		// BGM0
    SOUND_LABEL_GAME,			// BGM1
    SOUND_LABEL_RESULT,			// BGM2
    SOUND_LABEL_SE_SHOT,		// 弾発射音
    SOUND_LABEL_SE_RELOAD,		// リロード音
    SOUND_LABEL_SE_WALK01,      // 歩行音1
    SOUND_LABEL_SE_WALK02,      // 歩行音2
    SOUND_LABEL_SE_WALK03,      // 歩行音3
    SOUND_LABEL_SE_WALK04,      // 歩行音4
    SOUND_LABEL_SE_HIT01,      // ヒット音1
    SOUND_LABEL_SE_HIT02,      // ヒット音1
    SOUND_LABEL_SE_HIT03,      // ヒット音1
    SOUND_LABEL_SE_TYAKUTI,      // 着地音
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//=============================================================================
// クラス宣言
//=============================================================================
class CSound
{
public:
    CSound();
    ~CSound();

    HRESULT Init(HWND hWnd);
    void Uninit(void);
    HRESULT PlaySound(SOUND_LABEL label);
    void StopSound(SOUND_LABEL label);
    void StopSound(void);
    HRESULT VolumeSet(SOUND_LABEL label,float volume);

private:
    HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
    HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

    IXAudio2 *m_pXAudio2;								        // XAudio2オブジェクトへのインターフェイス
    IXAudio2MasteringVoice *m_pMasteringVoice;			        // マスターボイス
    IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	    // ソースボイス
    BYTE *m_apDataAudio[SOUND_LABEL_MAX];					    // オーディオデータ
    DWORD m_aSizeAudio[SOUND_LABEL_MAX];					    // オーディオデータサイズ

};

//=============================================================================
// プロトタイプ宣言
//=============================================================================
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);
HRESULT VolumeSet(SOUND_LABEL label,float volume);

#endif
