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

#ifndef RICOBJECTMODEL_H
#define RICOBJECTMODEL_H

#include <QAbstractItemModel>
#include "../riclib/ricfile.h"

class ricModel: public QAbstractItemModel{
	Q_OBJECT
	
	private:
		ricfile* file;
		
	public:
		ricModel( ricfile* source, QObject *parent = 0 );
		QVariant data( const QModelIndex &index, int role ) const;
		QModelIndex index( int row, int column, const QModelIndex &parent_index = QModelIndex()) const;
		QModelIndex parent( const QModelIndex &index ) const;
		int rowCount( const QModelIndex &parent = QModelIndex()) const;
		int columnCount( const QModelIndex &parent = QModelIndex()) const;
		QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
		
		
		void update();
		void reset_model();
		int index_level( const QModelIndex &index ) const;
		ricfile::ricObject* ricobject_at_index( const QModelIndex &index ) const;
};

#endif
