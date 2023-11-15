#ifndef SEARCHPAGE_H
#define SEARCHPAGE_H

#include <QLineEdit>
#include <QWidget>

class SearchPage : public QWidget
{
    Q_OBJECT
public:
    explicit SearchPage(QWidget *parent = nullptr);

private:
    QLineEdit *searchEdit;


signals:

};

#endif // SEARCHPAGE_H
