#include "QCode.h"
#include "./ui_QCode.h"


#include "titlebar.h"
#include "codeedit.h"
#include "filetree.h"
#include "leftlabel.h"

#include "searchlabel.h"


#include <QVBoxLayout>
#include <QTreeWidget>
#include <QSizePolicy>
#include <QSplitter>
#include <QToolTip>

//调用WIN API需要用到的头文件与库 [实现缩放]
#ifdef Q_OS_WIN
#include <qt_windows.h>
#include <Windowsx.h>
#include <qlist.h>
#include <QMessageBox>
#include <QFileDialog>
#include <QSettings>

#endif



QSettings *mSettings;    // 配置文件


QCode::QCode(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QCode)
{
    ui->setupUi(this);

    // 配置文件
    if (mSettings == nullptr) {
        mSettings = new QSettings("setting.ini", QSettings::IniFormat);
    }

    // 初始化界面
    initWidget();
    // 初始化信号槽连接
    initConnection();

    // 初始化最近菜单
    initRecentMenu();



}

QCode::~QCode()
{
    delete ui;
}



//// 拖动窗口边缘，实现缩放     // 会导致左侧按钮失效
//bool QCode::nativeEvent(const QByteArray &eventType, void *message, qintptr *result)
//{
//    Q_UNUSED(eventType)

//    MSG *param = static_cast<MSG *>(message);

//    switch (param->message)
//    {
//    case WM_NCHITTEST:
//    {
//        int nX = GET_X_LPARAM(param->lParam) - this->geometry().x();
//        int nY = GET_Y_LPARAM(param->lParam) - this->geometry().y();

//        // 如果鼠标位于子控件上，则不进行处理
//        if (childAt(nX, nY) != nullptr)              // 设置这个会导致 titlebar 无效
//            return QWidget::nativeEvent(eventType, message, result);

//        *result = HTCAPTION;

//        // 鼠标区域位于窗体边框，进行缩放
//        if ((nX > 0) && (nX < borderWidth))
//            *result = HTLEFT;

//        if ((nX > this->width() - borderWidth) && (nX < this->width()))
//            *result = HTRIGHT;

//        if ((nY > 0) && (nY < borderWidth))
//            *result = HTTOP;

//        if ((nY > this->height() - borderWidth) && (nY < this->height()))
//            *result = HTBOTTOM;

//        if ((nX > 0) && (nX < borderWidth) && (nY > 0)
//            && (nY < borderWidth))
//            *result = HTTOPLEFT;

//        if ((nX > this->width() - borderWidth) && (nX < this->width())
//            && (nY > 0) && (nY < borderWidth))
//            *result = HTTOPRIGHT;

//        if ((nX > 0) && (nX < borderWidth)
//            && (nY > this->height() - borderWidth) && (nY < this->height()))
//            *result = HTBOTTOMLEFT;

//        if ((nX > this->width() - borderWidth) && (nX < this->width())
//            && (nY > this->height() - borderWidth) && (nY < this->height()))
//            *result = HTBOTTOMRIGHT;

//        return true;
//    }
//    }

//    return QWidget::nativeEvent(eventType, message, result);
//}

