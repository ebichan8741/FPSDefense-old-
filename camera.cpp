//================================================================================================
//
// カメラ制御処理 [camera.cpp]
// Author : TAKUYA EBIHARA
//
//================================================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "GameMode.h"
#include "camera.h"
#include "meshField.h"
#include "input.h"
#include "sound.h"

//=================================================================================================
// 定数定義
//=================================================================================================
#define _Gravity    (9.8f)      // 重力
#define JUMP_FORCE  (100.0f)     // ジャンプ力

//*************************************************************************************************
// 初期化処理
//*************************************************************************************************
HRESULT CCamera::Init(void)
{
    m_vCameraPos  = D3DXVECTOR3(0.0f,10.0f,-140.0f);
    m_vCameraAt   = D3DXVECTOR3(0.0f,10.0f,10.0f);
    m_vVecUp      = D3DXVECTOR3(0.0f,1.0f,0.0f);
    m_vDir        = D3DXVECTOR3(0.0f,0.0f,1.0f);
    m_vMove       = D3DXVECTOR3(0.0f,0.0f,0.0f);
    m_vForward    = D3DXVECTOR3(0.0f,0.0f,1.0f);
    m_vLeft       = D3DXVECTOR3(1.0f,0.0f,0.0f);
    m_vUp         = D3DXVECTOR3(0.0f,1.0f,0.0f);
    m_fAngle      = 0.0f;
    m_fYaw        = 0.0f;
    m_fPitch      = 0.0f;
    m_bFps        = true;
    m_Force = 0.0f;
    m_Jump = false;
    m_OldMove = false;
    m_WalkInterval = 0;
    m_SwingAmp = 0.0f;
    m_SwingTime = 0.0f;
    return 0;
}

//*************************************************************************************************
// 終了処理
//*************************************************************************************************
void CCamera::Uninit(void)
{
}

//*************************************************************************************************
// 更新処理
//*************************************************************************************************
void CCamera::Update(void)
{
    // 変数宣言 -----------------------------------------------------------------------------------
    LPDIRECT3DDEVICE9 pDevice;
    D3DXVECTOR3 vForward(0.0f, 0.0f, 1.0f);
    D3DXVECTOR3 vLeft(1.0f, 0.0f, 0.0f);
    D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);
    D3DXVECTOR3 vAt(0.0f, 0.0f, 1.0f);
    D3DXMATRIX mtxTrans,mtxRot;
    D3DXVECTOR3 force;
    bool move;
    float dt = 0.03f;
    //---------------------------------------------------------------------------------------------
    if (CManager::GetMode() == MODE_GAME) {
        //デバイスの取得
        pDevice = CManager::GetRenderer()->GetDevice();

        force = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        move = false;

        // キー入力情報を取得 /////////////////////////////////////////////////////////////////////////
        //前進
        if (CManager::GetInputKeyboard()->GetKeyPress(DIK_W)) {
            force.x += m_vAt.x;
            force.z += m_vAt.z;
            move = true;
            // スプリント
            if (CManager::GetInputKeyboard()->GetKeyPress(DIK_LSHIFT) ||
                CManager::GetInputKeyboard()->GetKeyPress(DIK_RSHIFT)) {
                force.x += m_vAt.x;
                force.z += m_vAt.z;
            }
        }
        //後退
        if (CManager::GetInputKeyboard()->GetKeyPress(DIK_S)) {
            force.x -= m_vAt.x;
            force.z -= m_vAt.z;
            move = true;
        }
        //左移動
        if (CManager::GetInputKeyboard()->GetKeyPress(DIK_A)) {
            force.x -= m_vLeft.x;
            force.z -= m_vLeft.z;
            move = true;
        }
        //右移動
        if (CManager::GetInputKeyboard()->GetKeyPress(DIK_D)) {
            force.x += m_vLeft.x;
            force.z += m_vLeft.z;
            move = true;
        }

        //上昇
        if (CManager::GetInputKeyboard()->GetKeyPress(DIK_E)) {
            m_vCameraPos.y += 0.5f;
        }

        //下降
        if (CManager::GetInputKeyboard()->GetKeyPress(DIK_Q)) {
            m_vCameraPos.y -= 0.5f;
        }

        //カメラモード切替
        if (CManager::GetInputKeyboard()->GetKeyTrigger(DIK_H)) {
            m_bFps = !m_bFps;
        }

        // FPSモード
        if (m_bFps == true && CManager::GetMode() == MODE_GAME) {
            
            bool jump = m_Jump;

            // 重力計算
            if (m_vCameraPos.y >= CGameMode::GetMeshField()->GetHeight(m_vCameraPos) + 10) {
                m_Force -= _Gravity;

            }
            else {
                m_Jump = false;
                m_vCameraPos.y = CGameMode::GetMeshField()->GetHeight(m_vCameraPos) + 10;
            }
            // ジャンプ
            if (CManager::GetInputKeyboard()->GetKeyTrigger(DIK_SPACE) && m_Jump == false) {
                m_Force = JUMP_FORCE;
                m_Jump = true;
            }

            // 着地音
            if (jump == true && m_Jump == false) {
                CManager::GetSound()->PlaySound(SOUND_LABEL_SE_TYAKUTI);
            }

            m_vCameraPos.y += m_Force * dt;
        }

        // マウスが画面中央からどれだけ移動したかを取得し、値を変化させる
        // YawはY軸を中心とした横回転
        m_fYaw += (CManager::GetInputMouse()->GetMousePos().x - SCREEN_WIDTH / 2) / 800;
        // PitchはX軸を中心とした縦回転
        m_fPitch += (CManager::GetInputMouse()->GetMousePos().y - SCREEN_HEIGHT / 2) / 800;

        //上下回転制限
        if (m_fPitch < -(D3DX_PI / 2)) {
            m_fPitch = -(D3DX_PI / 2);
        }
        else if (m_fPitch > (D3DX_PI / 2)) {
            m_fPitch = D3DX_PI / 2;
        }

        // YawとPitchから前方の座標と左側、上側の座標を取得
        D3DXMatrixRotationYawPitchRoll(&m_mtxRot, m_fYaw, m_fPitch, 0.0f);
        D3DXVec3TransformNormal(&m_vForward, &vForward, &m_mtxRot);

        D3DXVec3TransformNormal(&m_vLeft, &vLeft, &m_mtxRot);
        D3DXVec3TransformNormal(&m_vUp, &vUp, &m_mtxRot);
        // どの方向を向いても前進速度一定にするためのX軸ベクトル
        D3DXMatrixRotationYawPitchRoll(&mtxRot, m_fYaw, 0.0f, 0.0f);
        D3DXVec3TransformNormal(&m_vAt, &vAt, &mtxRot);

        // マウスを画面の中心に置く
        CManager::GetInputMouse()->SetMousePos(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));

        if (move) {

            // 移動量加算
            m_vCameraPos += force;

            m_WalkInterval++;

            if (m_WalkInterval > 20)
                m_WalkInterval = 0;

            if (m_WalkInterval == 0)
            {
                if (rand() % 2 == 0)
                    CManager::GetSound()->PlaySound(SOUND_LABEL_SE_WALK02);
                else
                    CManager::GetSound()->PlaySound(SOUND_LABEL_SE_WALK04);
            }
        }
        else
        {
            if (m_OldMove)
            {
                if (rand() % 2 == 0)
                    CManager::GetSound()->PlaySound(SOUND_LABEL_SE_WALK02);
                else
                    CManager::GetSound()->PlaySound(SOUND_LABEL_SE_WALK04);
            }

            m_WalkInterval = 0;
            
        }

        m_OldMove = move;

        //注視点移動
        m_vCameraAt = m_vCameraPos + m_vForward;

        m_SwingTime += 2.0f;
        m_SwingAmp *= 0.9f;
        m_vCameraPos.y += m_SwingAmp * sinf(m_SwingTime);
        m_vCameraPos.x += m_SwingAmp * cosf(m_SwingTime * 1.5f);
        m_vCameraPos.z += m_SwingAmp * cosf(m_SwingTime * 2.0f);
    }


    //デバッグ表示
