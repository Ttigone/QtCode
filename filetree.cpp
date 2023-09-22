#include "filetree.h"

#include <QStandardItem>
#include <qtreeview.h>

FileTree::FileTree(QWidget *parent)
    : QWidget{parent}
{
    QStandardItemModel *treeModel = new QStandardItemModel;
    treeModel->setHorizontalHeaderLabels(QStringList(QStringLiteral("FileTree")));

    QStandardItem* itemRoot = treeModel->invisibleRootItem();
    // 创建并添加Item的第一个子节点
    QStandardItem* itemCam = new QStandardItem(QStringLiteral("相机"));
    itemRoot->appendRow(itemCam);

    // 向第一个子节点itemCam添加子节点数据
    QList<QStandardItem*> camList;
    camList.append(new QStandardItem("cam1"));
    camList.append(new QStandardItem("cam2"));
    camList.append(new QStandardItem("cam3"));
    itemCam->appendRows(camList);

    // 创建并添加Item的第二个子节点
    QStandardItem* itemImg = new QStandardItem(QStringLiteral("图片"));
    itemRoot->appendRow(itemImg);

    QTreeView *treeView = new QTreeView(this);
    treeView->setModel(treeModel);
}
