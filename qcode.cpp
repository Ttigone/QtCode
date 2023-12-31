#include "QCode.h"
#include "./ui_QCode.h"


#include "titlebar.h"
#include "codeedit.h"
#include "Label/leftlabel.h"
#include "Page/filepage.h"
#include "Page/searchpage.h"
#include "Page/settingspage.h"
#include "tabwidget.h"
#include "stackwidget.h"

#include <QVBoxLayout>
#include <QTreeWidget>
#include <QSizePolicy>
#include <QSplitter>
#include <QToolTip>
#include <QStackedWidget>
#include <QFrame>


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


//     设置无标题框
    setWindowFlags(Qt::FramelessWindowHint);

//    setWindowOpacity(0.8);  // 设置透明度

    // 设置初始大小
//    resize(1200, 800);

//    QToolTip::setShowDelay(10);


//    setAttribute(Qt::WA_StyledBackground);

    // 设置标题图标
    QIcon windowIcon(":/images/qc.png");
    setWindowIcon(windowIcon);

    recordStackWidgetCount = QSharedPointer<int>(new int);

    setStackWidgetCount() = 0;

//    move(0, 0);

    tabWidget = new TabWidget(this);

    // 为主界面窗口设置垂直布局
    vLayout = new QVBoxLayout;

    // 自定义标题框
    // titleBar = new TitleBar(this);           // 不同的情况   无法展开标题栏
   titleBar = new TitleBar();              // 底部会有一个黑方框
    // 安装过滤器
    //    installEventFilter(titleBar);
    
    // 设置底色 
    QWidget *widget = new QWidget();



    // 添加第一个 widget
    vLayout->addWidget(titleBar);

    line = new QFrame(this);

    line->setFrameShape(QFrame::HLine);//水平分割线
    line->setFrameShadow(QFrame::Raised);//设置线的阴影效果
//    line->move(0,titleBar->height() + 1);
    // line->move(0,titleBar->height() - 1);
    line->resize(this->width(),1);  // 不能实时动态更新，窗口放大时

    vLayout->addWidget(line);

    // 设置间隙
    vLayout->setSpacing(0);  // 与下一部件的间距

//    vLayout->setContentsMargins(5, 0, 5, 5);  // left top right bottom 边缘间距像素
    // vLayout->setContentsMargins(50, 50, 50, 50);  // left top right bottom 边缘间距像素
    setMargin(0, 0, 0, 0);

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

    taskBarView->addStretch(2);

    taskBarView->addWidget(settingLabel);


    filePage = new FilePage;

    searchPage = new SearchPage;

    StackWidget *stackWidget= new StackWidget(this);

//    QStackWidget *stackWidget= new QStackWidget(this);

//    stackWidget->addWidget(filePage->getStartWidget());
    stackWidget->addWidget(filePage);                // 第一个是 filePage
//    stackWidget->addWidget(sw);


    stackWidget->addWidget(filePage->getView());   // 如果成功打开文件夹，则显示

    stackWidget->addWidget(searchPage);

    stackWidget->setCurrentIndex(0);


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

    QWidget *tem = new QWidget(this);

    QVBoxLayout *vt = new QVBoxLayout;

    vt->addWidget(tabWidget);

    tem->setLayout(vt);

    QSplitter *splitter = new QSplitter(Qt::Horizontal);

    QWidget *tem2 = new QWidget(this);
    QHBoxLayout *vt2 = new QHBoxLayout;
    QLabel *label = new QLabel("terminal", this);
    vt2->addWidget(label);
    tem2->setLayout(vt2);

//    vt->addWidget(tem2);


    QSplitter *splitter2 = new QSplitter(Qt::Vertical);
    splitter2->addWidget(tem);
    splitter2->addWidget(tem2);
    splitter2->setStyleSheet("QSplitter::handle {background-color: rgb(13, 13, 13);}");
    splitter2->setHandleWidth(1);  // 设置 0 的话，样式表颜色失效
    splitter2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    splitter2->setSizes(QList<int>() << 500 << height() - 500);


    splitter->addWidget(stackWidget);           // 只有第一个界面与 tabWidget 的边界生效了样式表
    splitter->addWidget(splitter2);



