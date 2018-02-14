//================================================================================================
//
// カメラ制御処理 [camera.h]
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
    D3DXVECTOR3 m_vCameraPos;   //カメラの位置
    D3DXVECTOR3 m_vCameraAt;    //注視点の位置
    D3DXVECTOR3 m_vVecUp;       //カメラの上方向
    D3DXVECTOR3 m_vDir;         //注視点への方向ベクトル
    D3DXVECTOR3 m_vMove;        //カメラの移動量
    D3DXVECTOR3 m_vForward;     //カメラの前方
    D3DXVECTOR3 m_vLeft;        //カメラの左方
    D3DXVECTOR3 m_vUp;          //カメラの上方
    D3DXVECTOR3 m_vAt;
    D3DXVECTOR3 m_vRot;         //
    float m_fAngle;
    float m_fYaw;
    float m_fPitch;
    D3DXMATRIX m_mtxRot;        //回転行列
    D3DXMATRIX m_mtxViewInv;    //ビュー逆行列
    float m_Force;
    bool m_Jump;
    bool m_OldMove;
    int m_WalkInterval;
    float m_SwingAmp;
    float m_SwingTime;
};

#endif