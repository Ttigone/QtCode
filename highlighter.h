#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QObject>
#include <QSyntaxHighlighter>
#include <QRegularExpression>

class Highlighter : public QSyntaxHighlighter
{
public:
    explicit Highlighter(QTextDocument *parent = nullptr, QString font_family = "Consolas", int font_size = 13);

protected:
    virtual void highlightBlock(const QString &text);

private:
    void addNormalTextRule();

    void addNumberRule();

    void addStringRule();

    void addCommentRule();

    void addMultiLineCommentRule(const QString &text);

    void addKeyWordRule();

    void addClassNameRule();

    void addFuntionRule();

private:
    struct HighlightRule  // 高亮规则
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    QVector<HighlightRule> highlightRules;

    QString fontFamily;
    int fontSize;

};


#endif // HIGHLIGHTER_H
