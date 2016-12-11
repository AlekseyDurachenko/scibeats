// Copyright 2016, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
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
#include "soundscrollwidget.h"
#include "theme.h"
#include <QHBoxLayout>


SoundScrollWidget::SoundScrollWidget(QWidget *parent)
    : QWidget(parent)
    , m_dataSource(nullptr)
    , m_widgetWidth(0)
{
    createActions();
    createToolButtons();
    createHorizontalScrollBar();
    createLayout();

    updateScrollBar();
}

void SoundScrollWidget::setDataSource(SoundDataSource *dataSource)
{
    if (m_dataSource != dataSource) {
        if (m_dataSource) {
            disconnect(m_dataSource, 0, this, 0);
        }

        m_dataSource = dataSource;
        connect(m_dataSource, SIGNAL(dataChanged()),
                this, SLOT(dataSource_dataChanged()));
        connect(m_dataSource, SIGNAL(destroyed(QObject *)),
                this, SLOT(dataSource_destroyed()));

        updateScrollBar();
    }
}

void SoundScrollWidget::setGeometry(const QseGeometry &geometry)
{
    if (m_lastGeometry != geometry) {
        m_lastGeometry = geometry;
        updateScrollBar();
    }
}

void SoundScrollWidget::setWidgetWidth(int width)
{
    if (m_widgetWidth != width) {
        m_widgetWidth = width;
        updateScrollBar();
    }
}

void SoundScrollWidget::action_zoomInHorizontal_triggered()
{
    qint64 spp = m_lastGeometry.spp();
    if (spp > 0) {
        spp /= 2;

        if (spp < 2) {
            spp = -1;
        }
    }
    else {
        spp *= 2;
    }

    emit geometryChanged(m_lastGeometry.replaceSpp(spp));
}

void SoundScrollWidget::action_zoomOutHorizontal_triggered()
{
    qint64 spp = m_lastGeometry.spp();
    if (spp > 0) {
        spp *= 2;
    }
    else {
        spp /= 2;

        if (spp > -1) {
            spp = 2;
        }
    }

    emit geometryChanged(m_lastGeometry.replaceSpp(spp));
}

void SoundScrollWidget::action_zoomAllHorizontal_triggered()
{
    if (m_widgetWidth <= 0) {
        return;
    }

    if (!m_dataSource || m_dataSource->sound().isNull()) {
        return;
    }

    const qint64 x = 0;
    const double y = m_lastGeometry.y();
    const qint64 spp = m_dataSource->sound().sampleCount() / m_widgetWidth;
    const double height = m_lastGeometry.height();
    emit geometryChanged(QseGeometry(x, y, spp, height));
}

void SoundScrollWidget::action_zoomInVertical_triggered()
{
    const double height = m_lastGeometry.height() / 2.0;
    emit geometryChanged(m_lastGeometry.replaceHeight(height));
}

void SoundScrollWidget::action_zoomOutVertical_triggered()
{
    const double height = m_lastGeometry.height() * 2.0;
    emit geometryChanged(m_lastGeometry.replaceHeight(height));
}

void SoundScrollWidget::action_zoomAllVertical_triggered()
{
    emit geometryChanged(m_lastGeometry.replaceHeight(2.0));
}

void SoundScrollWidget::horizontalScrollBar_valueChanged(int value)
{
    if (m_lastGeometry.spp() > 0) {
        emit geometryChanged(m_lastGeometry.replaceX(value * m_lastGeometry.spp()));
    }
    else {
        emit geometryChanged(m_lastGeometry.replaceX(value));
    }
}

void SoundScrollWidget::dataSource_dataChanged()
{
    updateScrollBar();
}

void SoundScrollWidget::dataSource_destroyed()
{
    m_dataSource = nullptr;
    updateScrollBar();
}

