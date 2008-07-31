/******************************************************************************
 * Copyright (C) 2008 Teo Mrnjavac <teo.mrnjavac@gmail.com>                   *
 *                                                                            *
 * This program is free software; you can redistribute it and/or              *
 * modify it under the terms of the GNU General Public License as             *
 * published by the Free Software Foundation; either version 2 of             *
 * the License, or (at your option) any later version.                        *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.      *
 ******************************************************************************/
#ifndef TOKENLISTWIDGET_H
#define TOKENLISTWIDGET_H

#include "FilenameLayoutWidget.h"

#include <KListWidget>

class TokenListWidget
    : public KListWidget
{
    Q_OBJECT
    
    public:
        TokenListWidget( QWidget *parent = 0 );
    
    protected:
        void mouseDoubleClickEvent( QMouseEvent *event );
        void mousePressEvent( QMouseEvent *event );
        void mouseMoveEvent( QMouseEvent *event );
        void dragEnterEvent( QDragEnterEvent *event );
        void dragMoveEvent( QDragMoveEvent *event );
        void dropEvent( QDropEvent *event );
    
    private:
        void performDrag( QMouseEvent *event );
        QPoint m_startPos;
    signals:
        void onDoubleClick( QString text );
};

#endif    //TOKENLISTWIDGET_H

