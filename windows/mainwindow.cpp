#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setUIExitDebugMode();
    isDebugMode = false;

    // Initialize the editor and parser
    editor = new Editor();
    parser = new Parser();
    editorContext = new EvaluationContext();
    directContext = new EvaluationContext();

    // Connect the button to switch to debug mode with the slot function
    connect(ui->btnDebugMode, &QPushButton::clicked, this, &MainWindow::setUIForDebugMode);
    connect(ui->btnExitDebugMode, &QPushButton::clicked, this, &MainWindow::setUIExitDebugMode);

    // Connect the button to corresponding slot functions
    connect(ui->btnRunCode, &QPushButton::clicked, this, &MainWindow::runCode);
    connect(ui->btnLoadCode, &QPushButton::clicked, this, &MainWindow::loadCode);
    connect(ui->btnClearCode, &QPushButton::clicked, this, &MainWindow::cleanAll);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete editor;
    delete parser;
    delete editorContext;
    delete directContext;
}

/*
 * --------------------------------------------------------------------
 * Slots functions Below
 * --------------------------------------------------------------------
 */

void MainWindow::on_cmdLineEdit_editingFinished()
{
    QString cmd = ui->cmdLineEdit->text();
    ui->cmdLineEdit->setText("");

    parseCmdInput(cmd.toStdString());
}

void MainWindow::on_btnClearCode_clicked()
{
    cleanAll();
}

/*
 * --------------------------------------------------------------------
 * Class functions Below
 * --------------------------------------------------------------------
 */

// UI functions

void MainWindow::setUIForDebugMode()
{
    // UI operations
    ui->btnClearCode->setVisible(false);
    ui->btnLoadCode->setVisible(false);
    ui->btnDebugMode->setVisible(false);

    ui->btnExitDebugMode->setVisible(true);
    ui->btnDebugResume->setVisible(true);

    ui->labelSyntaxTree->setVisible(false);
    ui->treeDisplay->setVisible(false);

    ui->labelMonitor->setVisible(true);
    ui->monitorDisplay->setVisible(true);
    ui->labelBreakPoints->setVisible(true);
    ui->breakPointsDisplay->setVisible(true);

    // Variables operations
    isDebugMode = true;
}

void MainWindow::setUIExitDebugMode()
{
    // UI operations
    ui->btnClearCode->setVisible(true);
    ui->btnLoadCode->setVisible(true);
    ui->btnDebugMode->setVisible(true);

    ui->btnExitDebugMode->setVisible(false);
    ui->btnDebugResume->setVisible(false);

    ui->labelSyntaxTree->setVisible(true);
    ui->treeDisplay->setVisible(true);

    ui->labelMonitor->setVisible(false);
    ui->monitorDisplay->setVisible(false);
    ui->labelBreakPoints->setVisible(false);
    ui->breakPointsDisplay->setVisible(false);

    // Variables operations
    isDebugMode = false;
}

// Clear all code and initialize all variables
void MainWindow::cleanAll()
{
    ui->CodeDisplay->clear();
    ui->textBrowser->clear();
    ui->treeDisplay->clear();
    ui->monitorDisplay->clear();
    ui->breakPointsDisplay->clear();

    editor->clear();
    editorContext->clear();
    directContext->clear();
}

void MainWindow::refreshCodeDisplay()
{
    ui->CodeDisplay->clear();
    ui->CodeDisplay->append(editor->getAllStatements().c_str());
}

// Basic functions

