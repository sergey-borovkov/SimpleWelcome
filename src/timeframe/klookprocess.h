#ifndef KLOOKPROCESS_H
#define KLOOKPROCESS_H

#include <QObject>

class QRect;

class KlookProcess : public QObject
{
    Q_OBJECT
public:
    explicit KlookProcess(QObject *parent = 0);

    Q_INVOKABLE void show(int x, int y, int width, int height, const QString &path);
    Q_INVOKABLE void hide();

    void show(const QRect &rectangle, const QString &path);

private:
   bool m_processStarted;
};

#endif // KLOOKPROCESS_H
