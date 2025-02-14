/***************************************************************************
								  preferences  -  Preferences dialog
									  -------------------
	 begin                : Thu 18 Aug 2005
	 copyright            : (C) 2005 Michal Rudolf <mrudolf@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PREFERENCESDIALOG_H_INCLUDED
#define PREFERENCESDIALOG_H_INCLUDED

#include "ui_preferences.h"
#include "enginelist.h"

class QListWidgetItem;
class DownloadManager;

/** @ingroup GUI
The PreferencesDialog class is a dialog for configuring ChessX.
After changes are done, reconfigure() signal is emitted. */
class PreferencesDialog : public QDialog
{
    Q_OBJECT
public:
    /** Create ChessX Preferences dialog */
    PreferencesDialog(QWidget* parent = nullptr, Qt::WindowFlags f = nullptr);
    /** Destroy the dialog */
    virtual ~PreferencesDialog();

     static void setupIconInMenus(QObject* pObject);

public slots:
    void setAnchor(QString anchor);
    /** Restore Default values */
    void slotReset();
    /** Execute dialog, save options on exit */
    int exec();
    void slotApply();
    /** select given engine # for display */
    void slotSelectEngine(QListWidgetItem* current, QListWidgetItem* previous);
    /** user asked for a new engine entry */
    void slotAddEngine();
    /** user asked to remove current engine entry */
    void slotDeleteEngine();
    /** user changed the name of an engine */
    void slotEngineUp();
    /** User asked to move current engine up in the list. */
    void slotEngineDown();
    /** User asked to move current engine down in the list. */
    void slotEngineNameChange(const QString&);
    /** user wants to use file dialog to select engine command */
    void slotSelectEngineCommand();
    /** user wants file dialog to select directory in which engine will run */
    void slotSelectEngineDirectory();
    /** user wants file dialog to select path of an external exe */
    void slotSelectToolPath();
    /** user wants file dialog to select directory in which DataBases will be stored */
    void slotSelectDataBasePath();
    /** user wants option dialog to select parameters which will be sent at startup of engine */
    void slotShowOptionDialog();
    /** User pressed a flag to change the piece string */
    void slotChangePieceString();
    /** Start loading the language file selected in the combo-box */
    void slotLoadLanguageFile();
    /** Download dictionary was loaded */
    void slotFileLoaded(QUrl url, QString name);
    /** Download was not possible */
    void loadFileError(QUrl);


protected:
    /** Overridden to save size */
    virtual void done(int);

private:
    DownloadManager* downloadManager;
    Ui::PreferencesDialog ui;
    void restoreSettings();
    void saveSettings();
    /** Select given item in combo. If not found, select last one. */
    bool selectInCombo(QComboBox* combo, const QString& text);
    void restoreColorItem(ColorList* list, const QString& text, const QString& cfgname);
    void saveColorList(ColorList* list, const QStringList& cfgnames);

    EngineList engineList;
    /** Store any changes made to Engine fields in UI, into list */
    void updateEngineData(int index);
    /** Select engine file. */
    QString selectEngineFile(const QString& path = "");
    static int s_lastIndex;

signals:
    /** Signal emitted when changes are applied. */
    void reconfigure();
    /** Signal emitted when the slider for Icon resizing changes **/
    /** int is new value **/
    void iconsizeSliderSetting(int);
      
protected slots:
    /** Call restoration of layout from Settings storage */
    void restoreLayout();
    /** slot to emit signal on resizing icon slider change **/
    void sliderNewValue(int newValue);

    void buttonClicked(QAbstractButton *button);
private slots:
    void slotBtMoveFontClicked();
    void slotBtTextFontClicked();
    void SlotPieceEffectActivated(int index);
    void on_savePreferences_clicked();
    void on_toolSearchWebDestination_clicked();
};

#endif

