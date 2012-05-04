/*
 * Copyright (c) ROSA Laboratory 2011
 * License: GPLv3
 *
 * Authors:
 * Alexey Yermakov <alexey.yermakov@rosalab.ru>
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

#ifndef __appentity_h_
#define __appentity_h_

#include <QString>
#include <QList>
#include <QDeclarativeItem>

#include <KDebug>

#include <KService>
#include <KServiceGroup>

class AppEntity : public QDeclarativeItem
{
  Q_OBJECT

    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(bool isApp READ isApp)
    Q_PROPERTY(bool display READ display)
    Q_PROPERTY(QString iconName READ iconName)
    Q_PROPERTY(QString comment READ comment)

    Q_PROPERTY(QString genericName READ genericName)
    Q_PROPERTY(QString menuId READ menuId)
    Q_PROPERTY(QString entryPath READ entryPath)

    Q_PROPERTY(QStringList entries READ entries)
    
 public:
  AppEntity();
  AppEntity(KService *service);
  AppEntity(KServiceGroup *group);
  ~AppEntity();

  QString sourceName() const { return m_sourceName; };
  void setSourceName(QString value) { m_sourceName = value; };
  QString name() const { return m_name; };
  void setName(const QString &value) { m_name = value; };
  bool isApp() const { return m_isApp; };
  void setIsApp(const bool &value) { m_isApp = value; };
  bool display() const { return m_display; };
  void setDisplay(const bool &value) { m_display = value; };
  QString iconName() const { return m_iconName; };
  void setIconName(const QString &value) { m_iconName = value; };
  QString comment() const { return m_comment; };
  void setComment(const QString &value) { m_comment = value; };
  bool valid() const { return m_valid; };

  QString genericName() const { return m_genericName; };
  void setGenericName(const QString &value) { m_genericName = value; };
  QString menuId() const { return m_menuId; };
  void setMenuId(const QString &value) { m_menuId = value; };
  QString entryPath() const { return m_entryPath; };
  void setEntryPath(const QString &value) { m_entryPath = value; };

  QStringList entries() const { return m_entries; };
  
 private:
  // Group and App members
  QString m_sourceName;
  QString m_name;
  bool m_isApp;
  bool m_display;
  QString m_iconName;
  QString m_comment;
  bool m_valid;

  // App only members
  QString m_genericName;
  QString m_menuId;
  QString m_entryPath;

  // Group only members
  QStringList m_entries;
};

QDebug operator<<(QDebug dbg, const AppEntity *entity);

#endif // __appentity_h_
