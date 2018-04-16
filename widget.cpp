#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>
#include <QDebug>
#include <QTimer>
#include <QFile>
#include <Operation.h>

#define TIME 22000

extern "C++"
{
#include "Operation.h"
}

using namespace std;
int num=0;
extern QTimer *timer;
extern QTimer *tim;
int circle=1,total,sum=0,error,grade;  //ciecle is current number of exercise,total is the total number of exercises,sum is the total score
extern int *score;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->pushButton_4->setVisible(false);
    ui->pushButton_5->setVisible(false);//at the beginning,hide "xia yi ti" and "ti jiao" button
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()//OK Button
{
    QString a=tr("叮咚！题目已生成:");
    QString b=ui->lineEdit_2->text();
    QString str=a+b;
    QString exe;

    bool ok;
    int sign;
    total=b.toInt(&ok,10); //将字符串转换为int整形数据

    int numOfQuestion;
    numOfQuestion=ui->lineEdit_2->text().toInt(&ok,10);//问题数量

    bool add;
    add=ui->checkBox_6->checkState();//是否支持加法运算

    bool sub;
    sub=ui->checkBox_7->checkState();//是否支持减法运算

    bool mul;
    mul=ui->checkBox_8->checkState();//是否支持乘法运算

    bool div;
    div=ui->checkBox_9->checkState();//是否支持除法运算

    bool decimal;
    decimal=ui->checkBox_3->checkState();//是否支持小数运算

    bool real;
    real=ui->checkBox_4->checkState();//是否支持真分数运算

    bool power;
    power=ui->checkBox_5->checkState();//是否支持乘方运算

    bool brackets;
    brackets=ui->checkBox_10->checkState();//是否有括号

    bool cuoti;
    cuoti=ui->checkBox->checkState();
    if(cuoti==true)
        error=1;
    else
        error=0;

    bool chengji;
    chengji=ui->checkBox_2->checkState();
    if(chengji==true)
        grade=1;
    else
        grade=0;

    int numOfOperand;
    numOfOperand=ui->lineEdit->text().toInt(&ok,10);//操作数的个数
    double rangeOfQuestion;
    rangeOfQuestion=ui->lineEdit_3->text().toDouble();//操作数的最大值
    int precision;
    precision=ui->lineEdit_7->text().toInt(&ok,10);//小数精度

    Setting(numOfOperand,numOfQuestion,rangeOfQuestion,add,sub,mul,div,power,brackets,precision,real,decimal);
    GenerateAndCalc();
    /*if(sign==-1)
    {
        QMessageBox::warning(this, tr("提示"),tr("非法输入"));
        ui->lineEdit_2->clear();    //clear lineEdit_2, make users fill in the number of exercise again
        ui->lineEdit->clear();
        ui->lineEdit_3->clear();
        ui->lineEdit_6->clear();
        ui->lineEdit_7->clear();
        ui->checkBox->setCheckState(Qt::Unchecked);
        ui->checkBox_2->setCheckState(Qt::Unchecked);
        ui->checkBox_3->setCheckState(Qt::Unchecked);
        ui->checkBox_4->setCheckState(Qt::Unchecked);
        ui->checkBox_5->setCheckState(Qt::Unchecked);
        ui->checkBox_6->setCheckState(Qt::Unchecked);
        ui->checkBox_7->setCheckState(Qt::Unchecked);
        ui->checkBox_8->setCheckState(Qt::Unchecked);
        ui->checkBox_9->setCheckState(Qt::Unchecked);
        return;
    }//how to deal with error input?
    */

    score=(int *)malloc(sizeof(int)*total);  //allot saving spaces
    if(total==1)
        ui->pushButton_4->setEnabled(false);//if there is one exercise,disenable "xia yi ti" button
    for(int i=0;i<total;i++)
        score[i]=0;

    exe=QString(getExpression(circle-1));
    exe=tr("题目1:")+exe;
    ui->label_3->setText(exe);
    ui->lineEdit_2->clear();   //clear lineEdit_2, and users put the answer in it
    ui->lineEdit->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_7->clear();
    ui->pushButton->setVisible(false);
    ui->pushButton_2->setVisible(false);  //make "OK" and "Cancel" buttons unvisible
    ui->lineEdit->setEnabled(false);
    ui->lineEdit_3->setEnabled(false);
    ui->lineEdit_7->setEnabled(false);
    ui->checkBox->setEnabled(false);
    ui->checkBox_2->setEnabled(false);
    ui->checkBox_3->setEnabled(false);
    ui->checkBox_4->setEnabled(false);
    ui->checkBox_5->setEnabled(false);
    ui->checkBox_6->setEnabled(false);
    ui->checkBox_7->setEnabled(false);
    ui->checkBox_8->setEnabled(false);
    ui->checkBox_9->setEnabled(false);
    ui->checkBox_10->setEnabled(false);
    ui->pushButton_4->setVisible(true);
    ui->pushButton_5->setVisible(true);   //make "xia yi ti" and "ti jiao" buttons visible
    QMessageBox::information(this, tr("提示"), str);//tell users the exercise have been placed

    timer = new QTimer(this);
    tim = new  QTimer(this);

    connect(timer, SIGNAL(timeout()), this, SLOT(showTimelimit()));
    connect(tim, SIGNAL(timeout()), this, SLOT(on_pushButton_4_clicked()));

    timer->start(1000);
    tim->start(TIME);
    if(total==1)
    {
        disconnect(tim, SIGNAL(timeout()), this, SLOT(on_pushButton_4_clicked()));
        connect(tim, SIGNAL(timeout()), this, SLOT(on_pushButton_5_clicked()));
    }

    if(num>=20)
    {
        timer->stop();
 //       tim->stop();
    }
}

