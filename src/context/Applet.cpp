/****************************************************************************************
 * Copyright (c) 2007 Leo Franchi <lfranchi@gmail.com>                                  *
 * Copyright (c) 2009 Riccardo Iaconelli <riccardo@kde.org>                             *
 *                                                                                      *
 * This program is free software; you can redistribute it and/or modify it under        *
 * the terms of the GNU General Public License as published by the Free Software        *
 * Foundation; either version 2 of the License, or (at your option) any later           *
 * version.                                                                             *
 *                                                                                      *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY      *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
 * PARTICULAR PURPOSE. See the GNU General Pulic License for more details.              *
 *                                                                                      *
 * You should have received a copy of the GNU General Public License along with         *
 * this program.  If not, see <http://www.gnu.org/licenses/>.                           *
 ****************************************************************************************/

#include "Applet.h"
#include "Containment.h"
#include "Debug.h"

#include "PaletteHandler.h"
#include <Plasma/Animator>

#include <QGraphicsLayout>
#include <QGraphicsScene>
#include <QFontMetrics>
#include <QPainter>
#include "ContextView.h"

namespace Context
{

} // Context namespace

Context::Applet::Applet( QObject * parent, const QVariantList& args )
    : Plasma::Applet( parent, args )
    , m_collapsed( false )
    , m_animationId( 0 )
    , m_transient( 0 )
    , m_standardPadding( 6.0 )
{
    connect ( Plasma::Animator::self(), SIGNAL(customAnimationFinished ( int ) ), this, SLOT( animateEnd( int ) ) );
    setBackgroundHints(NoBackground);
}

Context::Applet::~Applet( )
{
    if ( m_animationId != 0)
        Plasma::Animator::self()->stopCustomAnimation( m_animationId );
}


QFont
Context::Applet::shrinkTextSizeToFit( const QString& text, const QRectF& bounds )
{
    Q_UNUSED( text );

    int size = 13; // start here, shrink if needed
    QFont font( QString(), size, QFont::Light );
    font.setStyleHint( QFont::SansSerif );
    font.setStyleStrategy( QFont::PreferAntialias );
    
    QFontMetrics fm( font );
    while( fm.height() > bounds.height() + 4 )
    {
        if( size < 0 )
        {
            size = 5;
            break;
        }
        size--;
        fm = QFontMetrics( QFont( QString(), size ) );
    }
    
    // for aesthetics, we make it one smaller
    size--;

    QFont returnFont( QString(), size, QFont::Light );
    font.setStyleHint( QFont::SansSerif );
    font.setStyleStrategy( QFont::PreferAntialias );
    
    return QFont( returnFont );
}

QString
Context::Applet::truncateTextToFit( QString text, const QFont& font, const QRectF& bounds )
{
    QFontMetrics fm( font );
    return fm.elidedText ( text, Qt::ElideRight, (int)bounds.width() );
}

void
Context::Applet::drawRoundedRectAroundText( QPainter* p, QGraphicsSimpleTextItem* t )
{
    p->save();
    p->setRenderHint( QPainter::Antialiasing );

    // Paint in integer coordinates, align to grid
    QRectF rect = t->boundingRect();
    QPointF pos = t->pos();
    rect.setX( qRound( rect.x() ) );
    rect.setY( qRound( rect.y() ) );
    rect.setHeight( qRound( rect.height() ) );
    rect.setWidth( qRound( rect.width() ) );
    rect.moveTopLeft( t->pos() );
    pos.setX( qRound( pos.x() ) );
    pos.setY( qRound( pos.y() ) );
    rect.moveTopLeft( pos );
    rect.adjust( -5, -2, 5, 2 );

    p->translate( 0.5, 0.5 );

    QPainterPath path;
    path.addRoundedRect( rect, 3, 3 );
    QColor col = PaletteHandler::highlightColor().lighter( 150 );
    col.setAlphaF( col.alphaF() * 0.7 );
    p->fillPath( path, col );

    col = PaletteHandler::highlightColor( 0.3, 0.5 );
    col.setAlphaF( col.alphaF() * 0.7 );
    p->setPen( col );
    p->drawRoundedRect( rect, 3, 3 );
    p->restore();
}

void
Context::Applet::addGradientToAppletBackground( QPainter* p )
{
        // tint the whole applet
    // draw non-gradient backround. going for elegance and style
    p->save();
    QPainterPath path;
    path.addRoundedRect( boundingRect().adjusted( 1, 1, -2, -2 ), 6, 6 );
    //p->fillPath( path, gradient );
    QColor highlight = PaletteHandler::highlightColor( 0.4, 1.05 );
    highlight.setAlphaF( highlight.alphaF() * 0.5 );
    p->fillPath( path, highlight );
    p->restore();

    p->save();
    p->translate( 0.5, 0.5 );
    QColor col = PaletteHandler::highlightColor( 0.3, 0.5 );
    col.setAlphaF( col.alphaF() * 0.7 );
    p->setPen( col );
    p->drawRoundedRect( boundingRect().adjusted( 1, 1, -2, -2 ), 6, 6 );
    p->restore();
}

