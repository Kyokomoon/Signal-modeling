#include "mainwindow.h"
#include <QtWidgets>


int i, j, TxPower = 23, AntGain = 4;
float f = 2.4, pl,it,d, distancePix = 10;
int target_x = 5, target_y = 5;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{




    gradientGroupBox_ = new QGroupBox(QStringLiteral("Signal Strength"));
    infoGroupBox_ = new QGroupBox(QStringLiteral("info: "));

        int height = 400;
        int width = 150;
        int border = 10;
        gradientGroupBox_->resize(150, 450);
        infoGroupBox_->resize(200,220);
        infoGroupBox_->move(0,500);
        QLinearGradient gr(0, 0, 1, height - 2 * border);
        gr.setColorAt(1.0f, Qt::blue);
        gr.setColorAt(0.8f, Qt::cyan);
        gr.setColorAt(0.6f, Qt::green);
        gr.setColorAt(0.4f, Qt::yellow);
        gr.setColorAt(0.0f, Qt::red);

    //blue,cyan,green,yellow,red

        scene = new QGraphicsScene();
        QPixmap map(1000, 1000);
        QPainter p(&map);
        p.setPen(QColor(0, 0, 0)); // <-- задание цвета
        for(i = 0; i < 1000; i ++){
            for(j=0; j<1000; j++){
                d = ((sqrt(pow((target_x - i),2) + pow((target_y - j),2)))*distancePix); // в метрах
                pl = 28 + 22*log10(d) + 20*log10(f); // в дБ (не понятно, нужно ли переводить в дБм или нужно оставлять в дБ)
                it = ((TxPower + AntGain) - (pl)); //в дБм (без перевода в дБм)
                if(it <= -134){
                    p.setPen(QColor(0, 0, 139));
                }
                else if(it < -124 && it > -134){
                    p.setPen(QColor(0, 0, 255));
                }
                else if(it < -114 && it > -124){
                    p.setPen(QColor(0, 206, 209));
                }
                else if(it < -104 && it > -114){
                    p.setPen(QColor(0, 250, 154));
                }
                else if(it < -94 && it > -104){
                    p.setPen(QColor(0, 255, 0));
                }
                else if(it < -84 && it > -94){
                    p.setPen(QColor(173, 255, 47));
                }
                else if(it < -74 && it > -84){
                    p.setPen(QColor(255, 255, 0));
                }
                else if(it < -64 && it > -74){
                    p.setPen(QColor(255, 215, 0));
                }
                else if(it < -54 && it > -64){
                    p.setPen(QColor(255, 140, 0));
                }
                else if(it > -54){
                    p.setPen(QColor(255, 69, 0));
                }
                else{
                    p.setPen(QColor(0, 0, 0));
                    //printf("\n%f", it);
                }
                p.drawPoint(i,j);
            }
        }
        p.setPen(QColor(255, 255, 255)); // <-- задание цвета
        for(i=(target_x-1) ;i<= (target_x + 1);i++){
            for(j=(target_y-1) ;j<= (target_y + 1);j++){
                p.drawPoint(i,j);
            }
        }

        char buf[30];
        QString T_Tx = "", T_An = "", T_F = "", T_PixDist, T_target_x, T_target_y;
        sprintf(buf, "%d", TxPower);
        T_Tx = buf;
        sprintf(buf, "%d", AntGain);
        T_An = buf;
        sprintf(buf, "%.3f", f);
        T_F = buf;
        sprintf(buf, "%.3f", distancePix);
        T_PixDist = buf;
        sprintf(buf, "%d", target_x);
        T_target_x = buf;
        sprintf(buf, "%d", target_y);
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

        QPushButton *Bu_Res = new QPushButton();
        Bu_Res->setGeometry(20,900,60,20);
        Bu_Res->setText("Restart");
        Bu_Res->move(10,800);
        connect(Bu_Res, SIGNAL (clicked()), this, SLOT (on_pushButton_clicked()));

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

        scene->addPixmap(map);
        view = new QGraphicsView(scene);
        setCentralWidget(view);

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

        label_ = new QLabel();
        label_->setPixmap(pm);

        QVBoxLayout *colorMapVBox = new QVBoxLayout;
        //QVBoxLayout *infoBox = new QVBoxLayout;
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

        gradientGroupBox_->setLayout(colorMapVBox);
        //infoGroupBox_->setLayout(infoBox);

        this->layout()->addWidget(gradientGroupBox_);
        //this->layout()->addWidget(infoGroupBox_);
        this->layout()->addWidget(Bu_Res);


}
void MainWindow:: on_pushButton_clicked(){
    TxPower = line_Tx->text().toInt();
    AntGain = line_An->text().toInt();
    f = line_F->text().toFloat();
    distancePix = line_PixDist->text().toFloat();
    target_x = line_target_x->text().toInt();
    target_y = line_target_y->text().toInt();
    QPixmap map(1000, 1000);
    QPainter p(&map);
    p.setPen(QColor(0, 0, 0)); // <-- задание цвета
    for(i = 0; i < 1000; i ++){
        for(j=0; j<1000; j++){
            d = ((sqrt(pow((target_x - i),2) + pow((target_y - j),2)))*distancePix); // в метрах
            pl = 28 + 22*log10(d) + 20*log10(f); // в дБ (не понятно, нужно ли переводить в дБм или нужно оставлять в дБ)
            it = ((TxPower + AntGain) - (pl)); //в дБм (без перевода в дБм)
            if(it <= -134){
                p.setPen(QColor(0, 0, 139));
            }
            else if(it < -124 && it > -134){
                p.setPen(QColor(0, 0, 255));
            }
            else if(it < -114 && it > -124){
                p.setPen(QColor(0, 206, 209));
            }
            else if(it < -104 && it > -114){
                p.setPen(QColor(0, 250, 154));
            }
            else if(it < -94 && it > -104){
                p.setPen(QColor(0, 255, 0));
            }
            else if(it < -84 && it > -94){
                p.setPen(QColor(173, 255, 47));
            }
            else if(it < -74 && it > -84){
                p.setPen(QColor(255, 255, 0));
            }
            else if(it < -64 && it > -74){
                p.setPen(QColor(255, 215, 0));
            }
            else if(it < -54 && it > -64){
                p.setPen(QColor(255, 140, 0));
            }
            else if(it > -54){
                p.setPen(QColor(255, 69, 0));
            }
            else{
                p.setPen(QColor(0, 0, 0));
                //printf("\n%f", it);
            }
            p.drawPoint(i,j);
        }
    }
    p.setPen(QColor(255, 255, 255)); // <-- задание цвета
    for(i=(target_x-1) ;i<= (target_x + 1);i++){
        for(j=(target_y-1) ;j<= (target_y + 1);j++){
            p.drawPoint(i,j);
        }
    }

    scene->addPixmap(map);
}
MainWindow::~MainWindow()
{

}