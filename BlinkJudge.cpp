//
// Created by Administrator on 2020/1/31.
//

#include "BlinkJudge.h"
bool BlinkJudge::judgeIfBlinkEye(EyeType eye, double nowEyeFeature) {
    // 眨眼的算法
    // 如果特征值小于阈值，那么算作装载（完全闭上了眼睛）
    if (nowEyeFeature < blinkThreshold) {
        if (eye == Left)leftEyeLoaded = true;
        else if (eye == Right)rightEyeLoaded = true;
        return false;
    }
        // 特征值大于阈值，如果已经装载，那么就发射（算是睁开了眼睛），否则无视
    else {
        if (eye == Left && leftEyeLoaded) {
            leftEyeLoaded = false;
            return true;
        } else if (eye == Right && rightEyeLoaded) {
            rightEyeLoaded = false;
            return true;
        }
        return false;
    }
}
