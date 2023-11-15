#ifndef STACKWIDGET_H
#define STACKWIDGET_H

#include <QEvent>
#include <QStackedWidget>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMimeData>

class StackWidget : public QStackedWidget
{
    Q_OBJECT
public:
    StackWidget(QWidget *parent = nullptr);

protected:

//    bool eventFilter(QObject *object, QEvent *event);
//    void dragEnterEvent(QDragEnterEvent *event) override;


//    void dropEvent(QDropEvent *event) override;
};

#endif // STACKWIDGET_H
