#pragma once

#include <QMainWindow>
#include "settings.h"
#include "editor.h"
#include "parser.h"
#include "tokenizer.h"

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
    void on_cmdLineEdit_editingFinished(); // Process the command line input
    void on_btnClearCode_clicked();        // Clear all states

private:
    Ui::MainWindow *ui;

    Editor *editor; // Code editor pointer
    Parser *parser; // Parser pointer

    bool isDebugMode = false; // Debug mode flag

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
};