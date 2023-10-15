#include "leftlabel.h"


#include <QPainter>
#include <QPixmap>


LeftLabel::LeftLabel(QWidget *parent)
    : QWidget{parent},
    explorerLabel(new SelfLabel(this)),
    searchLabel(new SelfLabel(this)),
    settingLabel(new SelfLabel(this))
{
    // 绘制控件
    // 文件探索
    drawExplorerLabel();
    // 搜索
    drawSearchLabel();
    // 设置
    drawSettingLabel();

}

void LeftLabel::drawExplorerLabel()
{
    QPixmap pix(200, 200);
    pix.fill(Qt::transparent);

    QPen pen(Qt::gray);
    pen.setWidthF(4);
    pen.setCapStyle(Qt::RoundCap);  // 转角处 圆滑处理

    QPainter painter(&pix);
    painter.setPen(pen);    // 设置笔颜色


    painter.setRenderHint(QPainter::Antialiasing);
    painter.scale(0.45, 0.45);

    painter.translate(QPoint(0, 140));        // 平移坐标系


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


    setLabel(explorerLabel);
}

void LeftLabel::drawSearchLabel()
{
    QPixmap pix(200, 200);
    pix.fill(Qt::transparent);

    QPen pen(Qt::gray);
    pen.setWidthF(4);
    pen.setCapStyle(Qt::RoundCap);  // 转角处 圆滑处理

    QPainter painter(&pix);

    painter.setPen(pen);

    painter.translate(QPoint(8, 73));        // 平移坐标系

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


    setLabel(searchLabel);
}

void LeftLabel::drawSettingLabel()
{
    QPixmap pix(200, 200);
    pix.fill(Qt::transparent);

    QPen pen(Qt::gray);
    pen.setWidthF(4);
    pen.setCapStyle(Qt::RoundCap);  // 转角处 圆滑处理

    QPainter painter(&pix);

    painter.setPen(pen);

    QPoint BEGIN_P(10, 71);
    painter.translate(BEGIN_P);        // 平移坐标系
//
    painter.setRenderHint(QPainter::Antialiasing);
//
    painter.scale(0.45, 0.45);
//

    QPoint CIRCLE_P(12, 28);
//
//    int radius = qMin(centerX, centerY) ;// 10px 作为边距

    painter.drawEllipse(CIRCLE_P, 8, 8);


    painter.translate(CIRCLE_P);  // 平移坐标系到圆中

//    painter.drawLine(QPoint(5, 0), QPoint(4, 10));
//    painter.drawLine(QPoint(5 + CIRCLE_P.x() - BEGIN_P.x(), 0 + CIRCLE_P.y() - BEGIN_P.y()), QPoint(4 + CIRCLE_P.x() - BEGIN_P.x() , 10 + CIRCLE_P.y() - BEGIN_P.y()));
//    painter.drawLine(QPoint(5 + CIRCLE_P.x() - BEGIN_P.x(), 0 + CIRCLE_P.y() - BEGIN_P.y()), QPoint(13 + CIRCLE_P.x() - BEGIN_P.x(), 0 + CIRCLE_P.y() - BEGIN_P.y()));
//    painter.drawLine(QPoint(13 + CIRCLE_P.x() - BEGIN_P.x(), 0 + CIRCLE_P.y() - BEGIN_P.y()), QPoint(14 + CIRCLE_P.x() - BEGIN_P.x(), 10 + CIRCLE_P.y() - BEGIN_P.y()));
    // 将绘制的曲线旋转 360 度

    painter.drawLine(QPoint(-5, -30), QPoint(-6, -17));
    painter.drawLine(QPoint(-5, -30), QPoint(5, -30));
    painter.drawLine(QPoint(5, -30), QPoint(6, -17));

    for (int i = 0; i < 7; ++i) {
        painter.rotate(360 / 8);
        painter.drawLine(QPoint(-5, -30), QPoint(-6, -17));
        painter.drawLine(QPoint(-5, -30), QPoint(5, -30));
        painter.drawLine(QPoint(5, -30), QPoint(6, -17));

    }




//    painter.drawLine(QPoint(5 + CIRCLE_P.x() - BEGIN_P.x(), 0 + CIRCLE_P.y() - BEGIN_P.y()), QPoint(4 + CIRCLE_P.x() - BEGIN_P.x() , 10 + CIRCLE_P.y() - BEGIN_P.y()));
//    painter.drawLine(QPoint(5 + CIRCLE_P.x() - BEGIN_P.x(), 0 + CIRCLE_P.y() - BEGIN_P.y()), QPoint(13 + CIRCLE_P.x() - BEGIN_P.x(), 0 + CIRCLE_P.y() - BEGIN_P.y()));
//    painter.drawLine(QPoint(13 + CIRCLE_P.x() - BEGIN_P.x(), 0 + CIRCLE_P.y() - BEGIN_P.y()), QPoint(14 + CIRCLE_P.x() - BEGIN_P.x(), 10 + CIRCLE_P.y() - BEGIN_P.y()));

//    painter.rotate(360 / 8);
//    painter.drawLine(QPoint(5, 0), QPoint(4, 10));
//    painter.drawLine(QPoint(5, 0), QPoint(13, 0));
//    painter.drawLine(QPoint(13, 0), QPoint(14, 10));

    settingLabel->setPixmap(pix);
    settingLabel->setToolTip("Manage");
//
    setLabel(settingLabel);
}

void LeftLabel::setLabel(SelfLabel* rl)
{
    storeLabel.append(rl);
}

SelfLabel* LeftLabel::getLabel(size_t index = 0)
{
    return storeLabel.at(index);
}
