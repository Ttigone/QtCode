#include <QEvent>
#include <QMouseEvent>
#include <QApplication>
#include <QPixmap>
#include <qmessagebox.h>

#include "titlebar.h"

//调用WIN API需要用到的头文件与库
#ifdef Q_OS_WIN
#pragma comment(lib, "user32.lib")
#include <qt_windows.h>
#endif



TitleBar::TitleBar(QWidget *parent)
    : QWidget(parent)
{
    setFixedHeight(23);  // 高度设定固定大小

    setContentsMargins(0, 0 ,0, 0);
//    setAttribute(Qt::WA_StyledBackground);

//    setStyleSheet("background-color: rgba(13, 13, 13, 50);");

    menuBar = new QAction(("Menu Bar"), this);
    commandCenter = new QAction(("Command Center"), this);
    layoutControls = new QAction(("Layout Controls"), this);

    menuBar->setCheckable(true);
    commandCenter->setCheckable(true);
    layoutControls->setCheckable(true);

    setStyleSheet("background-color:grey");

//    menuBar->setIcon(QIcon(":/images/yes.ico"));
//    commandCenter->setIcon(QIcon(":/images/yes.ico"));
//    layoutControls->setIcon(QIcon(":/images/yes.ico"));





    // 设置菜单栏左侧
    initAllAction();

    // 设置整个标题栏窗口
    initWidget();

    // 设置信号槽连接
    initConnection();

    // 设置快捷键
    initShortcutKey();

}

TitleBar::~TitleBar()
{

}

void TitleBar::initIcon()
{
    iconLabel = new QLabel();    // 有 bug
    //初始化图标Label
    iconLabel->setFixedSize(23, 23);
    iconLabel->setScaledContents(true);
    QPixmap pixmap(":/images/qc.png");
    iconLabel->setPixmap(pixmap);

}

void TitleBar::initFile()
{

//    fileMenu->setFixedSize(200, 200);
    // 创建专属菜单
    fileMenu = new QMenu("File", this);
    fileMenu->setStyleSheet(
                            "QMenu::item { background-color: rgba(13, 13, 13, 128); }"
//                            "QMenu { border: 0px; margin: 0px; padding: 10px; }"
                            "QMenu::separator { background-color:rgba(13, 13, 13, 50); }"
//                            "QMenu::icon { width: 0px; }"  // 覆盖了父亲的样式表设置  没有用
                            "QMenu::item { padding-left: 10px; }" // 单独设置有效  // 自动把 icon 给去掉了
                            "QMenu { border: 0px; margin: 0px; }"
//                            "QMenu { margin: 0px; }"
//                            "QMenu { border: 0px; }"
//                            "QMenu::item:hover { background-color: red; }"
//                            "QMenu::item:hover:checked {color: red; }"
                            "QMenu::item:selected { background-color: rgba(13, 13, 13, 50); }"    // 选择相应项，底色改变
    );  // 设置样式表只能出现一次  // 能够去掉边界   // 先后顺序不影响


    // 新建 第一部分 QAction
    newTextFile = new QAction("New Text File", this);
    newFile = new QAction("New File...", this);
    newWindow = new QAction("New Window", this);
    // 添加 第一部分 QAction
    fileMenu->addAction(newTextFile);
    fileMenu->addAction(newFile);
    fileMenu->addAction(newWindow);

    // 添加分割符
    fileMenu->addSeparator();


    // 新建 第二部分 QAction
    openFile = new QAction("Open File...", this);
    openFolder = new QAction("Open folder...", this);
    openRecent = new QAction("Open Recent", this);   // 这个还得再设置一个 QMenu
    // 添加 第二部分功能 QAction
    fileMenu->addAction(openFile);
    fileMenu->addAction(openFolder);

    // 次层级
    openRecentMenu = new QMenu(this);

    //    m_clear_all_recent = new QAction("clear history", this);
    //    m_clear_all_recent->setObjectName("clear_action");

    //    QAction *action = new QAction(this);


    openRecentMenu->setObjectName("recent");  // 设置对象名， 让父亲通过 findchild 寻找到


    //    m_open_recent_menu->addAction(m_clear_all_recent);
    openRecent->setMenu(openRecentMenu);

    fileMenu->addAction(openRecent);

    // 添加分隔符
    fileMenu->addSeparator();

    // 新建 第三部分 QAction
    save = new QAction("Save", this);
    saveAs = new QAction("Save As...", this);
    // 添加 第三部分 QAction
    fileMenu->addAction(save);
    fileMenu->addAction(saveAs);

    // 添加分隔符
    fileMenu->addSeparator();

    // 新建 第四部分 QAction
    closeEditor = new QAction("Close Editor", this);
    closeWindow = new QAction("Close Window", this);
    // 添加 第四部分功能 QAction
    fileMenu->addAction(closeEditor);
    fileMenu->addAction(closeWindow);
}

