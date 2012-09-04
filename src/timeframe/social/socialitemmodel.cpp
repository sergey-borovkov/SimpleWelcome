#include "socialitemmodel.h"

#include <QDate>
#include <QDebug>

#include "socialitem.h"


SocialItemModel::SocialItemModel(QHash<int, QByteArray> roles, QObject *parent)
    : ListModel(roles, parent)
{
}

SocialItemModel::~SocialItemModel()
{
}

/*
 * add social item to model
 */
void SocialItemModel::addSocialItem(SocialItem *item)
{
    if(!item) {
        return;
    }

    /* Check duplicates */
    QString strId = QString("%1-%2").arg(item->pluginName()).arg(item->id());
    if(m_idSet.contains(strId)) {
        return;
    }
    m_idSet.insert(strId);

    appendRow(item);
}
