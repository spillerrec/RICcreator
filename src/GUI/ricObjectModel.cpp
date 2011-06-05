/*
	This file is part of RICcreator.

	RICcreator is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	RICcreator is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with RICcreator.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ricObjectModel.h"
#include "../riclib/ricObjectChildren.h"
#include "../riclib/nxtVariable.h"


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
				return object->setting_amount();
			else
				return 0;
		}
		else
			return 0;
	}
}


int ricModel::columnCount(const QModelIndex &parent) const{
	if( !parent.isValid() )
		return 3;
	else{
		if( !parent.internalPointer() ){
			return 3;
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
			if( index.column() == 0 )
				//Return human-readable name of the ricObject
				switch( object->object_type() ){
					case ricfile::ricObject::RIC_OP_OPTIONS: return "Options";
					case ricfile::ricObject::RIC_OP_SPRITE: return "Sprite";
					case ricfile::ricObject::RIC_OP_VARMAP: return "VarMap";
					case ricfile::ricObject::RIC_OP_COPYBITS: return "CopyBits";
					case ricfile::ricObject::RIC_OP_PIXEL: return "Pixel";
					case ricfile::ricObject::RIC_OP_LINE: return "Line";
					case ricfile::ricObject::RIC_OP_RECTANGLE: return "Rectangle";
					case ricfile::ricObject::RIC_OP_CICLE: return "Circle";
					case ricfile::ricObject::RIC_OP_NUMBER: return "Number";
					case ricfile::ricObject::RIC_OP_ELLIPSE: return "Ellipse";
					case ricfile::ricObject::RIC_OP_POLYGON: return "Polygon";
					default: return "Unknown element";
				}
			else if( index.column() == 2 )
				//Return filesize of the ricObject
				return object->filesize();
			else
				//TODO: return resume of data for column 1
				return QVariant();
		}
		else
			return QVariant();
	}
	else{
		ricfile::ricObject* object = (ricfile::ricObject*) index.internalPointer();
		//Retrive the setting
		if( object->setting_amount() <= (unsigned int)index.row() )
			return QVariant();
		nxtVariable& setting = object->get_setting( index.row() );
		
		if( index.column() == 0 )
			switch( object->object_type() ){
				case ricfile::ricObject::RIC_OP_OPTIONS:{
						switch( index.row() ){
							case 0:	return "Options";
							case 1:	return "Height";
							case 2:	return "Width";
							default: return QVariant();
						}
					}
				case ricfile::ricObject::RIC_OP_SPRITE:{
						switch( index.row() ){
							case 0:	return "Sprite ID";
							default: return QVariant();
						}
					}
				case ricfile::ricObject::RIC_OP_VARMAP:{
						switch( index.row() ){
							case 0:	return "VarMap ID";
							case 1:	return "Values";	//TODO: what to do here
							default: return QVariant();
						}
					}
				case ricfile::ricObject::RIC_OP_COPYBITS:{
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
				case ricfile::ricObject::RIC_OP_PIXEL:{
						switch( index.row() ){
							case 0:	return "Copy options";
							case 1:	return "X";
							case 2:	return "Y";
							case 3:	return "Value";
							default: return QVariant();
						}
					}
				case ricfile::ricObject::RIC_OP_LINE:{
						switch( index.row() ){
							case 0:	return "Copy options";
							case 1:	return "Start X";
							case 2:	return "Start Y";
							case 3:	return "End X";
							case 4:	return "End Y";
							default: return QVariant();
						}
					}
				case ricfile::ricObject::RIC_OP_RECTANGLE:{
						switch( index.row() ){
							case 0:	return "Copy options";
							case 1:	return "X";
							case 2:	return "Y";
							case 3:	return "Width";
							case 4:	return "Height";
							default: return QVariant();
						}
					}
				case ricfile::ricObject::RIC_OP_CICLE:{
						switch( index.row() ){
							case 0:	return "Copy options";
							case 1:	return "X";
							case 2:	return "Y";
							case 3:	return "Radius";
							default: return QVariant();
						}
					}
				case ricfile::ricObject::RIC_OP_NUMBER:{
						switch( index.row() ){
							case 0:	return "Copy options";
							case 1:	return "X";
							case 2:	return "Y";
							case 3:	return "Value";
							default: return QVariant();
						}
					}
				case ricfile::ricObject::RIC_OP_ELLIPSE:{
						switch( index.row() ){
							case 0:	return "Copy options";
							case 1:	return "X";
							case 2:	return "Y";
							case 3:	return "Radius X";
							case 4:	return "Radius Y";
							default: return QVariant();
						}
					}
				case ricfile::ricObject::RIC_OP_POLYGON:{
						switch( index.row() ){
							case 0:	return "Copy options";
							case 1:	return "Points";
							default: return QVariant();
						}
					}
				default: return "Unknown element";
			}
		else if( index.column() == 2 ){
			return setting.filesize();	//return size of the setting
			
		}
		else if( index.column() == 1 ){
			switch( setting.var_type() ){
				case nxtVariable::TYPE_RIC_WORD:{
						ricfile::nxtVarRicWord& variable = (ricfile::nxtVarRicWord&)setting;
						
						if( variable.is_extended() ){
							QString display;
							if( variable.get_varmap() ){
								display = "V";
								display += QString::number( variable.get_varmap() );
								display += " ";
							}
							
							display += "P";
							display += QString::number( variable.get_parameter() );
							
							return display;
						}
						else
							return variable.value();
					}
					
				
				case nxtVariable::TYPE_UWORD:	return (unsigned int)(nxtVarWord&)setting;
				
				default: return QVariant();
			}
		}
		else
			return QVariant();
	}
}


QVariant ricModel::headerData( int section, Qt::Orientation orientation, int role ) const{
	if( role == Qt::DisplayRole ){
		if( orientation != Qt::Horizontal )
			return QVariant();
		
		switch( section ){
			case 0: return "Type";
			case 1: return "Data";
			case 2: return "Size";
			
			default: return QVariant();
		}
	}
	
	return QVariant();
}


void ricModel::update(){
//	reset();
	emit dataChanged( index(0,0), index( rowCount()-1, 0 ) );
}
void ricModel::reset_model(){
	reset();
//	emit dataChanged( index(0,0), index( rowCount()-1, 0 ) );
}



ricfile::ricObject* ricModel::ricobject_at_index( const QModelIndex &index ) const{
	ricfile::ricObject* object = NULL;
	
	if( !index.isValid() || index.internalPointer() == NULL )
		object = file->get_object( index.row() );
	else
		object = (ricfile::ricObject*) index.internalPointer();
	
	return object;
}


