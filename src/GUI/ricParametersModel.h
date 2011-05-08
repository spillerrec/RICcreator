#ifndef RICPARAMETERSMODEL_H
#define RICPARAMETERSMODEL_H

#include <QAbstractItemModel>
#include "../riclib/ricfile.h"

class ricfile_widget;

class ricParametersModel: public QAbstractItemModel{
	Q_OBJECT
	
	private:
		ricfile* file;
		ricfile_widget* container;
		
	public:
		ricParametersModel( ricfile* source, ricfile_widget* parent );
		QVariant data( const QModelIndex &index, int role ) const;
		QModelIndex index( int row, int column, const QModelIndex &parent_index = QModelIndex()) const;
		QModelIndex parent( const QModelIndex &index ) const;
		int rowCount( const QModelIndex &parent = QModelIndex()) const;
		int columnCount( const QModelIndex &parent = QModelIndex()) const;
		QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
		Qt::ItemFlags flags( const QModelIndex & index ) const;
		bool setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole );
		
		void update();
};


#endif