#include "mainwindow.h"
#include <QLabel>
#include <QVBoxLayout>
#include <cmath>
#include <QPainter>
#include <QLineF>
#include <QtMath>
#include <QtWidgets>
#include "heatmap.h"
#include "barrier.h"
#include "map_3d.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    Map_3D *map_3d = new Map_3D(200, 50, 800, 800, 1000, 1000);
    this->setFixedSize(1300, 1000);
    mapImage = QImage(1000, 1000, QImage::Format_RGB32);

    gradientGroupBox_ = new QGroupBox(QStringLiteral("Signal Strength"));
    infoGroupBox_ = new QGroupBox(QStringLiteral("info: "));
        infoGroupBox_->resize(200,220);
        infoGroupBox_->move(0,500);
        scene = new QGraphicsScene();
        view = new QGraphicsView(scene);
        map = new HeatMap();
        map->addBarrier(150,270,220,200, 1 ,Drywall,0);
        map->addBarrier(300,100,400,140, 1 ,Glass_IRR,1);
        map->addBarrier(600,500,600,550, 1 ,Beton,2);
        map->addBarrier(600,800,60,80, 2 ,Glass_P,3);
        //int i=1,s_x=150, s_y=300, st_s_x = s_x;
        /*while(i < 10){
            map->addBarrier(s_x,s_y,4,4, 2 ,Glass_P,i+3);
            s_x += 10;
            if(i%3==0){
                s_y += 10;
                s_x = st_s_x;
            }


        }*/
        map->calculateLoss();
        //map->paintMap();
        map->paintMap3d(&mapImage);
        mapImage.save("map_signal.png");


        QLabel* imageLabel = new QLabel();
        imageLabel->setPixmap(QPixmap::fromImage(mapImage));
        scene->addPixmap(QPixmap::fromImage(mapImage));
        view->setGeometry(170, 0, 1000, 1000);
        draw_gradient_panel();

        this->layout()->addWidget(map_3d->get_container());
        map_3d->render("map_signal.png");


        //setCentralWidget(map->get_view());
        //draw();




}
void MainWindow:: on_pushButton_clicked(){
    map = new HeatMap();
    map->set_TxPower(line_Tx->text().toInt());
    map->set_AntGain(line_An->text().toInt());
    map->set_value_f(line_F->text().toFloat());
    map->set_distancePix(line_PixDist->text().toFloat());
    map->set_target_x(line_target_x->text().toInt());
    map->set_target_y(line_target_y->text().toInt());

    map->addBarrier(40,100,100,70, 1 ,Glass_P,0);
    map->addBarrier(300,100,400,140, 1 ,Drywall,1);
    map->addBarrier(600,500,600,550, 1 ,Glass_IRR,2);
    map->addBarrier(600,800,60,80, 2 ,Beton,3);
    map->calculateLoss();
    map->paintMap();
    setCentralWidget(map->get_view());
    //this->layout()->addWidget(map->get_view());
    draw();
    draw_gradient_panel();

}

