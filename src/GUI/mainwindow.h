#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "about.h"
#include "programSettings.h"


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow( QString filenames = "", QWidget *parent = 0);
	about_widget about_window;
    ~MainWindow();

private:
    Ui::MainWindow *ui;
	programSettings perferences;
	

private slots:
	void exit();
	void new_file();
	void open_file();
	void open_file( QString filename );
	void save_file();
	void show_about();
	void close_tab();
	void close_tab( int tab );
	
};

#endif // MAINWINDOW_H