void Widget::on_pushButton_2_clicked()//Cancel
{
    ui->lineEdit_2->clear();    //clear lineEdit_2, make users fill in the number of exercise again
    ui->lineEdit->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_7->clear();
    ui->checkBox->setCheckState(Qt::Unchecked);
    ui->checkBox_2->setCheckState(Qt::Unchecked);
    ui->checkBox_3->setCheckState(Qt::Unchecked);
    ui->checkBox_4->setCheckState(Qt::Unchecked);
    ui->checkBox_5->setCheckState(Qt::Unchecked);
    ui->checkBox_6->setCheckState(Qt::Unchecked);
    ui->checkBox_7->setCheckState(Qt::Unchecked);
    ui->checkBox_8->setCheckState(Qt::Unchecked);
    ui->checkBox_9->setCheckState(Qt::Unchecked);
    ui->checkBox_10->setCheckState(Qt::Unchecked);
}

void Widget::on_pushButton_3_clicked()//Reset
{
    timerinit();
    timinit();

    ui->pushButton->setVisible(true);
    ui->pushButton_2->setVisible(true);//make "OK" and "Cancel" buttons visible
    ui->pushButton_4->setVisible(false);
    ui->pushButton_5->setVisible(false);//make "xia yi ti" and "ti jiao" buttons unvisible
    ui->lineEdit_2->clear();   //clear lineEdit_2, make users fill in the number of exercise again
    ui->label_3->setText(tr("请在下行中输入题目总数"));
    ui->pushButton_4->setEnabled(true);
    ui->pushButton_5->setEnabled(true);
    ui->lineEdit->setEnabled(true);
    ui->lineEdit_3->setEnabled(true);
    ui->lineEdit_7->setEnabled(true);
    ui->checkBox->setEnabled(true);
    ui->checkBox_2->setEnabled(true);
    ui->checkBox_3->setEnabled(true);
    ui->checkBox_4->setEnabled(true);
    ui->checkBox_5->setEnabled(true);
    ui->checkBox_6->setEnabled(true);
    ui->checkBox_7->setEnabled(true);
    ui->checkBox_8->setEnabled(true);
    ui->checkBox_9->setEnabled(true);
    ui->checkBox_10->setEnabled(true);
    ui->checkBox->setCheckState(Qt::Unchecked);
    ui->checkBox_2->setCheckState(Qt::Unchecked);
    ui->checkBox_3->setCheckState(Qt::Unchecked);
    ui->checkBox_4->setCheckState(Qt::Unchecked);
    ui->checkBox_5->setCheckState(Qt::Unchecked);
    ui->checkBox_6->setCheckState(Qt::Unchecked);
    ui->checkBox_7->setCheckState(Qt::Unchecked);
    ui->checkBox_8->setCheckState(Qt::Unchecked);
    ui->checkBox_9->setCheckState(Qt::Unchecked);
    ui->checkBox_10->setCheckState(Qt::Unchecked);
    ui->label_2->clear();
    circle=1;
    sum=0;
    if(score!=NULL)
    {
        free(score);
        score=NULL;
    }
}

