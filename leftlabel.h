#ifndef LEFTLABEL_H
#define LEFTLABEL_H

#include "selflabel.h"


#include <QWidget>



class LeftLabel : public QWidget
{
    Q_OBJECT
public:
    explicit LeftLabel(QWidget *parent = nullptr);

public:
    void setLabel(SelfLabel *);

    SelfLabel* getLabel(size_t);


private:
    QList<SelfLabel *> storeLabel;


};

#endif // LEFTLABEL_H
