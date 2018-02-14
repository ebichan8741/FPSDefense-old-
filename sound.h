//=============================================================================
//
// �T�E���h���� [sound.cpp]
// Author : TAKUYA EBIHARA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//=============================================================================
// �T�E���h�t�@�C���\���̐錾
//=============================================================================
typedef enum
{
    SOUND_LABEL_TITLE = 0,		// BGM0
    SOUND_LABEL_GAME,			// BGM1
    SOUND_LABEL_RESULT,			// BGM2
    SOUND_LABEL_SE_SHOT,		// �e���ˉ�
    SOUND_LABEL_SE_RELOAD,		// �����[�h��
    SOUND_LABEL_SE_WALK01,      // ���s��1
    SOUND_LABEL_SE_WALK02,      // ���s��2
    SOUND_LABEL_SE_WALK03,      // ���s��3
    SOUND_LABEL_SE_WALK04,      // ���s��4
    SOUND_LABEL_SE_HIT01,      // �q�b�g��1
    SOUND_LABEL_SE_HIT02,      // �q�b�g��1
    SOUND_LABEL_SE_HIT03,      // �q�b�g��1
    SOUND_LABEL_SE_TYAKUTI,      // ���n��
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//=============================================================================
// �N���X�錾
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

    IXAudio2 *m_pXAudio2;								        // XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
    IXAudio2MasteringVoice *m_pMasteringVoice;			        // �}�X�^�[�{�C�X
    IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	    // �\�[�X�{�C�X
    BYTE *m_apDataAudio[SOUND_LABEL_MAX];					    // �I�[�f�B�I�f�[�^
    DWORD m_aSizeAudio[SOUND_LABEL_MAX];					    // �I�[�f�B�I�f�[�^�T�C�Y

};

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);
HRESULT VolumeSet(SOUND_LABEL label,float volume);

#endif
