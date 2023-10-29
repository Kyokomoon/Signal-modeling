#include "heatmap.h"
#include "barrier.h"

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
                            Loss = calculation_glass(fr);
                            it_Loss = it_Loss + Loss;
                            break;
                         case 2:
                            Loss = calculation_IRR_glass(fr);
                            it_Loss = it_Loss + Loss;
                            break;
                         case 3:
                            Loss = calculation_concrete(fr);
                            it_Loss = it_Loss + Loss;
                            break;
                         case 4:
                            Loss = calculation_drywall(fr);
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



HeatMap::HeatMap()
{
    map = new QPixmap(massize, massize);

    scene = new QGraphicsScene();
    for(int j = 0; j<4;j++){
        for (int i = 0; i < massize; ++i){
            wals[j].Massiv[i] = new int [massize];
            loss_map[i] = new int[massize];
        }

    }

}
HeatMap::~HeatMap(){

    for(int j = 0; j<4;j++){
        for (int i = 0; i < massize; ++i){
            delete[] wals[j].Massiv[i];
            delete[] loss_map[i];
        }
        delete[] wals[j].Massiv;

    }
    delete[] loss_map;
    delete pixmap;
    delete view;
    delete scene;
}
void HeatMap::addBarrier(int x1, int y1, int x2,int y2, int type, int type_material, int i){
    if(type == 1){
        drawLine(x1,y1,x2,y2, type_material,&wals[i]);
        scene->addLine(x1, y1, x2, y2);
    }
    else{
        drawRect(x1,y1,x2,y2,type_material,&wals[i]);
        scene->addRect(x1, y1, x2, y2);

    }
}
void HeatMap::calculateLoss(){
    int i,j;
    float itt;
    for(i = 0; i < 1000; i ++){
        for(j=0; j< 1000; j++){
            d = ((sqrt(pow((target_x - i),2) + pow((target_y - j),2)))*distancePix); // в метрах
            pl = 28 + 22*log10(d) + 20*log10(f); // в дБ (не понятно, нужно ли переводить в дБм или нужно оставлять в дБ)
            itt = ((TxPower + AntGain) - (pl)); //в дБм (без перевода в дБм)
            checkWall(target_x,target_y,i,j,wals,f,itt, &it);
            loss_map[i][j] = it;
        }
    }

}
void HeatMap::paintMap(){
    int i,j;
    QPainter p(map);
    for(i = 0; i < 1000; i ++){
        for(j=0; j< 1000; j++){
            int minValue = -144;
                int maxValue = -44;
                int value = loss_map[i][j];

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
    p.setPen(QColor(255, 255, 255)); // <-- задание цвета
    for(i=(target_x-1) ;i<= (target_x + 1);i++){
        for(j=(target_y-1) ;j<= (target_y + 1);j++){
            p.drawPoint(i,j);
        }
    }
    scene->addPixmap(*map);

    view = new QGraphicsView(scene);
}
void HeatMap::drowAll(int x1, int y1, int x2,int y2, int type){
    if(type == 1){
        scene->addLine(x1, y1, x2, y2);
    }
    else{

        scene->addRect(x1, y1, x2, y2);

    }

}
void HeatMap::set_value_f(double F){
    f = F;
}
QGraphicsView *HeatMap::get_view(){
    return view;
}
int HeatMap::get_TxPower(){
    return TxPower;
}
int HeatMap::get_AntGain(){
    return AntGain;
}
float HeatMap::get_f(){
    return f;
}
float HeatMap::get_distancePix(){
    return distancePix;
}
int HeatMap::get_target_x(){
    return target_x;
}
int HeatMap::get_target_y(){
    return target_y;
}
void HeatMap::set_TxPower(int Tx){
    TxPower = Tx;
}
void HeatMap::set_AntGain(int Ant){
    AntGain = Ant;
}

void HeatMap::set_distancePix(float dist){
    distancePix = dist;
}
void HeatMap::set_target_x(int tx){
    target_x = tx;
}
void HeatMap::set_target_y(int ty){
    target_y = ty;
}
