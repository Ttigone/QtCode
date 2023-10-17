#include "tabwidget.h"



#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QMessageBox>
#include <qpushbutton.h>


TabWidget::TabWidget(QWidget *parent)
    : QTabWidget{parent}
{
    setAcceptDrops(true);
}
void TabWidget::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasUrls()) // 如果拖入的是URLs（文件或目录）
    {
        event->acceptProposedAction();
    }
}
//void TabWidget::dragMoveEvent(QDragMoveEvent *event) {
//    qDebug() << "m";
//}

//void TabWidget::dragLeaveEvent(QDragLeaveEvent *event) {
//    qDebug() << "l";
//}

void TabWidget::dropEvent(QDropEvent *event) {
    foreach (const QUrl &url, event->mimeData()->urls()) {
        QString filePath = url.toLocalFile();
        QFileInfo fileInfo(filePath);
        if (!filePath.isEmpty()) {
            if (fileInfo.isFile()) {
                emit openFileTriggered(filePath);
            } else if (fileInfo.isDir()) {
                emit openFileFolderTriggered(filePath);
            }
        }
    }
}