//    tabWidget->setStyleSheet("background-color:red");
//    tabWidget->setStyleSheet("TabWidget { border: 20px solid red; }");
//    splitter->addWidget(tabWidget);         // 无用
//    splitter->addWidget(tem);


    splitter->setStyleSheet("QSplitter::handle {background-color: rgb(13, 13, 13);}");
    splitter->setHandleWidth(1);  // 设置 0 的话，样式表颜色失效
    splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    splitter->setSizes(QList<int>() << 200 << width() - 200 - 30);

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
//            if (filePage->getStartWidget()->isHidden()) {
            if (filePage->isHidden()) {
//                filePage->getStartWidget()->show();
                filePage->show();
                if (stackWidget->isHidden()) {
                    stackWidget->show();
                }
                stackWidget->setCurrentIndex(0);
            } else {
//                filePage->getStartWidget()->hide();
                filePage->hide();
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

    settingsMenu = new QMenu(this);

    QAction *commandPalette = new QAction("Command Palette", this);

    QAction *profiles = new QAction("Profiles", this);
    QAction *settings = new QAction("Settings", this);
    QAction *keyboardShortcuts = new QAction("Keyboard Shortcuts", this);
    QAction *themes = new QAction("Themes", this);

    QAction *checkForUpdates = new QAction("Check for Updates", this);

    settingsMenu->addAction(commandPalette);

    settingsMenu->addSeparator();

    settingsMenu->addAction(profiles);
    settingsMenu->addAction(settings);
    settingsMenu->addAction(keyboardShortcuts);
    settingsMenu->addAction(themes);

    settingsMenu->addSeparator();

    settingsMenu->addAction(checkForUpdates);


    connect(settingLabel, &SelfLabel::clicked, this, [=]() {
        settingsMenu->exec(QCursor::pos());
    });


    settingsPage = new SettingsPage(this);

    connect(settings, &QAction::triggered, tabWidget, [=]() {
        tabWidget->addTab(settingsPage, "Settings");
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
//    connect(titleBar, &TitleBar::openFileTriggered, this, &QCode::openFileTriggered);
    connect(titleBar, &TitleBar::openFileTriggered, this, [=]() {
        openFileTriggered();        // 方便复用函数
    });
//    connect(titleBar, &TitleBar::openFolderTriggered, this, &QCode::openFolderTriggered);
    connect(titleBar, &TitleBar::openFolderTriggered, this, [=]() {
        openFolderTriggered();
    });
    connect(titleBar, &TitleBar::saveTriggered, this, &QCode::saveTriggered);
    connect(titleBar, &TitleBar::saveAsTriggered, this, &QCode::saveAsTriggered);
    connect(titleBar, &TitleBar::undoTriggered, this, &QCode::undoTriggered);
    connect(titleBar, &TitleBar::redoTriggered, this, &QCode::redoTriggered);
    connect(titleBar, &TitleBar::cutTriggered, this, &QCode::cutTriggered);
    connect(titleBar, &TitleBar::copyTriggered, this, &QCode::copyTriggered);
    connect(titleBar, &TitleBar::pasteTriggered, this, &QCode::pasteTriggered);
    connect(titleBar, &TitleBar::aboutTriggered, this, &QCode::aboutTriggered);

    connect(filePage, &FilePage::selectFileIndexChanged, this, &QCode::openFileTriggered);
    connect(filePage, &FilePage::openFileFromDrag, this, &QCode::openFileTriggered);

    connect(tabWidget, &QTabWidget::tabCloseRequested, this, &QCode::tabWidgetTabCloseRequested);
    connect(tabWidget, &TabWidget::openFileTriggered, this, &QCode::openFileTriggered);
    connect(tabWidget, &TabWidget::openFileFolderTriggered, this, &QCode::openFolderTriggered);

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

void QCode::resizeEvent(QResizeEvent *event)
{
    // 窗口改变时其事件响应
    line->resize(width(), 1);
}


void QCode::setMargin(int left, int top, int right, int bottom) {
    vLayout->setContentsMargins(left, top, right, bottom);  // left top right bottom 边缘间距像素
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

void QCode::openFileTriggered(const QString& file)
{
    filePath = file;
    if (filePath == "null") {
        QMessageBox::warning(this, "警告", "无法打开此文件 ");
        return;
    }
    if (filePath.isEmpty()) {
        filePath = QFileDialog::getOpenFileName(this, "打开文件");
    }

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

void QCode::openFolderTriggered(const QString& folder)  // 不能重复打开其他文件夹
{
    // 需要覆盖之前已经打开的文件夹，同时打开的页面也得更换
//    if (!filePage->hasFolder()) {
//        filePage->sloveOpenFolder();
//    }
    filePage->sloveOpenFolder(folder);
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









