//
// Created by Administrator on 2020/1/30.
//

#include "QtGUI.h"
#include "FaceDetector.h"

int main(int argc, char *argv[]) {
//    QApplication application(argc, argv);
//    QtGUI gui = QtGUI();
//    auto mainWidget = gui.createMainWidget();
//    mainWidget->show();
//    return application.exec();
//   上面的GUI总是出问题，之后再debug，用GUI的原因是可以实现GUI和detector在不同线程
// 下面不依赖于GUI作为控制
    std::cout << "start main\n";
    //FaceDetector faceDetector = FaceDetector(Dlib_frontal_face_detector);
    FaceDetector faceDetector = FaceDetector(Opencv_CascadeClassifier, true);
    faceDetector.Run();
}