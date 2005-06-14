/***************************************************************************
 * copyright            : (C) 2005 Seb Ruiz <me@sebruiz.net>               *
 **************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QUEUEMANAGER_H
#define QUEUEMANAGER_H


#include <playlist.h>
#include <playlistitem.h>

#include <kapplication.h>
#include <kdialogbase.h>    //baseclass
#include <klistview.h>

#include <qmap.h>
#include <qpainter.h>
#include <qptrlist.h>
#include <qsimplerichtext.h>
#include <qvbox.h>


class QueueList : public KListView
{
    public:
        QueueList( QWidget *parent, const char *name = 0 )
            : KListView( parent, name ) {};
        ~QueueList() {};

        void viewportPaintEvent( QPaintEvent *e )
        {
            if( e ) KListView::viewportPaintEvent( e );

            if( !childCount() && e )
            {
                QPainter p( viewport() );
                QString minimumText(i18n(
                        "<div align=center>"
                        "<h3>The Queue Manager</h3>"
                            "<br>To create a queue, "
                            "<b>drag</b> tracks from the playlist window, "
                            "<b>drop</b> them here to queue them.<br>"
                            "Drag and drop tracks within the manager to resort queue orders."
                        "</div>" ) );
                QSimpleRichText *t = new QSimpleRichText( minimumText, QApplication::font() );

                if ( t->width()+30 >= viewport()->width() || t->height()+30 >= viewport()->height() ) {
                    // too big for the window, so let's cut part of the text
                    delete t;
                    t = new QSimpleRichText( minimumText, QApplication::font());
                    if ( t->width()+30 >= viewport()->width() || t->height()+30 >= viewport()->height() ) {
                        //still too big, giving up
                        return;
                    }
                }

                const uint w = t->width();
                const uint h = t->height();
                const uint x = (viewport()->width() - w - 30) / 2 ;
                const uint y = (viewport()->height() - h - 30) / 2 ;

                p.setBrush( colorGroup().background() );
                p.drawRoundRect( x, y, w+30, h+30, (8*200)/w, (8*200)/h );
                t->draw( &p, x+15, y+15, QRect(), colorGroup() );
                delete t;
            }
        }
};

//////////////////////////////////////////////////////////////////////////////////////////
/// CLASS QueueManager
//////////////////////////////////////////////////////////////////////////////////////////

class QueueManager : public KDialogBase
{
    public:
        QueueManager( QWidget *parent = 0, const char *name = 0 );
        ~QueueManager() {};

        QPtrList<PlaylistItem> newQueue();

    private:
        void  insertItems();

        QMap<QListViewItem*, PlaylistItem*> m_map;
        QueueList *m_listview;
};


QueueManager::QueueManager( QWidget *parent, const char *name )
                    : KDialogBase( parent, name, false, i18n("Queue Manager"), Ok|Cancel )
{
    makeVBoxMainWidget();

    QHBox *box = new QHBox( mainWidget() );
    box->setSpacing( 5 );
    m_listview = new QueueList( box );
    m_listview->addColumn( i18n("Name") );
    m_listview->setResizeMode( QListView::LastColumn );
    m_listview->setSelectionMode( QListView::Extended );
    m_listview->setSorting( -1 );

    insertItems();
}

QPtrList<PlaylistItem>
QueueManager::newQueue()
{
    QPtrList<PlaylistItem> queue;
    for( QListViewItem *key = m_listview->firstChild(); key; key = key->nextSibling() )
    {
        queue.append( m_map[ key ] );
    }
    return queue;
}

void
QueueManager::insertItems()
{
    QPtrList<PlaylistItem> list = Playlist::instance()->m_nextTracks;
    QListViewItem *last = 0;

    for( PlaylistItem *item = list.first(); item; item = list.next() )
    {
        QString title = item->title();
        title.append( i18n(" - " ) );
        title.append( item->artist() );

        last = new QListViewItem( m_listview, last, title );
        last->setDragEnabled( true );
        last->setDropEnabled( true );
        m_map[ last ] = item;
    }
}

#endif
