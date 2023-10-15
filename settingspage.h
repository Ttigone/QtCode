#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include <QWidget>

// 左边是 listView 右边是 stackWidget, 两者被 Splitter 分割
// stackWidget 包含了 listView 对应的 widget
// 这个 widget 由 listView 组成，每个 listView 对应有许多情况

// 搜索栏界面，解决搜索的目标名称


class SettingsPage : public QWidget
{
    Q_OBJECT
public:
    explicit SettingsPage(QWidget *parent = nullptr);

signals:

};

#endif // SETTINGSPAGE_H
