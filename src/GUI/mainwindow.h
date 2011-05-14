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
	void save_file_as();
	void show_about();
	void close_tab();
	void close_tab( int tab );
	
	
	//Add new objects to file
	void add_options();
	void add_sprite();
	void add_copybits();
	void add_varmap();
	void add_pixel();
	void add_line();
	void add_rectangle();
	void add_circle();
	void add_number();
	void add_ellipse();
	void add_polyline();
	
private:
	void add_object( unsigned int object_type );
	
};

#endif // MAINWINDOW_H