#ifdef _DEBUG
    char text[1024];
    sprintf(text,"CameraPos  X : %f  Y : %f  Z : %f \n",m_vCameraPos.x,m_vCameraPos.y,m_vCameraPos.z);
    CDebug::AddText(text);
#endif

}

//*************************************************************************************************
// 描画処理
//*************************************************************************************************
void CCamera::Draw(void)
{
}

//*************************************************************************************************
// カメラの設定
//*************************************************************************************************
void CCamera::SetCamera(void)
{
    //---------------------------------------------------------------------------------------------
    // 変数宣言開始
    //---------------------------------------------------------------------------------------------
    D3DXMATRIX mtxView;                             //ビュー行列
    D3DXMATRIX mtxViewInv;                          //ビュー逆行列
    D3DXMATRIX mtxProj;                             //プロジェクション行列
    D3DXMATRIX mtxWorld;
    D3DXMATRIX mtxTrans;
    D3DXVECTOR3 vDir = D3DXVECTOR3(0.0f,0.0f,1.0f);
    float fLength;
    LPDIRECT3DDEVICE9 pDevice;
    //---------------------------------------------------------------------------------------------
    // 変数宣言終了
    //---------------------------------------------------------------------------------------------
     //デバイスの取得
    pDevice = CManager::GetRenderer()->GetDevice();

    //ビュー行列
    D3DXMatrixLookAtLH(&mtxView,&m_vCameraPos,&m_vCameraAt,&m_vUp);     //ビュー行列の作成(左手座標)
    D3DXMatrixInverse (&m_mtxViewInv,NULL,&mtxView);                    //ビュー行列の逆行列の作成

    pDevice->SetTransform(D3DTS_VIEW,&mtxView);                         //ビュー行列を移動できるようにする

    //プロジェクション行列
    D3DXMatrixPerspectiveFovLH(&mtxProj,D3DX_PI / 3.0f,(float)SCREEN_WIDTH / SCREEN_HEIGHT,1.0f,3000.0f);//画角,アスペクト比,near,far
    pDevice->SetTransform(D3DTS_PROJECTION,&mtxProj);    //プロジェクション行列を移動できるようにする

}

//*************************************************************************************************
// カメラ位置の取得
//*************************************************************************************************
D3DXVECTOR3 CCamera::GetCameraPos(void)
{
    return m_vCameraPos;
}

//*************************************************************************************************
// カメラ方向の取得
//*************************************************************************************************
D3DXVECTOR3 CCamera::GetCameraForward(void)
{
    return m_vForward;
}

//*************************************************************************************************
// カメラ方向の取得
//*************************************************************************************************
D3DXVECTOR3 CCamera::GetCameraLeft(void)
{
    return m_vLeft;
}

//*************************************************************************************************
// カメラ方向の取得
//*************************************************************************************************
D3DXVECTOR3 CCamera::GetCameraUp(void)
{
    return m_vUp;
}

//*************************************************************************************************
// カメラ回転行列の取得
//*************************************************************************************************
D3DXMATRIX CCamera::GetCameraRotMat(void)
{
    return m_mtxRot;
}

//*************************************************************************************************
// ビューの逆行列の取得
//*************************************************************************************************
D3DXMATRIX CCamera::GetViewInverse(void)
{
    return m_mtxViewInv;
}