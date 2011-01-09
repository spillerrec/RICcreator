#include "src/GUI/ricParametersModel.h"
#include "src/GUI/ricfile_widget.h"


ricParametersModel::ricParametersModel( ricfile* source, ricfile_widget* parent ): QAbstractItemModel( parent ){
	file = source;
	container = parent;
}

Qt::ItemFlags ricParametersModel::flags( const QModelIndex & index ) const{
	return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

QModelIndex ricParametersModel::index(int row, int column, const QModelIndex &parent_index) const{
	if( !parent_index.isValid() ){
		if( (unsigned int)row < 256 && column == 0 )
			return createIndex( row, column );
		else
			return QModelIndex();
	}
	
	return QModelIndex();
}


QModelIndex ricParametersModel::parent( const QModelIndex &index) const{
	return QModelIndex();
}


int ricParametersModel::rowCount(const QModelIndex &parent) const{
	if( !parent.isValid() )
		return 256;
	else
		return 0;
}


int ricParametersModel::columnCount(const QModelIndex &parent) const{
	if( !parent.isValid() )
		return 1;
	else
		return 0;
}


QVariant ricParametersModel::data( const QModelIndex &index, int role ) const{
	if( role == Qt::DisplayRole && index.isValid() && index.column() == 0 && index.row() < 256 ){
		return file->GetParameter( index.row() );
	}
	
	return QVariant();
}

bool ricParametersModel::setData( const QModelIndex &index, const QVariant &value, int role ){
	if( role == Qt::EditRole && index.isValid() && index.column() == 0 && index.row() < 256 ){
		
		bool success;
		int converted_value = value.toInt( &success );
		if( success && converted_value >= 0 && converted_value < 256 ){
			file->SetParameter( index.row(), converted_value );
			emit dataChanged( index, index );
			container->update_preview();
			return true;
		}
	}
	
	return false;
}


QVariant ricParametersModel::headerData( int section, Qt::Orientation orientation, int role ) const{
	if( role != Qt::DisplayRole )
		return QVariant();
	
	if( orientation == Qt::Horizontal ){
		switch( section ){
			case 0: return "Value";
			
			default: return QVariant();
		}
	}
	else if( orientation == Qt::Vertical ){
		return section;
	}
	else
		return QVariant();
}


void ricParametersModel::update(){
//	reset();
	emit dataChanged( index(0,0), index( 255, 0 ) );
	container->update_preview();
}



