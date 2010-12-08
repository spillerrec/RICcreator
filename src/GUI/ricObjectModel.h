#include <QAbstractListModel>

class ricfile;

class ricObjectModel : public QAbstractListModel{
	Q_OBJECT

	public:
		ricObjectModel(ricfile* source, QObject *parent = 0) : QAbstractListModel(parent){
			file = source;
		}

		int rowCount(const QModelIndex &parent = QModelIndex()) const;
		QVariant data(const QModelIndex &index, int role) const;
		void update();

	private:
		ricfile* file;
};