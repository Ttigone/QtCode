#ifndef QCODE_H
#define QCODE_H

#include <QFrame>
#include <QWidget>

class QVBoxLayout;
class TitleBar;
class FilePage;
class SearchPage;
class SettingsPage;
class TabWidget;
class StackWidget;

QT_BEGIN_NAMESPACE
namespace Ui { class QCode; }
QT_END_NAMESPACE

class QCode : public QWidget
{
    Q_OBJECT

public:
    QCode(QWidget *parent = nullptr);
    ~QCode();
//    bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result);

    // 初始化界面
    void initWidget();
    // 设置信号槽连接
    void initConnection();
    // 存储历史文件
    void saveHistory(QString path);
    // 获取历史文件
    QList<QString> getHistory();
    // 打开最近文件
    void openRecentFile();
    // 清除历史记录
    void clearHistoryRecord();
    // 初始化最近文件菜单
    void initRecentMenu();
    // 获取当前打开的文件数
    int getCurrentTableCount();

public:
    void resizeEvent(QResizeEvent *event) override;

public:
    void setMargin(int left, int top, int right, int bottom);
    
    int& setStackWidgetCount();
    int getStackWidgetCount();

private slots:
    void newTextFileTriggered();   // 处理 新建文本文件
    void newFileTriggered();        // 处理 新建文件
    void newWindowTriggered();      // 处理 打开新窗口
    void openFileTriggered(const QString& file = "");       // 处理 打开文件
    void openFolderTriggered(const QString& folder = "");     // 处理 打开文件夹
    void saveTriggered();            // 处理 保存文件
    void saveAsTriggered();         // 处理 另存为文件
    void closeEditorTriggered();    // 处理 关闭编辑
    void closeWindowTriggered();    // 处理 关闭窗口
    void undoTriggered();            // 处理 撤销
    void redoTriggered();            // 处理 返回
    void cutTriggered();             // 处理 剪切
    void copyTriggered();            // 处理 复制
    void pasteTriggered();           // 处理 黏贴
    void findTriggered();            // 处理 寻找
    void replaceTriggered();         // 处理 替换
    void selectAllTriggered();      // 处理 全选择
    void commandPaletteTriggered(); // 处理 打开命令
    void WelcomeTriggered();         // 处理 欢迎界面
    void showAllCommandsTriggered();  // 处理 展示全部指令
    void aboutTriggered();           // 处理 关于界面
    void tabWidgetTabCloseRequested(int index);  // 处理 关闭标签页

signals:

private:

    Ui::QCode *ui;
    TitleBar *titleBar;                              // 标题栏
    QFrame *line;
    FilePage *filePage;                              // 文件页面
    SearchPage *searchPage;                          // 搜索页面
    TabWidget *tabWidget;                           // 容纳编辑页面
    SettingsPage *settingsPage;                      // 设置界面
    QMenu *settingsMenu;                             // 设置菜单
    int borderWidth;                                 // 边框像素
    QString filePath;                                // 文件路径
    QSharedPointer<int> recordStackWidgetCount;      // 记录当前栈窗口储存大小
    QHash<QString, int> storeCurrentOpenFilePath;    // 存储当前页面打开的文件路径

    QVBoxLayout *vLayout;          // 为主界面窗口设置垂直布局
};
#endif // QCODE_H


