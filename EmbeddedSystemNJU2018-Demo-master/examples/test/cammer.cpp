//
// Created by hp on 2018/9/25.
//

#include<opencv2/opencv.hpp>
#include<iostream>
using namespace cv;
using namespace std;

int main() {
    VideoCapture cap;
    cap.open("D:\\BaiduNetdiskDownload\\一拳\\1.mp4"); //打开视频,等价于   VideoCapture cap("E://2.avi");

    if (!cap.isOpened())
        return;

    int width = cap.get(CV_CAP_PROP_FRAME_WIDTH);  //帧宽度
    int height = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //帧高度
    int frameRate = cap.get(CV_CAP_PROP_FPS);  //帧率 x frames/s
    int totalFrames = cap.get(CV_CAP_PROP_FRAME_COUNT); //总帧数

    cout << "视频宽度=" << width << endl;
    cout << "视频高度=" << height << endl;
    cout << "视频总帧数=" << totalFrames << endl;
    cout << "帧率=" << frameRate << endl;

    Mat frame;
    while (1) {
        cap >> frame;//等价于cap.read(frame);
        if (frame.empty())
            break;
        imshow("video", frame);
        if (waitKey(20) > 0)
            break;
    }
    cap.release();
    return 0;
}
