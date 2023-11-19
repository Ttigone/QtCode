#include "filepage.h"

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
    fileSuffixMap[""] = FileSuffix::none;
    fileSuffixMap["txt"] = FileSuffix::txt;

    // 不能打开的文件
//    fileSuffixMap["exe"] = FileSuffix::unused;
//    fileSuffixMap["dll"] = FileSuffix::unused;
//    fileSuffixMap["png"] = FileSuffix::unused;
//    fileSuffixMap["jpg"] = FileSuffix::unused;
//    fileSuffixMap["jpeg"] = FileSuffix::unused;
//    fileSuffixMap["gif"] = FileSuffix::unused;
//    fileSuffixMap["bmp"] = FileSuffix::unused;
//    fileSuffixMap["ico"] = FileSuffix::unused;
//    fileSuffixMap["tiff"] = FileSuffix::unused;
//    fileSuffixMap["tif"] = FileSuffix::unused;
//    fileSuffixMap["svg"] = FileSuffix::unused;
//    fileSuffixMap["svgz"] = FileSuffix::unused;
//    fileSuffixMap["psd"] = FileSuffix::unused;
//    fileSuffixMap["raw"] = FileSuffix::unused;
//    fileSuffixMap["webp"] = FileSuffix::unused;
//    fileSuffixMap["mp3"] = FileSuffix::unused;
//    fileSuffixMap["wav"] = FileSuffix::unused;
//    fileSuffixMap["wma"] = FileSuffix::unused;


}



// 自定义类没有 Qt 的窗口, 需要为类加一个主窗口
FilePage::FilePage(QWidget *parent)
    : QWidget{parent},
    currentRootPath(""),
    fileModel(new myFileSystemModel(this)),
    folderTreeView(new QTreeView(this))
{
    setAcceptDrops(true);

    folderTreeView->setAcceptDrops(true);
    folderTreeView->installEventFilter(this);
    folderTreeView->setStyleSheet("background-color:gray");
//    eventFilter(folderTreeView);
    // 得做一个打开文件夹的按钮
    // 将文件关联到 编辑区
    // 增加右键属性
//    startWidget = new QWidget(this);

//    startWidget = new StartWidget(this);

//    openFolderBtn = new QPushButton(startWidget),
    openFolderBtn = new QPushButton(this),
    openFolderBtn->setText("Open Folder");

    QVBoxLayout *vlayout = new QVBoxLayout();
    openFolderBtn->setMaximumHeight(30);

    vlayout->addWidget(openFolderBtn);

//    startWidget->setLayout(vlayout);
    setLayout(vlayout);

//    connect(openFolderBtn, &QPushButton::clicked, this, &FilePage::sloveOpenFolder);
    connect(openFolderBtn, &QPushButton::clicked, this, [=]() {
        sloveOpenFolder();
    });

//    qDebug() << this->height();
//    qDebug() << this->width();


}


QTreeView *FilePage::getView()
{
    return folderTreeView;
}

//QWidget *FilePage::getStartWidget()
//{
////    return startWidget;
//}

bool FilePage::hasFolder()
{
    return !currentPath.isEmpty();
}

QString& FilePage::getCurrentSelectFilePath()
{
    return currentSelectFilePath;
}

void FilePage::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) // 如果拖入的是URLs（文件或目录）
    {
        event->acceptProposedAction();
    }
}

void FilePage::dropEvent(QDropEvent *event) {
    foreach (const QUrl &url, event->mimeData()->urls()) {
        QString filePath = url.toLocalFile();
        QFileInfo fileInfo(filePath);
        if (!filePath.isEmpty()) {
            if (fileInfo.isFile()) {
                emit openFileFromDrag(filePath);
            } else if (fileInfo.isDir()) {
                sloveOpenFolder(filePath);
            }
        }
    }
}

bool FilePage::eventFilter(QObject* watched, QEvent* event) {
    if (watched == folderTreeView) { // 你可以通过这个条件判断来确保只处理来自QTreeView的事件
        switch (event->type()) {
        case QEvent::DragEnter: {
            QDragEnterEvent *dragEvent = static_cast<QDragEnterEvent*>(event);
            if (dragEvent->mimeData()->hasUrls()) {
                dragEvent->acceptProposedAction();
                return true;
            }
            break;
        }
        case QEvent::DragMove: {
            QDragMoveEvent *dragEvent = static_cast<QDragMoveEvent*>(event);
            dragEvent->acceptProposedAction();
            return true;
        }
        case QEvent::Drop: {
            QDropEvent *dropEvent = static_cast<QDropEvent*>(event);
            foreach (const QUrl &url, dropEvent->mimeData()->urls()) {
                QString filePath = url.toLocalFile();
                QFileInfo fileInfo(filePath);
                if (!filePath.isEmpty()) {
                    if (fileInfo.isFile()) {
                        emit openFileFromDrag(filePath);
                    } else if (fileInfo.isDir()) {
                        sloveOpenFolder(filePath);
                    }
                }
            }
            return true;
        }
        default:
            break;
        }
    }
    return QObject::eventFilter(watched, event); // 对于不想处理的事件，调用基类的方法
}


void FilePage::sloveOpenFolder(const QString& filePath)
{
    currentRootPath = QDir::rootPath();

    currentPath = filePath;
    if (currentPath.isEmpty()) {
        currentPath = QFileDialog::getExistingDirectory(this, "Open Folder", currentRootPath);
    }

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

        if (selected.indexes().count() > 0) {   // 处理不能打开的文件
            QModelIndex selectedIndex = selected.indexes().first();
            auto info = fileModel->fileInfo(selectedIndex);
            if (info.isFile()) {     // 是一个文件
                switch (fileModel->getFileSuffixMap()[info.suffix()]) {
                    case myFileSystemModel::FileSuffix::Cpp:
                    case myFileSystemModel::FileSuffix::h:
                    case myFileSystemModel::FileSuffix::hpp:
                    case myFileSystemModel::FileSuffix::none:
                    case myFileSystemModel::FileSuffix::txt:
                        currentSelectFilePath = fileModel->filePath(selectedIndex);
                        emit selectFileIndexChanged(currentSelectFilePath);
                        break;
                    default:
                        emit selectFileIndexChanged("null");
                        break;
                }
            }
        }
    });

    emit openFolder();
}

