/***************************************************************************
 *   (C) 2007 Marius Roets <roets.marius@gmail.com>                        *
 *   (C) 2007-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "filtermodel.h"

#include "database.h"
#include "filter.h"
#include "settings.h"
#include "tags.h"

#include <QtGui>

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

FilterModel::FilterModel(FilterX* filter, QObject* parent)
    : QAbstractItemModel(parent), m_filter(filter), m_modelUpdateStarted(0)
{
    setupColumns();
}

FilterModel::~FilterModel()
{
}

int FilterModel::rowCount(const QModelIndex& index) const
{
    if(index.isValid() || m_modelUpdateStarted)
    {
        return 0;
    }
    return m_filter ? m_filter->size() : 0;
}

int FilterModel::columnCount(const QModelIndex&) const
{
    return m_columnNames.count();
}

void FilterModel::addColumns(const QStringList& tags)
{
    m_columnNames.clear();
    m_columnTags.clear();

    m_columnNames << tr("Nr")
                  << tr("White")
                  << tr("White Elo")
                  << tr("Black")
                  << tr("Black Elo")
                  << tr("Event")
                  << tr("Site")
                  << tr("Round")
                  << tr("Date")
                  << tr("Result")
                  << tr("ECO")
                  << tr("Moves");

    m_columnTags << "Nr"
                 << TagNameWhite
                 << TagNameWhiteElo
                 << TagNameBlack
                 << TagNameBlackElo
                 << TagNameEvent
                 << TagNameSite
                 << TagNameRound
                 << TagNameDate
                 << TagNameResult
                 << TagNameECO
                 << TagNameLength;

    m_columnNames << tags;
    m_columnTags << tags;
}

bool FilterModel::canEditItem(const QModelIndex& index) const
{
    int col = index.column();
    if ((col!= 0) && (col!=11))
    {
        return true;
    }
    return false;
}

void FilterModel::cacheTags()
{
    m_columnTagIndex.clear();
    foreach(QString tagName, m_columnTags)
    {
       m_columnTagIndex.append(m_filter->database()->index()->getTagIndex(tagName));
    }
}

QStringList FilterModel::additionalTags()
{
    QString addTags = AppSettings->getValue("/GameList/AdditionalTags").toString();
    QStringList tags = addTags.split(QRegExp("[^a-zA-Z]"), Qt::SkipEmptyParts);
    return tags;
}

void FilterModel::setupColumns()
{
    addColumns(additionalTags());
}

void FilterModel::updateColumns()
{
    startUpdate();
    setupColumns();
    endUpdate();
}

void FilterModel::disableUpdate()
{
    ++m_modelUpdateStarted;
}

void FilterModel::enableUpdate()
{
    --m_modelUpdateStarted;
}

void FilterModel::startUpdate()
{
    if (!m_modelUpdateStarted) beginResetModel();
    ++m_modelUpdateStarted;
}

void FilterModel::endUpdate()
{
    --m_modelUpdateStarted;
    if (m_modelUpdateStarted==0)
    {
        cacheTags();
        endResetModel();
    }
}

bool FilterModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid())
    {
        GameId id = index.row();
        m_filter->database()->index()->setTag(m_columnTags[index.column()], value.toString(), id);
        emit dataChanged(index, index, QVector<int>() << role);
        TagIndex tag = m_columnTagIndex[index.column()];
        if (tag == TagNoIndex)
        {
            cacheTags();
        }
        return true;
    }
    return false;
}

void FilterModel::set(GameId game, int value)
{
    filter()->set(game, value);
    QModelIndex start = createIndex(game, 0, (void*) nullptr);
    QModelIndex end = createIndex(game, columnCount()-1, (void*) nullptr);
    emit dataChanged(start, end);
}

QVariant FilterModel::data(const QModelIndex &index, int role) const
{
    int sz = static_cast<int>(m_filter->size());
    if(index.isValid() && index.row() < sz)
    {
        GameId i = index.row();
        if (VALID_INDEX(i))
        {
            if ((role == Qt::DisplayRole) || (role == Qt::EditRole))
            {
                if(index.column() == 0)
                {
                    return i + 1;
                }

                QString tag = m_filter->database()->tagValue(i, m_columnTagIndex[index.column()]);
                if(tag == "?")
                {
                    tag.clear();
                }
                return tag;
            }
            else if(role == Qt::BackgroundRole)
            {
                QString medal = m_filter->database()->tagValue(i, "Medal");
                QColor bg(medal);
                if (bg.isValid())
                {
                    bg.setAlpha(80);
                    return QBrush(bg);
                }
            }
            else if(role == Qt::FontRole)
            {
                if(m_filter->database()->deleted(i))
                {
                    QFont font;
                    font.setStrikeOut(true);
                    return font;
                }
            }
            else if(role == Qt::ForegroundRole)
            {
                if(!m_filter->database()->getValidFlag(i))
                {
                    QVariant v = qApp->palette().color(QPalette::BrightText);
                    return v;
                }
                QVariant v = qApp->palette().color(QPalette::Text);
                return v;
            }
        }
    }
    return QVariant();
}

QVariant FilterModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
    {
        return QVariant();
    }

    if(orientation == Qt::Horizontal)
    {
        return QString("%1").arg(m_columnNames.at(section));
    }
    else
    {
        return QString("%1").arg(section);
    }
}

Qt::ItemFlags FilterModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);

    if(index.isValid())
    {
        if (canEditItem(index))
        {
            return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags | Qt::ItemIsSelectable | Qt::ItemIsEditable;
        }
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags | Qt::ItemIsSelectable;
    }
    else
    {
        return Qt::ItemIsDropEnabled | defaultFlags | Qt::ItemIsSelectable;
    }
}

QModelIndex FilterModel::index(int row, int column, const QModelIndex& parent) const
{
    if(parent.isValid())
    {
        return QModelIndex();
    }
    return createIndex(row, column, (void*) nullptr);
}

FilterX* FilterModel::filter()
{
    return m_filter;
}

void FilterModel::setFilter(FilterX* filter)
{
    m_filter = filter;
}

void FilterModel::invert()
{
    startUpdate();
    m_filter->invert();
    endUpdate();
}

void FilterModel::setAll(int value)
{
    startUpdate();
    m_filter->setAll(value);
    endUpdate();
}

void FilterModel::executeSearch(Search* search, FilterOperator searchOperator, int preSelect)
{
    FilterX* f;
    if (searchOperator==FilterOperator::NullOperator)
    {
        m_filter->cancel();
        f = new FilterX(*m_filter);
        f->setAll(preSelect); // ??
    }
    else
    {
        m_filter->wait();
        f = new FilterX(*m_filter);
    }

    connect(f, SIGNAL(searchFinished()), SLOT(endSearch()), Qt::QueuedConnection);
    connect(f, SIGNAL(searchProgress(int)), SIGNAL(searchProgress(int)), Qt::QueuedConnection);

    m_filter->lock(f);
    f->executeSearch(search, searchOperator);
}

void FilterModel::endSearch()
{
    startUpdate();
    FilterX* f = qobject_cast<FilterX*>(sender());
    if (f && f!= m_filter)
    {
        *m_filter = *f;
        m_filter->lock(nullptr);
        delete f;
    }
    endUpdate();
    emit searchFinished();
}


