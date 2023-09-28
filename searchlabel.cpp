#include "searchlabel.h"


#include <QPixmap>
#include <QPainter>


SearchLabel::SearchLabel(QWidget *parent)
    : QWidget{parent}
{

    searchLabel = new SelfLabel(this);

    QPixmap pix(200, 200);
//    QPixmap pix(400, 400);
    pix.fill(Qt::transparent);

    QPen pen(Qt::gray);
    pen.setWidthF(4);
    pen.setCapStyle(Qt::RoundCap);  // 转角处 圆滑处理



    QPainter painter(&pix);

    painter.setPen(pen);

    painter.translate(QPoint(8, 75));        // 平移坐标系

    painter.setRenderHint(QPainter::Antialiasing);

    painter.scale(0.5, 0.5);

    int centerX = 18;
    int centerY = 18;

    int radius = qMin(centerX, centerY) ;// 10px 作为边距

//     绘制放大镜的圆环
    painter.setBrush(Qt::transparent);
    painter.setPen(QPen(Qt::gray, 4));  // 5px 线宽

    painter.drawEllipse(QPoint(centerX, centerY), radius, radius);

    // 绘制放大镜的手柄
    painter.setPen(QPen(Qt::gray, 4));  // 8px 线宽

    painter.drawLine(centerX - radius * 0.7, centerY + radius * 0.7,
                     centerX - radius * 1.5, centerY + radius * 1.5);

    painter.end();

    searchLabel->setPixmap(pix);
    searchLabel->setToolTip("Search");


}

SelfLabel *SearchLabel::getLabel()
{
    return searchLabel;
}