void TitleBar::initEdit()
{
    editMenu = new QMenu("Edit", this);
    editMenu->setStyleSheet(
//            "QMenu:: icon { width: 0px; }"
            "QMenu::item { padding-left: 0px; }" // 单独设置有效
            "QMenu::item:hover { background-color: red; }"
            "QMenu::item:hover { "
            "    background-color: interlinear(x1:0, y1:0, x2:0, y2:1, stop:0 transparent, stop:0.85 transparent, stop:0.86 red , stop:1 yellow); "
            "}"
    );

    undo = new QAction("Uodo", this);
    redo = new QAction("Redo", this);
    editMenu->addAction(undo);
    editMenu->addAction(redo);
    editMenu->addSeparator();
    cut = new QAction("Cut", this);
    copy = new QAction("Copy", this);
    paste = new QAction("Paste", this);
    editMenu->addAction(cut);
    editMenu->addAction(copy);
    editMenu->addAction(paste);
    editMenu->addSeparator();
    find = new QAction("Find", this);
    replace = new QAction("Replace", this);
    editMenu->addAction(find);
    editMenu->addAction(replace);
}

void TitleBar::initSelection()
{
    selectionMenu = new QMenu("Selection", this);
//    selectionMenu->setStyleSheet("QMenu::icon { width: 0px; }");
    selectionMenu->setStyleSheet(
//            "QMenu::item { "
//            "    background-color: transparent; "
//            "    border: none; "
//            "    padding: 3px 10px; "
//            "} "
            "QMenu::item:hover { "
            "    background-color: interlinear(x1:0, y1:0, x2:0, y2:1, stop:0 transparent, stop:0.85 transparent, stop:0.86 red , stop:1 yellow); "
            "}"
    );
    selectAll = new QAction("Select All");
    selectionMenu->addAction(selectAll);
}

void TitleBar::initView()
{
    viewMenu = new QMenu("View", this);
//    viewMenu->setStyleSheet("background-color: rgba(13, 13, 13, 50);");
    commandPalette = new QAction("Command Palette...", this);
    viewMenu->addAction(commandPalette);
}

void TitleBar::initHelp()
{
    helpMenu = new QMenu("Help", this);
//    helpMenu->setStyleSheet("background-color: rgba(13, 13, 13, 50);");
//    helpMenu->setStyleSheet(
//            "QMenu::item { "
////            "    background-color: transparent; "  // 默认背景颜色  有
//            "    background-color: green; "  // 默认背景颜色  有
//            "    border: none; "
//            "    padding: 3px 10px; "
//            "} "
////            "QMenu::item:hover { "
////            "    background-color: red; "   // 鼠标悬停时的背景颜色
////            "}"
//            "QMenu::item:hover { "
//            "    background-color: interlinear(x1:0, y1:0, x2:0, y2:1, stop:0 black, stop:0.85 green, stop:0.86 red , stop:1 yellow); "
//            "}"
//    );
    welcome = new QAction("Welcome", this);
    showAllCommands = new QAction("Show All Commands", this);
    helpMenu->addAction(welcome);
    helpMenu->addAction(showAllCommands);
    helpMenu->addSeparator();
    about = new QAction("About", this);
    helpMenu->addAction(about);

}

