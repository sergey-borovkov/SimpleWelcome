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

#include "userinfoprovider.h"

UserInfoProvider::UserInfoProvider(void)
  : QObject(),
    m_userInfo(0)
{
}

UserInfoProvider::~UserInfoProvider(void)
{
  delete m_userInfo;
  m_userInfo = NULL;
}

void UserInfoProvider::init(void)
{
  m_userInfo = new KUser();
}

QString UserInfoProvider::getFullName(void)
{
  if(m_userInfo == NULL)
    return QString();
  
  return m_userInfo->fullName();
}

QString UserInfoProvider::getLoginName(void)
{
  if(m_userInfo == NULL)
    return QString();
  
  return m_userInfo->loginName();
}

QString UserInfoProvider::getUserName(void)
{
  if(m_userInfo == NULL)
    return QString();
  
  QString username = m_userInfo->fullName();

  if(username.isEmpty())
    username = m_userInfo->loginName();

  return username;
}

QString UserInfoProvider::getIconPath(void)
{
  if(m_userInfo == NULL)
    return QString();

  return m_userInfo->faceIconPath();
}


#include "userinfoprovider.moc"
