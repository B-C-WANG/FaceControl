# FaceControl
使用人脸识别数据进行鼠标控制
## 开发
### 工具
- C++，opencv，dlib
### 框架
- GUI中control panel负责控制函数调用，以及配置（如启动、暂停和关闭face detection）
- GUI中status panel负责展示数据（如展示面部位置，眨眼特征参数，yaw roll pitch等）
- 后台运行调用摄像头，人脸检测和特征点识别，pose求解等程序
## 流程、方案和算法
### 流程
1. 读取camera矩阵数据，使用人脸识别得到人脸的bounding box
2. 将bounding box内的image放入特征点检测，得到所有特征点的xy坐标
3. 根据xy坐标求得眼眶数据，脸部方向等数据
### 方案
1. 脸部检测方案，如果使用dlib::frontal_face_detector，耗时是特征点检测的20倍，不能满足fps要求，因此face检测使用Opencv的CascadeClassifier，可以近似达到30fps
2. 特征点检测可以使用dlib::shape_predictor，基本能够满足效率要求，不是性能瓶颈
### 算法
#### 眨眼检测
1. 睁开眼睛的状态下，录10秒钟的数据
2. 闭上眼睛的状态下，录10秒钟的数据
3. 得到睁眼和闭眼状态的正态分布，以3 sigma作为上下界，划分出来一个闭眼和睁眼的值域，这个区域可能会有交叉，以小于min(闭眼上界，睁眼下界)为闭眼，大于max(闭眼上界，睁眼下界)为睁眼进行判断
- 存在的问题：不同的头部姿态会有不同的睁眼和闭眼分布，因此在另一个姿态可能难以判断
#### 头部姿态检测
- 使用Opencv:SolvePNP求解Pose
- 参考https://blog.csdn.net/jacke121/article/details/102834801
- 参考https://blog.csdn.net/ChuiGeDaQiQiu/article/details/88623267


## API
### getRotateState
- 获得人脸旋转信息，目前仅包含上下左右0,1之间的数值，未来最好是yaw roll pitch更好
### onLeftEyeClose
- 左眼眨眼时的触发
## 其他内容
### 参考
- 眨眼检测：https://blog.csdn.net/Feeryman_Lee/article/details/103202788
- 头部姿态：https://blog.csdn.net/ChuiGeDaQiQiu/article/details/88623267，https://blog.csdn.net/jacke121/article/details/102834801

### debug
- cv::CV_8UC3的问题：opencv没有配置好，即使没有这个相关的bug，后面也会遇到link问题
- main程序没有运行，直接exit code -XXXXXXX：到cmake build debug文件夹中找到二进制程序，运行一下，应该是会报错找不到哪个dll，到CMAKELIST中设置的bin中复制相应的dll到exe文件同一目录下，然后运行测试，成功后再在ide中跑