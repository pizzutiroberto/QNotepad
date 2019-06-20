#include "Notepad.h"
#include "Document.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <iostream>
#include <string>
#include <QApplication>
#include <QPrinter>
#include <QPrintDialog>
#include <QFontDialog>
#include <QColorDialog>
#include "Settings.h"

Notepad* Notepad::_uniqueInstance = 0;
Notepad* Notepad::instance()
{
	if(_uniqueInstance == 0)
		_uniqueInstance = new Notepad();
	return _uniqueInstance;
}

Notepad::Notepad() : QWidget()
{
	_menuBar = new QMenuBar(this);
	_fileMenu = new QMenu("File");
	_newAct = new QAction(QIcon(":/icons/icons8-new.png"), "New", 0);
	_openAct = new QAction(QIcon(":/icons/icons8-opened_folder.png"), "Open", 0);
	_openRecentMenu = new QMenu("Open Recent...");
	_saveAct = new QAction(QIcon(":/icons/icons8-save_close.png"), "Save", 0);
	_saveAsAct = new QAction(QIcon(":/icons/icons8-save_as.png"), "Save as", 0);
    _saveAllAct = new QAction(QIcon(":/icons/icons8-save_all.png"), "Save all", 0);
	_closeAct = new QAction(QIcon(":/icons/icons8-delete_file.png"), "Close", 0);
	_printAct = new QAction(QIcon(":/icons/icons8-print.png"), "Print", 0);
	_exitAct = new QAction("Exit", 0);
	_editMenu = new QMenu("Edit");
	_undoAct = new QAction(QIcon(":/icons/icons8-undo.png"), "Undo", 0);
	_redoAct = new QAction(QIcon(":/icons/icons8-redo.png"), "Redo", 0);
	_changeFontAct = new QAction(QIcon(":/icons/icons8-choose_font.png"), "Change font", 0);
	_changeColorAct = new QAction(QIcon(":/icons/icons8-text_color.png"), "Change color", 0);
	_viewMenu = new QMenu("View");
	_wordWrapAct = new QAction("Word wrap", 0);
	_wordWrapAct->setCheckable(true);
	_aboutMenu = new QMenu("?");
	_aboutAct = new QAction(QIcon(":/icons/icons8-about.png"), "About Notepad", 0);
	_aboutQtAct = new QAction(QIcon(":/icons/qt.png"), "About Qt", 0);
	_menuBar->addMenu(_fileMenu);
	_fileMenu->addAction(_newAct);
	_fileMenu->addAction(_openAct);
	_fileMenu->addMenu(_openRecentMenu);
	_fileMenu->addAction(_saveAct);
	_fileMenu->addAction(_saveAsAct);
    _fileMenu->addAction(_saveAllAct);
	_fileMenu->addAction(_closeAct);
	_fileMenu->addSeparator();
	_fileMenu->addAction(_printAct);
	_fileMenu->addSeparator();
	_fileMenu->addAction(_exitAct);
	_menuBar->addMenu(_editMenu);
	_editMenu->addAction(_undoAct);
	_editMenu->addAction(_redoAct);
	_editMenu->addAction(_changeFontAct);
	_editMenu->addAction(_changeColorAct);
	_menuBar->addMenu(_viewMenu);
	_viewMenu->addAction(_wordWrapAct);
	_menuBar->addMenu(_aboutMenu);
	_aboutMenu->addAction(_aboutAct);
	_aboutMenu->addAction(_aboutQtAct);

	_toolBar = new QToolBar(this);
	_toolBar->addAction(_newAct);
	_toolBar->addAction(_openAct);
	_toolBar->addAction(_saveAct);
    _toolBar->addAction(_saveAsAct);
    _toolBar->addAction(_saveAllAct);
	_toolBar->addAction(_closeAct);
	_toolBar->addAction(_printAct);
	_toolBar->addSeparator();
	_toolBar->addAction(_undoAct);
	_toolBar->addAction(_redoAct);
    _toolBar->addSeparator();
    _toolBar->addAction(_changeFontAct);
    _toolBar->addAction(_changeColorAct);
	_toolBar->setIconSize(QSize(30,30));

	_tabWidget = new QTabWidget(this);
	_tabWidget->setTabsClosable(true);

	_statusBar = new QStatusBar(this);

	QVBoxLayout* layout = new QVBoxLayout();
	layout->setContentsMargins(0,0,0,0);
	layout->setSpacing(0);
	layout->addWidget(_menuBar);
	layout->addWidget(_toolBar);
	layout->addWidget(_tabWidget);
	layout->addWidget(_statusBar);
	setLayout(layout);

	setMinimumSize(800, 600);
	setWindowTitle(QString("QNotepad v" + version()));

	setDocumentActionsEnabled(false);
	initPersistentElems();

	connect(_aboutAct, SIGNAL(triggered()), this, SLOT(aboutClicked()));
	connect(_aboutQtAct, SIGNAL(triggered()), this, SLOT(aboutQtClicked()));
	connect(_openAct, SIGNAL(triggered()), this, SLOT(openClicked()));
	connect(_saveAct, SIGNAL(triggered()), this, SLOT(saveClicked()));
	connect(_saveAsAct, SIGNAL(triggered()), this, SLOT(saveClicked()));
    connect(_saveAllAct, SIGNAL(triggered()), this, SLOT(saveAllClicked()));
	connect(_newAct, SIGNAL(triggered()), this, SLOT(newClicked()));
	connect(_closeAct, SIGNAL(triggered()), this, SLOT(closeClicked()));
	connect(_printAct, SIGNAL(triggered()), this, SLOT(printClicked()));
	connect(_exitAct, SIGNAL(triggered()), QApplication::instance(), SLOT(quit()));
	connect(_undoAct, SIGNAL(triggered()), this, SLOT(undoClicked()));
	connect(_redoAct, SIGNAL(triggered()), this, SLOT(redoClicked()));
	connect(_changeFontAct, SIGNAL(triggered()), this, SLOT(changeFontClicked()));
	connect(_changeColorAct, SIGNAL(triggered()), this, SLOT(changeColorClicked()));
	connect(_tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(tabCloseButtonClicked(int)));
	connect(_tabWidget, SIGNAL(currentChanged(int)), this, SLOT(updateSaveButton()));
	connect(_tabWidget, SIGNAL(currentChanged(int)), this, SLOT(cursorPositionChanged()));
	connect(_wordWrapAct, SIGNAL(toggled(bool)), this, SLOT(wordWrapToggled(bool)));
}

