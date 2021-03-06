# FaceControl
- 最终功能：基于人脸识别数据进行鼠标控制——根据头部pose移动鼠标，眨眼控制鼠标左右点击
## 子项目
- MyImplCpp：自己使用cpp+opencv+dlib实现了一个，但是准确性较差，难以实用
- 3DDFAImpl：使用https://github.com/cleardusk/3DDFA作为检测引擎，脸部landmark和头部pose估计相对准确,但是对于闭眼难以识别,
## 子项目：3DDFAImpl
- 在ubuntu下测试通过，windows下因dlib安装问题并未测试
- 安装配置：查看子项目文件夹README.md，或原仓库https://github.com/cleardusk/3DDFA的README
- 本项目下的3DDFA需要额外下载原仓库有的models,train.config以及test.config信息,在release中可以下载压缩文件,解压到3DDFAImpl可直接使用
### 效果展示
![](./1.gif)
### 运行
- 运行cameraRun可以看到landmark以及头部pose信息,但是landmark仍然不能用于眨眼检测


## 子项目：MyImplCpp
### 效果展示
![](./fig1.png)

### 开发
#### 工具及安装配置
- C++，opencv，dlib，qt
- opencv，dlib和qt的安装配置见CMakeLists.txt注释
##### qtcharts配置
- windows下建议重新安装Qt，安装时选中Qt Charts模块
- 其他平台编译：参考：https://blog.csdn.net/qq_24489251/article/details/100035674
- 重要：如果是qt的项目，需要在.pro文件中添加QtCharts，而对于mingw项目，在CMakeList中改成find package以及target link中添加Qt5 Charts，具体查看CMakeLists中的描述
- qtcharts教程：https://doc.qt.io/archives/qt-5.13/qtcharts-examples.html
#### 框架
- GUI中使用Qt开发control panel负责控制函数调用，以及配置（如启动、暂停和关闭face detection）
- GUI中使用QtCharts开发status panel负责展示数据（如展示面部位置，眨眼特征参数，yaw roll pitch等）
- 后台运行调用摄像头，人脸检测和特征点识别，pose求解等程序
### 流程、方案和算法
#### 流程
1. 读取camera矩阵数据，使用人脸识别得到人脸的bounding box
2. 将bounding box内的image放入特征点检测，得到所有特征点的xy坐标
3. 根据xy坐标求得眼眶数据，脸部方向等数据
4. 数据考虑经过滤波或者平滑等处理过后展示，以及用于后续的pose等数据的最终检测
#### 方案
1. 脸部检测方案，如果使用dlib::frontal_face_detector，耗时是特征点检测的20倍，不能满足fps要求，因此face检测使用Opencv的CascadeClassifier，可以近似达到30fps
2. 特征点检测可以使用dlib::shape_predictor，基本能够满足效率要求，不是性能瓶颈
3. 脸部pose使用SolvePNP方法
#### 算法
##### 眨眼检测
1. 睁开眼睛的状态下，录10秒钟的数据
2. 闭上眼睛的状态下，录10秒钟的数据
3. 得到睁眼和闭眼状态的正态分布，以3 sigma作为上下界，划分出来一个闭眼和睁眼的值域，这个区域可能会有交叉，以小于min(闭眼上界，睁眼下界)为闭眼，大于max(闭眼上界，睁眼下界)为睁眼进行判断
- 存在的问题：不同的头部姿态会有不同的睁眼和闭眼分布，因此在另一个姿态可能难以判断
##### 头部姿态检测
- 使用Opencv:SolvePNP求解Pose
- 参考https://blog.csdn.net/jacke121/article/details/102834801
- 参考https://blog.csdn.net/ChuiGeDaQiQiu/article/details/88623267

### 改进方向
- 有很多其他库实现的更加准确、更高效和平滑的脸部识别，比如https://github.com/cleardusk/3DDFA，可以尝试用深度学习的方法改进
- 有很多现成的脸部识别特征点修改3D或live2d模型软件，能够非常准确地实现这样的功能，可以运行这些非开源的软件，尝试使用特殊的纯色模型通过图像识别来得到相关的信息
### 其他内容
#### 参考
- 眨眼检测：https://blog.csdn.net/Feeryman_Lee/article/details/103202788
- 头部姿态：https://blog.csdn.net/ChuiGeDaQiQiu/article/details/88623267，https://blog.csdn.net/jacke121/article/details/102834801
#### debug记录
- main程序没有运行，直接exit code -XXXXXXX：到cmake build debug文件夹中找到二进制程序，运行一下，应该是会报错找不到哪个dll，到CMAKELIST中设置的bin中复制相应的dll到exe文件同一目录下，然后运行测试，成功后再在ide中跑
- 提示继承自QWidget的对象有虚函数没有实现：将Q_OBJECT宏删掉，此时connect函数不能使用，改成QObject::connect，具体查看相应位置的代码
