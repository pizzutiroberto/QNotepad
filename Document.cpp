#include "Document.h"
#include <fstream>
#include <QFileDialog>
#include <QTabWidget>
#include <QWheelEvent>
#include <iostream>
#include "Notepad.h"

std::set<int> Document::_available_ids = std::set<int>();

Document::Document(QString filePath, QWidget* parent) : QTextEdit(parent)
{
	if(_available_ids.empty())
		for(int i=1; i<10000; i++)
			_available_ids.insert(i);
    
	_modified = false;
	_id = *_available_ids.begin();
	_available_ids.erase(_available_ids.begin());
    
	if(!filePath.isEmpty())
	{
		_filePath = filePath;
		std::ifstream f(filePath.toStdString());
		if(f.is_open())
		{
			std::string text;
			std::string line;
			while(std::getline(f, line))
				text += line + "\n";
			f.close();
			setPlainText(text.c_str());
		}
	}
	setFont(QFont("Helvetica Neue", 14));
	connect(this, SIGNAL(textChanged()), this, SLOT(textIsChanged()));
	connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(cursorChanged()));
}

Document::~Document()
{
	_available_ids.insert(_id);
}

void Document::save(bool newfile)
{
	if(_filePath.isEmpty() || newfile)
	{
		QString filePath = QFileDialog::getSaveFileName(this, "Select file to save", "C:/", "Text (*.txt)");
		if(!filePath.isEmpty())
			_filePath = filePath;
	}

	if(_filePath.isEmpty())
		return;
    
	std::ofstream f(_filePath.toStdString());
	if(f.is_open())
	{
		f << toPlainText().toStdString();
		f.close();
		_modified = false;
		int index = Notepad::instance()->tabWidget()->indexOf(this);
		Notepad::instance()->tabWidget()->setTabText(index, QFileInfo(_filePath).fileName());
		Notepad::instance()->updateSaveButton();
	}
}

void Document::textIsChanged()
{
	if(!_modified)
	{
		int index = Notepad::instance()->tabWidget()->indexOf(this);
		QString tabName = Notepad::instance()->tabWidget()->tabText(index);
		Notepad::instance()->tabWidget()->setTabText(index, tabName + "*");
		_modified = true;
		Notepad::instance()->updateSaveButton();
	}
	Notepad::instance()->cursorPositionChanged();
}

void Document::cursorChanged()
{
	Notepad::instance()->cursorPositionChanged();
}

void Document::wheelEvent(QWheelEvent *e)
{
	if(e->modifiers() == Qt::ControlModifier)
	{
		if(e->delta() > 0)
			zoomIn();
		else
			zoomOut();
		return;
	}
	QTextEdit::wheelEvent(e);
}
