#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setUIExitDebugMode();
    isDebugMode = false;
    isInputing = false;

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

    // Connect the command line input to the corresponding slot function
    connect(ui->cmdLineEdit, &QLineEdit::textChanged, this, &MainWindow::onCmdLineEditTextChanged);
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
    cmd = ui->cmdLineEdit->text();
    ui->cmdLineEdit->setText("");

    if (isInputing)
    {
        isInputing = false;
        return;
    }

    parseCmdInput(cmd.toStdString());
    cmd.clear();
}

void MainWindow::on_btnClearCode_clicked()
{
    cleanAll();
}

void MainWindow::onCmdLineEditTextChanged(const QString &text)
{
    if (!text.startsWith(" ? ") && isInputing)
    {
        QString newText = " ? " + text.mid(3);
        ui->cmdLineEdit->blockSignals(true); // 防止递归调用
        ui->cmdLineEdit->setText(newText);
        ui->cmdLineEdit->setCursorPosition(newText.length());
        ui->cmdLineEdit->blockSignals(false);
    }
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

bool MainWindow::input(string var, bool isDirect)
{
    try
    {
        isInputing = true;
        ui->cmdLineEdit->setText(" ? ");
        while (isInputing)
        { // 网上查阅了这个函数 会让其他线程执行一部分时间 可以使单线程不被循环阻塞 input操作时就不会卡死 当然用多线程应该也可以实现
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
        // remove the "?" at the beginning of the input
        string input = cmd.toStdString().substr(3);
        if (!parser->isConstant(input))
        {
            throw runtime_error("Invalid input: " + input);
        }
        if (isDirect)
        {
            directContext->setValue(var, stoi(input));
        }
        else{
            editorContext->setValue(var, stoi(input));
        }
        ui->textBrowser->append(cmd);
        ui->cmdLineEdit->setText("");
    }
    catch (exception &e)
    {
        isInputing = true;
        QMessageBox::warning(this, "Error::", e.what());
        return false;
    }
    return true;
}

// Basic functions
void MainWindow::runCode()
{
    editorContext->clear();
    ui->treeDisplay->clear();
    ui->monitorDisplay->clear();

    // print Syntax Tree in the treeDisplay
    ui->treeDisplay->append(editor->getAllSyntaxTrees().c_str());

    int currentLine = 0;
    auto it = editor->code.begin();

    while (it != editor->code.end())
    {
        if (it == editor->code.end())
        {
            throw runtime_error("Invalid line number");
        }

        currentLine = it->first;
        Statement *stmt = it->second;

        try
        {
            string cmpOp = "";
            int l_val = 0, r_val = 0, targetLineNum = 0;
            switch (stmt->type())
            {
            case PRINT:
                ui->textBrowser->append(QString::number(dynamic_cast<PrintStmt *>(stmt)->rootExp->eval(*editorContext)));
                break;
            case LET:
                dynamic_cast<LetStmt *>(stmt)->rootExp->eval(*editorContext);
                break;
            case INPUT:
                while (!input(dynamic_cast<InputStmt *>(stmt)->varName, false))
                {
                };
                // qDebug() << "input done";
                // // check the input var name and value
                // qDebug() << dynamic_cast<InputStmt *>(stmt)->varName.c_str() << " : " << directContext->getValue(dynamic_cast<InputStmt *>(stmt)->varName);
                // qDebug() << "editorContext: " << editorContext->getValue(dynamic_cast<InputStmt *>(stmt)->varName);
                break;
            case GOTO:
                currentLine = dynamic_cast<GotoStmt *>(stmt)->targetLineNum;
                it = editor->code.find(currentLine);
                continue;
            case IF:
                l_val = dynamic_cast<IfStmt *>(stmt)->lhs->eval(*editorContext);
                r_val = dynamic_cast<IfStmt *>(stmt)->rhs->eval(*editorContext);
                cmpOp = dynamic_cast<IfStmt *>(stmt)->cmpOp;
                targetLineNum = dynamic_cast<IfStmt *>(stmt)->targetLineNum;
                if ((cmpOp == "=" && l_val == r_val) ||
                    (cmpOp == "<" && l_val < r_val) ||
                    (cmpOp == ">" && l_val > r_val))
                {
                    currentLine = targetLineNum;
                    it = editor->code.find(currentLine);
                    continue;
                }
                break;
            case END:
            default:
                break;
            }
        }
        catch (exception &e)
        {
            QMessageBox::warning(this, "Error::", e.what());
            return;
        }

        ++it;
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
    else if(directCmd(cmdInput))
    {
        /*Do Nothing*/
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

// The LET, PRINT, and INPUT statements can be executed directly by typing them without a 
// line number, in which case they are evaluated immediately
bool MainWindow::directCmd(string cmd)
{
    Tokenizer tokenizer;
    vector<string> tokens = tokenizer.tokenize(cmd);
    // if the first token is not LET, PRINT, or INPUT, return false
    if (tokens.empty() || (tokens[0] != "LET" && tokens[0] != "PRINT" && tokens[0] != "INPUT"))
    {
        return false;
    }
    // add a virtual line number to the direct command
    tokens.insert(tokens.begin(), "10");
    Statement *stmt;
    try
    {
        stmt = parser->parseStatement(tokens);
    }
    catch (exception &e)
    {
        QMessageBox::warning(this, "Error::", e.what());
        return true;
    }
    if (stmt == NULL)
    {
        return true;
    }
    try{
        switch (stmt->type())
        {
        case PRINT:
            ui->textBrowser->append(QString::number(dynamic_cast<PrintStmt *>(stmt)->rootExp->eval(*directContext)));
            break;
        case LET:
            dynamic_cast<LetStmt *>(stmt)->rootExp->eval(*directContext);
            break;
        case INPUT:
            while (!input(dynamic_cast<InputStmt *>(stmt)->varName, true))
            {
            };
            break;
        default:
            QMessageBox::warning(this, "Error::", "Invalid direct command");
            delete stmt;
            return true;
        }
    }
    catch (exception &e)
    {
        QMessageBox::warning(this, "Error::", e.what());
        delete stmt;
        return true;
    }
    delete stmt;
    return true;
}