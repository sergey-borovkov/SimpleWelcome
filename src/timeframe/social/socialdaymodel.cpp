#include "socialdaymodel.h"
#include "socialdayitem.h"
#include "socialitem.h"
#include "socialitemmodel.h"

#include <QVariant>
#include <QRegExp>

SocialDayFilterModel::SocialDayFilterModel( QObject * parent )
    : QSortFilterProxyModel( parent )
{

}

QObject* SocialDayFilterModel::itemsModel(QDate date) const
{
    SocialDayModel* model = qobject_cast<SocialDayModel*>(sourceModel());
    if (model)
        return model->itemsModel(date);
    return 0;
}

//QString SocialDayFilterModel::url( int row )
//{
//    return data( index( row, 0 ), ItemModel::UrlRole).toString();
//}

///////////////////////////////////////////////////////////////////////////////

SocialDayModel::SocialDayModel( QHash<int, QByteArray> roles, QObject *parent )
    : ListModel( roles, parent )
{
}

SocialDayModel::~SocialDayModel()
{
    clear();
}

QVariant SocialDayModel::data( const QModelIndex &index, int role ) const
{
    if ( !index.isValid() ) {
        return QVariant();
    }

//    if ( role == SocialDayItem::CountRole ) {
//        return m_items.size();
//    }
//    else if(role == ItemsRole)
//    {
//        //QVariant v;
//        //v.setValue( m_items[index.row()]->model());
//        return 0;//m_items.value(index.row());
//    }
//    else if(role == CountRole)
//    {
//        return m_items.size();
//    }
//    else if(role == ItemsCountRole)
//    {
//        return m_items[index.row()]->getCount();
//    }
    return QVariant();
}

void SocialDayModel::appendRows(const QList<SocialDayItem *> &items)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount()+items.size()-1);
    foreach(SocialDayItem *item, items) {
        connect(item, SIGNAL(dataChanged()), SLOT(handleItemChange()));
        m_items.append(item);
    }
    endInsertRows();
}

QObject* SocialDayModel::itemsModel( QDate date ) const
{
    for (int i = 0; i < m_items.size(); i++)
    {
        if ( date == m_items.value(i)->date() )
            return m_items.value(i)->model();
    }
    return 0;
}

void SocialDayModel::handleItemChange()
{
    SocialDayItem* item = static_cast<SocialDayItem*>(sender());
    QModelIndex index = indexFromItem(item);
    if(index.isValid())
        emit dataChanged(index, index);
}

void SocialDayModel::newSocialItems(QList < SocialItem * > list)
{
    // qDebug() << "------new Activities--------";
    for (int i = 0; i < list.size() ; i++)
    {

        SocialItem* item = list.at(i);
        QString uniqueId = item->pluginName() + item->id();
        if (m_idSet.contains(uniqueId))
            continue;
        m_idSet.insert(uniqueId);

        int j = 0;
        bool flag = false;
        if (m_items.size() > 0)
        {
            while (m_items.at(j)->date() <= item->date())
            {
                if ( m_items.at(j)->date() == item->date())
                {
                    m_items.at(j)->addSocialItem(item);
                    flag = true;
                    break;
                }
                j++;
                if ( j == m_items.size() )
                {
                    break;
                }
            }
        }

        if (flag)
        {
            continue;
        }

        //qDebug() << "new gallery item" << item->getDate();
        //if (removeNullItem(item->getDate().year(), item->getDate().month()))
          //  j--;

        SocialDayItem * socialDayItem = new SocialDayItem(item->date());
        socialDayItem->addSocialItem(item);
        insertRow(j,socialDayItem);

        //insertRow(j+1,gallItem);
        //qDebug() <<" add new activity" << item->getDate();
    }    
}

//int SocialDayModel::getIndexByDate(int year, int month,  bool direction)
//{
//    //QDate date(year, month, day);
//    //return indexFromItem(find(date)).row();

//    GalleryItem* ptr=0;

////    for (int i = m_items.size() -1; i >= 0; i--)
////    {
////        if((m_items.at(i)->getDate().year() == year) && (m_items.at(i)->getDate().month() == month))
////        {
////            ptr = m_items.at(i);
////            break;
////        }
////    }

//    foreach(GalleryItem* item, m_items)
//    {
//        if((item->getDate().year() == year) && (item->getDate().month() == month))
//        {
//            ptr = item;
//            break;
//        }
//    }
//    if (ptr)
//        return indexFromItem(ptr).row();
//    return -1;
//}


//QDate SocialDayModel::getDateOfIndex(int listIndex)
//{
//    if (listIndex >= m_items.size() || listIndex < 0)
//        return QDate();
//    GalleryItem* item = m_items.at(listIndex);
//    QDate date = item->getDate();
//    return date;
//}

//void SocialDayModel::imageReady(QString url)
//{
//    if (m_urlHash.contains(url))
//    {
//        QDate date = m_urlHash[url];
//        GalleryItem* item = find(date);
//        if (item)
//            item->thumbnailReady(url);
//    }
//}

//void SocialDayModel::setActivityType(const QString& type)
//{
//    qDebug() << "123" <<type;
//    QRegExp filter;
//    if (type == "All")
//        filter = QRegExp("Image|Video");
//    else if (type == "Photo")
//        filter = QRegExp("Image");
//    else if (type == "Video")
//        filter = QRegExp("Video");
//    foreach(GalleryItem* item, m_items)
//    {
//        item->setActivityFilter(filter);
//    }
//}
