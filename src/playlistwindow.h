/***************************************************************************
  begin                : Fre Nov 15 2002
  copyright            : (C) Mark Kretschmann <markey@web.de>
                       : (C) Max Howell <max.howell@methylblue.com>
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef AMAROK_PLAYLISTWINDOW_H
#define AMAROK_PLAYLISTWINDOW_H

#include <enginecontroller.h>  //baseclass

#include <qwidget.h>           //baseclass
#include <ktoolbar.h>          //baseclass
#include <kxmlguiclient.h>     //baseclass (for XMLGUI)


namespace amaroK
{
    class ToolBar;
}

using amaroK::ToolBar;

class BrowserBar;
class ContextBrowser;
class CollectionBrowser;
class KLineEdit;
class KStatusBar;
class Playlist;


class PlaylistWindow : public QWidget, public KXMLGUIClient, public EngineObserver
{
        Q_OBJECT

    public:
        PlaylistWindow();
        ~PlaylistWindow();

        void init();

        void setFont( const QFont&, const QFont& );
        void setColors( const QPalette&, const QColor& );

        void createGUI(); //should be private but App::slowConfigToolbars requires it
        void recreateGUI();

        KStatusBar *statusBar() const { return m_statusbar; }

        virtual bool eventFilter( QObject*, QEvent* );

    public slots:
        void showHide();

    private slots:
        void savePlaylist() const;
        void slotAddLocation();

    protected:
        virtual void closeEvent( QCloseEvent* );
        void engineStateChanged( Engine::State );

    private:
        template <class B> void addBrowser( const char*, const QString&, const QString& );

        BrowserBar *m_browsers;
        Playlist   *m_playlist;
        KLineEdit  *m_lineEdit;
        KStatusBar *m_statusbar;
        ToolBar    *m_toolbar;
        int m_lastBrowser;
};


#endif //AMAROK_PLAYLISTWINDOW_H
