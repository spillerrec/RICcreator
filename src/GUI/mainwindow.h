#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


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
    Ui::MainWindow *ui;
	

private slots:
	void exit();
	void new_file();
	void open_file();
	void save_file();
	void close_tab();
	void close_tab( int tab );
	
};

#endif // MAINWINDOW_H