void QCode::initWidget()
{
    // 设置无标题框
//    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

//    setWindowOpacity(0.8);  // 设置透明度

    // 设置初始大小
//    resize(1200, 800);

//    QToolTip::setShowDelay(10);


    setAttribute(Qt::WA_StyledBackground);

    // 设置标题图标
    QIcon windowIcon(":/images/qc.png");
    setWindowIcon(windowIcon);


    tabWidget = new QTabWidget(this);


    // 为主界面窗口设置垂直布局
    QVBoxLayout *vLayout = new QVBoxLayout;

    // 自定义标题框
    titleBar = new TitleBar;
    // 安装过滤器
//    installEventFilter(titleBar);

    // 添加第一个 widget
    vLayout->addWidget(titleBar);

    // 设置间隙
    vLayout->setSpacing(0);  // 与下一部件的间距

    vLayout->setContentsMargins(5, 0, 5, 5);  // left top right bottom 边缘间距像素

    // 水平布局
    QHBoxLayout *hLayout = new QHBoxLayout;

    QVBoxLayout *taskBarView = new QVBoxLayout;       // left bar

    LeftLabel *labelList = new LeftLabel();

    //    qDebug() << labelList->getLabel(0);

    SelfLabel *explorer = labelList->getLabel(0);
//    explorer->setToolTip("Explorer");
    explorer->setMinimumWidth(20);
    explorer->setMaximumWidth(31);
    explorer->setMaximumHeight(40);


    //    qDebug() << explorer;
    //    qDebug() << explorer->pixmap();
    //    explorer->setText("1");
    //    // 最左侧按钮
    //    QLabel *explorer = new QLabel("Explorer");
//    explorer->setStyleSheet("color: rgb(204, 204, 204);");



//    SelfLabel *search = labelList->getLabel(1);

    SearchLabel *w = new SearchLabel();

    SelfLabel *search = w->getLabel();
//    qDebug() << search->toolTip();
    search->setMinimumSize(20, 40);
    search->setMaximumSize(31, 40);
//    qDebug() << search->toolTip();
//    search->setToolTip("Search");
//    search->setMinimumWidth(20);
//    search->setMaximumWidth(100);
//    search->setMaximumHeight(100);
//    search->setStyleSheet("background-color:red");


    taskBarView->addWidget(explorer);
    taskBarView->addWidget(search);




    taskBarView->addWidget(new QLabel(this));


    fileTree = new FileTree;


    fileTree->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    tabWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    fileTree->setMinimumSize(200, 800);


//    connect(explorer, &SelfLabel::clicked, [=](){
//        if (fileTree->isHidden()) {
//            fileTree->show();
//        } else {
//            fileTree->hide();
//        }
//    });



    hLayout->addLayout(taskBarView);

    hLayout->setSpacing(1);

    hLayout->addWidget(fileTree);

    hLayout->addWidget(tabWidget);


    vLayout->addLayout(hLayout);


//    hLayout->addStretch();  // 会导致 fileTree 显示错误



//    hLayout->setSpacing(1);

//    hLayout->addWidget(tabWidget);   // 覆盖

//    vLayout->addLayout(hLayout);

//    QTreeWidget *treeWidget = new QTreeWidget(this);      // left widget about dict
//    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);   // set Policy  useless


//    QSplitter *splitter = new QSplitter(Qt::Horizontal);


//    treeWidget->setMinimumWidth(150);
////    treeWidget->setMaximumWidth(900);
//    treeWidget->setHeaderHidden(true);
//    treeWidget->setStyleSheet("QTreeWidget{border: 0px solid gray;}");

//    tabWidget->setParent(splitter);
//    treeWidget->setParent(splitter);

//    splitter->addWidget(fileTree);
//    splitter->addWidget(tabWidget);


//    splitter->setStyleSheet("QSplitter::handle {background-color: rgb(13, 13, 13);}");
//    splitter->setHandleWidth(0);  // 设置 0 的话，样式表颜色失效
//    splitter->setStyleSheet("QSplitter::handle { border: 1px solid grey }");






//    vLayout->addWidget(splitter);        // add to vLayout

//    treeWidget->setSizePolicy(sizePolicy);

//    QList<int> initSizes;
//    initSizes << 50 << splitter->width() - 10;
//    splitter->setSizes(initSizes);


//    QTreeWidgetItem *item1 = new QTreeWidgetItem(treeWidget, QStringList(QString("item1")));
//    QTreeWidgetItem *item2 = new QTreeWidgetItem(treeWidget, QStringList(QString("item2")));
//    QTreeWidgetItem *item3 = new QTreeWidgetItem(treeWidget, QStringList(QString("item3")));
//    QTreeWidgetItem *item4 = new QTreeWidgetItem(treeWidget, QStringList(QString("item4")));

//    item1->setIcon(0, QIcon(":/images/qc.png"));
//    item2->setIcon(0, QIcon(":/images/qc.png"));
//    item3->setIcon(0, QIcon(":/images/qc.png"));
//    item4->setIcon(0, QIcon(":/images/qc.png"));

//    treeWidget->expandAll();


    // 应用界面布局
    setLayout(vLayout);

    // 可拖拽的边距
    borderWidth = 5;
}