void MainWindow::runCode()
{
    editorContext->clear();
    ui->treeDisplay->clear();
    ui->monitorDisplay->clear();

    // print Syntax Tree in the treeDisplay
    ui->treeDisplay->append(editor->getAllSyntaxTrees().c_str());

    // Run the code in the editor in order
    for (auto stmt : editor->code)
    {
        try
        {
            // The warning "transfer of control bypasses initialization" occurs because the initialization of 
            // cmpOp is bypassed if an exception is thrown during the evaluation of lhs or rhs. 
            // To fix this, you can move the declaration of cmpOp before the try block.
            string cmpOp;
            switch (stmt.second->type())
            {
            case PRINT:
                ui->textBrowser->append(QString::number(dynamic_cast<PrintStmt *>(stmt.second)->rootExp->eval(*editorContext)));
                break;
            case LET:
                dynamic_cast<LetStmt *>(stmt.second)->rootExp->eval(*editorContext);
                break;
            case INPUT:
                
                break;
            case GOTO:
                
                break;
            case IF:
                int l_val = dynamic_cast<IfStmt *>(stmt.second)->lhs->eval(*editorContext);
                int r_val = dynamic_cast<IfStmt *>(stmt.second)->rhs->eval(*editorContext);
                cmpOp = dynamic_cast<IfStmt *>(stmt.second)->cmpOp;
                int targetLineNum = dynamic_cast<IfStmt *>(stmt.second)->targetLineNum;
                Statement *targetStmt = NULL;
                if (cmpOp == "=")
                {
                    if (l_val == r_val)
                    {
                        // find the target line number
                        if (editor->code.find(targetLineNum) != editor->code.end())
                        {
                            targetStmt = editor->code[targetLineNum];
                        }
                        else
                        {
                            throw std::runtime_error("Invalid line number: " + to_string(targetLineNum));
                        }
                    }
                }
                else if (cmpOp == "<")
                {
                    if (l_val < r_val)
                    {
                        // find the target line number
                        if (editor->code.find(targetLineNum) != editor->code.end())
                        {
                            targetStmt = editor->code[targetLineNum];
                        }
                        else
                        {
                            throw std::runtime_error("Invalid line number: " + to_string(targetLineNum));
                        }
                    }
                }
                else if (cmpOp == ">")
                {
                    if (l_val > r_val)
                    {
                        // find the target line number
                        if (editor->code.find(targetLineNum) != editor->code.end())
                        {
                            targetStmt = editor->code[targetLineNum];
                        }
                        else
                        {
                            throw std::runtime_error("Invalid line number: " + to_string(targetLineNum));
                        }
                    }
                }
                break;
            case END:
                editorContext->clear();
                return;
            default:
                break;
            }
        }
        catch (exception &e)
        {
            QMessageBox::warning(this, "Error::", e.what());
            return;
        }
    }

    editorContext->clear();
}

void MainWindow::debugRunCode()
{
    editorContext->clear();
    editorContext->clear();
}

void MainWindow::loadCode()
{
    cleanAll();
    // 通过文件选择器选择一个文件
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Text Files (*.txt)"));
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, tr("Warning"), tr("Cannot read file %1:\n%2.").arg(fileName).arg(file.errorString()));
        return;
    }
    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        parseCmdInput(line.toStdString());
    }
}

// Helper function to trim leading and trailing spaces
string trim(const string &s)
{
    if (s.empty())
    {
        return s;
    }
    auto start = s.begin();
    while (start != s.end() && isspace(*start))
    {
        start++;
    }

    auto end = s.end();
    do
    {
        end--;
    } while (end >= start && isspace(*end));

    return string(start, end + 1);
}

// Parser functions
// Two kinds of commands:
// 1. RUN: run the code in the editor: e.g. RUN
// 2. Add a new statement to the editor: e.g. 10 PRINT "Hello, World!" (line number + statement)
void MainWindow::parseCmdInput(string cmdInput)
{
    // Elimitate the leading and trailing spaces
    cmdInput = trim(cmdInput);
    // Check if the command is a RUN command
    if (cmdInput == "RUN")
    {
        runCode();
    }
    else if (cmdInput == "LIST")
    {
        /*Do Nothing*/
    }
    else if (cmdInput == "CLEAR")
    {
        cleanAll();
    }
    else if (cmdInput == "QUIT")
    {
        cleanAll();
        this->close();
    }
    else if (cmdInput == "HELP")
    {
        // Append the help information to the result display
        ui->textBrowser->append(HELPINFO);
    }
    else if (cmdInput == "LOAD")
    {
        loadCode();
    }
    else
    {
        Tokenizer tokenizer;
        vector<string> tokens = tokenizer.tokenize(cmdInput);
        if (tokens.empty())
        {
            return;
        }
        // if token vector is only one element, then it is a line number,
        // clear the statement with this line number
        if (tokens.size() == 1)
        {
            if (parser->isValidLineNum(tokens[0]))
            {
                editor->removeStatement(stoi(tokens[0]));
                refreshCodeDisplay();
                return;
            }
        }
        // Try to parse the command as a new statement
        Statement *stmt;
        try
        {
            stmt = parser->parseStatement(tokens);
        }
        catch (exception &e) // If the command is not a valid statement, catch the exception and pop up a message box
        {
            QMessageBox::warning(this, "Error::", e.what());
            return;
        }
        if (stmt == NULL)
        {
            // QMessageBox::warning(this, "Error::", "NULL statement");
            return;
        }
        editor->addStatement(stmt->lineNum, stmt);
        refreshCodeDisplay();
    }
}
