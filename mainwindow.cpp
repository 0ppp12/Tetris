#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<QTime>
#include<QDebug>
#include <QSqlQuery>
#include<QSqlError>
#include<stdlib.h>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(1000, 800);
    ui->playWidget->setHidden(true);
    ui->recodesWidge->setHidden(true);
    ui->startWidget->setHidden(false);

    //启动定时器事件
    timeid=startTimer(1000);
    dateid=startTimer(1000*24*60*60);
    ui->lb_date->setText(QDate::currentDate().toString("yyyy-MM-dd"));
//    ui->listWidgetRecode->addItem("1-这是文字项测试");

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("tetris.db");
    if (!db.open()) {
        qDebug() << "Failed to open database:" << db.lastError().text();
    }else{
//        qDebug() << "Database opened successfully.";
        QSqlQuery query;
        query.exec("create table if not exists recode(id integer primary key,date varchar(32),time varchar(32),name varchar(32),score integer)");
    }

	//widgetGameArea 和 widgetNextArea 已在界面设计器内由普通QWidget分别提升成GameArea和NextArea
	//GameArea: 左侧游戏区域，自绘widget
	//NextArea: 右侧显示下个item的widget，也是自绘widget
	//游戏主运行逻辑在GameArea内，不过按键消息因为是MainWindow接受，通过ui->widgetGameArea->KeyPressed()函数调用传递下去
	//GameArea通过信号sigUpdateScore、sigUpdateLevel 通知MainWindow更新界面的得分和关卡
	//GameArea通过信号sigUpdateNextItem 通知 NextArea 刷新下一个元素
	connect(ui->widgetGameArea, &GameArea::sigUpdateScore, this, &MainWindow::slotUpdateScore);//通知更新界面
	connect(ui->widgetGameArea, &GameArea::sigUpdateLevel, this, &MainWindow::slotUpdateLevel);//通知更新速度
	connect(ui->widgetGameArea, &GameArea::sigUpdateNextItem, ui->widgetNextArea, &NextArea::slotUpdateNextItem);//通知更新下一个方块

//    ui->widgetGameArea->NewGame();//开始游戏
}

MainWindow::~MainWindow()
{
	delete ui;
    db.close();
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{//检测键盘按下事件
	ui->widgetGameArea->KeyPressed(e->key());
	QMainWindow::keyPressEvent(e);
}

void MainWindow::slotUpdateScore(int nScore)
{//接收更新分数信号
    ui->lb_score_value->setText(QString::number(nScore));
}

void MainWindow::slotUpdateLevel(int nSpeed)
{//接收更新关卡信号
    ui->lb_level_value->setText(QString::number(nSpeed));
}

void MainWindow::on_bt_up_clicked()
{
    QKeyEvent*ke=new QKeyEvent(QKeyEvent::KeyPress,Qt::Key_Up,Qt::NoModifier,"");
    QApplication::postEvent(this,ke);
}

void MainWindow::on_bt_left_clicked()
{
    QKeyEvent*ke=new QKeyEvent(QKeyEvent::KeyPress,Qt::Key_Left,Qt::NoModifier,"");
    QApplication::postEvent(this,ke);
}

void MainWindow::on_bt_down_clicked()
{
    QKeyEvent*ke=new QKeyEvent(QKeyEvent::KeyPress,Qt::Key_Down,Qt::NoModifier,"");
    QApplication::postEvent(this,ke);
}

void MainWindow::on_bt_right_clicked()
{
    QKeyEvent*ke=new QKeyEvent(QKeyEvent::KeyPress,Qt::Key_Right,Qt::NoModifier,"");
    QApplication::postEvent(this,ke);
}

void MainWindow::on_bt_space_clicked()
{
    QKeyEvent*ke=new QKeyEvent(QKeyEvent::KeyPress,Qt::Key_Space,Qt::NoModifier,"");
    QApplication::postEvent(this,ke);
}

void MainWindow::on_bt_enter_clicked()
{
    QKeyEvent*ke=new QKeyEvent(QKeyEvent::KeyPress,Qt::Key_Enter,Qt::NoModifier,"");
//    QApplication::postEvent(QApplication::focusWidget(),ke);
    QApplication::postEvent(this,ke);
}

void MainWindow::on_bt_start_clicked()
{
    ui->playWidget->setHidden(false);
    ui->startWidget->setHidden(true);
    ui->recodesWidge->setHidden(true);
    //判断之前有没有保存的游戏进度
    if(!exitGame){
        ui->widgetGameArea->NewGame();//开始游戏
    }

    //游戏开始播放背景音乐
    system("mplayer bg.mp3");
}

void MainWindow::timerEvent(QTimerEvent *e)
{
    if(e->timerId()==timeid){
        ui->lb_time->setText(QTime::currentTime().toString("hh:mm:ss"));
    }
    if(e->timerId()==dateid){
        ui->lb_date->setText(QDate::currentDate().toString("yyyy-MM-dd"));
    }
}

void MainWindow::on_bt_recode_clicked()
{
    QSqlQuery query;
    query.exec("select *from recode order by score desc limit 10");
    int i=1;
    while (query.next()) {
        QString date = query.value(1).toString();
        QString time = query.value(2).toString();
        QString name = query.value(3).toString();
        int score = query.value(4).toInt();
//        qDebug()<<i<<date<<time<<name<<score;
        ui->listWidgetRecode->addItem(QString("%1 %2 %3 %4 %5").arg(i++).arg(date).arg(time).arg(name).arg(score));
    }
    ui->recodesWidge->setHidden(false);
    ui->startWidget->setHidden(true);
}

void MainWindow::on_recode2start_clicked()
{
    ui->listWidgetRecode->clear();
    ui->recodesWidge->setHidden(true);
    ui->startWidget->setHidden(false);
}

void MainWindow::on_play2start_clicked()
{
    ui->playWidget->setHidden(true);
    ui->startWidget->setHidden(false);
    //发送一个enter键入信号（等会写保存游戏）
    QKeyEvent*ke=new QKeyEvent(QKeyEvent::KeyPress,Qt::Key_Enter,Qt::NoModifier,"");
    QApplication::postEvent(this,ke);
    exitGame=1;

    //返回菜单暂停背景音乐
    system("killall -9 mplayer");
}

