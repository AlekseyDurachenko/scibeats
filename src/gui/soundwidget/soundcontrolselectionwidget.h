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
#ifndef SOUNDCONTROLSELECTIONWIDGET_H
#define SOUNDCONTROLSELECTIONWIDGET_H


#include "sounddatasource.h"
#include "qseselection.h"
#include "qseposition.h"
#include <QDateTime>
#include <QLabel>


class SoundControlSelectionWidget : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(Section section READ section WRITE setSection)
    Q_PROPERTY(Representation representation READ representation WRITE setRepresentation)
public:
    enum Section {
        StartSection,
        EndSection,
        LengthSection
    };

    enum Representation {
        TimeRepresentation,
        SecondRepresentation,
        SampleRepresentation
    };

public:
    explicit SoundControlSelectionWidget(QWidget *parent = 0);
    explicit SoundControlSelectionWidget(Section section,
                                         Representation representation,
                                         QWidget *parent = 0);

    inline Section section() const;
    void setSection(Section section);

    inline Representation representation() const;
    void setRepresentation(Representation representation);

    inline SoundDataSource *dataSource() const;
    void setDataSource(SoundDataSource *dataSource);

    inline QseSelection *selection() const;
    void setSelection(QseSelection *selection);

    inline QsePosition *cursorPosition() const;
    void setCursorPosition(QsePosition *cursorPosition);

    inline QsePosition *playPosition() const;
    void setPlayPosition(QsePosition *playPosition);

    inline double sampleRate() const;

public slots:
    void setSampleRate(double smapleRate);
    void setSapleRate(int sampleRate);

private slots:
    void dataSource_destroyed();
    void selection_destroyed();
    void cursorPosition_destroyed();
    void playPosition_destroyed();

private slots:
    void updateWidget();
    void showContextMenu(const QPoint &pos);
    void switchRepresentation();

private:
    void updateMinimumWidth();
    QString minimumWidthText() const;

    QString displayText() const;
    qint64 calculateSelectionStart() const;
    qint64 calculateSelectionEnd() const;
    qint64 calculateSelectionLength() const;
    QString sampleToTime(qint64 sample) const;
    QString sampleToSeconds(qint64 sample) const;
    QString sampleToNumber(qint64 sample) const;

private:
    Section m_section;
    Representation m_representation;
    double m_sampleRate;
    SoundDataSource *m_dataSource;
    QseSelection *m_selection;
    QsePosition *m_cursorPosition;
    QsePosition *m_playPosition;
};

SoundControlSelectionWidget::Section SoundControlSelectionWidget::section() const
{
    return m_section;
}

SoundControlSelectionWidget::Representation SoundControlSelectionWidget::representation() const
{
    return m_representation;
}

SoundDataSource *SoundControlSelectionWidget::dataSource() const
{
    return m_dataSource;
}

QseSelection *SoundControlSelectionWidget::selection() const
{
    return m_selection;
}

QsePosition *SoundControlSelectionWidget::cursorPosition() const
{
    return m_cursorPosition;
}

QsePosition *SoundControlSelectionWidget::playPosition() const
{
    return m_playPosition;
}

double SoundControlSelectionWidget::sampleRate() const
{
    return m_sampleRate;
}


#endif // SOUNDCONTROLSELECTIONWIDGET_H
