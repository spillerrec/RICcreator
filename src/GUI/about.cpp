
#include "ui_about.h"
#include "about.h"

about_widget::about_widget(): QWidget(0), ui(new Ui_AboutForm){
	ui->setupUi(this);
	connect( ui->btn_close, SIGNAL( clicked() ), this, SLOT( action_close() ) );
}

