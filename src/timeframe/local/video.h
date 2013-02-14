/* KLook
 * Copyright (c) 2011-2012 ROSA  <support@rosalab.ru>
 * Authors: Julia Mineeva, Evgeniy Auzhin, Sergey Borovkov.
 * License: GPLv3
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


#ifndef VIDEO_H
#define VIDEO_H

#include <QGraphicsProxyWidget>
#include <Phonon/VideoPlayer>
#include <KUrl>

class QHBoxLayout;


class Video : public QGraphicsProxyWidget
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)    
    //Q_PROPERTY(bool paused READ paused)
    Q_PROPERTY(int videoWidth READ videoWidth)
    Q_PROPERTY(int videoHeight READ videoHeight)    
    Q_PROPERTY(bool ready READ isReady WRITE setReady NOTIFY ready)

public:
    Video(QGraphicsItem* parent = 0);
    ~Video();
    Q_INVOKABLE void playOrPause();
    Q_INVOKABLE void play();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void setSource(const KUrl& source);
    Q_INVOKABLE bool paused() const;
    Q_INVOKABLE bool playing() const;

public slots:
    QString source() const;    


    //bool paused() const;
    void setPause();

    bool isReady();
    void setReady(bool b);

    int videoWidth() const { return m_videoWidth; }
    int videoHeight() const { return m_videoHeight; }

    void onFinished();

    void onMetaDataChanged();
    void stateChanged(Phonon::State oldState, Phonon::State newState);


signals:
    void clicked(bool);
    void sourceChanged();    
    void playFinished();
    void ready();
    void sizeHintReady(int width, int height);

private:
    Phonon::VideoPlayer* m_player;

    QWidget*        m_wid;
    QHBoxLayout*    m_layout;

    int m_videoWidth;
    int m_videoHeight;
    bool m_isReady;
};

#endif // VIDEO_H
