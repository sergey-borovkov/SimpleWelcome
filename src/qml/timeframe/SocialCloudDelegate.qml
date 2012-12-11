/* Timeframe
 * Copyright (c) 2012 ROSA  <support@rosalab.ru>
 * Authors:
 *  Sergey Borovkov <sergey.borovkov@osinit.ru>
 *  Evgeniy Auzhin <evgeniy.augin@osinit.ru>
 *  Julia Mineeva <julia.mineeca@osinit.ru>
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
    property int margin: 10
    property int labelHeight: 20

    id: timeLineItem
    height: parent.height
    width: timeLine.width/3

    // using Rectangle because with Item there is painting bug...
    Rectangle {
        id: cloudBorder
        width: parent.width + 200
        height: parent.height / 2 - timeScale.height / 2 - 10
        x: -100
        y: (index % 2 === 1) ? parent.height / 2 + timeScale.height / 2 : 0

        color: "transparent"

        Loader {
            id: cloud
            anchors.fill: parent

            source: getSourceComponent(size)

            onLoaded: {
                var model = socialDayModel.itemsModel(date)
                cloud.item.cloudDate = date
                cloud.item.model = model
                var c = size - 1
                for(var i = 0; i < cloud.item.children.length; i++) {
                    var subChildren = cloud.item.children[i].children
                    for(var j = 0; j < subChildren.length; j++) {
                        if(subChildren[j].objectName === "cloudRect") {
                            subChildren[j].model = model
                            subChildren[j].initialize(c)
                            c--
                        }
                    }
                }
            }
        }
    }

    function getSourceComponent(count)
    {
        if (count === 0)
            return ""
        if (count === 1)
            return "clouds/SocialCloudOne.qml"
        else if (count  === 2)
            return "clouds/SocialCloudTwo.qml"
        else if (count === 3)
            return "clouds/SocialCloudThree.qml"
        else if (count > 3)
            return "clouds/SocialCloudFour.qml"
    }

}

