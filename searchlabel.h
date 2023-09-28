#ifndef SEARCHLABEL_H
#define SEARCHLABEL_H

#include "selflabel.h"

#include <QLabel>
#include <QWidget>

class SearchLabel : public QWidget
{
    Q_OBJECT
public:
    explicit SearchLabel(QWidget *parent = nullptr);

public:
    SelfLabel* getLabel();

private:

    SelfLabel *searchLabel;

};

#endif // SEARCHLABEL_H
