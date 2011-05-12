
#include "ui_ric_value.h"
#include "ric_value.h"

ric_value::ric_value( QWidget* parent, QString text, ricfile::nxtVarRicWord* value_object, QString tooltip ): QWidget(parent), ui(new Ui_ric_value_select){
	ui->setupUi(this);
	connect( ui->parameter_mode, SIGNAL( stateChanged( int ) ), this, SLOT( update_mode() ) );
	
	connect( ui->varmap_id, SIGNAL( valueChanged( int ) ), this, SLOT( write() ) );
	connect( ui->parameter, SIGNAL( valueChanged( int ) ), this, SLOT( write() ) );
	connect( ui->value, SIGNAL( valueChanged( int ) ), this, SLOT( write() ) );
	
	
	//Set settings
	ui->text->setText( text );
	ui->text->setToolTip( tooltip );
	change_value_object( value_object );
}


//Read the value in ricword and update the values
void ric_value::read(){
	if( ricword ){
		//Prevent it to overwrite ricword while reading, since this triggers the spinboxes valueChanged() signal
		ricfile::nxtVarRicWord* temp = ricword;
		ricword = NULL;
		
		//Get the correct values
		ui->varmap_id->setValue( temp->get_varmap() );
		ui->parameter->setValue( temp->get_parameter() );
		ui->value->setValue( temp->get_number() );
		ui->parameter_mode->setChecked( temp->is_extended() );
		
		//Reenable writing
		ricword = temp;
	}
	
	refresh_mode();	//Change mode if neccessary
}

//Write the values back into ricword
void ric_value::write(){
	if( !ricword )
		return;	//Nothing to write to...
	
	if( ui->parameter_mode->isChecked() )
		ricword->set_extended( ui->varmap_id->value(), ui->parameter->value() );
	else
		ricword->set_normal( ui->value->value() );
	
	emit value_changed();
}

//Change the ric value that is in use
void ric_value::change_value_object( ricfile::nxtVarRicWord* new_value_object ){
	ricword = new_value_object;
	if( ricword ){
		read();
		setEnabled( true );
	}
	else
		setEnabled( false );
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