qreal
Context::Applet::standardPadding()
{
    return  m_standardPadding;
}

void
Context::Applet::destroy()
{
    if ( Plasma::Applet::immutability() != Plasma::Mutable || m_transient ) {
        return; //don't double delete
    }
    m_transient = true;
    cleanUpAndDelete();
}

void
Context::Applet::cleanUpAndDelete()
{
    QGraphicsWidget *parent = dynamic_cast<QGraphicsWidget *>( parentItem() );
    //it probably won't matter, but right now if there are applethandles, *they* are the parent.
    //not the containment.

    //is the applet in a containment and is the containment have a layout? if yes, we remove the applet in the layout
    if ( parent && parent->layout() )
    {
        QGraphicsLayout *l = parent->layout();
        for ( int i = 0; i < l->count(); ++i )
        {
            if ( this == l->itemAt( i ) )
            {
                l->removeAt( i );
                break;
            }
        }
    }

    if ( Plasma::Applet::configScheme() ) {
        Plasma::Applet::configScheme()->setDefaults();
    }
    Plasma::Applet::scene()->removeItem( this );
    Plasma::Applet::deleteLater();
}

QSizeF
Context::Applet::sizeHint( Qt::SizeHint which, const QSizeF & constraint ) const
{
    return QSizeF( QGraphicsWidget::sizeHint( which, constraint ).width(), m_heightCurrent );
}


void
Context::Applet::resize( qreal wid, qreal hei)
{
    m_heightCollapseOff = hei;
    m_heightCurrent = hei;
    QGraphicsWidget::resize( wid, hei );
}

void
Context::Applet::setCollapseOn()
{
//    DEBUG_BLOCK
    if ( size().height() == m_heightCollapseOn )
        return;

    if ( m_animationId != 0 ) // warning we are moving right now
    {
        // stop the anim
        Plasma::Animator::self()->stopCustomAnimation( m_animationId );
        m_animationId = 0;
    }
    m_collapsed = false;
    m_animationId = Plasma::Animator::self()->customAnimation(20, 1000, Plasma::Animator::EaseInCurve, this, "animateOn" );
}

void
Context::Applet::setCollapseOff()
{
//    DEBUG_BLOCK
    if ( size().height() == m_heightCollapseOff )
        return;
    
    if ( m_animationId != 0 ) // warning we are moving right now
    {
        // stop the anim
        Plasma::Animator::self()->stopCustomAnimation( m_animationId );
        m_animationId = 0;        
    }
    m_collapsed = true ;
    m_animationId = Plasma::Animator::self()->customAnimation(20, 1000, Plasma::Animator::EaseInCurve, this, "animateOff" );
}

void
Context::Applet::setCollapseHeight( int h )
{
    m_heightCollapseOn = h;
}

void
Context::Applet::animateOn( qreal anim )
{
//    DEBUG_BLOCK
    m_heightCurrent = m_heightCollapseOff - ( m_heightCollapseOff - m_heightCollapseOn ) * anim ;
//    debug()<< "animate " << m_heightCurrent << " m_heightCollapseOff :" << m_heightCollapseOff << " | m_heightCollapseOn :" << m_heightCollapseOn ;
    emit sizeHintChanged(Qt::PreferredSize);
}

void
Context::Applet::animateOff( qreal anim )
{
//    DEBUG_BLOCK
    m_heightCurrent =  m_heightCollapseOn + ( m_heightCollapseOff - m_heightCollapseOn ) * anim ;
//    debug()<< "animate " << m_heightCurrent << " m_heightCollapseOff :" << m_heightCollapseOff << " | m_heightCollapseOn :" << m_heightCollapseOn ;
    emit sizeHintChanged(Qt::PreferredSize);
}

void
Context::Applet::animateEnd( int id )
{
    //DEBUG_BLOCK
    if( id == m_animationId )
    {
        if( !m_collapsed )
        {
            Plasma::Applet::resize( size().width(), m_heightCollapseOn );
            m_collapsed = true;
        }
        else
        {
            Plasma::Applet::resize( size().width(), m_heightCollapseOff );
            m_collapsed = false;
        }
        updateGeometry();
        emit sizeHintChanged(Qt::PreferredSize);
        m_animationId = 0;
    }
}



#include "Applet.moc"
