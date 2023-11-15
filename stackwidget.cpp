#include "stackwidget.h"

StackWidget::StackWidget(QWidget *parent) : QStackedWidget(parent)
{

}

//void StackWidget::dragEnterEvent(QDragEnterEvent *event)
//{
//    qDebug() << "a";
//    if (event->mimeData()->hasUrls()) // 如果拖入的是URLs（文件或目录）
//    {
//        event->acceptProposedAction();
//    }
//}

//void StackWidget::dropEvent(QDropEvent *event)
//{
//    qDebug() << "d";
//}

//bool QStackedWidget::eventFilter(QObject *object, QEvent *event) {
//    if(object == currentWidget()) {
//        return false;
//    }

//    return QStackedWidget::eventFilter(object, event);
//}
