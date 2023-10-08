#ifndef FILEPAGE_H
#define FILEPAGE_H

#include <QWidget>
#include <QTreeView>
#include <QFileSystemModel>
#include <QPushButton>
#include <QVBoxLayout>



class myFileSystemModel : public QFileSystemModel
{
public:

    explicit myFileSystemModel(QObject *parent);

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

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        QString filePath = this->filePath(index);
        QFileInfo fileInfo(filePath);
        QString fileSuffix = fileInfo.suffix();
        if (role == Qt::DecorationRole) {
//            return QVariant();  // 返回一个无效的QVariant来隐藏图标
            if (fileInfo.isFile() && (fileSuffix == "cpp" || fileSuffix == "cc")) {
                return QIcon(":/images/C++.ico");
            } else {
                return QVariant();
            }
        }
        return QFileSystemModel::data(index, role);
    }
};




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


public slots:

    void sloveOpenFolder();

signals:
    void openFolder();

    void selectFileIndexChanged(QString&);

private:
    QString currentRootPath;

    QString currentPath;

    QString currentSelectFilePath;

    QPushButton *openFolderBtn;

    myFileSystemModel *fileModel;

    QWidget *startWidget;

    QTreeView *folderTreeView;
};

#endif // FILEPAGE_H
