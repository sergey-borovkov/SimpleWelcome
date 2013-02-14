/* Timeframe
 * Copyright (c) 2012 ROSA  <support@rosalab.ru>
 * Authors:
 *  Sergey Borovkov <sergey.borovkov@osinit.ru>
 *  Evgeniy Auzhin <evgeniy.augin@osinit.ru>
 *  Julia Mineeva <julia.mineeva@osinit.ru>
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

import QtQuick 1.1
import "clouds"

Item {
    id: timeLineItem
    height: parent.height
    width: timeLine.width/3

    property int margin: 10
    property int labelHeight: 20

    Item {
        id: cloudBorder
        width: parent.width + 200
        height: parent.height / 2 - timeScale.height / 2 - 20
        y: (index % 2 === 1) ? parent.height / 2 + timeScale.height / 2 : 0
        x: -100

        Component.onDestruction: {
            var m = localDayModel.itemsModel(date)
            var c = 0
            var objects = (m.count() >= 7) ? cloud.item.children[0].children : cloud.item.children
            for(i = 0; i < objects.length; ++i) {
                if('url' in objects[i]) {
                    objects[i].url = m.url(c)
                    timelinePreviewGenerator.cancel(objects[i].image)
                    c++
                }
            }
        }

        Loader {
            id: cloud
            anchors.fill: parent

            source: getSourceComponent(size)

            function getSourceComponent(count)
            {
                if (count === 1)
                    return "clouds/CloudOne.qml"
                else if (count === 2)
                    return "clouds/CloudTwo.qml"
                else if (count === 3)
                    return "clouds/CloudThree.qml"
                else if (count === 4)
                    return "clouds/CloudFour.qml"
                else if (count === 5)
                    return "clouds/CloudFive.qml"
                else if (count === 6)
                    return "clouds/CloudSix.qml"
                return "clouds/CloudSeven.qml"
            }

            onLoaded: {
                cloud.item.cloudDate = date
                cloud.item.model = localDayModel.itemsModel(date)
                var c = 0, model = cloud.item.model
                // in CloudSeven.qml cloudrects are nested in another item element
                var objects = (model.count() >= 7) ? cloud.item.children[0].children : cloud.item.children
                var i
                for(i = 0; i < objects.length; ++i) {
                    if('url' in objects[i]) {
                        objects[i].url = model.url(c)
                        objects[i].type = model.type(c)
                        objects[i].image.source = "image://timeline/" + model.url(c) + "/rounded" + "%" + objects[i].image
                        c++
                    }
                }
            }
        }
    }
}
