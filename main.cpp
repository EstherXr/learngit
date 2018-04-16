#include "widget.h"
#include <QApplication>
#include "malloc.h"
//#include "sizeyunsuan.h"
//#include "mathematic.h"
//#include "wen.h"

using namespace std;

extern "C++"
{
#include "Operation.h"
}

QTimer *timer=NULL;
QTimer *tim=NULL;
int *score=NULL;  //score is used for saveing the score of each exercise
extern int total;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();    //displaying UI

    return a.exec();
}
