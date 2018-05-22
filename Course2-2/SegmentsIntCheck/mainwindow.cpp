#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "convexhull.h"
#include "bezier.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    scene = new CustomScene;
    scene->points = new QGraphicsItemGroup;
    ui->setupUi(this);
    removeToolBar(ui->mainToolBar);
    connect(ui->ConvHull, SIGNAL (released()), this, SLOT (handleConvHull()));
    connect(ui->Bezier_1, SIGNAL (released()), this, SLOT (handleBezier_1()));
    ui->graphicsView->setScene(scene);
}

void MainWindow::handleConvHull()
{
    scene->deleteLines();
    QVector<QPointF> pts;
    foreach (auto item, scene->items())
    {
        pts.push_back(item->pos() + QPointF(5, 5));
    }
    ConvexHull::calculate(pts);
    scene->drawLines(pts, true);
}

void MainWindow::handleBezier_1()
{
    scene->deleteLines();
    QVector<QPointF> pts;
    foreach (auto item, scene->items())
    {
        pts.push_back(item->pos() + QPointF(5, 5));
    }
    qint64 split = 10000;
    qreal step = 1.0 / split;
    QVector<QPointF> todraw(split + 1);
    for (size_t i = 0; i <= split; ++i)
        todraw[i] = Bezier::getBezierPoint(pts, step * i);
    scene->drawLines(todraw, false);
}

std::string myPFF(QPointF p)
{
    return std::string("X: ") + std::to_string(p.x()) + std::string(" Y: ") + std::to_string(p.y());
}

void CustomScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    const QPointF p = event->scenePos();
    auto chp = dynamic_cast<PointItem *>(itemAt(p, QTransform()));
    if (!chp)
    {
        PointItem * pt = new PointItem;
        pt->setRect(0,0,10,10);
        pt->moveBy(p.x()-5, p.y()-5);
        pt->setPen(QPen(Qt::darkGreen));
        pt->setBrush(QBrush(Qt::green));
        points->addToGroup(pt);
        addItem(pt);

        return;
    }
    QGraphicsScene::mousePressEvent(event);
}

void CustomScene::deleteLines()
{
    foreach (QGraphicsItem * item, items())
    {
        if (!dynamic_cast<PointItem *>(item))
            delete item;
    }
}

void CustomScene::drawLines(QVector<QPointF> & pts, bool closed)
{
    auto it1 = pts.begin();
    QPen pen(QBrush(Qt::black), 2);
    QLineF * line;
    for (auto it2 = pts.begin() + 1; it2 != pts.end(); ++it1, ++it2)
    {
        line = new QLineF(*it1, *it2);
        addLine(*line, pen);
    }
    if (!closed)
        return;
    line = new QLineF(*pts.begin(), *it1);
    addLine(*line, pen);
}

MainWindow::~MainWindow()
{
    delete ui;
}
