#ifndef ABOUT_H
#define ABOUT_H

#include <QWidget>



class about_widget: public QWidget{
	Q_OBJECT
	
	private:
		class Ui_AboutForm *ui;
	
	public:
		explicit about_widget();
	
	
	private slots:
		void action_close(){
			hide();
		}
};

#endif