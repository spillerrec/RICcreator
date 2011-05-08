
#include "ui_ric_value.h"
#include "ric_value.h"

ric_value::ric_value( QWidget* parent, QString text, ricfile::nxtVarRicWord* value_object, QString tooltip ): QWidget(parent), ui(new Ui_ric_value_select){
	ui->setupUi(this);
	connect( ui->parameter_mode, SIGNAL( stateChanged( int ) ), this, SLOT( update_mode() ) );
	
	
	//Set settings
	ui->text->setText( text );
	ui->text->setToolTip( tooltip );
	ricword = value_object;
	read();
}


//Read the value in ricword and update the values
void ric_value::read(){
	//Get the correct values
	ui->parameter_mode->setChecked( ricword->is_extended() );
	ui->varmap_id->setValue( ricword->get_varmap() );
	ui->parameter->setValue( ricword->get_parameter() );
	ui->value->setValue( ricword->get_number() );
	
	refresh_mode();	//Change mode if neccessary
}

//Write the values back into ricword
void ric_value::write(){
	if( ui->parameter_mode->isChecked() )
		ricword->set_extended( ui->varmap_id->value(), ui->parameter->value() );
	else
		ricword->set_normal( ui->value->value() );
	
	emit value_changed();
}


//Make sure the right spinboxes are visible
void ric_value::refresh_mode(){
	if( ui->parameter_mode->isChecked() ){
		ui->value->hide();
		ui->varmap_id->show();
		ui->parameter->show();
	}
	else{
		ui->varmap_id->hide();
		ui->parameter->hide();
		ui->value->show();
	}
}

void ric_value::update_mode(){
	refresh_mode();
	write();
}