void TitleBar::initBtn()
{
    minimizeButton = new QPushButton(this);
    maximizeButton = new QPushButton(this);
    closeButton = new QPushButton(this);

    //设置按钮的固定大小、图片、取消边框
    minimizeButton->setIconSize(QSize(30,22));
    minimizeButton->setIcon(QIcon(":/images/minimize.png"));
    minimizeButton->setFlat(true);
    minimizeButton->setAttribute(Qt::WA_Hover);
    minimizeButton->setStyleSheet(
        "QPushButton:hover {"
            "border: 1px #EA2;"  // 设置了这个才会在鼠标悬浮时显示颜色
            "background-color: rgba(13, 13, 13, 50); "
        "}"
        "QPushButton:pressed {"
            "background-color: rgba(13, 13, 13, 20); "
        "}"
    );

    maximizeButton->setIconSize(QSize(27,22));
    maximizeButton->setIcon(QIcon(":/images/maximize.png"));
    maximizeButton->setFlat(true);
    maximizeButton->setStyleSheet(
        "QPushButton:hover {"
            "border: 1px #EA2;"  // 设置了这个才会在鼠标悬浮时显示颜色
            "background-color: rgba(13, 13, 13, 50); "
        "}"
        "QPushButton:pressed {"
            "background-color: rgba(13, 13, 13, 20); "
        "}"
    );

    closeButton->setIconSize(QSize(27,22));
    closeButton->setIcon(QIcon(":/images/close.png"));
    closeButton->setFlat(true);
    closeButton->setStyleSheet(
            "QPushButton:hover {"
            "border: 1px #EA2;"  // 设置了这个才会在鼠标悬浮时显示颜色
            "background-color: rgba(13, 13, 13, 50); "
            "}"

            "QPushButton:pressed {"
            "background-color: rgba(13, 13, 13, 20); "
            "}"
    );

    //设置窗口部件的名称
    minimizeButton->setObjectName("minimizeButton");
    maximizeButton->setObjectName("maximizeButton");
    closeButton->setObjectName("closeButton");

    //给按钮设置静态tooltip，当鼠标移上去时显示tooltip
    minimizeButton->setToolTip("Minimize");
    maximizeButton->setToolTip("Maximize");
    closeButton->setToolTip("Close");


}

void TitleBar::initMenu()
{
    menu = new QMenuBar(this);   // 菜单栏
    // 为菜单栏添加 QMenu 控件
    menu->addMenu(fileMenu);
    menu->addMenu(editMenu);
    menu->addMenu(selectionMenu);
    menu->addMenu(viewMenu);
    menu->addMenu(helpMenu);

//    menu->setStyleSheet("QMenu::item { padding-left: 0px; }");
//    menu->setStyleSheet("QMenu { border: 0px; margin: 0px; padding: 0px; }");
    menu->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

//    menu->setStyleSheet("QMenu { background-color: #f0f0f0; margin: 0px; }"
//                        "QMenu::icon { width: 0px; }");
//    menu->setStyleSheet("QMenu::icon { width: 0px; }");
}

void TitleBar::initWidget()
{
    //标题栏布局
    layout = new QHBoxLayout(this);  // 水平布局
    layout->setSpacing(0);
    layout->addWidget(iconLabel);
    // 无效
//    layout->addStrut(300);
//    layout->setSpacing(100);             // 间隙
    // layout->addStretch();

//    layout->setSpacing(5);             // 间隙

   layout->addWidget(menu);         // 添加菜单栏

    layout->addStretch(1);             // 增加一个弹簧

    layout->addWidget(minimizeButton);
    layout->addWidget(maximizeButton);
    layout->addWidget(closeButton);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    this->setLayout(layout);
}

void TitleBar::initAllAction()
{
    // 设置图标
    initIcon();

    // 设置文件菜单
    initFile();

    // 设置编辑菜单
    initEdit();

    // 设置选择菜单
    initSelection();

    // 设置按钮
    initBtn();

    // 设置视图菜单
    initView();

    // 设置帮助菜单
    initHelp();


    // 设置菜单栏
    initMenu();
}

