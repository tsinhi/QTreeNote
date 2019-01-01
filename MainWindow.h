#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QWebEnginePage>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>


namespace Ui {
class MainWindow;

}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();
private:
    void initNoteUi();
    void initNote();
    void initApp();
    void initTreeViewMenu();
private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
