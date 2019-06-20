#include "Settings.h"

Settings* Settings::instance()
{
	static Settings uniqueInstance;
	return & uniqueInstance;
}

Settings::Settings() : _settings("unicas", "QNotepad")
{
	_wordWrap = false;
	read();
}

Settings::~Settings()
{
	write();
}

void Settings::read()
{
	if(_settings.contains("wordWrap"))
		_wordWrap = _settings.value("wordWrap").toBool();

	size_t size = _settings.beginReadArray("recentFiles");
	for (int i=0; i<size; i++)
	{
		_settings.setArrayIndex(i);
		recentFiles().push_back( _settings.value("filepath").toString() );
	}
	_settings.endArray();
}

void Settings::write()
{
	_settings.setValue("wordWrap", _wordWrap);
	_settings.beginWriteArray("recentFiles");
	int count = 0;
	for (auto & f : _recentFiles)
	{
		_settings.setArrayIndex(count++);
		_settings.setValue("filepath", f);
	}
	_settings.endArray();
}

void Settings::setWordWrap(bool active)
{
	_wordWrap = active;
}

void Settings::addRecentFile(QString s)
{
	_recentFiles.remove(s);
	_recentFiles.push_front(s);
}
