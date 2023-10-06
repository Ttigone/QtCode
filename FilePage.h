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

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override {
        if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
            // 修改或去除列头
            switch (section) {
            case 0:
                return QStringLiteral("proj");
            case 1:
                return QVariant(); // 去除 "Size" 列
            case 2:
                return QStringLiteral("ty");
            case 3:
                return QStringLiteral("ti");
            default:
                return QFileSystemModel::headerData(section, orientation, role);
            }
        }

        return QFileSystemModel::headerData(section, orientation, role);
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

public slots:

    void sloveOpenFolder();

signals:
    void openFolder();


private:
    QString currentRootPath;

    QPushButton *openFolderBtn;

    myFileSystemModel *fileModel;

    QWidget *startWidget;

    QTreeView *folderTreeView;
};

#endif // FILEPAGE_H
