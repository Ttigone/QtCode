#include "FilePage.h"

#include <QFileDialog>
#include <QStandardItem>


myFileSystemModel::myFileSystemModel(QObject *parent)
    : QFileSystemModel(parent)
{
    // 不在里面的为 other
    fileSuffixMap["cpp"] = FileSuffix::Cpp;
    fileSuffixMap["cc"] = FileSuffix::Cpp;
    fileSuffixMap["h"] = FileSuffix::h;
    fileSuffixMap["hpp"] = FileSuffix::hpp;
    fileSuffixMap[""] = FileSuffix::other;

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

//    folderTreeView->setModel(nullptr);

    // 由于是在打开文件夹之后才为 folderTreeView 设置相应的 Model 故此处信号槽连接时，没有相应的 信号发出者
//    QObject::connect(folderTreeView->selectionModel(), &QItemSelectionModel::selectionChanged, [=](const QItemSelection &selected, const QItemSelection &deselected) {

//        Q_UNUSED(deselected);

//        if (selected.indexes().count() > 0) {
//            QModelIndex selectedIndex = selected.indexes().first();
//            auto info = fileModel->fileInfo(selectedIndex);
//            if (info.isFile()) {
//                QString path = fileModel->filePath(selectedIndex);
//                qDebug() << "Selected path:" << path;
//            }
//        }
//    });


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
    return !currentPath.isEmpty();
}

QString& FilePage::getCurrentSelectFilePath()
{
    return currentSelectFilePath;
}

void FilePage::sloveOpenFolder()
{
    currentRootPath = QDir::rootPath();

    currentPath = QFileDialog::getExistingDirectory(this, "Open Folder", currentRootPath);

    if (currentPath.isEmpty()) {    // 未能够正确打开文件夹
        return;
    }

    fileModel->setRootPath(currentPath);


    folderTreeView->setModel(fileModel);

    folderTreeView->setRootIndex(fileModel->index(currentPath));

    folderTreeView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);   // 水平 ，垂直

    folderTreeView->setFrameShape(QFrame::NoFrame);

    folderTreeView->hideColumn(1);
    folderTreeView->hideColumn(2);
    folderTreeView->hideColumn(3);

    // 由于是在打开文件夹之后才为 folderTreeView 设置相应的 Model 故此处信号槽连接时，没有相应的 信号发出者
    QObject::connect(folderTreeView->selectionModel(), &QItemSelectionModel::selectionChanged, [=](const QItemSelection &selected, const QItemSelection &deselected) {

        Q_UNUSED(deselected);

        if (selected.indexes().count() > 0) {
            QModelIndex selectedIndex = selected.indexes().first();
            auto info = fileModel->fileInfo(selectedIndex);
            if (info.isFile()) {
//                QString path = fileModel->filePath(selectedIndex);
//                qDebug() << "Selected path:" << path;
                currentSelectFilePath = fileModel->filePath(selectedIndex);
                emit selectFileIndexChanged(currentSelectFilePath);
            }
        }
    });

    emit openFolder();
}

