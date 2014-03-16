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
#ifndef PLUGIN_DATASOURCE_CABSTRACTWIDGET_H
#define PLUGIN_DATASOURCE_CABSTRACTWIDGET_H

#include "csound.h"
#include <QWidget>
class QNetworkAccessManager;

// ----------------------------------------------------------------------
namespace plugin {
namespace datasource {
// ----------------------------------------------------------------------

class CAbstractWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CAbstractWidget(QNetworkAccessManager *network,
            QWidget *parent = 0, Qt::WindowFlags f = 0);

    inline QNetworkAccessManager *network() const;
    virtual void abortSoundCreation() = 0;

signals:
    void soundCreated(const CSound& sound);
    void aboutSoundToBeCreated();

private:
    QNetworkAccessManager *m_network;

};

QNetworkAccessManager *CAbstractWidget::network() const
{
    return m_network;
}

// ----------------------------------------------------------------------
} // namespace datasource
} // namespace plugin
// ----------------------------------------------------------------------

#endif // PLUGIN_DATASOURCE_CABSTRACTWIDGET_H
