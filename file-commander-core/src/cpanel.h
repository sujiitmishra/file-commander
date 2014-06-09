#ifndef CPANEL_H
#define CPANEL_H

#include <vector>
#include <map>
#include <memory>

#include "QtCoreIncludes"

#include "cfilesystemobject.h"
#include "historylist/chistorylist.h"

enum Panel {LeftPanel, RightPanel, UnknownPanel};

struct PanelContentsChangedListener
{
	virtual void panelContentsChanged(Panel p) = 0;
};

class QFileSystemWatcher;

class CPanel : public QObject
{
	Q_OBJECT

public:
	void addPanelContentsChangedListener(PanelContentsChangedListener * listener);

	CPanel(Panel position);
	// Sets the current directory
	FileOperationResultCode setPath (const QString& path);
	// Navigates up the directory tree
	void navigateUp ();
	// Go to the previous location from history
	bool navigateBack();
	// Go to the next location from history, if any
	bool navigateForward();
	const CHistoryList<QString>& history() const;

	// Info on the dir this panel is currently set to
	QString currentDirPath () const;
	QString currentDirName () const;

	void setCurrentItemInFolder(const QString& dir, qulonglong currentItemHash);
	// Returns hash of an item that was the last selected in the specified dir
	qulonglong currentItemInFolder(const QString& dir) const;

	// Enumerates objects in the current directory
	void refreshFileList ();
	// Returns the current list of objects on this panel
	const std::vector<CFileSystemObject>& list () const;

	// Access to the corresponding item
	const CFileSystemObject& itemByIndex (size_t index) const;
	CFileSystemObject& itemByIndex (size_t index);

	bool itemHashExists(const qulonglong hash) const;
	const CFileSystemObject& itemByHash (qulonglong hash) const;
	CFileSystemObject& itemByHash (qulonglong hash);

	// Calculates directory size, stores it in the corresponding CFileSystemObject and sends data change notification
	void calculateDirSize(qulonglong dirHash);

	void sendContentsChangedNotification() const;

	// Settings have changed
	void settingsChanged();

private slots:
	void contentsChanged (QString path);

private:
	QDir                                       _currentDir;
	std::vector<CFileSystemObject>             _list;
	CHistoryList<QString>                      _history;
	std::map<QString, qulonglong /*hash*/>     _cursorPosForFolder;
	std::map<qulonglong, size_t>               _indexByHash;
	std::shared_ptr<QFileSystemWatcher>        _watcher;
	std::vector<PanelContentsChangedListener*> _panelContentsChangedListeners;
	const Panel                                _panelPosition;
};

#endif // CPANEL_H
