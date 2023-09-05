#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QKeySequence>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenuBar>
#include <QPushButton>
#include <QTimer>

class TitleBar : public QWidget
{
    Q_OBJECT

public:
    explicit TitleBar(QWidget *parent = nullptr);
    ~TitleBar();

    void initIcon();

    void initFile();

    void initEdit();

    void initSelection();

    void initView();

    void initHelp();

    void initBtn();

    void initMenu();

    void initWidget();

    void initAllAction();

    void initConnection();

    void initShortcutKey();
protected:

    // 双击标题栏进行界面的最大化/还原
    virtual void mouseDoubleClickEvent(QMouseEvent *event);

    // 进行界面的拖动
    virtual void mousePressEvent(QMouseEvent *event);

    // 设置界面标题与图标
    virtual bool eventFilter(QObject *obj, QEvent *event);


private slots:

    // 进行最小化、最大化/还原、关闭操作
    void onClicked();

    //    void onMenuHovered(QAction *action);

    //    void showMenu();

signals:
    // 新建文本文件信号
    void newTextFileTriggered();
    // 新建文件信号
    void newFileTriggered();
    // 新建窗口信号
    void newWindowTriggered();
    // 打开文件信号
    void openFileTriggered();
    // 打开文件夹信号
    void openFolderTriggered();
    // 打开最近文件信号
    void openRecentTriggered();
    // 保存信号
    void saveTriggered();
    // 另存为信号
    void saveAsTriggered();
    // 关闭编辑器信号
    void closeEditorTriggered();
    // 关闭窗口信号
    void closeWindowTriggered();
    // 撤销信号
    void undoTriggered();
    // 重做信号
    void redoTriggered();
    // 剪切信号
    void cutTriggered();
    // 复制信号
    void copyTriggered();
    // 黏贴信号
    void pasteTriggered();
    // 查找信号
    void findTriggered();
    // 替换信号
    void replaceTriggered();
    // 全选信号
    void selectAllTriggered();
    // 命令面板信号
    void commandPaletteTriggered();
    // 欢迎界面信号
    void welcomeTriggered();
    // 展示全部指令信号
    void showAllCommandsTriggered();
    // 关于界面信号
    void aboutTriggered();

private:

    // 最大化/还原
    void updateMaximize();


private:

    QMenuBar *menu;             // 菜单栏

    // 菜单栏 的 QAction
    QAction *file;
    QAction *edit;
    QAction *selection;
    QAction *view;
    QAction *help;

    // m_file 下属的 QAction
    QAction *newTextFile;
    QAction *newFile;
    QAction *newWindow;
    QAction *openFile;
    QAction *openFolder;
    QAction *openRecent;

    QAction *save;
    QAction *saveAs;
    QAction *closeEditor;
    QAction *closeWindow;



    // m_edit 下属的 QAction
    QAction *undo;
    QAction *redo;
    QAction *cut;
    QAction *copy;
    QAction *paste;
    QAction *find;                // 内容查找
    QAction *replace;             // 查找并替换

    // m_selection 下属的 QAction
    QAction *selectAll;

    // m_view 下属的 QAction
    QAction *commandPalette;

    // m_help 下属的 QAction
    QAction *welcome;
    QAction *showAllCommands;
    QAction *about;


private:

    // 顶层级的 QMenu
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *selectionMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;

    // 次层级的 QMenu;
    QMenu *openRecentMenu;

private:

    // 最右侧功能按钮
    QLabel *iconLabel; //标题栏图标
    QLabel *titleLabel; //标题栏标题
    QPushButton *minimizeButton; //最小化按钮
    QPushButton *maximizeButton; //最大化/还原按钮
    QPushButton *closeButton; //关闭按钮

private:

    QHBoxLayout *layout;

};

#endif // TITLEBAR_H
