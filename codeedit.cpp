#include "codeedit.h"
#include "highlighter.h"

#include <QScrollBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextBlock>

CodeEdit::CodeEdit(QWidget *parent)
    : QPlainTextEdit{parent}
{

    lineNumberWidget = new LineNumber(this);


//    m_line_number_widget->setAttribute(Qt::WA_StyledBackground);


    // 不换行
//    setLineWrapMode(QPlainTextEdit::NoWrap);


    initFont();

    initHighlighter();

    initConnection();

    hitghlightCurrentLine();

    updateLineNumberAreaWidth();

//    setLineWrapMode(QPlainTextEdit::NoWrap)
}

CodeEdit::~CodeEdit() noexcept
{
    if (lineNumberWidget) {
        delete lineNumberWidget;
        lineNumberWidget = nullptr;
    }
}

void CodeEdit::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberWidget);

    // 绘制序列框背景色                                             最后一个值是透明度
    painter.fillRect(event->rect(), QColor(13, 13, 13 , 200));

    QTextBlock block = firstVisibleBlock();

    int block_number = block.blockNumber();

    int cursor_top = blockBoundingGeometry(textCursor().block()).translated(contentOffset()).top();


    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();

    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        // 绘制数字
        if (cursor_top == top) {   // 位于当前行的绘制
//            painter.setPen(QColor(204, 204, 204, 240));   // 文字颜色
            painter.setPen(QColor(204, 204, 204));   // 文字颜色
            painter.drawText(0, top, getLineNumberAreaWidth() - 3, bottom - top, Qt::AlignCenter, QString::number(block_number + 1));
        } else {  // 非当前行的绘制
//            painter.setPen(QColor(100, 100, 100));
            painter.setPen(QColor(100, 100, 100));
            painter.drawText(0, top, getLineNumberAreaWidth() - 3, bottom - top, Qt::AlignRight, QString::number(block_number + 1));
        }

        block = block.next();

        top = bottom;

        bottom = top + (int) blockBoundingRect(block).height();
        ++block_number;
    }
}

void CodeEdit::lineNumberAreaMousePressEvent(QMouseEvent *event)
{
    QTextBlock block = document()->findBlockByLineNumber(event->position().y() / fontMetrics().height() + verticalScrollBar()->value());
    setTextCursor(QTextCursor(block));
}

void CodeEdit::lineNumberAreaWheelEvent(QWheelEvent *event)
{
    if (event->angleDelta().y() != 0) {   // 垂直滚动
        verticalScrollBar()->setValue(verticalScrollBar()->value() - event->angleDelta().y() / 2);
    } else {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - event->angleDelta().x() / 2);
    }
//    qDebug() << event->angleDelta().y();

}

bool CodeEdit::saveFile()
{
    QString file_name;
    if (fileName.isEmpty()) {
        file_name = QFileDialog::getSaveFileName(this, "保存文件");  // 调用对话框的时候有一些 bug, 保存不能预先自己定义
        fileName = file_name;
    } else {
        file_name = fileName;
    }
    QFile file(file_name);
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "警告", "无法保存文件: " + file.errorString());
        return false;
    }
    QTextStream out(&file);

    out << toPlainText();

    file.close();

    isSave = true;
    return true;
}

bool CodeEdit::saveAsFile()
{

    QString file_name = QFileDialog::getSaveFileName(this, "另存文件");
    QFile file(file_name);
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "警告", "无法保存文件: " + file.errorString());
        return false;
    }
    fileName = file_name;
    QTextStream out(&file);
    out << toPlainText();
    file.close();

    isSave = true;
    return true;
}

void CodeEdit::setFileName(QString file_name)
{
    fileName = file_name;
}

QString CodeEdit::getFileName()
{
    return fileName;
}

bool CodeEdit::checkSaved()
{
    return isSave;
}

void CodeEdit::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);
    lineNumberWidget->setGeometry(0, 0, getLineNumberAreaWidth(), contentsRect().height());
}

void CodeEdit::initFont()
{
    this->setFont(QFont("Consolas", 13));
}

void CodeEdit::initHighlighter()
{
    new Highlighter{this->document()};
}

int CodeEdit::getLineNumberAreaWidth()
{
    return 16 + QString::number(blockCount()).length() * fontMetrics().horizontalAdvance(QChar('0'));
}

void CodeEdit::initConnection()
{
    connect(this, &QPlainTextEdit::cursorPositionChanged, this, &CodeEdit::hitghlightCurrentLine);

    connect(this, &QPlainTextEdit::updateRequest, this, &CodeEdit::updateLineNumberArea);

    connect(this, &QPlainTextEdit::blockCountChanged, this, &CodeEdit::updateLineNumberAreaWidth);

    connect(this, &QPlainTextEdit::textChanged, this, &CodeEdit::updateSaveState);
}

void CodeEdit::hitghlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extra_selections;

    QTextEdit::ExtraSelection selection;
    selection.format.setBackground(QColor(0, 100, 100, 20));
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = this->textCursor();

    extra_selections.append(selection);
    this->setExtraSelections(extra_selections);


}

void CodeEdit::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy) {
        lineNumberWidget->scroll(0, dy);
    } else {
        lineNumberWidget->update(0, rect.y(), getLineNumberAreaWidth(), rect.height());
    }
}

void CodeEdit::updateLineNumberAreaWidth()
{
    setViewportMargins(getLineNumberAreaWidth(), 0, 0, 0);
}

void CodeEdit::updateSaveState()
{
    isSave = false;
}