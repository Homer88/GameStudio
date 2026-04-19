#include "codeeditor.h"
#include <QVBoxLayout>
#include <QPainter>
#include <QTextDocument>
#include <QScrollBar>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QKeyEvent>
#include <QApplication>
#include <QPalette>


// ============================================================================
// LineNumberArea Implementation
// ============================================================================

LineNumberArea::LineNumberArea(CodeEditor *editor)
    : QWidget(editor), m_codeEditor(editor)
{
}

QSize LineNumberArea::sizeHint() const
{
    return QSize(m_codeEditor->lineNumberAreaWidth(), 0);
}

void LineNumberArea::paintEvent(QPaintEvent *event)
{
    m_codeEditor->lineNumberAreaPaintEvent(event);
}


// ============================================================================
// SyntaxHighlighter Implementation
// ============================================================================

SyntaxHighlighter::SyntaxHighlighter(CodeEditor *editor)
    : QObject(editor), m_editor(editor)
{
    // Setup default colors
    m_keywordFormat.setForeground(QColor(0, 0, 170)); // Dark blue
    m_keywordFormat.setFontWeight(QFont::Bold);
    
    m_typeFormat.setForeground(QColor(0, 128, 128)); // Teal
    m_typeFormat.setFontWeight(QFont::Bold);
    
    m_stringFormat.setForeground(QColor(0, 150, 0)); // Green
    
    m_commentFormat.setForeground(QColor(128, 128, 128)); // Gray
    m_commentFormat.setFontItalic(true);
    
    m_numberFormat.setForeground(QColor(170, 0, 170)); // Purple
    
    setupCppRules();
    m_editor->installEventFilter(this);
}

void SyntaxHighlighter::setColors(const QColor &textColor, const QColor &highlightColor,
                                   const QColor &keywordColor, const QColor &stringColor,
                                   const QColor &commentColor, const QColor &typeColor)
{
    m_keywordFormat.setForeground(keywordColor);
    m_stringFormat.setForeground(stringColor);
    m_commentFormat.setForeground(commentColor);
    m_typeFormat.setForeground(typeColor);
    
    applyHighlighting();
}

void SyntaxHighlighter::setupCppRules()
{
    HighlightingRule rule;
    
    // Keywords
    QStringList keywordPatterns = {
        "\\bauto\\b", "\\bbreak\\b", "\\bcase\\b", "\\bcatch\\b", "\\bclass\\b",
        "\\bconst\\b", "\\bconstexpr\\b", "\\bcontinue\\b", "\\bdefault\\b", "\\bdo\\b",
        "\\belse\\b", "\\benum\\b", "\\bexplicit\\b", "\\bexport\\b", "\\bextern\\b",
        "\\bfalse\\b", "\\bfinal\\b", "\\bfor\\b", "\\bfriend\\b", "\\bgoto\\b",
        "\\bif\\b", "\\binline\\b", "\\bnamespace\\b", "\\bnew\\b", "\\bnoexcept\\b",
        "\\bnullptr\\b", "\\boperator\\b", "\\boverride\\b", "\\bprivate\\b", "\\bprotected\\b",
        "\\bpublic\\b", "\\bregister\\b", "\\breinterpret_cast\\b", "\\breturn\\b",
        "\\bsigned\\b", "\\bsizeof\\b", "\\bstatic\\b", "\\bstatic_assert\\b",
        "\\bstatic_cast\\b", "\\bstruct\\b", "\\bswitch\\b", "\\btemplate\\b",
        "\\bthis\\b", "\\bthrow\\b", "\\btrue\\b", "\\btry\\b", "\\btypedef\\b",
        "\\btypeid\\b", "\\btypename\\b", "\\bunion\\b", "\\bunsigned\\b", "\\busing\\b",
        "\\bvirtual\\b", "\\bvolatile\\b", "\\bwhile\\b"
    };
    
    for (const QString &pattern : keywordPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = m_keywordFormat;
        m_highlightingRules.append(rule);
    }
    
    // Types
    QStringList typePatterns = {
        "\\bbool\\b", "\\bchar\\b", "\\bdouble\\b", "\\bfloat\\b", "\\bint\\b",
        "\\blong\\b", "\\bshort\\b", "\\bsigned\\b", "\\bunsigned\\b", "\\bvoid\\b",
        "\\bwchar_t\\b", "\\bint8_t\\b", "\\bint16_t\\b", "\\bint32_t\\b", "\\bint64_t\\b",
        "\\buint8_t\\b", "\\buint16_t\\b", "\\buint32_t\\b", "\\buint64_t\\b",
        "\\bQString\\b", "\\bQList\\b", "\\bQVector\\b", "\\bQMap\\b", "\\bQHash\\b",
        "\\bQWidget\\b", "\\bQObject\\b", "\\bQStringList\\b"
    };
    
    for (const QString &pattern : typePatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = m_typeFormat;
        m_highlightingRules.append(rule);
    }
    
    // Numbers
    rule.pattern = QRegExp("\\b[0-9]+\\.?[0-9]*([eE][+-]?[0-9]+)?[fFlLuU]*\\b");
    rule.format = m_numberFormat;
    m_highlightingRules.append(rule);
    
    // Strings (double quotes)
    rule.pattern = QRegExp("\"[^\"\\\\]*(\\\\.[^\"\\\\]*)*\"");
    rule.format = m_stringFormat;
    m_highlightingRules.append(rule);
    
    // Strings (single quotes - characters)
    rule.pattern = QRegExp("'[^'\\\\]*(\\\\.[^'\\\\]*)*'");
    rule.format = m_stringFormat;
    m_highlightingRules.append(rule);
    
    // Single-line comments
    rule.pattern = QRegExp("//[^\n]*");
    rule.format = m_commentFormat;
    m_highlightingRules.append(rule);
    
    // Multi-line comments
    m_commentStartExpression = QRegExp("/\\*");
    m_commentEndExpression = QRegExp("\\*/");
}

