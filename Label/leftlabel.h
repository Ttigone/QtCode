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
    void drawExplorerLabel();

    void drawSearchLabel();

    void drawSettingLabel();



    void setLabel(SelfLabel *);

    SelfLabel* getLabel(size_t);


signals:
    void labelChanged(int);

private:
    SelfLabel *explorerLabel;

    SelfLabel *searchLabel;

    SelfLabel *settingLabel;

    QList<SelfLabel *> storeLabel;



};

#endif // LEFTLABEL_H
