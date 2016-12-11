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
#include "soundcontrolselectionwidget.h"
#include <QFontDatabase>
#include <QFontMetrics>
#include <QMenu>


SoundControlSelectionWidget::SoundControlSelectionWidget(QWidget *parent)
    : SoundControlSelectionWidget(StartSection, TimeRepresentation, parent)
{
}

SoundControlSelectionWidget::SoundControlSelectionWidget(
    SoundControlSelectionWidget::Section section,
    SoundControlSelectionWidget::Representation representation,
    QWidget *parent)
    : QLabel(parent)
    , m_section(section)
    , m_representation(representation)
{
    setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    setFrameStyle(QFrame::Panel | QFrame::Sunken);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(showContextMenu(QPoint)));

    m_dataSource = nullptr;
    m_selection = nullptr;
    m_cursorPosition = nullptr;
    m_playPosition = nullptr;
    m_sampleRate = 1.0;

    updateMinimumWidth();
    updateWidget();
}

void SoundControlSelectionWidget::setSection(Section section)
{
    if (m_section != section) {
        m_section = section;
        updateMinimumWidth();
        updateWidget();
    }
}

void SoundControlSelectionWidget::setRepresentation(Representation representation)
{
    if (m_representation != representation) {
        m_representation = representation;
        updateMinimumWidth();
        updateWidget();
    }
}

void SoundControlSelectionWidget::setDataSource(SoundDataSource *dataSource)
{
    if (m_dataSource) {
        disconnect(m_dataSource, 0, this, 0);
    }

    m_dataSource = dataSource;
    connect(m_dataSource, SIGNAL(dataChanged()), this, SLOT(updateWidget()));
    connect(m_dataSource, SIGNAL(destroyed(QObject *)), this, SLOT(dataSource_destroyed()));

    updateWidget();
}

void SoundControlSelectionWidget::setSelection(QseSelection *selection)
{
    if (m_selection) {
        disconnect(m_selection, 0, this, 0);
    }

    m_selection = selection;
    connect(selection, SIGNAL(selectionChanged()), this, SLOT(updateWidget()));
    connect(selection, SIGNAL(destroyed(QObject *)), this, SLOT(selection_destroyed()));

    updateWidget();
}

void SoundControlSelectionWidget::setCursorPosition(QsePosition *cursorPosition)
{
    if (m_cursorPosition) {
        disconnect(m_cursorPosition, 0, this, 0);
    }

    m_cursorPosition = cursorPosition;
    connect(m_cursorPosition, SIGNAL(indexChanged()), this, SLOT(updateWidget()));
    connect(m_cursorPosition, SIGNAL(destroyed(QObject *)), this, SLOT(cursorPosition_destroyed()));

    updateWidget();
}

void SoundControlSelectionWidget::setPlayPosition(QsePosition *playPosition)
{
    if (m_playPosition) {
        disconnect(m_playPosition, 0, this, 0);
    }

    m_playPosition = playPosition;
    connect(m_playPosition, SIGNAL(indexChanged()), this, SLOT(updateWidget()));
    connect(m_playPosition, SIGNAL(destroyed(QObject *)), this, SLOT(playPosition_destroyed()));

    updateWidget();
}

void SoundControlSelectionWidget::setSampleRate(double sampleRate)
{
    if (m_sampleRate != sampleRate) {
        m_sampleRate = sampleRate;
        updateWidget();
    }
}

void SoundControlSelectionWidget::setSapleRate(int sampleRate)
{
    if (m_sampleRate != sampleRate) {
        m_sampleRate = sampleRate;
        updateWidget();
    }
}

void SoundControlSelectionWidget::dataSource_destroyed()
{
    m_dataSource = nullptr;
}

void SoundControlSelectionWidget::selection_destroyed()
{
    m_selection = nullptr;
}

void SoundControlSelectionWidget::cursorPosition_destroyed()
{
    m_cursorPosition = nullptr;
}

void SoundControlSelectionWidget::playPosition_destroyed()
{
    m_playPosition = nullptr;
}

void SoundControlSelectionWidget::updateWidget()
{
    setText(displayText());
}

