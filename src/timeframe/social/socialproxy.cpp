#include "socialproxy.h"
#include "socialitem.h"
#include "socialplugin.h"
#include "socialmodel.h"

#include <QtGui/QWidget>
#include <QtCore/QDebug>

SocialProxy::SocialProxy(QList<ISocialModule *> plugins, QObject *parent) :
    QObject(parent), m_plugins(plugins), m_model(0)
{
    connect(plugins[0], SIGNAL(authorizationStatusChanged(int)), SLOT(authenticated()));
    connect(plugins[0]->requestManager(), SIGNAL(newSocialItems(QList<SocialItem*>)), SLOT(newItems(QList<SocialItem*>)));
//    QWidget *w = plugin->authenticationWidget();
//    w->show();
}

SocialProxy::~SocialProxy()
{

}

void SocialProxy::setModel(SocialModel *model)
{
    m_model = model;
    m_model->setParent(this);
    m_plugins[0]->requestManager()->queryWall(QDate(), QDate());
}

SocialModel *SocialProxy::model() const
{
    return m_model;
}

void SocialProxy::authenticated()
{
    m_plugins[0]->requestManager()->queryWall(QDate(), QDate());
}

void SocialProxy::newItem(SocialItem *item)
{
    qDebug() << "New item added";
    m_model->appendRow(item);
}

void SocialProxy::newItems(QList<SocialItem *> items)
{
    qDebug() << "NewItems" << items.size();
    if(m_model)
        m_model->appendRows(items);
}
