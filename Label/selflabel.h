#ifndef SELFLABEL_H
#define SELFLABEL_H

#include <QLabel>
#include <QWidget>
#include <QMouseEvent>


class SelfLabel : public QLabel
{
    Q_OBJECT
public:
    explicit SelfLabel(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent* event) override;

signals:
    void clicked();

};

#endif // SELFLABEL_H
