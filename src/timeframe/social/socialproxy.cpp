#include "socialproxy.h"
#include "socialitem.h"
#include "socialplugin.h"
#include "socialmodel.h"

#include <QtGui/QWidget>
#include <QtCore/QDebug>

SocialProxy::SocialProxy(QList<ISocialModule *> plugins, QObject *parent) :
    QObject(parent), m_plugins(plugins), m_model(0)
{
    if(plugins.size())
    {
        QObject *ob = dynamic_cast<QObject *>(plugins[0]->requestManager());
        if(ob)
            connect(ob, SIGNAL(newSocialItems(QList<SocialItem*>)), SLOT(newItems(QList<SocialItem*>)));

        QObject *p = dynamic_cast<QObject *>(plugins[0]);
        connect(p, SIGNAL(authorized()), SLOT(authenticated()));
        if(!m_plugins[0]->isAuthorized()) {
            QWidget *w = plugins[0]->authenticationWidget();
            w->show();
        }
    }
}

SocialProxy::~SocialProxy()
{
    qDeleteAll(m_plugins);
}

void SocialProxy::setModel(SocialModel *model)
{
    m_model = model;
    m_model->setParent(this);
    if(m_plugins.size())
        m_plugins[0]->requestManager()->queryWall(QDate(), QDate());
}

SocialModel *SocialProxy::model() const
{
    return m_model;
}

void SocialProxy::authenticated()
{
    m_plugins[0]->requestManager()->queryWall(QDate(), QDate());
    if(m_plugins[0]->authenticationWidget())
        m_plugins[0]->authenticationWidget()->hide();
}

void SocialProxy::newItem(SocialItem *item)
{
    newItems(QList<SocialItem *>() << item);
}

void SocialProxy::newItems(QList<SocialItem *> items)
{
    if(m_model)
        m_model->appendRows(items);
}
