#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QVBoxLayout>
#include <QGraphicsItemGroup>

#include <QVector>
#include <QPointF>
#include <QPair>
#include <QStack>

#include <algorithm>

namespace Ui {
class MainWindow;
}

class CustomScene : public QGraphicsScene
{
    Q_OBJECT

public:
    void deleteLines();
    void drawLines(QVector<QPointF> & pts, bool closed);
    QGraphicsItemGroup * points;
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

class PointItem : public QGraphicsEllipseItem
{
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) { delete this; }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void mychk(const QString & str);

private slots:
    void handleConvHull();
    void handleBezier_1();

private:
    Ui::MainWindow *ui;
    CustomScene * scene;
};

#endif // MAINWINDOW_H
