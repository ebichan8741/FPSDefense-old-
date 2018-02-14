//================================================================================================
//
// �J�������䏈�� [camera.cpp]
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
// �萔��`
//=================================================================================================
#define _Gravity    (9.8f)      // �d��
#define JUMP_FORCE  (100.0f)     // �W�����v��

//*************************************************************************************************
// ����������
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
// �I������
//*************************************************************************************************
void CCamera::Uninit(void)
{
}

//*************************************************************************************************
// �X�V����
//*************************************************************************************************
void CCamera::Update(void)
{
    // �ϐ��錾 -----------------------------------------------------------------------------------
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
        //�f�o�C�X�̎擾
        pDevice = CManager::GetRenderer()->GetDevice();

        force = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        move = false;

        // �L�[���͏����擾 /////////////////////////////////////////////////////////////////////////
        //�O�i
        if (CManager::GetInputKeyboard()->GetKeyPress(DIK_W)) {
            force.x += m_vAt.x;
            force.z += m_vAt.z;
            move = true;
            // �X�v�����g
            if (CManager::GetInputKeyboard()->GetKeyPress(DIK_LSHIFT) ||
                CManager::GetInputKeyboard()->GetKeyPress(DIK_RSHIFT)) {
                force.x += m_vAt.x;
                force.z += m_vAt.z;
            }
        }
        //���
        if (CManager::GetInputKeyboard()->GetKeyPress(DIK_S)) {
            force.x -= m_vAt.x;
            force.z -= m_vAt.z;
            move = true;
        }
        //���ړ�
        if (CManager::GetInputKeyboard()->GetKeyPress(DIK_A)) {
            force.x -= m_vLeft.x;
            force.z -= m_vLeft.z;
            move = true;
        }
        //�E�ړ�
        if (CManager::GetInputKeyboard()->GetKeyPress(DIK_D)) {
            force.x += m_vLeft.x;
            force.z += m_vLeft.z;
            move = true;
        }

        //�㏸
        if (CManager::GetInputKeyboard()->GetKeyPress(DIK_E)) {
            m_vCameraPos.y += 0.5f;
        }

        //���~
        if (CManager::GetInputKeyboard()->GetKeyPress(DIK_Q)) {
            m_vCameraPos.y -= 0.5f;
        }

        //�J�������[�h�ؑ�
        if (CManager::GetInputKeyboard()->GetKeyTrigger(DIK_H)) {
            m_bFps = !m_bFps;
        }

        // FPS���[�h
        if (m_bFps == true && CManager::GetMode() == MODE_GAME) {
            
            bool jump = m_Jump;

            // �d�͌v�Z
            if (m_vCameraPos.y >= CGameMode::GetMeshField()->GetHeight(m_vCameraPos) + 10) {
                m_Force -= _Gravity;

            }
            else {
                m_Jump = false;
                m_vCameraPos.y = CGameMode::GetMeshField()->GetHeight(m_vCameraPos) + 10;
            }
            // �W�����v
            if (CManager::GetInputKeyboard()->GetKeyTrigger(DIK_SPACE) && m_Jump == false) {
                m_Force = JUMP_FORCE;
                m_Jump = true;
            }

            // ���n��
            if (jump == true && m_Jump == false) {
                CManager::GetSound()->PlaySound(SOUND_LABEL_SE_TYAKUTI);
            }

            m_vCameraPos.y += m_Force * dt;
        }

        // �}�E�X����ʒ�������ǂꂾ���ړ����������擾���A�l��ω�������
        // Yaw��Y���𒆐S�Ƃ�������]
        m_fYaw += (CManager::GetInputMouse()->GetMousePos().x - SCREEN_WIDTH / 2) / 800;
        // Pitch��X���𒆐S�Ƃ����c��]
        m_fPitch += (CManager::GetInputMouse()->GetMousePos().y - SCREEN_HEIGHT / 2) / 800;

        //�㉺��]����
        if (m_fPitch < -(D3DX_PI / 2)) {
            m_fPitch = -(D3DX_PI / 2);
        }
        else if (m_fPitch > (D3DX_PI / 2)) {
            m_fPitch = D3DX_PI / 2;
        }

        // Yaw��Pitch����O���̍��W�ƍ����A�㑤�̍��W���擾
        D3DXMatrixRotationYawPitchRoll(&m_mtxRot, m_fYaw, m_fPitch, 0.0f);
        D3DXVec3TransformNormal(&m_vForward, &vForward, &m_mtxRot);

        D3DXVec3TransformNormal(&m_vLeft, &vLeft, &m_mtxRot);
        D3DXVec3TransformNormal(&m_vUp, &vUp, &m_mtxRot);
        // �ǂ̕����������Ă��O�i���x���ɂ��邽�߂�X���x�N�g��
        D3DXMatrixRotationYawPitchRoll(&mtxRot, m_fYaw, 0.0f, 0.0f);
        D3DXVec3TransformNormal(&m_vAt, &vAt, &mtxRot);

        // �}�E�X����ʂ̒��S�ɒu��
        CManager::GetInputMouse()->SetMousePos(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));

        if (move) {

            // �ړ��ʉ��Z
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

        //�����_�ړ�
        m_vCameraAt = m_vCameraPos + m_vForward;

        m_SwingTime += 2.0f;
        m_SwingAmp *= 0.9f;
        m_vCameraPos.y += m_SwingAmp * sinf(m_SwingTime);
        m_vCameraPos.x += m_SwingAmp * cosf(m_SwingTime * 1.5f);
        m_vCameraPos.z += m_SwingAmp * cosf(m_SwingTime * 2.0f);
    }


    //�f�o�b�O�\��
