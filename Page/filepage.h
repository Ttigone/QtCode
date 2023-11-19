#ifndef FILEPAGE_H
#define FILEPAGE_H

#include <QWidget>
#include <QTreeView>
#include <QFileSystemModel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMimeData>
// 不支持 png 类型文件

class myFileSystemModel : public QFileSystemModel
{
public:

    explicit myFileSystemModel(QObject *parent);


    // 无后缀 是 none
    // 其他后缀 是 other
    enum FileSuffix {
        other = 0,
        Cpp,
        h,
        hpp,
        txt,
        none
    };


protected:

    // 自定义首行信息
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override {
        if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
            // 修改或去除列头
            switch (section) {
            case 0:
                return QStringLiteral("proj");
            case 1:
                return QVariant(); // 去除 "Size" 列
            case 2:
                return QVariant(); // 去除 "Type" 列
            case 3:
                return QVariant(); // 去除 "Date Modified" 列
            default:
                return QFileSystemModel::headerData(section, orientation, role);
            }
        }

        return QFileSystemModel::headerData(section, orientation, role);
    }

    // 在视图显示中去除系统自带图标

    // 这些重载函数与构造函数的执行顺序是怎样的？ 跟程序执行代码有关？
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        QString filePath = this->filePath(index);
        QFileInfo fileInfo(filePath);
        QString fileSuffix = fileInfo.suffix();
        if (role == Qt::DecorationRole) {
//            return QVariant();  // 返回一个无效的QVariant来隐藏图标
            if (fileInfo.isFile()) {
                switch (fileSuffixMap[fileSuffix]) {
                    case FileSuffix::Cpp:
                        return QIcon(":/images/IconOf-C++.ico");
                            break;
                    case FileSuffix::h:
                        return QIcon(":/images/IconOf-H.ico");
                            break;
                default:
                    return QVariant();
                        break;
                }
            }
        }
        return QFileSystemModel::data(index, role);
    }

public:
    const QHash<QString, FileSuffix>& getFileSuffixMap() {
        return fileSuffixMap;
    }

private:
    QHash<QString, FileSuffix> fileSuffixMap;       // 字符串 和 相应枚举值的对应
};


//class StartWidget : public QWidget {
//    Q_OBJECT
//public:
//    explicit  StartWidget(QWidget *parent = nullptr) {
//        setAcceptDrops(true);                   // 让拖拽事件响应
//    }
//
//protected:
//    void dragEnterEvent(QDragEnterEvent *event) override {
//        if (event->mimeData()->hasUrls()) // 如果拖入的是URLs（文件或目录）
//        {
//            event->acceptProposedAction();
//        }
//    }
//
//    void dropEvent(QDropEvent *event) override {
//        qDebug() << "s";
//    }
//};

class FilePage: public QWidget
{
    Q_OBJECT
public:
    explicit FilePage(QWidget *parent = nullptr);

public:
    QTreeView* getView();

    QWidget* getStartWidget();

    bool hasFolder();

    QString& getCurrentSelectFilePath();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;

    void dropEvent(QDropEvent *event) override;

    bool eventFilter(QObject* watched, QEvent* event) override;

public slots:

    void sloveOpenFolder(const QString& filePath = "");    // 添加路径

signals:
    void openFileFromDrag(const QString& filePath);

    void openFolder();

    void selectFileIndexChanged(const QString& file = "");

private:
    QString currentRootPath;

    QString currentPath;

    QString currentSelectFilePath;

    QPushButton *openFolderBtn;

    myFileSystemModel *fileModel;

//    QWidget *startWidget;
//    StartWidget *startWidget;

    QTreeView *folderTreeView;        // 添加拖拽事件？
};

#endif // FILEPAGE_H