Notepad::~Notepad() { }

void Notepad::initPersistentElems()
{
	_wordWrapAct->setChecked(Settings::instance()->wordWrap());
	wordWrapToggled(_wordWrapAct->isChecked());
	updateRecentFiles();
}

void Notepad::setDocumentActionsEnabled(bool enabled)
{
	if(!enabled)
		_statusBar->showMessage("Click on New to create a new document");
	_saveAct->setEnabled(enabled);
	_saveAsAct->setEnabled(enabled);
    _saveAllAct->setEnabled(enabled);
	_closeAct->setEnabled(enabled);
	_undoAct->setEnabled(enabled);
	_redoAct->setEnabled(enabled);
	_changeColorAct->setEnabled(enabled);
	_changeFontAct->setEnabled(enabled);
	_printAct->setEnabled(enabled);
}

void Notepad::aboutClicked()
{
	QString text("<html><h1>QNotepad v");
	text += version() + "</h1>"
		"<big>A tool for multi-document editing</big><br><br>"
		"Developed by:<ul>"
		"<li><b>Alessandro Bria<br></b>"
		"Assistant Professor at University of Cassino</li>"
		"<li><b>Roberto Pizzuti</b><br>"
		"Computer Engineering Student at University of Cassino</li></html>";
	QMessageBox::about(this, "About QNotepad", text);
}

void Notepad::aboutQtClicked()
{
	QMessageBox::aboutQt(this, "About QNotepad");
}

void Notepad::newClicked()
{
	setDocumentActionsEnabled(true);
	Document* doc = new Document();
	_tabWidget->addTab(doc, QString("new ") + QString::number(doc->id()));
	_tabWidget->setCurrentWidget(doc);
}

