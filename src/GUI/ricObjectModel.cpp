#include "src/GUI/ricObjectModel.h"
#include "src/riclib/ricfile.h"
#include "src/riclib/ricObject.h"


ricModel::ricModel( ricfile* source, QObject *parent ): QAbstractItemModel( parent ){
	file = source;
}


QModelIndex ricModel::index(int row, int column, const QModelIndex &parent) const{
	if( parent.isValid() )
		return QModelIndex();
	
	if( column != 0 )
		return QModelIndex();
	
	ricfile::ricObject* object = file->get_object( row );
	if( object ){
		return createIndex( row, column, object );
	}
	else
		return QModelIndex();
}


QModelIndex ricModel::parent( const QModelIndex &index) const{
	return QModelIndex();
}


int ricModel::rowCount(const QModelIndex &parent) const{
	if( !parent.isValid() ){
		return file->object_amount();
	}
	else{
		return 0;
	}
}


int ricModel::columnCount(const QModelIndex &parent) const{
	if( !parent.isValid() )
		return 1;
	else{
		return 0;
	}
}


QVariant ricModel::data( const QModelIndex &index, int role ) const{
	if( !index.isValid() )
		return QVariant();
	
	if( role != Qt::DisplayRole )
		return QVariant();
	
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
			case RIC_OP_ELLIPSE: return "Ellipse";
			default: return "Unknown element";
		}
	}
	else
		return QVariant();
}


QVariant ricModel::headerData( int section, Qt::Orientation orientation, int role ) const{
	if( role != Qt::DisplayRole )
		return QVariant();
	
	if( orientation != Qt::Horizontal )
		return QVariant();
	
	switch( section ){
		case 0: return "Type";
		case 1: return "Data";
		
		default: return QVariant();
	}
}


void ricModel::update(){
	reset();
//	emit dataChanged( index(0,0), index( rowCount()-1, 0 ) );
}



