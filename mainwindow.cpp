#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QLabel>
#include <QDir>

#include "data/systemdata.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , new_project_ui_page(new Ui::new_project_ui)
{
    ui->setupUi(this);


    new_project_ui_page_parent = new QWidget(this);
    new_project_ui_page_parent->setWindowFlags(Qt::Dialog);
    new_project_ui_page->setupUi(new_project_ui_page_parent);
    connect(new_project_ui_page->pa,&QPushButton::clicked,this,&MainWindow::on_pa_clicked);
    connect(new_project_ui_page->ok_btn,&QPushButton::clicked,this,&MainWindow::on_create_new_project);
    new_project_ui_page_parent->hide();
    new_project_ui_page_parent->setWindowTitle(tr("新建项目"));


    maybe_save = false;
    connect( ui->actionOpen,&QAction::triggered,this,&MainWindow::open);
    connect(ui->actionNew,&QAction::triggered,this,&MainWindow::newProject);
    this->ui->statusbar->showMessage(tr("请新建或打开项目!"));

    // 新建项目页

    QWidget* new_project_page = ui->new_project_page;
    new_project_page->hide();
    QLabel* new_project_page_title = new QLabel(new_project_page);
    new_project_page_title->setText(tr("新建项目"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::newProject(){
    new_project_ui_page_parent->show();
    return true;
}


void MainWindow::open(){
    qDebug("open ......");
    if(maybeSave()){
        return ;
    }
    QString fileName = QFileDialog::getOpenFileName(this,QString(),QString(),"*.wtt");
    if (!fileName.isEmpty())
        loadFile(fileName);
}

bool MainWindow::save(){

    return true;
}

bool MainWindow::maybeSave()
//! [40] //! [41]
{
    if(!maybe_save){
        return false;
    }
    const QMessageBox::StandardButton ret
        = QMessageBox::warning(this, tr("Application"),
                               tr("The document has been modified.\n"
                                  "Do you want to save your changes?"),
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    switch (ret) {
    case QMessageBox::Save:
        return save();
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}

///
/// \brief MainWindow::loadFile  加载工程文件
/// \param file_name
/// \return
///
bool MainWindow::loadFile(const QString& file_name){
    SystemData::Instance()->SetCurrentPath(file_name);
    QString title = this->window_title_base+ "("+file_name+")";

    QFileInfo file(file_name);
    project_name = file.fileName();
    if(!file.exists()){
        title += tr("项目不存在");
        this->setWindowTitle(title);
        this->ui->statusbar->showMessage(tr("项目不存在")+":"+file_name);
        return false;
    }
    project_path = file.absolutePath();
    this->setWindowTitle(title);
    setTipText(tr("项目打开成功!"));
    // 加载用户数据
    user_data->Init(file_name);
    return true;
}

void MainWindow::setTipText(const QString& tip){
     this->ui->statusbar->showMessage(tip);
}

///
/// \brief MainWindow::setTitleBase     设置窗口标题 只能设置一次
/// \param str
///
void MainWindow::setTitleBase(const QString& str){
    this->window_title_base = str;
    this->setWindowTitle(str);
}

///
/// \brief MainWindow::on_pa_clicked    新建项目页面中浏览按钮
///
void MainWindow::on_pa_clicked()
{
    QString newProjectPath = QFileDialog::getExistingDirectory(this);
    new_project_ui_page->new_project_path->setText(newProjectPath);
}


void MainWindow::on_create_new_project(){
    QString projectName = new_project_ui_page->new_project_name->text();
    QString projectPath = new_project_ui_page->new_project_path->text();
    if(projectPath.length() == 0){
        QMessageBox::warning(this,tr("新建项目"),tr("项目名称不能为空"));
        return ;
    }
    if(projectName.length() == 0){
        QMessageBox::warning(this,tr("新建项目"),tr("项目路径不能为空"));
        return ;
    }
    QDir projectDir(projectPath);
    projectDir.mkdir(projectName);
    projectDir.cd(projectName);
    QString project = projectDir.absolutePath()+"/"+projectName+".wtt";
    QFile projectFile(project);
//    projectFile.setFileName(projectName+".wtt");
    projectFile.open(QFile::WriteOnly);
    projectFile.close();
    setTipText(tr("新建项目成功！")+project);
    new_project_ui_page_parent->hide();
    loadFile(project);
}