void SyntaxHighlighter::rehighlight()
{
    applyHighlighting();
}

bool SyntaxHighlighter::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_editor && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Tab) {
            QTextCursor cursor = m_editor->textCursor();
            if (!cursor.hasSelection()) {
                cursor.insertText("    ");
                return true;
            }
        }
    }
    return QObject::eventFilter(watched, event);
}

void SyntaxHighlighter::applyHighlighting()
{
    // This is a simplified approach - in a real implementation,
    // you would use QSyntaxHighlighter properly
    // For now, we'll just trigger a repaint
    m_editor->viewport()->update();
}


// ============================================================================
// CodeEditor Implementation
// ============================================================================

CodeEditor::CodeEditor(QWidget *parent)
    : QPlainTextEdit(parent)
    , m_syntaxHighlighter(nullptr)
    , m_modified(false)
{
    m_lineNumberArea = new LineNumberArea(this);
    
    connect(this, &CodeEditor::blockCountChanged, this, &CodeEditor::updateLineNumberAreaWidth);
    connect(this, &CodeEditor::updateRequest, this, &CodeEditor::updateLineNumberArea);
    connect(this, &CodeEditor::modificationChanged, this, &CodeEditor::documentWasModified);
    connect(this, &CodeEditor::textChanged, this, [this]() {
        bool wasModified = m_modified;
        m_modified = true;
        if (!wasModified) {
            emit modificationChanged(true);
        }
    });
    
    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
    
    // Default font settings
    QFont font("Consolas", 12);
    font.setStyleHint(QFont::Monospace);
    setFont(font);
    
    // Create syntax highlighter
    m_syntaxHighlighter = new SyntaxHighlighter(this);
}

CodeEditor::~CodeEditor()
{
}

void CodeEditor::setSyntaxHighlighter(SyntaxHighlighter *highlighter)
{
    if (m_syntaxHighlighter) {
        delete m_syntaxHighlighter;
    }
    m_syntaxHighlighter = highlighter;
}

SyntaxHighlighter* CodeEditor::syntaxHighlighter() const
{
    return m_syntaxHighlighter;
}

void CodeEditor::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Error"), 
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName).arg(file.errorString()));
        return;
    }
    
    QTextStream in(&file);
    if (file.size() > 10 * 1024 * 1024) { // 10MB limit
        QMessageBox::warning(this, tr("Warning"),
                             tr("File too large to open."));
        file.close();
        return;
    }
    
    QString content = in.readAll();
    setPlainText(content);
    file.close();
    
    m_currentFilePath = fileName;
    m_modified = false;
    
    emit fileNameChanged(fileName);
    emit modificationChanged(false);
}

