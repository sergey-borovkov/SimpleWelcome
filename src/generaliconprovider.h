#pragma once

#include <QDeclarativeImageProvider>

#include <QtCore/QString>

class UserInfoProvider;
class DataSource_Search;
class DataSource_Documents;
class SizesCalculator;

class GeneralIconProvider : public QDeclarativeImageProvider
{
public:
    GeneralIconProvider(QString path_to_assets, SizesCalculator *constants);

    void setUserInfoProvider(UserInfoProvider *userInfoProvider) {
        m_userInfoProvider = userInfoProvider;
    }
    void setSearchDataSource(DataSource_Search *searchDataSource) {
        m_searchDataSource = searchDataSource;
    }
    void setDocumentsDataSource(DataSource_Documents *documentsDataSource) {
        m_documentsDataSource = documentsDataSource;
    }

    QPixmap requestPixmap(const QString &name, QSize *size, const QSize &requestedSize);

private:
    UserInfoProvider *m_userInfoProvider;
    DataSource_Search *m_searchDataSource;
    DataSource_Documents *m_documentsDataSource;
    SizesCalculator *constants;
    QString m_pathToAssets;
};