void SoundControlSelectionWidget::showContextMenu(const QPoint &pos)
{
    QMenu menu(this);
    QAction *timeAction = menu.addAction(tr("Time"));
    QAction *secondsAction = menu.addAction(tr("Seconds"));
    QAction *samplesAction = menu.addAction(tr("Samples"));

    QActionGroup actionGroup(this);
    actionGroup.addAction(timeAction);
    actionGroup.addAction(secondsAction);
    actionGroup.addAction(samplesAction);

    timeAction->setCheckable(true);
    secondsAction->setCheckable(true);
    samplesAction->setCheckable(true);

    switch (m_representation) {
    case TimeRepresentation:
        timeAction->setChecked(true);
        break;
    case SecondRepresentation:
        secondsAction->setChecked(true);
        break;
    case SampleRepresentation:
        samplesAction->setChecked(true);
        break;
    }

    timeAction->setProperty("representation", TimeRepresentation);
    secondsAction->setProperty("representation", SecondRepresentation);
    samplesAction->setProperty("representation", SampleRepresentation);

    connect(timeAction, SIGNAL(triggered(bool)),
            this, SLOT(switchRepresentation()));
    connect(secondsAction, SIGNAL(triggered(bool)),
            this, SLOT(switchRepresentation()));
    connect(samplesAction, SIGNAL(triggered(bool)),
            this, SLOT(switchRepresentation()));

    menu.exec(mapToGlobal(pos));
}

void SoundControlSelectionWidget::switchRepresentation()
{
    setRepresentation(
        static_cast<Representation>(
            sender()->property("representation").toInt()));
}

void SoundControlSelectionWidget::updateMinimumWidth()
{
#if QT_VERSION >= 0x050000
    const QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
#else
    QFont fixedFont;
    fixedFont.setStyleHint(QFont::Monospace);
#endif
    const QFontMetrics fontMetrics(fixedFont);
    setMinimumWidth(fontMetrics.width(minimumWidthText()));
}

QString SoundControlSelectionWidget::minimumWidthText() const
{
    switch (m_representation) {
    case TimeRepresentation:
        return QString("000000000000");
    case SecondRepresentation:
        return QString("000000000000");
    case SampleRepresentation:
        return QString("000000000000");
    }

    return QString();
}

QString SoundControlSelectionWidget::displayText() const
{
    qint64 sample = -1;
    if (m_dataSource && m_selection && m_cursorPosition && m_playPosition) {
        switch (m_section) {
        case StartSection:
            sample = calculateSelectionStart();
            break;
        case EndSection:
            sample = calculateSelectionEnd();
            break;
        case LengthSection:
            sample = calculateSelectionLength();
            break;
        }
    }

    if (sample >= 0) {
        switch (m_representation) {
        case TimeRepresentation:
            return sampleToTime(sample);
        case SecondRepresentation:
            return sampleToSeconds(sample);
        case SampleRepresentation:
            return QString::number(sample);
        }
    }

    return QString();
}

qint64 SoundControlSelectionWidget::calculateSelectionStart() const
{
    if (m_dataSource) {
        if (m_playPosition && !m_playPosition->isNull()) {
            return m_playPosition->index();
        }

        if (m_cursorPosition && !m_cursorPosition->isNull()) {
            return m_cursorPosition->index();
        }

        if (m_selection && !m_selection->isNull()) {
            return m_selection->selectedRange().first();
        }

        if (!m_dataSource->sound().isNull()) {
            return 0;
        }
    }

    return -1;
}

qint64 SoundControlSelectionWidget::calculateSelectionEnd() const
{
    if (m_dataSource) {
        if (m_selection && !m_selection->isNull()) {
            return m_selection->selectedRange().last();
        }

        if (!m_dataSource->sound().isNull()) {
            return m_dataSource->sound().sampleCount();
        }
    }

    return -1;
}

qint64 SoundControlSelectionWidget::calculateSelectionLength() const
{
    if (m_dataSource) {
        if (m_selection && !m_selection->isNull()) {
            return m_selection->selectedRange().count();
        }

        if (!m_dataSource->sound().isNull()) {
            return m_dataSource->sound().sampleCount();
        }
    }

    return -1;
}

QString SoundControlSelectionWidget::sampleToTime(qint64 sample) const
{
    QDateTime dateTime;
    dateTime.setTimeSpec(Qt::UTC);
    dateTime.setTime_t(0);
    dateTime = dateTime.addMSecs(static_cast<qint64>((sample / (m_sampleRate / 1000.0))));
    return dateTime.toString("hh:mm:ss.zzz");
}

QString SoundControlSelectionWidget::sampleToSeconds(qint64 sample) const
{
    return QString::number(sample / m_sampleRate, 'f', 6);
}

QString SoundControlSelectionWidget::sampleToNumber(qint64 sample) const
{
    return QString::number(sample);
}
