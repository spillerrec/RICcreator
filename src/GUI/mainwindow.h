#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QGraphicsScene>


#include "src/riclib/ricfile.h"


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
	QImage image;
	QGraphicsScene scene;
	ricfile graphics;
	

private slots:
	void exit();
	void new_file();
	void open_file();
	void save_file();
	void update_preview();
};

#endif // MAINWINDOW_H
