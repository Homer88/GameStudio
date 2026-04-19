#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QWidget>
#include <QVector>
#include <QTimer>
#include <QTextBlock>
#include <QTextStream>

class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
class QScrollBar;

class LineNumberArea;
class SyntaxHighlighter;

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit CodeEditor(QWidget *parent = nullptr);
    ~CodeEditor();

    void setSyntaxHighlighter(SyntaxHighlighter *highlighter);
    SyntaxHighlighter* syntaxHighlighter() const;

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
    SyntaxHighlighter *m_syntaxHighlighter;
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


class SyntaxHighlighter : public QObject
{
    Q_OBJECT

public:
    explicit SyntaxHighlighter(CodeEditor *editor);
    
    void setColors(const QColor &textColor, const QColor &highlightColor,
                   const QColor &keywordColor, const QColor &stringColor,
                   const QColor &commentColor, const QColor &typeColor);
    
    void rehighlight();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    struct HighlightingRule {
        QRegExp pattern;
        QTextCharFormat format;
    };
    
    QVector<HighlightingRule> m_highlightingRules;
    QRegExp m_commentStartExpression;
    QRegExp m_commentEndExpression;
    
    QTextCharFormat m_keywordFormat;
    QTextCharFormat m_typeFormat;
    QTextCharFormat m_stringFormat;
    QTextCharFormat m_commentFormat;
    QTextCharFormat m_numberFormat;
    
    CodeEditor *m_editor;
    
    void setupCppRules();
    void applyHighlighting();
};

#endif // CODEEDITOR_H
