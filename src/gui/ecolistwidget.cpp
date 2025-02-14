/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "ecolistwidget.h"
#include "ecopositions.h"
#include "ui_tagdetailwidget.h"
#include "database.h"
#include "databaseinfo.h"
#include "tags.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

ECOListWidget::ECOListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TagDetailWidget)
{
    setObjectName("ECOListWidget");
    ui->setupUi(this);
    m_filterModel = new QStringListModel(ui->tagList);
    ui->tagList->setModel(m_filterModel);
    ui->renameItem->setVisible(false);

    setObjectName("ECOListWidget");
    connect(ui->filterEdit, SIGNAL(textChanged(QString)), SLOT(findECO(QString)));
    connect(ui->filterDatabase, SIGNAL(clicked()), SLOT(filterSelectedECO()));
    connect(ui->addFilter, SIGNAL(clicked()), SLOT(filterSelectedECOAdd()));
    connect(ui->tagList, SIGNAL(doubleClicked(QModelIndex)), SLOT(filterSelectedECO()));

    selectECO(QString());
    QItemSelectionModel* selectionModel = ui->tagList->selectionModel();
    connect(selectionModel, SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(selectionChangedSlot()));

    ui->detailText->setOpenLinks(true);
    ui->detailText->setOpenExternalLinks(false);

    connect(ui->detailText, SIGNAL(anchorClicked(QUrl)), SLOT(slotLinkClicked(QUrl)));

    slotReconfigure();
}

ECOListWidget::~ECOListWidget()
{
    delete ui;
}

void ECOListWidget::slotReconfigure()
{
    ui->tagList->slotReconfigure();
}

void ECOListWidget::selectionChangedSlot()
{
    const QModelIndexList& selection = ui->tagList->selectionModel()->selectedRows();
    if(selection.count())
    {
        QString ts = selection[0].data().toString().section(" ",0,0);
        ecoSelected(ts);
    }
    else
    {
        ecoSelected(QString());
    }
}

void ECOListWidget::findECO(const QString& s)
{
    if(s.isEmpty())
    {
        m_filterModel->setStringList(m_list);
    }
    else
    {
        QStringList newList = m_list.filter(QRegExp(s, Qt::CaseInsensitive));
        m_filterModel->setStringList(newList);
        if (newList.count()==1)
        {
            selectECO(newList.at(0));
        }
    }
}

void ECOListWidget::slotSelectECO(const QString& eco)
{
    m_filterModel->setStringList(m_list);
    ui->filterEdit->clear();
    selectECO(eco);
}

void ECOListWidget::ecoSelected(const QString& eco)
{
    if(!eco.isEmpty())
    {
        m_eco.setCode(eco);
        ui->filterDatabase->setEnabled(true);
        ui->addFilter->setEnabled(true);
        ui->renameItem->setEnabled(true);
        QString opName = EcoPositions::findEcoNameDetailed(eco);

        QString head = QString("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\"><html><head><title>%1</title><meta name='qrichtext' content='1'><meta http-equiv=\"Content-type\" content=\"text/html;charset=UTF-8\"></head>").arg(eco);
        QString text = QString("%1<body><h1><a href='eco:%2'>%3</a></h1><p>%4</p><p>%5</p><p>%6%7</p>%8</body></html>")
                       .arg(head,
                            m_eco.name(),
                            m_eco.name(),
                            opName,
                            m_eco.formattedGameCount(),
                            m_eco.formattedRating(),
                            m_eco.formattedScore(),
                            m_eco.listOfPlayers()
                            );
        ui->detailText->setHtml(text);
    }
    else
    {
        ui->filterDatabase->setEnabled(false);
        ui->addFilter->setEnabled(false);
        ui->renameItem->setEnabled(false);
        ui->detailText->setText(tr("<html><body><i>No ECO code chosen.</i></body></html>"));
    }
}

void ECOListWidget::selectECO(const QString& eco)
{
    ecoSelected(eco);
    if(!eco.isEmpty())
    {
        const QStringList& list = m_filterModel->stringList();
        int row = list.indexOf(eco);
        if(row >= 0)
        {
            QModelIndex index = m_filterModel->index(row, 0);
            if(index.isValid())
            {
                ui->tagList->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);
                ui->tagList->scrollTo(index);
            }
        }
    }
}

void ECOListWidget::filterSelectedECO()
{
    const QModelIndexList& selection = ui->tagList->selectionModel()->selectedRows();
    if(selection.count())
    {
        QString ts = selection[0].data().toString().section(" ",0,0);
        emit filterRequest(ts);
    }
}

void ECOListWidget::filterSelectedECOAdd()
{
    const QModelIndexList& selection = ui->tagList->selectionModel()->selectedRows();
    if(selection.count())
    {
        QString ts = selection[0].data().toString().section(" ",0,0);
        ts.prepend("+");
        emit filterRequest(ts);
    }
}

void ECOListWidget::setDatabase(DatabaseInfo* dbInfo)
{
    Database* db = dbInfo->database();
    ui->detailText->setText(tr("<html><i>No ECO code chosen.</i></html>"));
    m_eco.setDatabase(db);
    m_list.clear();
    if(db && db->index())
    {
        m_list = db->index()->tagValues(TagNameECO);
    }
    m_list.sort();
    QStringList::Iterator iter = m_list.begin();
    while (iter != m_list.end())
    {
        QString eco = *iter;
        *iter = eco + " " + EcoPositions::findEcoNameDetailed(eco);
        ++iter;
    }

    findECO(ui->filterEdit->text());
    m_filterModel->sort(0);
}

void ECOListWidget::slotLinkClicked(const QUrl& url)
{
    if(url.scheme().startsWith("player"))
    {
        QString tag = (url.scheme().contains("white")) ? TagNameWhite : TagNameBlack;
        emit filterEcoPlayerRequest(tag, ui->detailText->documentTitle(), url.path(), url.fragment());
        emit filterEcoPlayerRequest(url.path(), ui->detailText->documentTitle());
    }
    else if(url.scheme().startsWith("eco"))
    {
        emit filterRequest(url.path());
    }
    else if(url.scheme().startsWith("result"))
    {
        emit filterEcoPlayerRequest(TagNameECO, ui->detailText->documentTitle(), "", url.path());
    }
}
