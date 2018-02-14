//================================================================================================
//
// �J�������䏈�� [camera.h]
// Author : TAKUYA EBIHARA
//
//================================================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

class CCamera
{
public:
    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    void SetCamera(void);
    D3DXVECTOR3 GetCameraPos(void);
    D3DXVECTOR3 GetCameraForward(void);
    D3DXVECTOR3 GetCameraLeft(void);
    D3DXVECTOR3 GetCameraUp(void);
    D3DXMATRIX  GetCameraRotMat(void);
    D3DXMATRIX GetViewInverse(void);
    bool m_bFps;
    void Swing(float SwingAmp) { m_SwingAmp = SwingAmp; }

private:
    D3DXVECTOR3 m_vCameraPos;   //�J�����̈ʒu
    D3DXVECTOR3 m_vCameraAt;    //�����_�̈ʒu
    D3DXVECTOR3 m_vVecUp;       //�J�����̏����
    D3DXVECTOR3 m_vDir;         //�����_�ւ̕����x�N�g��
    D3DXVECTOR3 m_vMove;        //�J�����̈ړ���
    D3DXVECTOR3 m_vForward;     //�J�����̑O��
    D3DXVECTOR3 m_vLeft;        //�J�����̍���
    D3DXVECTOR3 m_vUp;          //�J�����̏��
    D3DXVECTOR3 m_vAt;
    D3DXVECTOR3 m_vRot;         //
    float m_fAngle;
    float m_fYaw;
    float m_fPitch;
    D3DXMATRIX m_mtxRot;        //��]�s��
    D3DXMATRIX m_mtxViewInv;    //�r���[�t�s��
    float m_Force;
    bool m_Jump;
    bool m_OldMove;
    int m_WalkInterval;
    float m_SwingAmp;
    float m_SwingTime;
};

#endif