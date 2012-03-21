#ifndef __appiconprovider_h_
#define __appiconprovider_h_

#include <QDeclarativeImageProvider>

class AppIconProvider : public QDeclarativeImageProvider
{
 public:
  AppIconProvider(void);
  ~AppIconProvider(void);

  QPixmap requestPixmap(const QString &name, QSize *size, const QSize &requestedSize);
};

#endif // __appiconprovider_h_
