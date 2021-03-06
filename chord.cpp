#include "chord.h"

void Chord::ChordsInicjalization()
{
    for(int i = 0; i < 12; i++)
    {
        DurChords[i].clear();
    }

    DurChords[0].append("C");

    DurChords[1].append("C#");
    DurChords[1].append("Cis");

    DurChords[2].append("D");

    DurChords[3].append("D#");
    DurChords[3].append("Dis");

    DurChords[4].append("E");

    DurChords[5].append("F");

    DurChords[6].append("F#");
    DurChords[6].append("Fis");

    DurChords[7].append("G");

    DurChords[8].append("G#");
    DurChords[8].append("Gis");

    DurChords[9].append("A");

    DurChords[10].append("B");

    DurChords[11].append("H");
}

Chord::Chord()
    : m_chord_name(""),
      m_is_dur(false),
      m_chord_index(-1),
      m_shift(0)
{
    ChordsInicjalization();
}

Chord::Chord(QString ch, int sh)
    : m_chord_name(ch),
      m_is_dur(false),
      m_chord_index(-1),
      m_shift(sh % 12)
{
    ChordsInicjalization();
    FindChordIndex();
}

int Chord::ChordIndex() const
{
    return m_chord_index;
}

int Chord::Shift() const
{
    return m_shift;
}

void Chord::SetShift(int s)
{
    //przycięcie do jednej oktawy
    m_shift = (s < 0) ? 12 + s % 12 : s % 12;
}

void Chord::SetChordName(QString chord)
{
    m_chord_index = -1;
    m_chord_name = chord;
    FindChordIndex();
}

QString Chord::ChordName() const
{
    return m_chord_name;
}

QString Chord::ShiftChord() const
{
    if(m_chord_index != -1)
    {
        int m_shift_index = (m_chord_index + m_shift) % 12;
        QString tmp = m_chord_name;

        if(m_chord_name.size() > 1)
        {
            if(m_chord_name.at(1) == '#')
            {
                tmp.remove(0, 2);
            }
            else if(m_chord_name.size() > 2 && m_chord_name.at(1) == 'i' && m_chord_name.at(2) == 's')
            {
                tmp.remove(0, 3);
            }
            else
            {
                tmp.remove(0, 1);
            }
        }
        else
        {
            tmp.remove(0, 1);
        }

        QString tmp2 = DurChords[m_shift_index].at(0);

        if(!m_is_dur)
        {
            tmp2[0] = tmp2.at(0).toLower();
        }
        tmp.insert(0, tmp2);

        return tmp;
    }

    return m_chord_name;
}

void Chord::FindChordIndex()
{
    m_chord_name = m_chord_name.trimmed();
    if(m_chord_name.size() > 0)
    {
        QString ch = m_chord_name.at(0);

        if(ch.at(0).isUpper())
        {
            m_is_dur = true;
        }
        else
        {
            m_is_dur = false;
        }

        ch = ch.toUpper();

        //Sprawdzenie czy to nie jest jakiś Fis, C#, etc
        if(m_chord_name.size() > 1 && m_chord_name.at(1) == '#')
        {
            ch += "#";
        }
        else if(m_chord_name.size() > 2 && m_chord_name.at(1) == 'i' && m_chord_name.at(2) == 's')
        {
            ch += "is";
        }

        for(int i = 0; i < 12; i++)
        {
            if(DurChords[i].contains(ch))
            {
                m_chord_index = i;
                break;
            }
        }
    }
}

QStringList ShiftChordsList(QStringList chords, int shift)
{
    Chord tmp_chord;
    tmp_chord.SetShift(shift);
    QStringList result;
    QString tmp1, tmp2, tmp3;

    for(int i = 0; i < chords.size(); i++)
    {
        tmp1 = chords.at(i);
        for(int j = 0; j < tmp1.size(); j++)
        {
            if(tmp1.at(j) != '(' && tmp1.at(j) != ')' && tmp1.at(j) != '-')
                tmp2.append(tmp1.at(j));
            else
            {
                tmp_chord.SetChordName(tmp2);
                tmp3.append(tmp_chord.ShiftChord());
                tmp3.append(tmp1.at(j));
                tmp2.clear();
            }
        }
        if(!tmp2.isEmpty())
        {
            tmp_chord.SetChordName(tmp2);
            tmp3.append(tmp_chord.ShiftChord());
        }
        result << tmp3;

        tmp3.clear();
        tmp2.clear();
        tmp1.clear();
    }

    return result;
}

QString ShiftChordString(QStringList chords, int shift)
{
    QString result = "";
    QStringList tmp = ShiftChordsList(chords, shift);

    for(int i = 0; i < tmp.size(); i++)
    {
        result += tmp.at(i);
        result += " ";
    }

    result = result.trimmed();

    return result;
}
