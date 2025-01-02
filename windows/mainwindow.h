#pragma once

#include <QMainWindow>
#include "settings.h"
#include "editor.h"
#include "parser.h"
#include "tokenizer.h"
#include "utils.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_cmdLineEdit_editingFinished();              // Process the command line input
    void on_btnClearCode_clicked();                     // Clear all states
    void onCmdLineEditTextChanged(const QString &text); // Process the command line input

private:
    Ui::MainWindow *ui;

    Editor *editor;                   // Code editor pointer
    Parser *parser;                   // Parser pointer
    EvaluationContext *editorContext; // Evaluation context for the editor
    EvaluationContext *directContext; // Evaluation context for the direct commands

    QString cmd; // Command line input

    bool isDebugMode = false; // Debug mode flag
    bool isInputing = false;  // Input flag

    // UI functions
    void setUIForDebugMode();  // Set the UI for debug mode
    void setUIExitDebugMode(); // Set the UI for normal mode
    void cleanAll();           // Clear all code and initialize all variables
    void refreshCodeDisplay(); // Refresh the code display in the editor

    // Basic functions
    void runCode();      // Run the code in the editor
    void debugRunCode(); // Debug the code in the editor
    void loadCode();     // Load the code from a file

    // Parser functions
    void parseCmdInput(string cmdInput); // Parse the command line input
    bool directCmd(string cmd);          // Process the direct command

    bool input(string var, bool isDirect); // Input a variable
};