#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

extern int num_ques ;
extern int scalemax_op ;
extern int scalemin_op ;
extern int num_op ;
extern int num_float ;
extern bool is_real ;
extern bool is_decimal ;
extern bool is_power ;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void showTimelimit();

    void timerinit();

    void timinit();

    void memory();

    void on_pushButton_6_clicked();

    void grade_memory();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
