#include "searchpage.h"

#include <QVBoxLayout>
#include <QLabel>


SearchPage::SearchPage(QWidget *parent)
    : QWidget{parent}
{

    searchEdit = new QLineEdit(this);

    QVBoxLayout *vlayout = new QVBoxLayout;

    vlayout->addWidget(searchEdit);

    vlayout->addWidget(new QLabel("SearchPage", this));

    setLayout(vlayout);


}
