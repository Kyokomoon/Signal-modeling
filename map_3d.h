#ifndef MAP_3D_HPP
#define MAP_3D_HPP

#include <iostream>

#include <QtDataVisualization/Q3DTheme>
#include <QtDataVisualization/QCustom3DItem>
#include <QtDataVisualization/QCustom3DLabel>
#include <QtGui/QImage>

#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtDataVisualization/QHeightMapSurfaceDataProxy>
#include <QtDataVisualization/QSurface3DSeries>
#include <QtWidgets/QSlider>
#include <QtWidgets/QLabel>
#include <QtCore/QPropertyAnimation>




using namespace QtDataVisualization;


class QWidget;

class Map_3D
{
public:
    Map_3D(int pos_x, int pos_y, int size_x, int size_y, int size_map_x, int size_map_y);

    QWidget *get_container();
    void render(char *file_texture);
    QSurface3DSeries *get_QSurface3DSeries();
    Q3DSurface *graph;
    void clicked(int type);
private:
    int pos_x, pos_y;
    int size_x, size_y;
    int size_map_x, size_map_y;
    QWidget *container;
    QSurface3DSeries *layerOneSeries;

    char filename1[FILENAME_MAX] = "../resource/refinery.obj";
    char filename2[FILENAME_MAX] = "../lab3/map_heights.png";


};

#endif // MAP_3D_HPP
