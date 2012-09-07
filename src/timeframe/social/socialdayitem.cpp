#include "socialdayitem.h"
#include "socialitemmodel.h"
#include "socialitem.h"
#include "../listmodel.h"

SocialItemFilterModel::SocialItemFilterModel(QObject * parent)
    : QSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);
    setFilterRole(SocialItem::PluginName);
}

QString SocialItemFilterModel::imageUrl(int row)
{
    return data(index(row, 0), SocialItem::ImageUrl).toString();
}

QString SocialItemFilterModel::text(int row)
{
    return data(index(row, 0), SocialItem::Text).toString();
}

int SocialItemFilterModel::likesCount(int row)
{
    return data(index(row, 0), SocialItem::Likes).toInt();
}

void SocialItemFilterModel::like(int row)
{
    int likes = likesCount(row);
    setData(index(row, 0), likes + 1, SocialItem::Likes);
}

int SocialItemFilterModel::commentsCount(int row)
{
    return data(index(row, 0), SocialItem::CommentCount).toInt();
}

QObject *SocialItemFilterModel::comments(int row)
{
    QVariant v = data(index(row, 0), SocialItem::Comments);
    QList<CommentItem *> comments = qvariant_cast< QList<CommentItem *> >(v);

    QList<ListItem *> t;    
    foreach(CommentItem *item, comments)
        t.append(item);    

    ListModel *model = new ListModel(CommentItem::roleNames(), this);
    model->appendRows(t);
    return model;
}

QString SocialItemFilterModel::pluginName(int row)
{
    return data(index(row, 0), SocialItem::PluginName).toString();
}

SocialDayItem::SocialDayItem(const QDate &date, QObject *parent)
    : QObject(parent)
    , m_date(date)
{
    m_itemModel = new SocialItemModel(SocialItem::roleNames(), this);
    m_model = new SocialItemFilterModel(this);
    m_model->setSourceModel(m_itemModel);
    m_model->setDynamicSortFilter(true);
}

SocialDayItem::~SocialDayItem()
{
}

QString SocialDayItem::id() const
{
    return QString();
}

QVariant SocialDayItem::data(int role) const
{
    if(role == DateRole) {
        return QVariant(m_date);
    } else if(role == ItemsCountRole) {
        return QVariant(m_model->rowCount(QModelIndex()));
    }
    return QVariant();
}


void SocialDayItem::setDate(const QDate &d)
{
    m_date = d;
    emit dataChanged();
}

void SocialDayItem::addSocialItem(SocialItem* item)
{
    if(!m_types.contains(item->pluginName())) {
        m_types += (item->pluginName() + ";");
    }
    m_itemModel->addSocialItem(item);
    emit dataChanged();
}

QDate SocialDayItem::date()
{
    return m_date;
}

int SocialDayItem::count()
{
    return m_model->rowCount(QModelIndex());
}

SocialItemFilterModel *SocialDayItem::model()
{
    return m_model;
}

QString SocialDayItem::types() const
{
    return m_types;
}

void SocialDayItem::setSocialFilter(const QRegExp& filter)
{
    m_model->setFilterRegExp(filter);
    emit dataChanged();
}
