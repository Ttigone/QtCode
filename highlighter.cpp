#include "highlighter.h"

#include <QFile>

Highlighter::Highlighter(QTextDocument *parent, QString x_fontFamily, int x_fontSize) : QSyntaxHighlighter(parent)
{
    fontFamily = x_fontFamily;
    fontSize = x_fontSize;
    // 普通文本
    addNormalTextRule();
    // 数字
    addNumberRule();
    // 字符串
    addStringRule();
    // 单行注释
    addCommentRule();
    // 关键字
    addKeyWordRule();
    // 类名
    addClassNameRule();
    // 函数名
    addFuntionRule();
}

void Highlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightRule &rule, highlightRules) {
        QRegularExpression reg_exp(rule.pattern);
        int  index = reg_exp.match(text).capturedStart();
        while (index >= 0) {
            int length = reg_exp.match(text).capturedLength();
            setFormat(index, length, rule.format);
            index = reg_exp.match(text, index + length).capturedStart();
        }
    }

    // 单独实现多行注释
    addMultiLineCommentRule(text);
}

void Highlighter::addNormalTextRule()
{
    HighlightRule rule;
    rule.pattern = QRegularExpression("[a-z0-9A-Z]+");

    QTextCharFormat normal_text_format;

    normal_text_format.setFont(QFont("Consolas", 13));
    normal_text_format.setForeground(QColor(0, 100, 100));

    rule.format = normal_text_format;

    highlightRules.append(rule);

}

void Highlighter::addNumberRule()
{
    HighlightRule rule;
    rule.pattern = QRegularExpression("\\b\\d+|\\d+\\.\\d+\\b");

    QTextCharFormat numberFormat;

    numberFormat.setFont(QFont("Consolas", 13));
    numberFormat.setForeground(QColor(200, 0, 0));

    rule.format = numberFormat;

    highlightRules.append(rule);
}

void Highlighter::addStringRule()
{
    QTextCharFormat numberFormat;
    numberFormat.setFont(QFont("Consolas", 13));
    numberFormat.setForeground(QColor(0, 180, 180));

    HighlightRule rule;

    rule.format = numberFormat;

    // 单引号
    rule.pattern = QRegularExpression("'[^']*'");
    highlightRules.append(rule);

    // 双引号
    rule.pattern = QRegularExpression("\"[^\"]*\"");
    highlightRules.append(rule);

    // 反引号
    rule.pattern = QRegularExpression("`[^`]*`");
    highlightRules.append(rule);

}

void Highlighter::addCommentRule()
{
    QTextCharFormat commentFormat;
    commentFormat.setFont(QFont(fontFamily, fontSize));
    commentFormat.setForeground(Qt::darkGreen);
    commentFormat.setFontItalic(true);

    HighlightRule rule;

    rule.format = commentFormat;

    // 单行注释
    rule.pattern = QRegularExpression("\\/\\/.*$");
    highlightRules.append(rule);

}

void Highlighter::addMultiLineCommentRule(const QString &text)
{
    setCurrentBlockState(0);

    // /*
    QRegularExpression comment_start_expression("\\/\\*");
    // */
    QRegularExpression comment_end_expression("\\*\\/");

    QTextCharFormat multi_line_comment_format;
    multi_line_comment_format.setFont(QFont(fontFamily, fontSize));
    multi_line_comment_format.setForeground(Qt::darkGreen);
    multi_line_comment_format.setFontItalic(true);
    //    int start_index = comment_start_expression.match(text).capturedStart();  // 没有捕获到则返回 -1
    int start_index = 0;
    if (previousBlockState() != 1) {
        start_index = comment_start_expression.match(text).capturedStart();
    }
    while (start_index >= 0) {
        int end_index = comment_end_expression.match(text, start_index).capturedStart();
        //        int end_index = comment_end_expression.match(text ).capturedStart();
        int comment_length = 0;
        if (end_index == -1) { // 多行注释
            setCurrentBlockState(1);
            comment_length = text.length() - start_index;
        } else { // 单行注释
            comment_length = end_index - start_index + comment_end_expression.match(text).capturedLength();
        }

        setFormat(start_index,
                  comment_length,
                  multi_line_comment_format);
        start_index = comment_start_expression.match(text, comment_length + start_index).capturedStart();
    }
}

void Highlighter::addKeyWordRule()
{
    QFile file(":/config/keywords.txt");
    QTextStream keywords_stream(&file);

    HighlightRule rule;
    QTextCharFormat keyword_format;
    keyword_format.setFont(QFont(fontFamily, fontSize));
    keyword_format.setForeground(Qt::darkMagenta);
    rule.format = keyword_format;

    if (file.open(QIODevice::ReadOnly)) {
        keywords_stream.seek(0);
        QString line;
        while (!keywords_stream.atEnd()) {
            line = keywords_stream.readLine();
            rule.pattern = QRegularExpression("\\b" + line + "\\b");
            highlightRules.append(rule);
        }
        file.close();
    }
}

void Highlighter::addClassNameRule()
{
    HighlightRule rule;

    QTextCharFormat classNameFormat;
    classNameFormat.setFont(QFont(fontFamily, fontSize));
    classNameFormat.setForeground(QColor(150, 20, 100));
    classNameFormat.setFontWeight(99);


    rule.format = classNameFormat;

    rule.pattern = QRegularExpression("\\b[A-Z]+\\w*");
    highlightRules.append(rule);
}

void Highlighter::addFuntionRule()
{
    HighlightRule rule;

    QTextCharFormat funtion_format;
    funtion_format.setFont(QFont(fontFamily, fontSize));
    funtion_format.setForeground(Qt::darkYellow);


    rule.format = funtion_format;

    rule.pattern = QRegularExpression("\\w+\\(");
    highlightRules.append(rule);

    rule.pattern = QRegularExpression("\\)");
    highlightRules.append(rule);
}