void Widget::on_pushButton_4_clicked()//下一题按钮
{
    timerinit();
    timinit();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTimelimit()));
    tim = new QTimer(this);
    connect(tim, SIGNAL(timeout()), this, SLOT(on_pushButton_4_clicked()));

    timer->start(1000);
    tim->start(TIME);

    if(num>=20)
    {
        timer->stop();
//        tim->stop();
    }

    circle++;
    QString c="题目";
    QString d=QString::number(circle, 10);
    QString str1;
    QString e,exe,g;
    string f;

    if(circle<=total)
    {
        if(circle==total)
        {
            ui->pushButton_4->setEnabled(false);
            disconnect(timer, SIGNAL(timeout()), this, SLOT(showTimelimit()));
            disconnect(tim, SIGNAL(timeout()), this, SLOT(on_pushButton_4_clicked()));
            connect(timer, SIGNAL(timeout()), this, SLOT(showTimelimit()));
            connect(tim, SIGNAL(timeout()), this, SLOT(on_pushButton_5_clicked()));
        }
        exe= QString(getExpression(circle-1));
        str1=c+d+":"+exe;
        ui->label_3->setText(str1);
        e=ui->lineEdit_2->text();
        g=QString(getAnswer(circle-2));
        if(QString::compare(e,g)==0)
            score[circle-2]=1;
        else
        {
            score[circle-2]=0;    //judge whether the answer is correct
            if(error==1)
            {
                memory();
            }
        }
        ui->lineEdit_2->clear();
    }
}

void Widget::on_pushButton_5_clicked()//hand in button
{
    timerinit();
    timinit();

    QString a,b,e,str,f;
    int i;
    e=ui->lineEdit_2->text();
    f=QString(getAnswer(circle-1));
    if(QString::compare(e,f)==0)
        score[circle-1]=1;
    else
    {
        score[circle-1]=0;    //judge whether the answer is correct
        if(error==1)
        {
            circle++;
            memory();
            circle--;
        }
    }
    ui->lineEdit_2->clear();
    for(i=0;i<total;i++)
    {
        sum+=score[i];
    }

    if(grade==1)
    {
        qDebug()<<"grade save";
        grade_memory();
    }
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_5->setEnabled(false);
    ui->checkBox->setCheckState(Qt::Unchecked);
    ui->checkBox_2->setCheckState(Qt::Unchecked);
    ui->checkBox_3->setCheckState(Qt::Unchecked);
    ui->checkBox_4->setCheckState(Qt::Unchecked);
    ui->checkBox_5->setCheckState(Qt::Unchecked);
    ui->checkBox_6->setCheckState(Qt::Unchecked);
    ui->checkBox_7->setCheckState(Qt::Unchecked);
    ui->checkBox_8->setCheckState(Qt::Unchecked);
    ui->checkBox_9->setCheckState(Qt::Unchecked);
    ui->checkBox_10->setCheckState(Qt::Unchecked);
    ui->label_3->clear();
    ui->label_2->clear();
    a=tr("提交成功！您的成绩为:");
    b=QString::number(sum, 10);
    str=a+b;
    QMessageBox::information(this, tr("提示"), str);  //give the score
}

