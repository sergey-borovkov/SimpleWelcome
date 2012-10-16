#pragma once

#include "appitem.h"
#include "datasource.h"

#include <KDE/KFileItemList>

class QMLConstants;

class DataSource_Documents : public DataSource
{
    Q_OBJECT
    Q_PROPERTY(QString qmlGroupName READ getQmlGroupName WRITE setQmlGroupName)

public:
    explicit DataSource_Documents(QObject* parent, QMLConstants *inConstants);
    Q_INVOKABLE virtual int getItemCount();
    Q_INVOKABLE QString itemUrlDnd(int id);
    QIcon getIcon(QString destination);

signals:
    void newItemData(QVariantMap itemData, QString group);
    void resetContent();
    void runDesktopFile(QString desktopFile);
    void updateItemData(int id, QString field, QString data);

public slots:
    virtual void itemClicked(int newIndex);
    virtual void getContent();
    void updateContent();

private slots:
    void resultPreviewJob(const KFileItem& item, const QPixmap& pixmap);
    void previewFailed(const KFileItem& item);

private:
    void createDocumentsPreviews(KFileItemList list);

    AppItemList docsList;
    QStringList m_previewJobPlugins;
    QMLConstants *constants;
    QMap<QString, QPixmap> m_pixmaps;
};
