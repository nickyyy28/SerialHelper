#include "mainwindow.h"
#include <QApplication>
//#include <opencv2/opencv.hpp>
//#include <Eigen/Dense>

//using namespace std;
//using namespace cv;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