bool CodeEditor::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Error"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName).arg(file.errorString()));
        return false;
    }
    
    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << toPlainText();
    QApplication::restoreOverrideCursor();
    
    if (file.error() != QFile::NoError) {
        QMessageBox::warning(this, tr("Error"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName).arg(file.errorString()));
        return false;
    }
    
    m_currentFilePath = fileName;
    m_modified = false;
    
    emit fileNameChanged(fileName);
    emit modificationChanged(false);
    
    return true;
}

QString CodeEditor::currentFilePath() const
{
    return m_currentFilePath;
}

void CodeEditor::setCurrentFilePath(const QString &path)
{
    m_currentFilePath = path;
}

bool CodeEditor::isModified() const
{
    return m_modified;
}

void CodeEditor::updateEditorSettings(const QString &fontFamily, int fontSize,
                                       const QColor &textColor, const QColor &highlightColor)
{
    QFont font(fontFamily, fontSize);
    font.setStyleHint(QFont::Monospace);
    setFont(font);
    
    updateColors(textColor, highlightColor);
}

void CodeEditor::closeEvent(QCloseEvent *event)
{
    if (m_modified) {
        QMessageBox::StandardButton ret = QMessageBox::warning(
            this, tr("Unsaved Changes"),
            tr("The document has been modified.\nDo you want to save your changes?"),
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        
        if (ret == QMessageBox::Save) {
            QString savePath = m_currentFilePath;
            if (savePath.isEmpty()) {
                savePath = QFileDialog::getSaveFileName(this, tr("Save File"), "", 
                                                        tr("C++ Files (*.cpp *.h);;All Files (*)"));
            }
            if (!savePath.isEmpty()) {
                if (saveFile(savePath)) {
                    event->accept();
                } else {
                    event->ignore();
                }
            } else {
                event->ignore();
            }
        } else if (ret == QMessageBox::Cancel) {
            event->ignore();
        } else {
            event->accept();
        }
    } else {
        event->accept();
    }
}

void CodeEditor::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);
    
    QRect cr = contentsRect();
    m_lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::keyPressEvent(QKeyEvent *event)
{
    // Handle auto-indentation
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        QTextCursor cursor = textCursor();
        QString currentLine = cursor.block().text();
        
        // Calculate indentation
        int indent = 0;
        for (const QChar &ch : currentLine) {
            if (ch == ' ') {
                indent++;
            } else if (ch == '\t') {
                indent += 4;
            } else {
                break;
            }
        }
        
        // Check for opening brace
        if (currentLine.trimmed().endsWith('{')) {
            indent += 4;
        }
        
        QPlainTextEdit::keyPressEvent(event);
        
        // Apply indentation
        QString indentStr(indent, ' ');
        cursor.insertText(indentStr);
        return;
    }
    
    QPlainTextEdit::keyPressEvent(event);
}

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;
    
    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;
        QColor lineColor = QColor(255, 255, 225); // Light yellow
        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }
    
    setExtraSelections(extraSelections);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy) {
        m_lineNumberArea->scroll(0, dy);
    } else {
        m_lineNumberArea->update(0, rect.y(), m_lineNumberArea->width(), rect.height());
    }
    
    if (rect.contains(viewport()->rect())) {
        updateLineNumberAreaWidth(0);
    }
}

void CodeEditor::documentWasModified()
{
    m_modified = document()->isModified();
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(m_lineNumberArea);
    painter.fillRect(event->rect(), QColor(240, 240, 240));
    
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());
    
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(QColor(100, 100, 100));
            painter.drawText(0, top, m_lineNumberArea->width() - 5, fontMetrics().height(),
                             Qt::AlignRight, number);
        }
        
        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}

int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }
    
    int space = 10 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
    return space;
}

void CodeEditor::updateColors(const QColor &textColor, const QColor &highlightColor)
{
    QPalette pal = palette();
    pal.setColor(QPalette::Base, Qt::white);
    pal.setColor(QPalette::Text, textColor);
    setPalette(pal);
    
    // Update current line highlight color
    QList<QTextEdit::ExtraSelection> extraSelections;
    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;
        selection.format.setBackground(highlightColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }
    setExtraSelections(extraSelections);
}
