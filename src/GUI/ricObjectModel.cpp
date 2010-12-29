#include "src/GUI/ricObjectModel.h"
#include "src/riclib/ricfile.h"
#include "src/riclib/ricObject.h"


ricModel::ricModel( ricfile* source, QObject *parent ): QAbstractItemModel( parent ){
	file = source;
}


int ricModel::index_level( const QModelIndex &index ) const{
	if( !index.isValid() )
		return 0;
	
	if( index.internalPointer() == NULL )
		return 0;
	else
		return 1;
}


QModelIndex ricModel::index(int row, int column, const QModelIndex &parent_index) const{
	if( !parent_index.isValid() ){
		if( (unsigned int)row < file->object_amount() )
			return createIndex( row, column, (void*)NULL );
		else
			return QModelIndex();
	}
	
	switch( index_level( parent_index ) ){
		case 0:{
				if( (unsigned int)parent_index.row() >= file->object_amount() )
					return QModelIndex();
				
				return createIndex( row, column, file->get_object( parent_index.row() ) );
			}break;
		
		case 1: return QModelIndex();
	}
	
	
	return QModelIndex();
}


QModelIndex ricModel::parent( const QModelIndex &index) const{
	switch( index_level( index ) ){
		case 0: return QModelIndex();
	}
	
	int parent_row = file->object_index( (ricfile::ricObject*)index.internalPointer() );
	if( parent_row >= 0 )
		return this->index( parent_row, 0 );
	else
		return QModelIndex();
}


int ricModel::rowCount(const QModelIndex &parent) const{
	if( !parent.isValid() ){
		return file->object_amount();
	}
	else{
		if( !parent.internalPointer() ){
			ricfile::ricObject* object = file->get_object( parent.row() );
			if( object )
				switch( object->object_type() ){
					case RIC_OP_OPTIONS: return 3;
					case RIC_OP_SPRITE: return 1;
					case RIC_OP_VARMAP: return 2;
					case RIC_OP_COPYBITS: return 8;
					case RIC_OP_PIXEL: return 3;
					case RIC_OP_LINE: return 5;
					case RIC_OP_RECTANGLE: return 5;
					case RIC_OP_CICLE: return 4;
					case RIC_OP_NUMBER: return 4;
					case RIC_OP_ELLIPSE: return 5;
					default: return 1;
				}
			else
				return 0;
		}
		else
			return 0;
	}
}


int ricModel::columnCount(const QModelIndex &parent) const{
	if( !parent.isValid() )
		return 1;
	else{
		if( !parent.internalPointer() ){
			return 1;
		}
		else
			return 0;
	}
}


QVariant ricModel::data( const QModelIndex &index, int role ) const{
	if( role != Qt::DisplayRole )
		return QVariant();
		
	if( !index.isValid() || index.internalPointer() == NULL ){
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
	else{
		ricfile::ricObject* object = (ricfile::ricObject*) index.internalPointer();
			switch( object->object_type() ){
				case RIC_OP_OPTIONS:{
						switch( index.row() ){
							case 0:	return "Options";
							case 1:	return "Height";
							case 2:	return "Width";
							default: return QVariant();
						}
					}
				case RIC_OP_SPRITE:{
						switch( index.row() ){
							case 0:	return "Sprite ID";
							default: return QVariant();
						}
					}
				case RIC_OP_VARMAP:{
						switch( index.row() ){
							case 0:	return "VarMap ID";
							case 1:	return "Values";	//TODO: what to do here
							default: return QVariant();
						}
					}
				case RIC_OP_COPYBITS:{
						switch( index.row() ){
							case 0:	return "Copy options";
							case 1:	return "Sprite ID";
							case 2:	return "Sprite X";
							case 3:	return "Sprite Y";
							case 4:	return "Sprite width";
							case 5:	return "Sprite height";
							case 6:	return "X";
							case 7:	return "Y";
							default: return QVariant();
						}
					}
				case RIC_OP_PIXEL:{
						switch( index.row() ){
							case 0:	return "Copy options";
							case 1:	return "X";
							case 2:	return "Y";
							default: return QVariant();
						}
					}
				case RIC_OP_LINE:{
						switch( index.row() ){
							case 0:	return "Copy options";
							case 1:	return "Start X";
							case 2:	return "Start Y";
							case 3:	return "End X";
							case 4:	return "End Y";
							default: return QVariant();
						}
					}
				case RIC_OP_RECTANGLE:{
						switch( index.row() ){
							case 0:	return "Copy options";
							case 1:	return "X";
							case 2:	return "Y";
							case 3:	return "Width";
							case 4:	return "Height";
							default: return QVariant();
						}
					}
				case RIC_OP_CICLE:{
						switch( index.row() ){
							case 0:	return "Copy options";
							case 1:	return "X";
							case 2:	return "Y";
							case 3:	return "Radius";
							default: return QVariant();
						}
					}
				case RIC_OP_NUMBER:{
						switch( index.row() ){
							case 0:	return "Copy options";
							case 1:	return "X";
							case 2:	return "Y";
							case 3:	return "Value";
							default: return QVariant();
						}
					}
				case RIC_OP_ELLIPSE:{
						switch( index.row() ){
							case 0:	return "Copy options";
							case 1:	return "X";
							case 2:	return "Y";
							case 3:	return "Radius X";
							case 4:	return "Radius Y";
							default: return QVariant();
						}
					}
				default: return "Unknown element";
			}
	}
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



