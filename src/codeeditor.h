#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QWidget>
#include <QVector>
#include <QTimer>
#include <QTextBlock>
#include <QTextStream>
#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QTextCharFormat>

class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
class QScrollBar;

class LineNumberArea;
class CodeEditorHighlighter;

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit CodeEditor(QWidget *parent = nullptr);
    ~CodeEditor();

    void setSyntaxHighlighter(CodeEditorHighlighter *highlighter);
    CodeEditorHighlighter* syntaxHighlighter() const;

    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    QString currentFilePath() const;
    void setCurrentFilePath(const QString &path);
    bool isModified() const;

    void updateEditorSettings(const QString &fontFamily, int fontSize, 
                              const QColor &textColor, const QColor &highlightColor);
    
    // Public methods for LineNumberArea
    int lineNumberAreaWidth();
    void lineNumberAreaPaintEvent(QPaintEvent *event);

signals:
    void modificationChanged(bool changed);
    void fileNameChanged(const QString &fileName);

protected:
    void closeEvent(QCloseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rect, int dy);
    void documentWasModified();

private:
    void updateColors(const QColor &textColor, const QColor &highlightColor);

    LineNumberArea *m_lineNumberArea;
    CodeEditorHighlighter *m_syntaxHighlighter;
    QString m_currentFilePath;
    bool m_modified;
};


class LineNumberArea : public QWidget
{
public:
    explicit LineNumberArea(CodeEditor *editor);

    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    CodeEditor *m_codeEditor;
};


class CodeEditorHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    explicit CodeEditorHighlighter(QTextDocument *parent = nullptr);
    
    void setColors(const QColor &keywordColor, const QColor &typeColor,
                   const QColor &stringColor, const QColor &commentColor,
                   const QColor &numberColor);
    
protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    
    QVector<HighlightingRule> m_highlightingRules;
    QRegularExpression m_commentStartExpression;
    QRegularExpression m_commentEndExpression;
    
    QTextCharFormat m_keywordFormat;
    QTextCharFormat m_typeFormat;
    QTextCharFormat m_stringFormat;
    QTextCharFormat m_commentFormat;
    QTextCharFormat m_numberFormat;
    
    void setupCppRules();
};

#endif // CODEEDITOR_H
