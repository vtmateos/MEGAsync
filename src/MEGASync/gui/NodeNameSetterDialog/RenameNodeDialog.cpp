#include "RenameNodeDialog.h"

#include <Utilities.h>
#include <MegaApplication.h>
#include <mega/types.h>

#include <QFileInfo>
#include <memory>

///RENAME REMOTE FILE/FOLDER REIMPLMENETATION
RenameNodeDialog::RenameNodeDialog(QWidget *parent)
    :NodeNameSetterDialog(parent)
{
}

QString RenameNodeDialog::dialogText()
{
   return isFile() ? enterNewFileNameText() : enterNewFolderNameText();
}

NodeNameSetterDialog::LineEditSelection RenameNodeDialog::lineEditSelection()
{
    NodeNameSetterDialog::LineEditSelection selection;
    auto text = lineEditText();

    if(isFile())
    {
        QFileInfo remoteNode(text);
        selection.length = remoteNode.baseName().length();
    }
    else
    {
        selection.length = text.length();
    }

    return selection;
}

QString RenameNodeDialog::enterNewFileNameText() const
{
    return tr("Enter new file name");
}

QString RenameNodeDialog::enterNewFolderNameText() const
{
    return tr("Enter new folder name");
}

void RenameNodeDialog::title()
{
    isFile() ? setWindowTitle(tr("Rename file")) : setWindowTitle(tr("Rename folder"));
}

///RENAME REMOTE FILE/FOLDER REIMPLMENETATION
RenameRemoteNodeDialog::RenameRemoteNodeDialog(const QString &nodePath, QWidget *parent)
    : RenameNodeDialog(parent)
{
    mNodeToRename = std::unique_ptr<mega::MegaNode>(MegaSyncApp->getMegaApi()->getNodeByPath(nodePath.toUtf8().constData()));
    if(mNodeToRename)
    {
        mNodeName = QString::fromUtf8(mNodeToRename->getName());
    }
}

RenameRemoteNodeDialog::RenameRemoteNodeDialog(std::unique_ptr<mega::MegaNode> node, QWidget* parent)
    : RenameNodeDialog(parent),mNodeToRename(std::move(node))
{
    mNodeName = QString::fromUtf8(mNodeToRename->getName());
}

void RenameRemoteNodeDialog::onDialogAccepted()
{
    if (mNodeToRename)
    {
        std::shared_ptr<mega::MegaNode> parentNode(MegaSyncApp->getMegaApi()->getParentNode(mNodeToRename.get()));
        if(!checkAlreadyExistingNode(getName(), parentNode))
        {
            MegaSyncApp->getMegaApi()->renameNode(mNodeToRename.get(), getName().toUtf8().constData(), mDelegateListener.get());
        }
    }
    //Folder already exists
    else
    {
        done(QDialog::Rejected);
    }
}

void RenameRemoteNodeDialog::onRequestFinish(mega::MegaApi *, mega::MegaRequest *request, mega::MegaError *e)
{
    if (request->getType() == mega::MegaRequest::TYPE_RENAME)
    {
        auto handle = request->getNodeHandle();
        if(handle == mNodeToRename->getHandle())
        {
            if (e->getErrorCode() == mega::MegaError::API_OK)
            {
                if(handle && handle == mNodeToRename->getHandle())
                {
                    done(QDialog::Accepted);
                }
            }
            else
            {
                showError(QString::fromStdString(e->getErrorString()));
            }
        }
    }
}

bool RenameRemoteNodeDialog::isFile()
{
    return mNodeToRename && mNodeToRename->isFile();
}

QString RenameRemoteNodeDialog::lineEditText()
{
    return mNodeName;
}


///RENAME LOCAL FILE/FOLDER
RenameLocalNodeDialog::RenameLocalNodeDialog(const QString& path, QWidget *parent)
    :RenameNodeDialog(parent), mNodePath(path)
{
}

void RenameLocalNodeDialog::onDialogAccepted()
{
    auto newFileName(getName());

    if (!mNodePath.isEmpty())
    {
        QFile file(mNodePath);
        if(file.exists())
        {
            QFileInfo fileInfo(mNodePath);
            fileInfo.setFile(fileInfo.path(), newFileName);
            if(fileInfo.exists())
            {
                showAlreadyExistingNodeError(fileInfo.isFile());
                return;
            }
            else
            {
                if(file.rename(QDir::toNativeSeparators(fileInfo.filePath())))
                {
                    done(QDialog::Accepted);
                    return;
                }
            }
        }
    }

    done(QDialog::Rejected);
}

bool RenameLocalNodeDialog::isFile()
{
    QFileInfo localNode(mNodePath);
    if(localNode.exists())
    {
        if(localNode.isFile())
        {
            return true;
        }
    }

    return false;
}

QString RenameLocalNodeDialog::lineEditText()
{
    if(isFile())
    {
        QFileInfo localNode(mNodePath);
        return localNode.fileName();
    }
    else
    {
        QDir dir(mNodePath);
        return dir.dirName();
    }
}
