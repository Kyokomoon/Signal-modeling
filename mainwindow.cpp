#include "mainwindow.h"
#include <QtWidgets>
#include <iostream>

const int massize=1000;
const int Glass_P = 1;
const int Glass_IRR = 2;
const int Beton = 3;
const int Drywall = 4;
int i, j, TxPower = 23, AntGain = 4;
float f = 2.4, pl,it,d, distancePix = 10;
int target_x = 400, target_y = 550;

struct WALL{
    //int Massiv[massize][massize];
    int** Massiv = new int *[massize];
    int material;
};


void drawLine(int x1, int y1, int x2, int y2, int material, WALL *wall) {
    const int deltaX = abs(x2 - x1);
    const int deltaY = abs(y2 - y1);
    const int signX = x1 < x2 ? 1 : -1;
    const int signY = y1 < y2 ? 1 : -1;
    int error = deltaX - deltaY;
    wall->material = material;
    while(x1 != x2 || y1 != y2)
   {
        if(x1 < 1000 && y1 < 1000 && x1 >= 0 && y1 >= 0){
            wall->Massiv[x1][y1] = 1;
        }

        int error2 = error * 2;
        if(error2 > -deltaY)
        {
            error -= deltaY;
            x1 += signX;
        }
        if(error2 < deltaX)
        {
            error += deltaX;
            y1 += signY;
        }
    }
    wall->Massiv[x2][y2] = 1;


}

void drawRect(int x1, int y1, int x2, int y2, int material, WALL *wal){
    WALL pre_wall1,pre_wall2,pre_wall3,pre_wall4,it_wall;
    WALL wals[4];
    int i=0,j=0;
    for(int j = 1; j<4;j++){
        for (int i = 0; i < massize; ++i)
            wals[j].Massiv[i] = new int [massize];
    }
    drawLine(x1,y1,x1+x2,y1, material, wal);
    drawLine(x1,y1,x1,y1+y2, material, &wals[1]);
    drawLine(x1+x2,y1,x1+x2,y1+y2, material,&wals[2]);
    drawLine(x1,y1+y2,x1+x2,y1+y2, material, &wals[3]);
    for(i=0;i<1000;i++){
        for(j=0;j<1000;j++){
            if(wals[1].Massiv[i][j] == 1 || wals[2].Massiv[i][j] == 1 || wals[3].Massiv[i][j] == 1){
                wal->Massiv[i][j] = 1;
            }
        }
    }

    for(int j = 1; j<4;j++){
        for (int i = 0; i < massize; ++i){

            delete [] wals[j].Massiv[i];
        }
    delete [] wals[j].Massiv;
    }
}





void checkWall(int x1, int y1, int x2, int y2, WALL * wals, float fr, float itt, float *it){

    const int deltaX = abs(x2 - x1);
    const int deltaY = abs(y2 - y1);
    const int signX = x1 < x2 ? 1 : -1;
    const int signY = y1 < y2 ? 1 : -1;
    int error = deltaX - deltaY;
    float Loss = 0, it_Loss=0, Pl_tw, Pl_ittw=0;
    //int count = 0;
    while(x1 != x2 || y1 != y2)
   {
        for(int i = 0;i<4;i++){
            if(Loss !=0){
                Loss = 0;
                break;
            }
            if(wals[i].Massiv[x1][y1] == 1){
                switch(wals[i].material){
                        case 1:
                            Loss = 2 + 0.2*fr;
                            it_Loss = it_Loss + Loss;
                            break;
                         case 2:
                            Loss = 23 + 0.3*fr;
                            it_Loss = it_Loss + Loss;
                            break;
                         case 3:
                            Loss = 5 + 4*fr;
                            it_Loss = it_Loss + Loss;
                            break;
                         case 4:
                            Loss = 4.85 + 0.12*fr;
                            it_Loss = it_Loss + Loss;
                            break;
                }
                Pl_tw = 5 - 10*log10(pow(10,(Loss/-10)));
                Pl_ittw = Pl_ittw + Pl_tw;
                break;
            }

        }
        int error2 = error * 2;
        if(error2 > -deltaY)
        {
            error -= deltaY;
            x1 += signX;
        }
        if(error2 < deltaX)
        {
            error += deltaX;
            y1 += signY;
        }
    }
    //x2;
    //y2;
    *it = itt - it_Loss;
}



struct RGB {
    int red;
    int green;
    int blue;
};