void TitleBar::initConnection()
{
    //连接三个按钮的信号槽
    connect(minimizeButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    connect(maximizeButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    connect(closeButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));


    connect(newTextFile, &QAction::triggered, this, &TitleBar::newTextFileTriggered);
    connect(newFile, &QAction::triggered, this, &TitleBar::newFileTriggered);
    connect(newWindow, &QAction::triggered, this, &TitleBar::newWindowTriggered);
    connect(openFile, &QAction::triggered, this, &TitleBar::openFileTriggered);
    connect(openFolder, &QAction::triggered, this, &TitleBar::openFolderTriggered);
    connect(openRecent, &QAction::triggered, this, &TitleBar::openRecentTriggered);
    connect(save, &QAction::triggered, this, &TitleBar::saveTriggered);
    connect(saveAs, &QAction::triggered, this, &TitleBar::saveAsTriggered);
    connect(closeEditor, &QAction::triggered, this, &TitleBar::closeEditorTriggered);
    connect(closeWindow, &QAction::triggered, this, &TitleBar::closeWindowTriggered);

    connect(undo, &QAction::triggered, this, &TitleBar::undoTriggered);
    connect(redo, &QAction::triggered, this, &TitleBar::redoTriggered);
    connect(cut, &QAction::triggered, this, &TitleBar::cutTriggered);
    connect(copy, &QAction::triggered, this, &TitleBar::copyTriggered);
    connect(paste, &QAction::triggered, this, &TitleBar::pasteTriggered);
    connect(find, &QAction::triggered, this, &TitleBar::findTriggered);
    connect(replace, &QAction::triggered, this, &TitleBar::replaceTriggered);

    connect(selectAll, &QAction::triggered, this, &TitleBar::selectAllTriggered);

    connect(commandPalette, &QAction::triggered, this, &TitleBar::commandPaletteTriggered);

    connect(welcome, &QAction::triggered, this, &TitleBar::welcomeTriggered);
    connect(showAllCommands, &QAction::triggered, this, &TitleBar::showAllCommandsTriggered);
    connect(about, &QAction::triggered, this, &TitleBar::aboutTriggered);

    connect(menuBar, &QAction::triggered, this, &TitleBar::contextMenuBtnClicked);
    connect(commandCenter, &QAction::triggered, this, &TitleBar::contextMenuBtnClicked);
    connect(layoutControls, &QAction::triggered, this, &TitleBar::contextMenuBtnClicked);


}

void TitleBar::initShortcutKey()
{
    // 绑定快捷键
    save->setShortcut(QKeySequence(tr("Ctrl+s")));
}


// BUG 在最大化时，点击最小化，再点击恢复按钮，再点击最大化按钮时出现无响应，显示 resotre, 点击 2 次恢复

//双击标题栏进行界面的最大化/还原
void TitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event); //没有实质性的作用，只是用来允许event可以不使用，用来避免编译器警告

    emit maximizeButton->clicked();
//    update();
}

//进行界面的拖动  [一般情况下，是界面随着标题栏的移动而移动，所以我们将事件写在标题栏中比较合理]
void TitleBar::mousePressEvent(QMouseEvent *event)
{
//    QAbstractButton::mousePressEvent(event);
//#ifdef Q_OS_WIN
//    if (ReleaseCapture())
//    {
//        QWidget *pWindow = this->window();
//        if (pWindow->isWindow())
//        {
//            SendMessage(HWND(pWindow->winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
//        }
//    }
//    event->ignore();
//#else
//#endif

//    windowPoint = this->pos().toPointF();  // 窗口坐标
    windowPoint = qobject_cast<QWidget *>(parent())->pos().toPointF();  // 窗口坐标
    mousePoint = event->globalPosition();       // 鼠标坐标
    distanceForWindowAndMouse = mousePoint - windowPoint;             // 鼠标点击处相对于窗口左上角的坐标
//    qDebug() << distanceForWindowAndMouse.x() << distanceForWindowAndMouse.y();
    QWidget::mousePressEvent(event);
}

