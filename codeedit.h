#ifndef CODEEDIT_H
#define CODEEDIT_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QPainter>

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


private:
    LineNumber *lineNumberWidget;

    QString fileName;

    bool isSave = false;
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