// Функция для вычисления промежуточного цвета
RGB interpolateColor(RGB color1, RGB color2, int val1, int val2, int value) {
    int range = val2 - val1;
    int rangeValue = value - val1;

    float percent = (float)rangeValue / range;

    RGB result;
    result.red = color1.red + (color2.red - color1.red) * percent;
    result.green = color1.green + (color2.green - color1.green) * percent;
    result.blue = color1.blue + (color2.blue - color1.blue) * percent;

    return result;
}



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



        scene = new QGraphicsScene();
        QPixmap map(1000, 1000);
        QPainter p(&map);
        p.setPen(QColor(0, 0, 0)); // <-- задание цвета

        WALL wals[4];
        for(int j = 0; j<4;j++){
            for (int i = 0; i < massize; ++i){
                wals[j].Massiv[i] = new int [massize];
            }

        }

        drawLine(40,100,100,70, Glass_IRR,&wals[0]);

        drawLine(300,100,400,140,Beton,&wals[1]);

        drawLine(500,500,600,550 ,Drywall,&wals[2]);

        drawRect(600,800,60,80, Glass_P ,&wals[3]);

        float itt;
        for(i = 0; i < 1000; i ++){
            for(j=0; j< 1000; j++){
                d = ((sqrt(pow((target_x - i),2) + pow((target_y - j),2)))*distancePix); // в метрах
                pl = 28 + 22*log10(d) + 20*log10(f); // в дБ (не понятно, нужно ли переводить в дБм или нужно оставлять в дБ)
                itt = ((TxPower + AntGain) - (pl)); //в дБм (без перевода в дБм)
                checkWall(target_x,target_y,i,j,wals,f,itt, &it);



                int minValue = -144;
                    int maxValue = -44;
                    int value = it;

                    RGB red = { 255, 0, 0 };
                    RGB orange = { 255, 165, 0 };
                    RGB yellow = { 255, 255, 0 };
                    RGB green = { 0, 255, 0 };
                    RGB blue = { 0, 0, 255 };

                    RGB color;

                    if (value <= maxValue && value >= minValue) {
                        if (value <= -44 && value >= -64) {
                            color = interpolateColor(red, orange, minValue, -64, value);
                        }
                        else if (value <= -64 && value >= -84) {
                            color = interpolateColor(orange, yellow, -64, -84, value);
                        }
                        else if (value <= -84 && value >= -104) {
                            color = interpolateColor(yellow, green, -84, -104, value);
                        }
                        else if (value <= -104 && value >= -124) {
                            color = interpolateColor(green, blue, -104, -124, value);
                        }
                        else if (value <= -124 && value >= -144) {
                            color = interpolateColor(blue, red, -124, -144, value);
                        }
                    }

                p.setPen(QColor(color.red, color.green, color.blue));
                p.drawPoint(i,j);
            }
        }
        for(int j = 0; j<4;j++){
            for (int i = 0; i < massize; ++i){

                delete [] wals[j].Massiv[i];

            }
        delete [] wals[j].Massiv;

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


        //Добавление препядствий и высчитывание их точек


        scene->addRect(600, 800, 60, 80);

        scene->addLine(40, 100, 100, 70);

        scene->addLine(300, 100, 400, 140);

        scene->addLine(500, 500, 600, 550);





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

    WALL wals[4];
    for(int j = 0; j<4;j++){
        for (int i = 0; i < massize; ++i){
            wals[j].Massiv[i] = new int [massize];

        }

    }

    drawLine(40,100,100,70, Glass_IRR,&wals[0]);

    drawLine(300,100,400,140,Beton,&wals[1]);

    drawLine(500,500,600,550 ,Drywall,&wals[2]);

    drawRect(600,800,60,80, Glass_P ,&wals[3]);

    float itt;
    for(i = 0; i < 1000; i ++){
        for(j=0; j< 1000; j++){
            d = ((sqrt(pow((target_x - i),2) + pow((target_y - j),2)))*distancePix); // в метрах
            pl = 28 + 22*log10(d) + 20*log10(f); // в дБ (не понятно, нужно ли переводить в дБм или нужно оставлять в дБ)
            itt = ((TxPower + AntGain) - (pl)); //в дБм (без перевода в дБм)
            checkWall(target_x,target_y,i,j,wals,f,itt, &it);



            int minValue = -144;
                int maxValue = -44;
                int value = it;

                RGB red = { 255, 0, 0 };
                RGB orange = { 255, 165, 0 };
                RGB yellow = { 255, 255, 0 };
                RGB green = { 0, 255, 0 };
                RGB blue = { 0, 0, 255 };

                RGB color;

                if (value <= maxValue && value >= minValue) {
                    if (value <= -44 && value >= -64) {
                        color = interpolateColor(red, orange, minValue, -64, value);
                    }
                    else if (value <= -64 && value >= -84) {
                        color = interpolateColor(orange, yellow, -64, -84, value);
                    }
                    else if (value <= -84 && value >= -104) {
                        color = interpolateColor(yellow, green, -84, -104, value);
                    }
                    else if (value <= -104 && value >= -124) {
                        color = interpolateColor(green, blue, -104, -124, value);
                    }
                    else if (value <= -124 && value >= -144) {
                        color = interpolateColor(blue, red, -124, -144, value);
                    }
                }

            p.setPen(QColor(color.red, color.green, color.blue));
            p.drawPoint(i,j);
        }
    }
    for(int j = 0; j<4;j++){
        for (int i = 0; i < massize; ++i){

            delete [] wals[j].Massiv[i];

        }
    delete [] wals[j].Massiv;

    }
    p.setPen(QColor(255, 255, 255)); // <-- задание цвета
    for(i=(target_x-1) ;i<= (target_x + 1);i++){
        for(j=(target_y-1) ;j<= (target_y + 1);j++){
            p.drawPoint(i,j);
        }
    }

    scene->addPixmap(map);
    scene->addRect(600, 800, 60, 80);

    scene->addLine(40, 100, 100, 70);

    scene->addLine(300, 100, 400, 140);

    scene->addLine(500, 500, 600, 550);

}

MainWindow::~MainWindow()
{

}
