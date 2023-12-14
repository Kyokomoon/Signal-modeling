#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QtWidgets>
#include <QMainWindow>

class HeatMap;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QGraphicsScene* scene;
    QGraphicsView* view;
    QLabel *label_;
    QGroupBox *gradientGroupBox_, *infoGroupBox_;
    QLineEdit *line_Tx, *line_An, *line_F, *line_PixDist, *line_target_x, *line_target_y;
    HeatMap *map;
    QImage mapImage;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void draw_gradient_panel();
    void draw();

private slots:
    void on_pushButton_clicked();
};
#endif // MAINWINDOW_H
