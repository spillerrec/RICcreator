#include "src/GUI/ricObjectModel.h"
#include "src/riclib/ricObject.h"

int ricObjectModel::rowCount(const QModelIndex &parent) const{
	return file->object_amount();
}

QVariant ricObjectModel::data(const QModelIndex &index, int role) const{
	if (!index.isValid())
		return QVariant();

	if (index.row() >= rowCount())
		return QVariant();

	if (role == Qt::DisplayRole){
		ricfile::ricObject* object = file->get_object( index.row() );
		if( object != 0 ){
			switch( object->object_type() ){
				case RIC_OP_OPTIONS: return "Options";
				case RIC_OP_SPRITE: return "Sprite";
				case RIC_OP_VARMAP: return "VarMap";
				case RIC_OP_COPYBITS: return "CopyBits";
				case RIC_OP_PIXEL: return "Pixel";
				case RIC_OP_LINE: return "Line";
				case RIC_OP_RECTANGLE: return "Rectangle";
				case RIC_OP_CICLE: return "Circle";
				case RIC_OP_NUMBER: return "Number";
				default: return "Unknown element";
			}
		}
		else
			return QVariant();
	}
	else
		return QVariant();
 }
 
void ricObjectModel::update(){
	emit dataChanged( index(0), index( rowCount()-1 ) );
	//reset();
}