void Notepad::openClicked()
{
	QString filePath;

	if(QObject::sender() == _openAct)
		filePath = QFileDialog::getOpenFileName(this, "Select a file", "C:/", "All (*.*)");
	else
		filePath = dynamic_cast<QAction*>(QObject::sender())->text();
    
	if (filePath.isEmpty())
		return;

	for(int i=0; i<_tabWidget->count(); i++)
		if(dynamic_cast<Document*>(_tabWidget->widget(i))->filePath() == filePath)
		{
			_tabWidget->setCurrentIndex(i);
			return;
		}

	Document* doc = new Document(filePath);
	_tabWidget->addTab(doc, QFileInfo(filePath).fileName());
	_tabWidget->setCurrentWidget(doc);
	setDocumentActionsEnabled(true);
	Settings::instance()->addRecentFile(filePath);
	updateRecentFiles();
}

void Notepad::saveClicked()
{
	if(currentDocument())
		currentDocument()->save(QObject::sender() == _saveAsAct);
}

void Notepad::saveAllClicked()
{
    if(_tabWidget->count() > 0)
    {
        for (int i = 0; i < _tabWidget->count(); i++)
        {
            _tabWidget->setCurrentIndex(i);
            saveClicked();
        }
    }
}

void Notepad::closeClicked()
{
	if(currentDocument())
		tabCloseButtonClicked(_tabWidget->currentIndex());
}

void Notepad::tabCloseButtonClicked(int index)
{
	Document* doc = dynamic_cast<Document*>(_tabWidget->widget(index));

	if(doc->modified())
	{
		QMessageBox::StandardButton choice = QMessageBox::question(this, "Save", "Save file before closing?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
		if(choice == QMessageBox::Yes)
            doc->save();
		else if(choice == QMessageBox::No)
			;
		else if(choice == QMessageBox::Cancel)
			return;
	}
	_tabWidget->removeTab(index);
	delete doc;
    
	if(_tabWidget->count() == 0)
		setDocumentActionsEnabled(false);
}

void Notepad::updateRecentFiles()
{
	for(auto & a : _openRecentMenu->actions())
	{
		_openRecentMenu->removeAction(a);
		delete a;
	}
    
	for(auto & f : Settings::instance()->recentFiles())
	{
		QAction* ithFileAct = new QAction(f, 0);
		_openRecentMenu->addAction(ithFileAct);
		connect(ithFileAct, SIGNAL(triggered()), this, SLOT(openClicked()));
	}
}

void Notepad::updateSaveButton()
{
	if(currentDocument())
		_saveAct->setEnabled(currentDocument()->modified());
}

void Notepad::printClicked()
{
	QPrinter printer;
	QPrintDialog printDialog(&printer, this);
	if (printDialog.exec() == QDialog::Accepted) {
		currentDocument()->print(&printer);
	}
}

void Notepad::undoClicked()
{
	if(currentDocument())
		currentDocument()->undo();
}

void Notepad::redoClicked()
{
	if(currentDocument())
		currentDocument()->redo();
}

void Notepad::cursorPositionChanged()
{
	if(currentDocument())
	{
		int line   = currentDocument()->textCursor().blockNumber();
		int column =  currentDocument()->textCursor().positionInBlock();
		_statusBar->showMessage(QString("Line: " + QString::number(line) + "   Column: " + QString::number(column)));
	}
}

void Notepad::changeFontClicked()
{
	if(!currentDocument())
		return;

	bool ok;
	QFont font = QFontDialog::getFont(&ok, currentDocument()->font(), this);
    
	if(ok)
		currentDocument()->setFont(font);
}

void Notepad::changeColorClicked()
{
	if(!currentDocument())
		return;

    QColor color = QColorDialog::getColor(currentDocument()->textColor(), this);

    if(color.isValid())
        currentDocument()->setTextColor(color);
}

void Notepad::wordWrapToggled(bool checked)
{
	Settings::instance()->setWordWrap(checked);
	for (int i=0; i<_tabWidget->count(); i++)
		dynamic_cast<Document*>(_tabWidget->widget(i))->setWordWrapMode(checked ? QTextOption::WrapAnywhere : QTextOption::NoWrap);
}