MainWindow::~MainWindow()
{

}
void MainWindow::draw(){
    map->drowAll(40,100,100,70, 1);
    map->drowAll(300,100,400,140, 1);
    map->drowAll(600,500,600,550, 1);
    map->drowAll(600,800,60,80, 2);
}
void MainWindow::draw_gradient_panel(){
    gradientGroupBox_ = new QGroupBox(QStringLiteral("Signal Strength"));
    //gradientGroupBox_->setGeometry(0, 150, 150, 500);
    gradientGroupBox_->resize(150, 450);
    int height = 400;
    int width = 150;
    int border = 10;
    QLinearGradient gr(0, 0, 1, height - 2 * border);
    gr.setColorAt(1.0f, Qt::blue);
    gr.setColorAt(0.8f, Qt::cyan);
    gr.setColorAt(0.6f, Qt::green);
    gr.setColorAt(0.4f, Qt::yellow);
    gr.setColorAt(0.0f, Qt::red);

    //blue,cyan,green,yellow,red
    QPixmap pm(width, height);
    pm.fill(Qt::transparent);
    QPainter pmp(&pm);
    pmp.setBrush(QBrush(gr));
    pmp.setPen(Qt::NoPen);
    pmp.drawRect(border, border, 35, height - 2 * border);
    pmp.setPen(Qt::black);
    int step = (height - 2 * border) / 10;
    for (int i = 0; i < 11; i++) {
        int yPos = i * step + border;
        pmp.drawLine(border, yPos, 55, yPos);
        pmp.drawText(60, yPos + 2, QString("%1 [dBm]").arg(-44 - (i * 10)));
    }
    char buf[30];
    QString T_Tx = "", T_An = "", T_F = "", T_PixDist, T_target_x, T_target_y;
    sprintf(buf, "%d", map->get_TxPower());
    T_Tx = buf;
    sprintf(buf, "%d", map->get_AntGain());
    T_An = buf;
    sprintf(buf, "%.3f", map->get_f());
    T_F = buf;
    sprintf(buf, "%.3f", map->get_distancePix());
    T_PixDist = buf;
    sprintf(buf, "%d", map->get_target_x());
    T_target_x = buf;
    sprintf(buf, "%d", map->get_target_y());
    T_target_y = buf;

    line_Tx= new QLineEdit();
    line_Tx->setGeometry(0,0,50,20);
    line_Tx->move(100,515);
    line_Tx->setText(T_Tx);

    line_An = new QLineEdit();
    line_An->setGeometry(0,0,50,20);
    line_An->move(100,555);
    line_An->setText(T_An);

    line_F = new QLineEdit();
    line_F->setGeometry(0,0,50,20);
    line_F->move(100,595);
    line_F->setText(T_F);

    line_PixDist = new QLineEdit();
    line_PixDist->setGeometry(0,0,50,20);
    line_PixDist->move(100,635);
    line_PixDist->setText(T_PixDist);

    line_target_x = new QLineEdit();
    line_target_x->setGeometry(0,0,50,20);
    line_target_x->move(100,675);
    line_target_x->setText(T_target_x);

    line_target_y = new QLineEdit();
    line_target_y->setGeometry(0,0,50,20);
    line_target_y->move(100,715);
    line_target_y->setText(T_target_y);



    QLabel *label1 = new QLabel();
    label1->setText("TxPower[dBm]");
    label1->resize(80,15);
    label1->move(10,520);

    QLabel *label2 = new QLabel();
    label2->setText("AntGain[dBi]");
    label2->resize(80,15);
    label2->move(10,560);

    QLabel *label3 = new QLabel();
    label3->setText("Frequency[GHz]");
    label3->resize(80,15);
    label3->move(10,600);

    QLabel *label4 = new QLabel();
    label4->setText("pixDist[m]");
    label4->resize(80,15);
    label4->move(10,640);

    QLabel *label5 = new QLabel();
    label5->setText("target_x");
    label5->resize(80,15);
    label5->move(10,680);

    QLabel *label6 = new QLabel();
    label6->setText("target_y");
    label6->resize(80,15);
    label6->move(10,720);
    QPushButton *Bu_Res = new QPushButton();
    Bu_Res->setGeometry(20,900,60,20);
    Bu_Res->setText("Restart");
    Bu_Res->move(10,800);
    connect(Bu_Res, SIGNAL (clicked()), this, SLOT (on_pushButton_clicked()));



    //QVBoxLayout *infoBox = new QVBoxLayout;


    label_ = new QLabel();
    label_->setPixmap(pm);

    QVBoxLayout *colorMapVBox = new QVBoxLayout;
    colorMapVBox->addWidget(label_);
    this->layout()->addWidget(label1);
    this->layout()->addWidget(line_Tx);

    this->layout()->addWidget(label2);
    this->layout()->addWidget(line_An);

    this->layout()->addWidget(label3);
    this->layout()->addWidget(line_F);

    this->layout()->addWidget(label4);
    this->layout()->addWidget(line_PixDist);

    this->layout()->addWidget(label5);
    this->layout()->addWidget(line_target_x);

    this->layout()->addWidget(label6);
    this->layout()->addWidget(line_target_y);
    this->layout()->addWidget(gradientGroupBox_);
    this->layout()->addWidget(Bu_Res);
    gradientGroupBox_->setLayout(colorMapVBox);
    this->layout()->addWidget(gradientGroupBox_);
}
