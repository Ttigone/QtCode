#ifndef TABWIDGET_H
#define TABWIDGET_H

#include "codeedit.h"


#include <QTabWidget>
#include <QFileInfo>
#include <QFile>


class TabWidget : public QTabWidget
{
    Q_OBJECT
public:
    TabWidget(QWidget* parent = nullptr);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
//    void dragMoveEvent(QDragMoveEvent *event) override;
//    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

signals:
    void openFileTriggered(const QString& filePath);
    void openFileFolderTriggered(const QString& filePath);
};

#endif // TABWIDGET_H
