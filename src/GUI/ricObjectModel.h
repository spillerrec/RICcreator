#include <QAbstractItemModel>
#include "src/riclib/ricfile.h"

class ricModel: public QAbstractItemModel{
	Q_OBJECT
	
	private:
		ricfile* file;
		QVariant convert_ric_word( const ricfile::nxtVarRicWord* variable ) const;
		
	public:
		ricModel( ricfile* source, QObject *parent = 0 );
		QVariant data( const QModelIndex &index, int role ) const;
		QModelIndex index( int row, int column, const QModelIndex &parent_index = QModelIndex()) const;
		QModelIndex parent( const QModelIndex &index ) const;
		int rowCount( const QModelIndex &parent = QModelIndex()) const;
		int columnCount( const QModelIndex &parent = QModelIndex()) const;
		QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
		
		
		void update();
		int index_level( const QModelIndex &index ) const;
};


