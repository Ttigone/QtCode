#ifndef CODEEDIT_H
#define CODEEDIT_H

#include <QWidget>
#include <QApplication>
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
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override {
        QPlainTextEdit::mouseReleaseEvent(event);
        timer->stop();
    }

//    void wheelEvent(QWheelEvent *event) override {            // BUG painter 绘制出现问题
//        if (QApplication::keyboardModifiers() == Qt::ControlModifier) {
//            if (event->angleDelta().y() > 0) {
//                zoomIn();
//            } else {
//                zoomOut();
//            }
//        } else {
//            QPlainTextEdit::wheelEvent(event);
//        }
//    }

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
