#include "qcode.h"
#include "./ui_qcode.h"


#include "titlebar.h"
#include <QVBoxLayout>


//调用WIN API需要用到的头文件与库 [实现缩放]
#ifdef Q_OS_WIN
#include <qt_windows.h>
#include <Windowsx.h>
#include <qlist.h>
#endif






QCode::QCode(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QCode)
{
    ui->setupUi(this);


//    setWindowFlag(Qt::FramelessWindowHint);



    QVBoxLayout *vLayout = new QVBoxLayout(this);


    // 自定义标题框
    TitleBar *titleBar = new TitleBar(this);

    // 添加第一个 widget
    vLayout->addWidget(titleBar);

    // 设置间隙
    vLayout->setSpacing(0);  // 与下一部件的间距

    vLayout->setContentsMargins(5, 0, 5, 5);  // left top right bottom 边缘间距像素

    // 添加第二个 widget
    vLayout->addWidget(ui->tabWidget);

    // 应用界面布局
    setLayout(vLayout);


//    borderWidth = 5;

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

