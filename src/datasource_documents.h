#pragma once

#include "appitem.h"
#include "datasource.h"

#include <KDE/KFileItemList>

class SizesCalculator;
namespace KIO {
    class PreviewJob;
}
class KJob;

class DataSource_Documents : public DataSource
{
    Q_OBJECT
    Q_PROPERTY(QString qmlGroupName READ getQmlGroupName WRITE setQmlGroupName)

public:
    explicit DataSource_Documents(QObject *parent, SizesCalculator *inConstants);
    Q_INVOKABLE virtual int getItemCount() const;
    Q_INVOKABLE virtual QVariantMap getContent(int index) const;

    Q_INVOKABLE QString itemUrlDnd(int id) const;
    QPixmap getPreview(QString destination) const;

signals:
    void newItemData(QVariantMap itemData, QString group);
    void runDesktopFile(QString desktopFile);
    void updateItemData(int id, QString field, QVariant data);

public slots:
    virtual void itemClicked(int newIndex);
    void updateContent();
    void updateThumbnails();
    void iconSizeChanged();

private slots:
    void resultPreviewJob(const KFileItem &item, const QPixmap &pixmap);
    void previewFailed(const KFileItem &item);
    void previewJobFinished(KJob *job);

private:
    void createDocumentsPreviews(KFileItemList list);

    AppItemList docsList;
    QStringList m_previewJobPlugins;
    SizesCalculator *constants;
    QMap<QString, QPixmap> m_pixmaps;
    KIO::PreviewJob* runningJob;
};
