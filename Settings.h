#pragma once

#include <QSettings>
#include <list>

class Settings
{
	private:

		QSettings _settings;
		bool _wordWrap;
		std::list<QString> _recentFiles;

		Settings();

		void read();
		void write();

	public:
    
		static Settings* instance();
		~Settings();

		bool wordWrap() { return _wordWrap; }
		std::list<QString> & recentFiles() { return _recentFiles; }
		void setWordWrap(bool active);
		void addRecentFile(QString s);
};