void QCode::initConnection()
{
    // 将菜单栏的 新建文本文件信号关联到匹配的槽函数
    connect(titleBar, &TitleBar::newTextFileTriggered, this, &QCode::newTextFileTriggered);
    connect(titleBar, &TitleBar::newWindowTriggered, this, &QCode::newWindowTriggered);

    connect(titleBar, &TitleBar::openFileTriggered, this, &QCode::openFileTriggered);

    connect(titleBar, &TitleBar::saveTriggered, this, &QCode::saveTriggered);
    connect(titleBar, &TitleBar::saveAsTriggered, this, &QCode::saveAsTriggered);

    connect(titleBar, &TitleBar::undoTriggered, this, &QCode::undoTriggered);
    connect(titleBar, &TitleBar::redoTriggered, this, &QCode::redoTriggered);
    connect(titleBar, &TitleBar::cutTriggered, this, &QCode::cutTriggered);
    connect(titleBar, &TitleBar::copyTriggered, this, &QCode::copyTriggered);
    connect(titleBar, &TitleBar::pasteTriggered, this, &QCode::pasteTriggered);

    connect(titleBar, &TitleBar::aboutTriggered, this, &QCode::aboutTriggered);

    connect(tabWidget, &QTabWidget::tabCloseRequested, this, &QCode::tabWidgetTabCloseRequested);



}

void QCode::saveHistory(QString path)
{
    // 读取历史
    QList<QString> lists = getHistory();
    lists.append(path);

            foreach (QString str, lists) {
            if (str == path) {
                lists.removeOne(str);
            }
        }
    lists.append(path);

    // 打开开始写入
    mSettings->beginWriteArray("history");
    for (int i = 0; i < lists.size(); ++i) {
        mSettings->setArrayIndex(i);

        // 保留字符串
        mSettings->setValue("path", lists.at(i));
    }


    // 关闭开始写入
    mSettings->endArray();

    initRecentMenu();
}

QList<QString> QCode::getHistory()
{
    int size = mSettings->beginReadArray("history");

    QList<QString> lists;
    for (int i = 0; i < size; ++i) {
        mSettings->setArrayIndex(i);
        QString path = mSettings->value("path").toString();
        lists.append(path);
    }
    mSettings->endArray();

    return lists;
}

void QCode::openRecentFile()
{
    QAction *action = (QAction *)sender();
    QString fileName = action->text();

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "警告", "无法打开此文件: " + file.errorString());
        return;
    }
    QTextStream in(&file);
    QString content_text = in.readAll();

    CodeEdit *codeEditor = new CodeEdit(this);
    codeEditor->setPlainText(content_text);
    tabWidget->addTab(codeEditor, fileName);
    tabWidget->setCurrentIndex(tabWidget->count() - 1);

    file.close();

    saveHistory(file_name);

    initRecentMenu();
}

void QCode::clearHistoryRecord()
{
    mSettings->remove("history");
    initRecentMenu();
}

void QCode::initRecentMenu()
{
    QMenu *recent = this->findChild<QMenu *>("recent");

    qDebug() << recent;  // 能找到

    // 去重
    QList<QObject *> chlists = recent->children().toList();
    QSet<QObject *> set_lists(chlists.cbegin(), chlists.cend());

            foreach (QObject *ch, set_lists) {
            QAction *action = (QAction *)ch;
            recent->removeAction(action);
        }

    // BUG
    QList<QString> lists = getHistory();          // 获取曾经未删除历史记录


    for (int i = lists.size() - 1; i >= 0; --i) {
        recent->addAction(lists.at(i), this, &QCode::openRecentFile);  // 添加新记录
    }


    if (lists.size() > 0) {
        recent->addSeparator();
        recent->addAction("Clear Recently Opened", this, &QCode::clearHistoryRecord);
    }
}

