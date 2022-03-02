#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <ui_new_project.h>
#include "data/userdata.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
class new_project_ui;
}


QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool maybeSave();

    bool loadFile(const QString& file_name);


    void setTitleBase(const QString& str);


    void setTipText(const QString& tip);

private slots:
    void open();
    bool save();
    bool newProject();

    void on_pa_clicked();
    void on_create_new_project();

private:
    QString window_title_base;          //< 软件窗口名称


private:
    UserData* user_data;
    bool maybe_save;                    //< 是否需要保存
    QString project_path;               //< 项目路径
    QString project_name;               //< 项目名字

private:
    Ui::MainWindow *ui;
    QWidget* new_project_ui_page_parent;
    Ui::new_project_ui *new_project_ui_page;
};
#endif // MAINWINDOW_H
