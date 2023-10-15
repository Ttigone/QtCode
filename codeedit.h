#ifndef CODEEDIT_H
#define CODEEDIT_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QPainter>
#include <QTimer>
#include <QScrollBar>


class LineNumber;

class CodeEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit CodeEdit(QWidget *parent = nullptr);
    ~CodeEdit();

    void lineNumberAreaPaintEvent(QPaintEvent *event);

    void lineNumberAreaMousePressEvent(QMouseEvent *event);

    void lineNumberAreaWheelEvent(QWheelEvent *event);

    bool saveFile();

    bool saveAsFile();

    void setFileName(QString file_name);

    QString getFileName();

    bool checkSaved();
protected:
    void resizeEvent(QResizeEvent *event) override;


    void mouseMoveEvent(QMouseEvent *e) override {       // 鼠标移动事件
        QPlainTextEdit::mouseMoveEvent(e);
        const int margin = 3;
        if (e->buttons() & Qt::LeftButton) {
            if (e->position().y() < margin) {
                scrollStep = -5; // 向上滚动的像素数
                timer->start();
            } else if (e->position().y() > height() - margin) {
                scrollStep = 4; // 向下滚动的像素数                    // 设置 1 时是 一行行显示
                timer->start();
            } else {
                timer->stop();
            }
        } else {
            timer->stop();
        }
    }

    void mouseReleaseEvent(QMouseEvent *e) override {
        QPlainTextEdit::mouseReleaseEvent(e);
        timer->stop();
    }
private:
    void initFont();

    void initHighlighter();

    int getLineNumberAreaWidth();

    void initConnection();

private slots:
    void hitghlightCurrentLine();

    void updateLineNumberArea(const QRect &, int);

    void updateLineNumberAreaWidth();

    void updateSaveState();

    void onTimeout() {
        verticalScrollBar()->setValue(verticalScrollBar()->value() + scrollStep);  // qDeBug()
    }


private:
    LineNumber *lineNumberWidget;

    QString fileName;

    bool isSave = false;

    QTimer *timer;
    int scrollStep;
};




class LineNumber : public QWidget
{
    Q_OBJECT
public:
    explicit LineNumber(CodeEdit *editor = nullptr) : QWidget(editor) {
        codeEditor = editor;
    }

signals:

protected:
    void paintEvent(QPaintEvent *event) override {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

    void mousePressEvent(QMouseEvent *event) override {
        codeEditor->lineNumberAreaMousePressEvent(event);
    }

    void wheelEvent(QWheelEvent *event) override {
        codeEditor->lineNumberAreaWheelEvent(event);
    }
private:
    CodeEdit *codeEditor;

};


#endif // CODEEDIT_H
