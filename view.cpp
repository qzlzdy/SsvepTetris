#include "view.h"

#include <QVBoxLayout>

using namespace ehdu;

View::View(QWidget *parent): QFrame(parent){
    setFrameStyle(Sunken | StyledPanel);
    graphicsView = new QGraphicsView(this);
    graphicsView->setRenderHint(QPainter::Antialiasing, false);
    graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    graphicsView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    QVBoxLayout *topLayout = new QVBoxLayout;
    topLayout->addWidget(graphicsView);
    setLayout(topLayout);
}

QGraphicsView *View::view() const{
    return graphicsView;
}
