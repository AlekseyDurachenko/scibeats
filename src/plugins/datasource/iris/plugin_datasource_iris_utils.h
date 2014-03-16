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
#ifndef PLUGIN_DATASOURCE_IRIS_UTILS_H
#define PLUGIN_DATASOURCE_IRIS_UTILS_H

#include <QDomNode>

// ----------------------------------------------------------------------
namespace plugin {
namespace datasource {
namespace iris {
// ----------------------------------------------------------------------

/*!
 * \brief calcDistance calculate the distance between two point on earth
 * \return distance in meters
 */
double calcDistance(double lat1, double lon1, double lat2, double lon2);

// This function select the value from node by path.
//
// For example: path == "value" for xml bellow return the "sometext"
//  <time>
//    <value>sometext</value>
//  </time>
//
// For example: path == "value/subvalue" for xml bellow return the "sometext"
//  <time>
//    <value>
//      <subvalue>sometext</subvalue>
//    </value>
//  </time>
QString extractValueFromXmlPath(const QDomNode& root, const QString &path);

// ----------------------------------------------------------------------
} // namespace iris
} // namespace datasource
} // namespace plugin
// ----------------------------------------------------------------------

#endif // PLUGIN_DATASOURCE_IRIS_UTILS_H
