#ifndef QCODE_H
#define QCODE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class QCode; }
QT_END_NAMESPACE

class QCode : public QWidget
{
    Q_OBJECT

public:
    QCode(QWidget *parent = nullptr);
    ~QCode();


//    bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result);

private:
    Ui::QCode *ui;



    int borderWidth;



};
#endif // QCODE_H
