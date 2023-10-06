#include "FilePage.h"

#include <QFileDialog>
#include <QStandardItem>


myFileSystemModel::myFileSystemModel(QObject *parent)
    : QFileSystemModel(parent)
{

}

FilePage::FilePage(QWidget *parent)
    : QWidget{parent},
    currentRootPath(""),
    openFolderBtn(new QPushButton(this)),
    fileModel(new myFileSystemModel(this)),
    folderTreeView(new QTreeView(this))
{
    // 得做一个打开文件夹的按钮
    // 将文件关联到 编辑区
    // 增加右键属性
    startWidget = new QWidget(this);

    openFolderBtn->setText("Open Folder");

    QVBoxLayout *vlayout = new QVBoxLayout(startWidget);
    openFolderBtn->setMaximumHeight(30);

    vlayout->addWidget(openFolderBtn);



    connect(openFolderBtn, &QPushButton::clicked, this, &FilePage::sloveOpenFolder);



//    QString lp = QDir::currentPath();
//    QString s = QFileDialog::getExistingDirectory(this, "open folder", lp);
//    lp = s;
//    fileModel->setRootPath(s);



//    QStandardItemModel *treeModel = new QStandardItemModel;
//    treeModel->setHorizontalHeaderLabels(QStringList(QStringLiteral("projectname")));

//    QStandardItem* itemRoot = treeModel->invisibleRootItem();
//    // 创建并添加Item的第一个子节点
//    QStandardItem* itemCam = new QStandardItem(QStringLiteral("string1"));
//    itemRoot->appendRow(itemCam);

//    // 向第一个子节点itemCam添加子节点数据
//    QList<QStandardItem*> camList;
//    camList.append(new QStandardItem("file1"));
//    camList.append(new QStandardItem("file2"));
//    camList.append(new QStandardItem("file3"));
//    itemCam->appendRows(camList);

//    // 创建并添加Item的第二个子节点
//    QStandardItem* itemImg = new QStandardItem(QStringLiteral("string2"));
//    itemRoot->appendRow(itemImg);

//    treeView->setModel(fileModel);
//    treeView->setRootIndex(fileModel->index(lp));
//    treeView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);   // 水平 ，垂直
}

QTreeView *FilePage::getView()
{
    return folderTreeView;
}

QWidget *FilePage::getStartWidget()
{
    return startWidget;
}

bool FilePage::hasFolder()
{
    return !currentRootPath.isEmpty();
}

void FilePage::sloveOpenFolder()
{
    currentRootPath = QDir::rootPath();

    QString currentPath = QFileDialog::getExistingDirectory(this, "Open Folder", currentRootPath);


    fileModel->setRootPath(currentPath);

    folderTreeView->setModel(fileModel);

    folderTreeView->setRootIndex(fileModel->index(currentPath));

    folderTreeView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);   // 水平 ，垂直

    folderTreeView->setFrameShape(QFrame::NoFrame);

    folderTreeView->hideColumn(1);
    folderTreeView->hideColumn(2);
    folderTreeView->hideColumn(3);  // 卡顿


    emit openFolder();
}

