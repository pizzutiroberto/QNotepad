#pragma once

#include <QWidget>
#include <QMenuBar>
#include <QAction>
#include <QToolBar>
#include <QTabWidget>
#include <QStatusBar>
#include "Document.h"

class Notepad : public QWidget
{
	Q_OBJECT

	private:
    
        QMenuBar* _menuBar;
        QMenu* _fileMenu;
        QAction* _newAct;
        QAction* _openAct;
        QMenu* _openRecentMenu;
        QAction* _saveAct;
        QAction* _saveAsAct;
        QAction* _saveAllAct;
        QAction* _closeAct;
        QAction* _printAct;
        QAction* _exitAct;
        QMenu* _editMenu;
        QAction* _undoAct;
        QAction* _redoAct;
        QAction* _changeFontAct;
        QAction* _changeColorAct;
        QMenu* _viewMenu;
        QAction* _wordWrapAct;
        QMenu* _aboutMenu;
        QAction* _aboutAct;
        QAction* _aboutQtAct;
    
        QToolBar* _toolBar;
        QTabWidget* _tabWidget;
        QStatusBar* _statusBar;

		static Notepad* _uniqueInstance;
		Notepad(); 

		void setDocumentActionsEnabled(bool enabled);
		void initPersistentElems();

	public:

		static Notepad* instance();
		~Notepad();

		Document* currentDocument() {
			if(_tabWidget->count())
				return dynamic_cast<Document*>(_tabWidget->currentWidget());
			else
				return 0;
		}
    
		QTabWidget* tabWidget() { return _tabWidget; }
		QAction* saveAction() { return _saveAct; }
		QString version() { return "1.0.0"; }

	public slots:

		void aboutClicked();
		void aboutQtClicked();
		void openClicked();
		void saveClicked();
        void saveAllClicked();
		void newClicked();
		void closeClicked();
		void printClicked();
		void undoClicked();
		void redoClicked();
		void changeFontClicked();
		void changeColorClicked();
		void tabCloseButtonClicked(int);
		void cursorPositionChanged();
		void updateSaveButton();
		void updateRecentFiles();
		void wordWrapToggled(bool);
};
