#include "leftlabel.h"


#include <QPainter>
#include <QPixmap>


LeftLabel::LeftLabel(QWidget *parent)
    : QWidget{parent}
{

    QPixmap pix(200, 200);
    pix.fill(Qt::transparent);

    QPen pen(Qt::gray);
    pen.setWidthF(4);
    pen.setCapStyle(Qt::RoundCap);  // 转角处 圆滑处理

    QPainter painter(&pix);
    painter.setPen(pen);    // 设置笔颜色

    SelfLabel *explorerLabel = new SelfLabel(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.scale(0.5, 0.5);

    painter.translate(QPoint(0, 125));        // 平移坐标系


    painter.drawLine(QPoint(5, 20), QPoint(5, 70));
    painter.drawLine(QPoint(5, 70), QPoint(43, 70));
    painter.drawLine(QPoint(43, 70), QPoint(43, 60));
    painter.drawLine(QPoint(25, 60), QPoint(61, 60));
    painter.drawLine(QPoint(25, 60), QPoint(25, 10));
    painter.drawLine(QPoint(61, 60), QPoint(61, 25));
    painter.drawLine(QPoint(5, 20), QPoint(25, 20));
    painter.drawLine(QPoint(25, 10), QPoint(45, 10));
    painter.drawLine(QPoint(45, 10), QPoint(45, 25));
    painter.drawLine(QPoint(61, 25), QPoint(45, 25));
    painter.drawLine(QPoint(45, 10), QPoint(61, 25));

    painter.end();


    explorerLabel->setPixmap(pix);

    explorerLabel->setToolTip("Explorer");

//    explorerLabel->setText("12312");  // 设置文字会覆盖绘制的内容

    setLabel(explorerLabel);


//    QPixmap pix2(200, 200);
////    pix2.fill(Qt::transparent);

//    QPen pen2(Qt::red);
//    pen2.setWidthF(4);
//    pen2.setCapStyle(Qt::RoundCap);  // 转角处 圆滑处理

//    SelfLabel *searchLabel = new SelfLabel(this);

//    QPainter painter2(&pix2);

//    painter2.setPen(pen2);



////    painter2.setRenderHint(QPainter::Antialiasing, true);

//    int centerX = width() / 2;
//    int centerY = height() / 2;

//    // 设定放大镜的大小（半径）
//    int radius = qMin(centerX, centerY) ;// 10px 作为边距

//    // 绘制放大镜的圆环
////    painter2.setBrush(Qt::transparent);
//    painter2.setPen(QPen(Qt::red, 8));  // 5px 线宽
//    painter2.drawEllipse(QPoint(centerX, centerY), radius, radius);

//    // 绘制放大镜的手柄
//    painter2.setPen(QPen(Qt::red, 8));  // 8px 线宽
////    painter.drawLine(centerX + radius * 0.5, centerY + radius * 0.5,
////                     centerX + radius, centerY + radius);

////    painter.drawLine(centerX - radius * 0.3, centerY + radius * 0.3,
////                     centerX - radius * 1.5, centerY + radius * 1.5);
//    painter2.drawLine(centerX - radius * 0.7, centerY + radius * 0.6,
//                     centerX - radius * 1.5, centerY + radius * 1.5);


//    painter2.scale(2.5, 2.5);

//    painter2.end();

//    searchLabel->setPixmap(pix2);

//    searchLabel->setToolTip("Search");

////    searchLabel->setText("123");

//    setLabel(searchLabel);

}

void LeftLabel::setLabel(SelfLabel* rl)
{
    storeLabel.append(rl);
}

SelfLabel* LeftLabel::getLabel(size_t index = 0)
{
    return storeLabel.at(index);
}
