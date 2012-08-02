#include "feeditem.h"

#include <QtCore/QDebug>
#include <QtCore/QDateTime>
#include <QtCore/QUrl>

#include <qjson/parser.h>

QString FeedItem::pluginName() const
{
    return QLatin1String( "VKontakte" );
}

FeedItem::Type FeedItem::type() const
{
    return Post;
}

QString FeedItem::id() const
{
    return m_id;
}

QVariant FeedItem::data( int role ) const
{
    return m_data.value( role );
}

QDate FeedItem::date() const
{
    //qDebug() << "123" <<  QDate::fromString( data( Date ).toString(), QString("d MM yyyy") );
    return QDate::fromString( data( Date ).toString(), QString("d MM yyyy") );
}

void FeedItem::fillFromMap( QVariantMap map )
{
    // http://developers.facebook.com/docs/reference/api/post/
    if ( map.contains( "id" ) ) {
        m_id = map.value("id").toString();
    }
    if ( map.contains( "text") ) {
        QString message = map.value( "text" ).toString();

        m_data.insert( Text, message );
    }

    if ( map.contains( "date" ) ) {
        uint t  = map.value( "date" ).toUInt();
        QDateTime dt;
        dt.setTime_t( t );
        QDate date = dt.date();
        m_data.insert( Date, date.toString( "d MM yyyy" ) );
    }

    if ( map.contains( "attachments" ) ) {
        QVariantList attachmentList = map[ "attachments" ].toList();
        //qDebug() << "FeedItem::fillFromMap:   it is attachmentss list!!!";

        foreach ( QVariant item, attachmentList ) {
            QVariantMap map = item.toMap();

            if ( map.contains( "type" ) ) {
                //                    qDebug() << "***********       attachments type = " << map.value( "type" ).toString();
                QString typeAttachment = map.value( "type" ).toString();

                if ( typeAttachment == "photo" ) {
                    QVariantMap photoMap = map[ "photo" ].toMap();

                    if ( photoMap.contains( "src" ) ) {
                        m_data.insert( ImageUrl, photoMap.value( "src" ).toString() );
                        //qDebug() <<  "FeedItem::fillFromMap:   " << map.value( "type" ).toString() << " - " << photoMap.value( "src" ).toString();
                    }
                }

                if ( typeAttachment == "audio" ) {
                    QVariantMap audioMap = map[ "audio" ].toMap();

                    if ( audioMap.contains( "title" ) ) {
                        m_data.insert( Audio, audioMap.value( "performer" ).toString() + " - " + audioMap.value( "title" ).toString());
                        //qDebug() <<  "FeedItem::fillFromMap:   " << map.value( "type" ).toString() << " - " << audioMap.value( "performer" ).toString() + " - " + audioMap.value( "title" ).toString();
                    }
                }
            }
        }
    }


    if ( map.contains( "likes" ) ) {
        QVariantMap likesMap = map[ "likes" ].toMap();
        if ( likesMap.contains( "count" ) ) {
            m_data.insert( Likes, likesMap.value( "count" ).toString() );
        }
    }

    if ( map.contains( "comments" ) ) {
        QVariantMap commentsMap = map[ "comments" ].toMap();
        if ( commentsMap.contains( "count" ) ) {
            m_data.insert( Comments, commentsMap.value( "count" ).toString() );
        }
    }

    m_data.insert( PluginName, pluginName() );
}
