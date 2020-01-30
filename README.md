# FaceControl
使用人脸识别数据进行鼠标控制
## 开发框架
- 使用C++，opencv
## 算法
- 确定中心点，使用基于中心点的个点直角坐标的特征进行判断
- 先需要转向到极致，然后找到边界条件，包括上下左右的边界条件
## API
### getRotateState
- 获得人脸旋转信息，目前仅包含上下左右0,1之间的数值，未来最好是yaw roll pitch更好
### onLeftEyeClose
- 左眼眨眼时的触发

## 参考
- https://blog.csdn.net/Feeryman_Lee/article/details/103202788

## 常见bug
- cv::CV_8UC3的问题：发现是opencv的源文件的问题，可能版本太新了