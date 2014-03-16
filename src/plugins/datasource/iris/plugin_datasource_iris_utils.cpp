// Copyright 2014, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#include "plugin_datasource_iris_utils.h"
#include <QStringList>
#include <math.h>

// ----------------------------------------------------------------------
namespace plugin {
namespace datasource {
namespace iris {
// ----------------------------------------------------------------------

double calcDistance(double lat1, double lon1, double lat2, double lon2)
{
    double pi = 3.14159265358979;
    double radius = 6371116.0;

    return radius * acos(sin(lat1*pi/180) * sin(lat2*pi/180) +
            cos(lat1*pi/180) * cos(lat2*pi/180) * cos(lon1*pi/180 - lon2*pi/180));
}

QString extractValueFromXmlPath(const QDomNode &root, const QString &path)
{
    QStringList nodeNamePath = path.split("/");

    QString nodeName = nodeNamePath.takeFirst();
    QDomNode node = root.firstChild();
    while (!node.isNull())
    {
        if (node.nodeName() == nodeName)
        {
            if (nodeNamePath.count())
            {
                nodeName = nodeNamePath.takeFirst();
                continue;
            }
            else
            {
                return node.toElement().text();
            }
        }
        node = node.nextSibling();
    }

    return QString();
}

// ----------------------------------------------------------------------
} // namespace iris
} // namespace datasource
} // namespace plugin
// ----------------------------------------------------------------------