void Widget::showTimelimit(){
    QString a;
    a=QString::number(20-num,10);
    ui->label_2->setText(a);
    if(num>=20)
    {
        timer->stop();
        return;
    }
    num++;
}

void Widget::timerinit(){
    if(timer!=NULL){
        timer->stop();
        delete timer;
        timer=NULL;
    }
    num=0;
//    return;
}

void Widget::timinit(){
    if(tim!=NULL){
        tim->stop();
        delete tim;
        tim=NULL;
    }
    num=0;
}

void Widget::memory(){
    int c;
    qDebug()<<circle;
    qDebug()<<"qitianhaodashabi";
    c=circle-2;
    qDebug()<<c;

    QFile file("wa.txt");

    //判断文件是否存在
    if(file.exists()){
        qDebug()<<"The file exists";
    }else{
        qDebug()<<"The file does not exist";
    }
    //已读写方式打开文件，
    //如果文件不存在会自动创建文件
    if(!file.open(QIODevice::ReadWrite)){
        qDebug()<<"打开失败";
    }else{
        qDebug()<<"打开成功";
    }

    //获得文件大小
    qint64 pos;
    pos = file.size();
    //重新定位文件输入位置，这里是定位到文件尾端
    file.seek(pos);
    string a;
        a=string(getExpression(c));
        QString content = QString::fromStdString(a);
        //QString content = ui->lineEdit->text();

        //写入文件
        qint64 length = -1;
        length = file.write(content.toLatin1(),content.length());

        qDebug()<<content;
        if(length == -1){
            qDebug()<<"写入文件失败";
        }else{
            qDebug()<<"写入文件成功";
        }

        a=string(getAnswer(c));
        a="="+a;
        content = QString::fromStdString(a);
        content = content +"\r\n";

        length = file.write(content.toLatin1(),content.length());

        qDebug()<<content;
        if(length == -1){
            qDebug()<<"写入文件失败";
        }else{
            qDebug()<<"写入文件成功";
        }
    //关闭文件
    file.close();
}

void Widget::on_pushButton_6_clicked()
{
    QFile file("wa.txt");

    //判断文件是否存在
    if(file.exists()){
        qDebug()<<"The file exists";
    }else{
        qDebug()<<"The file does not exist";
    }
    //只写方式打开文件，
    //如果文件不存在会自动创建文件
    if(!file.open(QIODevice::WriteOnly)){
        qDebug()<<"打开失败";
    }else{
        qDebug()<<"打开成功";
    }
    file.close();
}

void Widget::grade_memory(){
    QFile file("grade.txt");

    //判断文件是否存在
    if(file.exists()){
        qDebug()<<"The file exists";
    }else{
        qDebug()<<"The file does not exist";
    }
    //已读写方式打开文件，
    //如果文件不存在会自动创建文件
    if(!file.open(QIODevice::ReadWrite)){
        qDebug()<<"打开失败";
    }else{
        qDebug()<<"打开成功";
    }

    //获得文件大小
    qint64 pos;
    pos = file.size();
    //重新定位文件输入位置，这里是定位到文件尾端
    file.seek(pos);

    QString content;
    content = QString::number(sum,10);
    content =content +","+QString::number(total,10)+"\r\n";

    qint64 length = -1;
    length = file.write(content.toLatin1(),content.length());

    qDebug()<<content;
    if(length == -1){
        qDebug()<<"写入文件失败";
    }else{
        qDebug()<<"写入文件成功";
    }
    //关闭文件
    file.close();
}
