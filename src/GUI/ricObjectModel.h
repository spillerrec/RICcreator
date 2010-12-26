#include <QAbstractItemModel>

class ricfile;

class ricModel: public QAbstractItemModel{
	Q_OBJECT
	
	private:
		ricfile* file;
		
	public:
		ricModel( ricfile* source, QObject *parent = 0 );
		QVariant data( const QModelIndex &index, int role ) const;
		QModelIndex index( int row, int column, const QModelIndex &parent = QModelIndex()) const;
		QModelIndex parent( const QModelIndex &index ) const;
		int rowCount( const QModelIndex &parent = QModelIndex()) const;
		int columnCount( const QModelIndex &parent = QModelIndex()) const;
		QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
		
		
		void update();
};