#ifdef _DEBUG
    char text[1024];
    sprintf(text,"CameraPos  X : %f  Y : %f  Z : %f \n",m_vCameraPos.x,m_vCameraPos.y,m_vCameraPos.z);
    CDebug::AddText(text);
#endif

}

//*************************************************************************************************
// �`�揈��
//*************************************************************************************************
void CCamera::Draw(void)
{
}

//*************************************************************************************************
// �J�����̐ݒ�
//*************************************************************************************************
void CCamera::SetCamera(void)
{
    //---------------------------------------------------------------------------------------------
    // �ϐ��錾�J�n
    //---------------------------------------------------------------------------------------------
    D3DXMATRIX mtxView;                             //�r���[�s��
    D3DXMATRIX mtxViewInv;                          //�r���[�t�s��
    D3DXMATRIX mtxProj;                             //�v���W�F�N�V�����s��
    D3DXMATRIX mtxWorld;
    D3DXMATRIX mtxTrans;
    D3DXVECTOR3 vDir = D3DXVECTOR3(0.0f,0.0f,1.0f);
    float fLength;
    LPDIRECT3DDEVICE9 pDevice;
    //---------------------------------------------------------------------------------------------
    // �ϐ��錾�I��
    //---------------------------------------------------------------------------------------------
     //�f�o�C�X�̎擾
    pDevice = CManager::GetRenderer()->GetDevice();

    //�r���[�s��
    D3DXMatrixLookAtLH(&mtxView,&m_vCameraPos,&m_vCameraAt,&m_vUp);     //�r���[�s��̍쐬(������W)
    D3DXMatrixInverse (&m_mtxViewInv,NULL,&mtxView);                    //�r���[�s��̋t�s��̍쐬

    pDevice->SetTransform(D3DTS_VIEW,&mtxView);                         //�r���[�s����ړ��ł���悤�ɂ���

    //�v���W�F�N�V�����s��
    D3DXMatrixPerspectiveFovLH(&mtxProj,D3DX_PI / 3.0f,(float)SCREEN_WIDTH / SCREEN_HEIGHT,1.0f,3000.0f);//��p,�A�X�y�N�g��,near,far
    pDevice->SetTransform(D3DTS_PROJECTION,&mtxProj);    //�v���W�F�N�V�����s����ړ��ł���悤�ɂ���

}

//*************************************************************************************************
// �J�����ʒu�̎擾
//*************************************************************************************************
D3DXVECTOR3 CCamera::GetCameraPos(void)
{
    return m_vCameraPos;
}

//*************************************************************************************************
// �J���������̎擾
//*************************************************************************************************
D3DXVECTOR3 CCamera::GetCameraForward(void)
{
    return m_vForward;
}

//*************************************************************************************************
// �J���������̎擾
//*************************************************************************************************
D3DXVECTOR3 CCamera::GetCameraLeft(void)
{
    return m_vLeft;
}

//*************************************************************************************************
// �J���������̎擾
//*************************************************************************************************
D3DXVECTOR3 CCamera::GetCameraUp(void)
{
    return m_vUp;
}

//*************************************************************************************************
// �J������]�s��̎擾
//*************************************************************************************************
D3DXMATRIX CCamera::GetCameraRotMat(void)
{
    return m_mtxRot;
}

//*************************************************************************************************
// �r���[�̋t�s��̎擾
//*************************************************************************************************
D3DXMATRIX CCamera::GetViewInverse(void)
{
    return m_mtxViewInv;
}