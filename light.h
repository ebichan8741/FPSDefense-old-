//================================================================================================
//
// åıåπêßå‰èàóù [light.h]
// Author : TAKUYA EBIHARA
//
//================================================================================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

class CLight
{
public:
    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    void SetLight(void);

};
#endif