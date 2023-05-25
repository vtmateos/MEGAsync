#include "Utilities.h"
#include "syncs/model/BackupItemModel.h"
#include "syncs/control/SyncController.h"
#include "syncs/gui/SyncTooltipCreator.h"

#include <QCoreApplication>
#include <QIcon>
#include <QFileInfo>


BackupItemModel::BackupItemModel(QObject *parent)
    : SyncItemModel(parent)
{
}

QVariant BackupItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal)
    {
        switch(section)
        {
        case Column::ENABLED:
            if(role == Qt::ToolTipRole)
                return tr("Sort by state");
            break;
        case Column::LNAME:
            if(role == Qt::DisplayRole)
                return tr("Local Folder");
            if(role == Qt::ToolTipRole)
                return tr("Sort by name");
            break;
        case Column_STATE:
            if(role == Qt::DisplayRole)
                return tr("State");
            if(role == Qt::ToolTipRole)
                return tr("Sort by backup state");
            break;
        case Column_FILES:
            if(role == Qt::DisplayRole)
                return tr("Files");
            if(role == Qt::ToolTipRole)
                return tr("Sort by file count");
            break;
        case Column_FOLDERS:
            if(role == Qt::DisplayRole)
                return tr("Folders");
            if(role == Qt::ToolTipRole)
                return tr("Sort by folder count");
            break;
        case Column_UPLOADS:
            if(role == Qt::DisplayRole)
                return tr("Uploads");
            if(role == Qt::ToolTipRole)
                return tr("Sort by Uploads");
        }
    }
    return QVariant();
}

int BackupItemModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return kColumns;
}

void BackupItemModel::fillData()
{
    setMode(mega::MegaSync::SyncType::TYPE_BACKUP);
}

void BackupItemModel::sendDataChanged(int row)
{
    emit dataChanged(index(row, Column::ENABLED, QModelIndex()),
                     index(row, Column::MENU, QModelIndex()),
                     QVector<int>()<< Qt::CheckStateRole << Qt::DecorationRole << Qt::ToolTipRole);
}

QVariant BackupItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    std::shared_ptr<SyncSettings> sync = getList().at(index.row());

    if(role == Qt::UserRole)
        return QVariant::fromValue(sync);

    switch(index.column())
    {
        case Column::ENABLED:
            if(role == Qt::ToolTipRole)
            {
                return sync->isEnabled() ? tr("Backup is enabled") : tr("Backup is disabled");
            }
            break;
    case Column::LNAME:
        if(role == Qt::DecorationRole)
        {
            if(sync->getRunState() == mega::MegaSync::RUNSTATE_RUNNING)
            {
                QIcon syncIcon;
                syncIcon.addFile(QLatin1String(":/images/backup_ico_black_default.png"), QSize(STATES_ICON_SIZE, STATES_ICON_SIZE), QIcon::Normal);
                syncIcon.addFile(QLatin1String(":/images/backup_ico_black_default.png"), QSize(STATES_ICON_SIZE, STATES_ICON_SIZE), QIcon::Selected);
                return syncIcon;
            }
        }
        else if(role == Qt::DisplayRole)
        {
            return SyncController::getSyncNameFromPath(sync->getLocalFolder(true));
        }
        else if(role == Qt::ToolTipRole)
        {
            QString toolTip;
            toolTip += SyncTooltipCreator::createForLocal(sync->getLocalFolder());
            return toolTip;
        }
        break;
    }
    return SyncItemModel::data(index,role);
}

