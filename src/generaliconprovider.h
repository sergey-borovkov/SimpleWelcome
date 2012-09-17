#pragma once

#include <QDeclarativeImageProvider>

#include <QtCore/QString>

class UserInfoProvider;
class DataSource_Search;
class QMLConstants;

class GeneralIconProvider : public QDeclarativeImageProvider
{
public:
    GeneralIconProvider(QString path_to_assets, QMLConstants *constants);

    void setUserInfoProvider(UserInfoProvider *userInfoProvider) { m_userInfoProvider = userInfoProvider; }
    void setSearchGridModel(DataSource_Search *searchGridModel) { m_searchGridModel = searchGridModel; }

    QPixmap requestPixmap(const QString &name, QSize *size, const QSize &requestedSize);

private:
    UserInfoProvider *m_userInfoProvider;
    DataSource_Search *m_searchGridModel;
    QMLConstants *constants;
    QString m_pathToAssets;
};
