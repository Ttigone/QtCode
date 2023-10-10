#include "QCode.h"
#include "./ui_QCode.h"


#include "titlebar.h"
#include "codeedit.h"
#include "leftlabel.h"
#include "filepage.h"
#include "searchpage.h"


#include <QVBoxLayout>
#include <QTreeWidget>
#include <QSizePolicy>
#include <QSplitter>
#include <QToolTip>
#include <QStackedWidget>



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

    recordStackWidgetCount = QSharedPointer<int>(new int);

    setStackWidgetCount() = 0;


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

    SelfLabel *explorerLabel = labelList->getLabel(0);
    explorerLabel->setMaximumSize(30, 40);

//    explorer->setStyleSheet("QLabel{background-color: rgb(13, 13, 13);}");

    SelfLabel *searchLabel = labelList->getLabel(1);
    searchLabel->setMaximumSize(30, 40);

//    search->setStyleSheet("QLabel{background-color:red;}");

    SelfLabel *settingLabel = labelList->getLabel(2);
    settingLabel->setMaximumSize(30, 40);
//    settingLabel->setStyleSheet("QLabel{background-color:red;}");


    taskBarView->addWidget(explorerLabel);
    taskBarView->addWidget(searchLabel);

    taskBarView->addStretch();

    taskBarView->addWidget(settingLabel);



    taskBarView->addWidget(new QLabel(this));


    filePage = new FilePage;

    searchPage = new SearchPage;

    QStackedWidget *stackWidget= new QStackedWidget(this);

    stackWidget->addWidget(filePage->getStartWidget());

    stackWidget->addWidget(filePage->getView());   // 如果成功打开文件夹，则显示

    stackWidget->addWidget(searchPage);

    connect(filePage, &FilePage::openFolder, this, [=](){
//        filePage->getStartWidget()->hide();
        stackWidget->setCurrentIndex(1);
    });   // 检测打开文件夹信号



//    filePage->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

//    searchPage->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    filePage->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    searchPage->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    stackWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    tabWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

//    filePage->getView()->setMinimumSize(200, 800);

//    searchPage->setMinimumSize(200, 800);

    stackWidget->setMinimumWidth(150);  // 可最小缩放比例

    stackWidget->setMaximumWidth(this->width() * 0.8);              // 右侧编辑区域的最小可视化区域



    QSplitter *splitter = new QSplitter(Qt::Horizontal);

    splitter->addWidget(stackWidget);           // 只有第一个界面与 tabWidget 的边界生效了样式表
    splitter->addWidget(tabWidget);


    splitter->setStyleSheet("QSplitter::handle {background-color: rgb(13, 13, 13);}");
    splitter->setHandleWidth(0);  // 设置 0 的话，样式表颜色失效
    splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    splitter->setSizes(QList<int>() << 200 << width() - 200 - 30);


    // 将 leftlabel 中传出一个点击 label 的信号，根据这个信号决定应该显示哪个 stackWidget

//    connect(explorer, &SelfLabel::clicked, this, [=](){
//        if (filePage->getView()->isHidden()) {
//            filePage->getView()->show();
//            if (stackWidget->isHidden()) {
//                stackWidget->show();
//            }
//            stackWidget->setCurrentWidget(filePage->getView());
//        } else {
//            filePage->getView()->hide();
//            if (!stackWidget->isHidden()) {
//                stackWidget->hide();
//            }
//        }
//    });

    connect(explorerLabel, &SelfLabel::clicked, this, [=](){    // 是否能简化
        if (filePage->hasFolder()) {
            if (filePage->getView()->isHidden()) {
                filePage->getView()->show();
                if (stackWidget->isHidden()) {
                    stackWidget->show();
                }
                stackWidget->setCurrentWidget(filePage->getView());
            } else {
                filePage->getView()->hide();
                if (!stackWidget->isHidden()) {
                    stackWidget->hide();
                }
            }
        } else {
            if (filePage->getStartWidget()->isHidden()) {
                filePage->getStartWidget()->show();
                if (stackWidget->isHidden()) {
                    stackWidget->show();
                }
                stackWidget->setCurrentIndex(0);
            } else {
                filePage->getStartWidget()->hide();
                if (!stackWidget->isHidden()) {
                    stackWidget->hide();
                }
            }
            stackWidget->setCurrentIndex(0);
        }
    });


    connect(searchLabel, &SelfLabel::clicked, this, [=](){
        if (searchPage->isHidden()) {
            searchPage->show();
            if (stackWidget->isHidden()) {
                stackWidget->show();
            }
            stackWidget->setCurrentWidget(searchPage);
        } else {
            searchPage->hide();
            if (!stackWidget->isHidden()) {
                stackWidget->hide();
            }
        }
    });




    hLayout->addLayout(taskBarView);

