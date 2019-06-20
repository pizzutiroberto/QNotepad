#pragma once

#include <QTextEdit>
#include <set>

class Document : public QTextEdit
{
	Q_OBJECT

	private:

		QString _filePath;
		bool _modified;
		int _id;
		static std::set<int> _available_ids;

	public:

		Document(QString filePath = "", QWidget* parent = 0);
		~Document();

		void save(bool newfile = false);
		bool modified() { return _modified; }
		bool isnew() { return _filePath.isEmpty(); }
		int id() { return _id; }
		QString filePath() { return _filePath; }
		virtual void wheelEvent(QWheelEvent *e);

	public slots:

		void textIsChanged();
		void cursorChanged();
};