void TitleBar::mouseMoveEvent(QMouseEvent *event)
{
//    this->move((event->globalPosition() - distanceForWindowAndMouse).toPoint());
    qobject_cast<QWidget *>(parent())->move((event->globalPosition() - distanceForWindowAndMouse).toPoint());
//    qDebug() << event->globalPosition().x() << event->globalPosition().y();
}

void TitleBar::contextMenuEvent(QContextMenuEvent *event) {
    QWidget::contextMenuEvent(event);

    QMenu tem;
    tem.addAction(menuBar);
    tem.addAction(commandCenter);
    tem.addAction(layoutControls);

    tem.exec(QCursor::pos());

}


//使用事件过滤器监听标题栏所在的窗体，所以当窗体标题、图标等信息发生改变时，标题栏也应该随之改变
bool TitleBar::eventFilter(QObject *obj, QEvent *event)
{
    switch ( event->type() ) //判断发生事件的类型
    {
    case QEvent::WindowTitleChange: //窗口标题改变事件
    {
        QWidget *pWidget = qobject_cast<QWidget *>(obj); //获得发生事件的窗口对象
        if (pWidget)
        {
            //窗体标题改变，则标题栏标题也随之改变
            titleLabel->setText(pWidget->windowTitle());
            return true;
        }
    }
    break;

    case QEvent::WindowIconChange: //窗口图标改变事件
    {
        QWidget *pWidget = qobject_cast<QWidget *>(obj);
        if (pWidget)
        {
            //窗体图标改变，则标题栏图标也随之改变
            QIcon icon = pWidget->windowIcon();
            iconLabel->setPixmap(icon.pixmap(iconLabel->size()));
            return true;
        }
    }
    break;

    case QEvent::Resize:
        updateMaximize(); //最大化/还原
        return true;

    default:
        return QWidget::eventFilter(obj, event);
    }

    return QWidget::eventFilter(obj, event);
}

//进行最小化、最大化/还原、关闭操作
void TitleBar::onClicked()
{
//    update();
    //QObject::Sender()返回发送信号的对象的指针，返回类型为QObject *
    QPushButton *pButton = qobject_cast<QPushButton *>(sender());

    QWidget *pWindow = this->window(); //获得标题栏所在的窗口

    if (pWindow->isWindow())
    {
        //判断发送信号的对象使哪个按钮
        if (pButton == minimizeButton)
        {
            pWindow->showMinimized(); //窗口最小化显示
        }
        else if (pButton == maximizeButton)
        {
            pWindow->isMaximized() ? pWindow->showNormal() : pWindow->showMaximized();  //窗口最大化/还原显示
        }
        else if (pButton == closeButton)
        {
            pWindow->close(); //窗口关闭
        }
    }
}

void TitleBar::contextMenuBtnClicked() {
    if ((QAction *)sender() == menuBar) {

    } else if ((QAction *)sender() == commandCenter) {

    } else if ((QAction *)sender() == layoutControls) {

    }
}

//最大化/还原
void TitleBar::updateMaximize()
{
    QWidget *pWindow = this->window(); //获得标题栏所在的窗口

    if (pWindow->isWindow())
    {
        bool bMaximize = pWindow->isMaximized(); //判断窗口是不是最大化状态，是则返回true，否则返回false
        if (bMaximize)
        {
            //目前窗口是最大化状态，则最大化/还原的toolTip设置为"Restore"
            maximizeButton->setToolTip(tr("Restore"));
            //设置按钮的属性名为"maximizeProperty"
            maximizeButton->setProperty("maximizeProperty", "restore");
        }
        else
        {
            //目前窗口是还原状态，则最大化/还原的toolTip设置为"Maximize"
            maximizeButton->setToolTip(tr("Maximize"));
            //设置按钮的属性名为"maximizeProperty"
            maximizeButton->setProperty("maximizeProperty", "maximize");
        }

        maximizeButton->setStyle(QApplication::style());
    }
}