int QCode::getCurrentTableCount()
{
    return tabWidget->count();
}

void QCode::newTextFileTriggered()
{
    tabWidget->addTab(new CodeEdit(this), "Untitled.txt");
}

void QCode::newFileTriggered()
{

}

void QCode::newWindowTriggered()
{
    QCode *new_window = new QCode();
    new_window->show();
}

void QCode::openFileTriggered()
{
    file_name = QFileDialog::getOpenFileName(this, "打开文件");
    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "警告", "无法打开此文件: " + file.errorString());
        return;
    }
    QTextStream in(&file);
    QString content_text = in.readAll();


    CodeEdit *code_editor = new CodeEdit(this);

    code_editor->setPlainText(content_text);
    tabWidget->addTab(code_editor, file_name);

    tabWidget->setCurrentIndex(tabWidget->count() - 1);


    file.close();

    saveHistory(file_name);

    initRecentMenu();
}

void QCode::openFolderTriggered()
{

}

void QCode::saveTriggered()
{

    CodeEdit *codeEditor = (CodeEdit *)tabWidget->currentWidget();

    if (codeEditor) {
        if (codeEditor->saveFile()) {
            QString fileName = codeEditor->getFileName();
            tabWidget->setTabText(tabWidget->currentIndex(), fileName);
            saveHistory(fileName);
            initRecentMenu();
        }
    }


}

void QCode::saveAsTriggered()
{


    CodeEdit *codeEditor = (CodeEdit *)tabWidget->currentWidget();

    if (codeEditor) {
        if (codeEditor->saveAsFile()) {
            QString fileName = codeEditor->getFileName();
            tabWidget->setTabText(tabWidget->currentIndex(), fileName);
            saveHistory(fileName);
            initRecentMenu();
        }
    }



}

void QCode::closeEditorTriggered()
{

}

void QCode::closeWindowTriggered()
{

}

void QCode::undoTriggered()
{
    CodeEdit *codeEditor = (CodeEdit *)tabWidget->currentWidget();
    if (codeEditor) {
        codeEditor->undo();
    }
}

void QCode::redoTriggered()
{
    CodeEdit *codeEditor = (CodeEdit *)tabWidget->currentWidget();
    if (codeEditor) {
        codeEditor->redo();
    }
}

void QCode::cutTriggered()
{
    CodeEdit *codeEditor = (CodeEdit *)tabWidget->currentWidget();
    if (codeEditor) {
        codeEditor->cut();
    }
}

void QCode::copyTriggered()
{
    CodeEdit *codeEditor = (CodeEdit *)tabWidget->currentWidget();
    if (codeEditor) {
        codeEditor->copy();
    }
}

void QCode::pasteTriggered()
{
    CodeEdit *codeEditor = (CodeEdit *)tabWidget->currentWidget();
    if (codeEditor) {
        codeEditor->paste();
    }
}

void QCode::findTriggered()
{

}

void QCode::replaceTriggered()
{

}

void QCode::selectAllTriggered()
{

}

void QCode::commandPaletteTriggered()
{

}

void QCode::WelcomeTriggered()
{

}

void QCode::showAllCommandsTriggered()
{

}

void QCode::aboutTriggered()
{
    QMessageBox::about(this, "QCode", "乞丐版-vscode");
}

void QCode::tabWidgetTabCloseRequested(int index)
{
    CodeEdit *codeEditor = (CodeEdit *)tabWidget->currentWidget();
    if (!codeEditor->checkSaved()) {
        QMessageBox::StandardButton btn = QMessageBox::question(this, "警告", "文件未保存, 是否保存文件", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);


        if (btn == QMessageBox::Yes) {
            if (codeEditor->saveFile()) {
                QString fileName = codeEditor->getFileName();
                saveHistory(fileName);
                initRecentMenu();
            }
            return;
        } else if (btn == QMessageBox::Cancel) {
            return;
        }
    }
//    delete tabWidget->currentWidget();
    tabWidget->removeTab(index);
    delete codeEditor;
}








