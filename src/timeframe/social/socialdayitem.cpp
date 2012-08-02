#include "socialdayitem.h"
#include "socialitemmodel.h"
#include "socialitem.h"

#include <QDebug>


SocialItemFilterModel::SocialItemFilterModel( QObject * parent )
    : QSortFilterProxyModel( parent )
{
}

//QString SocialItemFilterModel::url( int row )
//{
//    return data( index( row, 0 ), ItemModel::UrlRole).toString();
//}

///////////////////////////////////////////////////////////////////////////////


SocialDayItem::SocialDayItem( const QDate &date, QObject *parent )
    : QObject( parent )
    , m_date( date )
{
    m_itemModel = new SocialItemModel( SocialItem::roleNames(), this );
    m_model = new SocialItemFilterModel( this );
    m_model->setSourceModel( m_itemModel );
    m_model->setDynamicSortFilter( true );
//    m_model->setFilterRole( SocialItem::PluginName );
    //m_model->setFilterFixedString("Video");
    //m_model->setFilterRegExp("Video");
}

SocialDayItem::~SocialDayItem()
{
    qDebug() << "delete item";
}

QString SocialDayItem::id() const
{
    return QString();
}

QVariant SocialDayItem::data( int role ) const
{
    if ( role == DateRole ) {
        return QVariant( m_date );
    }
    else if ( role == ItemsCountRole ) {
        return QVariant( m_model->rowCount( QModelIndex() ) );
    }
    return QVariant();
}


void SocialDayItem::setDate( const QDate &d )
{
    m_date = d;
    emit dataChanged();
}

void SocialDayItem::addSocialItem( SocialItem* item )
{
    if ( !m_types.contains( item->pluginName() ) ) {
        m_types += (item->pluginName() + ";");
    }
    m_itemModel->addSocialItem( item );
    emit dataChanged();
}

QDate SocialDayItem::date()
{
    return m_date;
}

int SocialDayItem::count()
{
    return m_model->rowCount( QModelIndex() );
}

SocialItemFilterModel *SocialDayItem::model()
{
    return m_model;
}

QString SocialDayItem::types() const
{
    return m_types;
}

void SocialDayItem::setSocialFilter( const QRegExp& filter )
{
   m_model->setFilterRegExp( filter );
   emit dataChanged();
}
