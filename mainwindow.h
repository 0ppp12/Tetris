#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QKeyEvent>
#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
//主界面设计
namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

    void timerEvent(QTimerEvent *e);
protected:
    void keyPressEvent(QKeyEvent *e);//重写按键按下事件方法

private slots:
    void slotUpdateScore(int nScore);//接收分数更新通知（槽）
    void slotUpdateLevel(int nSpeed);//接收关卡更新通知（槽）
    void on_bt_left_clicked();
    void on_bt_up_clicked();
    void on_bt_enter_clicked();
    void on_bt_down_clicked();
    void on_bt_right_clicked();
    void on_bt_space_clicked();
    void on_bt_start_clicked();
    void on_bt_recode_clicked();
    void on_play2start_clicked();
    void on_recode2start_clicked();

private:
	Ui::MainWindow *ui;
    int timeid,dateid,exitGame=0;
    QSqlDatabase db;
};

#endif
