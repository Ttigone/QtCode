#include "selflabel.h"

SelfLabel::SelfLabel(QWidget *parent)
    : QLabel{parent}
{

}

void SelfLabel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit clicked();
    }
}
