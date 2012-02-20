/*
 * Copyright (c) ROSA Laboratory 2011
 * License: GPLv3
 *
 * Authors:
 * Alexey Yermakov <alexey.yermakov@rosalab.ru>
 *
 * Based on plasma-windowed by:
 * 2006-2008 Aaron Seigo <aseigo@kde.org>
 * 2009 Marco Martin <notmart@gmail.com>
 *
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation; either version 3,
 *   or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "appprovider.h"

#include <KRun>
#include <QApplication>

AppProvider::AppProvider(void) : QObject()
{

}

AppProvider::~AppProvider(void)
{
  // Clearing AppEntities
  for(QHash<QString, AppEntity*>::iterator it = m_appEntities.begin(); it != m_appEntities.end(); it++)
    {
      delete it.value();
    }
}

void AppProvider::init(void)
{
  KServiceGroup::Ptr group = KServiceGroup::group("");

  // Getting root groups
  
  QList<KServiceGroup::Ptr> rootGroups = group->groupEntries();

  for(QList<KServiceGroup::Ptr>::iterator it = rootGroups.begin(); it != rootGroups.end(); it++)
    {
      m_rootGroups.append((*it)->name());
    }
  
  _deepExtract(const_cast<KServiceGroup*>(group.data()));

  //_printDebug();
}

QStringList AppProvider::getEntityNames(void)
{
  QStringList names;

  for(QHash<QString, AppEntity*>::iterator it = m_appEntities.begin(); it != m_appEntities.end(); it++)
    {
      names.append(it.key());
    }

  return names;
}

QStringList AppProvider::getRootGroups(void)
{
  return m_rootGroups;
}

void AppProvider::runEntity(const QString &name)
{
  if(! m_appEntities.contains(name))
    return;

  QString desktopFile = m_appEntities[name]->entryPath();

  KRun *krunner = new KRun(KUrl(desktopFile), QApplication::activeWindow());

  Q_UNUSED(krunner);

}


void AppProvider::_printDebug(void)
{
  for(QHash<QString, AppEntity*>::iterator it = m_appEntities.begin(); it != m_appEntities.end(); it++)
    {
      //kDebug() << it.key();
      kDebug() << it.value();
    }
}

void AppProvider::_deepExtract(KServiceGroup *group)
{
  if(! group || ! group->isValid())
    {
      //kDebug() << "Group is not valid!!!";
      return;
    };
  
  KServiceGroup::List list = group->entries(true, // sorted
                                            true, // exclude no display
                                            false, // exclude separator items
                                            false // don't sort by generic name
                                            );

  for(KServiceGroup::List::ConstIterator it = list.begin(); it != list.end(); it++)
    {

      KSycocaEntry *sycocaEntry = const_cast<KSycocaEntry*>((*it).data());
      
      if(sycocaEntry->isType(KST_KService))
        {
          KService *service = static_cast<KService *>(sycocaEntry);

          AppEntity *entity = new AppEntity(service);

          m_appEntities.insert(service->name(), entity);
        }
      else if(sycocaEntry->isType(KST_KServiceGroup))
        {
          KServiceGroup *group = static_cast<KServiceGroup *>(sycocaEntry);

          AppEntity *entity = new AppEntity(group);

          m_appEntities.insert(group->relPath(), entity);

          // Recursivly 
          _deepExtract(group);
        }
    }

}

#include "appprovider.moc"
