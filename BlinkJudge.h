//
// Created by Administrator on 2020/1/31.
//

#ifndef FACECONTROL_BLINKJUDGE_H
#define FACECONTROL_BLINKJUDGE_H

#include "FaceDetector.h"

class BlinkJudge {
public:
    bool isLeftEyeBlinked;
    bool isRightEyeBlinked;
// 眨眼的阈值，需要先大于这个值，然后小于这个值，然后再次大于这个值，就算作眨眼
    float blinkThreshold = 0.1;

    bool judgeIfBlinkEye(EyeType eye, double nowEyeFeature);
    // 多线程引用
    FaceDetector & faceDetector;

    // 收集睁开眼睛的数据
    void WarmOpenEyes();
    // 收集闭上眼睛的数据
    void WarmCloseEyes();


private:

    bool leftEyeLoaded;// 当眼睛闭上，小于blinkThreshold就是装载了，当眼睛睁开，大于上面的阈值，如果装载了就是能够发射，然后算作一次眨眼
    bool rightEyeLoaded;
};


#endif //FACECONTROL_BLINKJUDGE_H
