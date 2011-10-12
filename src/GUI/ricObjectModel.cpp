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

//IMPORTANT!!! This doesn't mach any longer, correct it!!!

#include "ricObjectModel.h"

#include "../riclib/ricfile.h"
#include "../riclib/nxtVariable.h"

#include "ricObjectTexter.h"


int ricModel::index_level( const QModelIndex &index ) const{
	if( !index.isValid() )
		return 0;
	
	if( index.internalPointer() == NULL )
		return 0;
	else
		return 1;
}


QModelIndex ricModel::index(int row, int column, const QModelIndex &parent_index) const{
	if( !file )
		return QModelIndex();
	
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
	if( !file )
		return QModelIndex();
	
	switch( index_level( index ) ){
		case 0: return QModelIndex();
	}
	
	int parent_row = file->object_index( (ricObject*)index.internalPointer() );
	if( parent_row >= 0 )
		return this->index( parent_row, 0 );
	else
		return QModelIndex();
}


int ricModel::rowCount(const QModelIndex &parent) const{
	if( !file )
		return 0;
	
	if( !parent.isValid() ){
		return file->object_amount();
	}
	else{
		if( !parent.internalPointer() ){
			ricObject* object = file->get_object( parent.row() );
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
	if( !file )
		return QVariant();
	
	if( role != Qt::DisplayRole )
		return QVariant();
		
	if( !index.isValid() || index.internalPointer() == NULL ){
		ricObject* object = file->get_object( index.row() );
		if( object != 0 ){
			switch( index.column() ){
				case 0:	return ricObjectTexter::object_name( object->object_type() );
				case 1:	return QVariant(); //TODO: return resume of data for column 1
				case 2:	return object->filesize();
				default:	return QVariant();
			}
		}
		else
			return QVariant();
	}
	else{
		ricObject* object = (ricObject*) index.internalPointer();
		//Retrive the setting
		if( object->setting_amount() <= (unsigned int)index.row() )
			return QVariant();
		nxtVariable* setting = object->get_setting( index.row() );
		if( !setting )
			return QVariant();
		
		switch( index.column() ){
			case 0:	return ricObjectTexter::object_property_name( object->object_type(), index.row() );
			case 1:	return ricObjectTexter::nxtVarToStr( setting );
			case 2:	return setting->filesize();	//return size of the setting
			default:	return QVariant();
		}
	}
}


QVariant ricModel::headerData( int section, Qt::Orientation orientation, int role ) const{
	if( role == Qt::DisplayRole ){
		if( orientation != Qt::Horizontal )
			return QVariant();
		
		switch( section ){
			case 0: return tr( "Type" );
			case 1: return tr( "Data" );
			case 2: return tr( "Size" );
			
			default: return QVariant();
		}
	}
	
	return QVariant();
}


void ricModel::update(){
	emit dataChanged( index(0,0), index( rowCount()-1, 0 ) );
}
void ricModel::reset_model(){
	reset();
}



ricObject* ricModel::ricobject_at_index( const QModelIndex &index ) const{
	ricObject* object = NULL;
	
	if( !file )
		return NULL;
	
	if( !index.isValid() || index.internalPointer() == NULL )
		object = file->get_object( index.row() );
	else
		object = (ricObject*) index.internalPointer();
	
	return object;
}