//    hLayout->addWidget(stackWidget);

//    hLayout->addWidget(tabWidget);

    hLayout->addWidget(splitter);

    vLayout->addLayout(hLayout);



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
    connect(titleBar, &TitleBar::openFolderTriggered, this, &QCode::openFolderTriggered);

    connect(titleBar, &TitleBar::saveTriggered, this, &QCode::saveTriggered);
    connect(titleBar, &TitleBar::saveAsTriggered, this, &QCode::saveAsTriggered);

    connect(titleBar, &TitleBar::undoTriggered, this, &QCode::undoTriggered);
    connect(titleBar, &TitleBar::redoTriggered, this, &QCode::redoTriggered);
    connect(titleBar, &TitleBar::cutTriggered, this, &QCode::cutTriggered);
    connect(titleBar, &TitleBar::copyTriggered, this, &QCode::copyTriggered);
    connect(titleBar, &TitleBar::pasteTriggered, this, &QCode::pasteTriggered);

    connect(titleBar, &TitleBar::aboutTriggered, this, &QCode::aboutTriggered);

    connect(tabWidget, &QTabWidget::tabCloseRequested, this, &QCode::tabWidgetTabCloseRequested);

    connect(filePage, &FilePage::selectFileIndexChanged, this, &QCode::openProjectFile);



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

void QCode::openRecentFile()     // 应该是打开最近文件夹
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

    saveHistory(fileName);

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

int& QCode::setStackWidgetCount()
{
    return *(recordStackWidgetCount.data());
}

int QCode::getStackWidgetCount()
{
    return *(recordStackWidgetCount.data());
}


void QCode::newTextFileTriggered()
{
    QString initText = "Untitled";
    QString fileTitle = initText + (QChar)(getCurrentTableCount() == 0 ?  ' ' : getCurrentTableCount() + 48) +  ".txt";
    tabWidget->addTab(new CodeEdit(this), fileTitle);
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
    filePath = QFileDialog::getOpenFileName(this, "打开文件");

    if (storeCurrentOpenFilePath.contains(filePath)) {
        // 文件已经被打开 应切换到相应界面
        tabWidget->setCurrentIndex(storeCurrentOpenFilePath[filePath]);
    } else {
        QFile file(filePath);

        if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this, "警告", "无法打开此文件: " + file.errorString());
            return;
        }
        QTextStream in(&file);
        QString contentText = in.readAll();

        CodeEdit *codeEditor = new CodeEdit(this);

        codeEditor->setPlainText(contentText);
        tabWidget->addTab(codeEditor, filePath);                            // 添加新页面
        storeCurrentOpenFilePath[filePath] = tabWidget->count() - 1;        // 保存当前页面的新路径
        tabWidget->setCurrentIndex(tabWidget->count() - 1);                 // 切换到新页面

        file.close();
        saveHistory(filePath);
        initRecentMenu();
    }

}

void QCode::openFolderTriggered()  // 不能重复打开其他文件夹
{
    // 需要覆盖之前已经打开的文件夹，同时打开的页面也得更换
//    if (!filePage->hasFolder()) {
//        filePage->sloveOpenFolder();
//    }
    filePage->sloveOpenFolder();

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

void QCode::openProjectFile(const QString& projectFileName)
{
    if (storeCurrentOpenFilePath.contains(projectFileName)) {
        tabWidget->setCurrentIndex(storeCurrentOpenFilePath[projectFileName]);
    } else {
        QFile file(projectFileName);

        if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this, "警告", "无法打开此文件: " + file.errorString());
            return;
        }

        QTextStream in(&file);
        QString contentText = in.readAll();

        CodeEdit *codeEditor = new CodeEdit(this);

        codeEditor->setPlainText(contentText);

        tabWidget->addTab(codeEditor, projectFileName);
        tabWidget->setCurrentIndex(tabWidget->count() - 1);

        storeCurrentOpenFilePath[projectFileName] = tabWidget->count() - 1;

        file.close();
    }
}








