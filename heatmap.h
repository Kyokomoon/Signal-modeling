#ifndef HEATMAP_H
#define HEATMAP_H
#include <QPainter>
#include <QtWidgets>
const int massize=1000;
struct WALL{
    int** Massiv = new int *[1000];
    int material;
};
struct RGB {
    int red;
    int green;
    int blue;
};

class HeatMap
{
    QLabel *label_;
    QGroupBox *gradientGroupBox_, *infoGroupBox_;
    QLineEdit *line_Tx, *line_An, *line_F, *line_PixDist, *line_target_x, *line_target_y;
    QPainter p;
    QPixmap *map;
    QPixmap *pixmap;
    QGraphicsScene* scene;
    QGraphicsView *view;
public:
    HeatMap();
    void calculateLoss();
    void addBarrier(int x1, int y1, int x2, int y2, int type,int type_material, int i);
    void paintMap();
    void drowAll(int x1, int y1, int x2,int y2, int type);
    void set_value_f(double F);
    ~HeatMap();
    QGraphicsView *get_view();
    int get_TxPower();
    int get_AntGain();
    float get_f();
    float get_distancePix();
    int get_target_x();
    int get_target_y();
    void set_TxPower(int Tx);
    void set_AntGain(int Ant);
    void set_distancePix(float dist);
    void set_target_x(int tx);
    void set_target_y(int ty);
private:

    float f = 2.4, pl,it,d, distancePix = 10;
    int target_x = 400, target_y = 550;
    int TxPower = 23, AntGain = 4;
    WALL wals[4];
    int** loss_map = new int *[1000];
};

#endif // HEATMAP_H