void SoundScrollWidget::createActions()
{
    m_zoomInHorizontalAction = new QAction(this);
    m_zoomInHorizontalAction->setText(tr("Zoom In"));
    m_zoomInHorizontalAction->setIcon(theme::icon(theme::Icon::ActionZoomIn));
    connect(m_zoomInHorizontalAction, SIGNAL(triggered(bool)),
            this, SLOT(action_zoomInHorizontal_triggered()));

    m_zoomOutHorizontalAction = new QAction(this);
    m_zoomOutHorizontalAction->setText(tr("Zoom Out"));
    m_zoomOutHorizontalAction->setIcon(theme::icon(theme::Icon::ActionZoomOut));
    connect(m_zoomOutHorizontalAction, SIGNAL(triggered(bool)),
            this, SLOT(action_zoomOutHorizontal_triggered()));

    m_zoomAllHorizontalAction = new QAction(this);
    m_zoomAllHorizontalAction->setText(tr("Zoom All Horizontal"));
    m_zoomAllHorizontalAction->setIcon(theme::icon(theme::Icon::ActionZoomAllHorizontal));
    connect(m_zoomAllHorizontalAction, SIGNAL(triggered(bool)),
            this, SLOT(action_zoomAllHorizontal_triggered()));


    m_zoomInVerticalAction = new QAction(this);
    m_zoomInVerticalAction->setText(tr("Zoom In"));
    m_zoomInVerticalAction->setIcon(theme::icon(theme::Icon::ActionZoomIn));
    connect(m_zoomInVerticalAction, SIGNAL(triggered(bool)),
            this, SLOT(action_zoomInVertical_triggered()));

    m_zoomOutVerticalAction = new QAction(this);
    m_zoomOutVerticalAction->setText(tr("Zoom Out"));
    m_zoomOutVerticalAction->setIcon(theme::icon(theme::Icon::ActionZoomOut));
    connect(m_zoomOutVerticalAction, SIGNAL(triggered(bool)),
            this, SLOT(action_zoomOutVertical_triggered()));

    m_zoomAllVerticalAction = new QAction(this);
    m_zoomAllVerticalAction->setText(tr("Zoom All Vertical"));
    m_zoomAllVerticalAction->setIcon(theme::icon(theme::Icon::ActionZoomAllVertical));
    connect(m_zoomAllVerticalAction, SIGNAL(triggered(bool)),
            this, SLOT(action_zoomAllVertical_triggered()));
}

void SoundScrollWidget::createToolButtons()
{
    m_zoomInHorizontalToolButton = new QToolButton(this);
    m_zoomInHorizontalToolButton->setDefaultAction(m_zoomInHorizontalAction);

    m_zoomOutHorizontalToolButton = new QToolButton(this);
    m_zoomOutHorizontalToolButton->setDefaultAction(m_zoomOutHorizontalAction);

    m_zoomAllHorizontalToolButton = new QToolButton(this);
    m_zoomAllHorizontalToolButton->setDefaultAction(m_zoomAllHorizontalAction);


    m_zoomInVerticalToolButton = new QToolButton(this);
    m_zoomInVerticalToolButton->setDefaultAction(m_zoomInVerticalAction);

    m_zoomOutVerticalToolButton = new QToolButton(this);
    m_zoomOutVerticalToolButton->setDefaultAction(m_zoomOutVerticalAction);

    m_zoomAllVerticalToolButton = new QToolButton(this);
    m_zoomAllVerticalToolButton->setDefaultAction(m_zoomAllVerticalAction);
}

void SoundScrollWidget::createHorizontalScrollBar()
{
    m_horizontalScrollBar = new QScrollBar(this);
    m_horizontalScrollBar->setOrientation(Qt::Horizontal);
    m_horizontalScrollBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    connect(m_horizontalScrollBar, SIGNAL(valueChanged(int)),
            this, SLOT(horizontalScrollBar_valueChanged(int)));
}

void SoundScrollWidget::createLayout()
{
    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(m_zoomAllVerticalToolButton);
    hbox->addWidget(m_zoomInVerticalToolButton);
    hbox->addWidget(m_zoomOutVerticalToolButton);
    hbox->addWidget(m_horizontalScrollBar);
    hbox->addWidget(m_zoomInHorizontalToolButton);
    hbox->addWidget(m_zoomOutHorizontalToolButton);
    hbox->addWidget(m_zoomAllHorizontalToolButton);
    hbox->setSpacing(0);
    hbox->setContentsMargins(0, 0, 0, 0);
    setLayout(hbox);
}

void SoundScrollWidget::updateScrollBar()
{
    if (!m_dataSource || m_dataSource->sound().isNull()) {
        m_horizontalScrollBar->blockSignals(true);
        m_horizontalScrollBar->setRange(0, 0);
        m_horizontalScrollBar->setValue(0);
        m_horizontalScrollBar->blockSignals(false);
        return;
    }

    const qint64 sampleCount = m_dataSource->sound().sampleCount();
    qint64 scrollMax = sampleCount;
    if (m_lastGeometry.spp() > 0) {
        scrollMax = sampleCount / m_lastGeometry.spp()
                    + ((sampleCount % m_lastGeometry.spp()) ? 1 : 0)
                    - ((m_widgetWidth > 20) ? m_widgetWidth - 20 : 0);
    }
    else if (m_widgetWidth > 0) {
        scrollMax -= (m_widgetWidth - 20) / -m_lastGeometry.spp();
    }

    if (scrollMax < 0) {
        scrollMax = 0;
    }

    m_horizontalScrollBar->blockSignals(true);
    m_horizontalScrollBar->setRange(0, scrollMax);
    if (m_lastGeometry.spp() > 0) {
        m_horizontalScrollBar->setValue(m_lastGeometry.x() / m_lastGeometry.spp());
    }
    else {
        m_horizontalScrollBar->setValue(m_lastGeometry.x());
    }
    m_horizontalScrollBar->blockSignals(false);
}
