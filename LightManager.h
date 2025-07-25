/**
 * @file LightManager.h
 
 * @brief light를 기능시키는 클래스
 * @version 0.1
 * @date 2023-12-06
 * 

 * 
 */

#ifndef LIGHTMANAGER_H
#define LIGHTMANAGER_H

/**
 * @brief Light를 기능시키는 클래스
 * 
 */
class LightManager
{
public:
    LightManager();
    void SwapLight();
private:
    bool isLightOn = true;
};

#endif // LIGHTMANAGER_H
