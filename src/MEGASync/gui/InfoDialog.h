#ifndef INFODIALOG_H
#define INFODIALOG_H

#include <QDialog>
#include <QTimer>
#include <QProcess>
#include <QDateTime>
#include <QPainter>
#include "GuestWidget.h"
#include "SettingsDialog.h"
#include "MenuItemAction.h"
#include "control/Preferences.h"
#include "control/MegaController.h"
#include "model/SyncModel.h"
#include "QCustomTransfersModel.h"
#include <QGraphicsOpacityEffect>
#include "HighDpiResize.h"
#include "Utilities.h"
#include "FilterAlertWidget.h"
#include "QtPositioningBugFixer.h"
#include "TransferQuota.h"
#include "SyncsMenu.h"
#include "control/SyncController.h"
#include "AddBackupDialog.h"
#include "BackupsWizard.h"

#include <memory>
#ifdef _WIN32
#include <chrono>
#endif

namespace Ui {
class InfoDialog;
}

class MegaApplication;
class InfoDialog : public QDialog, public mega::MegaTransferListener
{
    Q_OBJECT

    enum {
        STATE_STARTING,
        STATE_PAUSED,
        STATE_WAITING,
        STATE_INDEXING,
        STATE_UPDATED,
        STATE_SYNCING,
        STATE_TRANSFERRING,
    };



public:

    enum {
        STATE_NONE = -1,
        STATE_LOGOUT = 0,
        STATE_LOGGEDIN = 1,
        STATE_LOCKED_EMAIL = mega::MegaApi::ACCOUNT_BLOCKED_VERIFICATION_EMAIL,
        STATE_LOCKED_SMS = mega::MegaApi::ACCOUNT_BLOCKED_VERIFICATION_SMS
    };

    explicit InfoDialog(MegaApplication *app, QWidget *parent = 0, InfoDialog* olddialog = nullptr);
    ~InfoDialog();

    PSA_info* getPSAdata();
    void setUsage();
    void setTransfer(mega::MegaTransfer *transfer);
    void refreshTransferItems();
    void transferFinished(int error);
    void setIndexing(bool indexing);
    void setWaiting(bool waiting);
    void setSyncing(bool value);
    void setTransferring(bool value);
    void setOverQuotaMode(bool state);
    void setAccountType(int accType);
    void setDisabledSyncTags(QSet<int> tags);
    void addSync(mega::MegaHandle h);
    void addBackup();
    void clearUserAttributes();
    void setPSAannouncement(int id, QString title, QString text, QString urlImage, QString textButton, QString linkButton);
    bool updateOverStorageState(int state);
    void updateNotificationsTreeView(QAbstractItemModel *model, QAbstractItemDelegate *delegate);

    void reset();

    QCustomTransfersModel *stealModel();

    void onTransferFinish(mega::MegaApi* api, mega::MegaTransfer *transfer, mega::MegaError* e) override;
    void onTransferStart(mega::MegaApi *api, mega::MegaTransfer *transfer) override;
    void onTransferUpdate(mega::MegaApi *api, mega::MegaTransfer *transfer) override;

#ifdef __APPLE__
    void moveArrow(QPoint p);
#endif

    void on_bStorageDetails_clicked();
    void regenerateLayout(int blockState = mega::MegaApi::ACCOUNT_NOT_BLOCKED, InfoDialog* olddialog = nullptr);
    HighDpiResize highDpiResize;
#ifdef _WIN32
    std::chrono::steady_clock::time_point lastWindowHideTime;
#endif

    void setUnseenNotifications(long long value);
    void setUnseenTypeNotifications(long long all, long long contacts, long long shares, long long payment);
    long long getUnseenNotifications() const;
    int getLoggedInMode() const;
    void showNotifications();

    void move(int x, int y);

private:
    InfoDialog() = default;
    void drawAvatar(QString email);
    void animateStates(bool opt);
    void updateTransfersCount();
    void hideEvent(QHideEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void moveEvent(QMoveEvent *) override;

public slots:

    void pauseResumeClicked();
    void generalAreaClicked();
    void dlAreaClicked();
    void upAreaClicked();

    void pauseResumeHovered(QMouseEvent *event);
    void generalAreaHovered(QMouseEvent *event);
    void dlAreaHovered(QMouseEvent *event);
    void upAreaHovered(QMouseEvent *event);

    void addSync();
    void onAddSync(mega::MegaSync::SyncType type = mega::MegaSync::TYPE_TWOWAY);
    void onAddBackup();
   void onAllUploadsFinished();
   void onAllDownloadsFinished();
   void onAllTransfersFinished();
   void updateDialogState();

   void enableTransferOverquotaAlert();
   void enableTransferAlmostOverquotaAlert();
   void setBandwidthOverquotaState(QuotaState state);

private slots:
    void on_bSettings_clicked();
    void on_bUpgrade_clicked();
    void on_bUpgradeOverDiskQuota_clicked();
    void openFolder(QString path);
    void onOverlayClicked();
    void on_bTransferManager_clicked();
    void on_bAddSync_clicked();
    void on_bAddBackup_clicked();
    void on_bUpload_clicked();
    void onUserAction(int action);
    void resetLoggedInMode();

    void on_tTransfers_clicked();
    void on_tNotifications_clicked();
    void on_bActualFilter_clicked();
    void applyFilterOption(int opt);
    void on_bNotificationsSettings_clicked();

    void on_bDiscard_clicked();
    void on_bBuyQuota_clicked();

    void onAnimationFinished();
    void onAnimationFinishedBlockedError();

    void sTabsChanged(int tab);

    void on_bDismissSyncSettings_clicked();
    void on_bOpenSyncSettings_clicked();
    void on_bDismissBackupsSettings_clicked();
    void on_bOpenBackupsSettings_clicked();
    void on_bDismissAllSyncsSettings_clicked();
    void on_bOpenAllSyncsSettings_clicked();

    void setAvatar();

signals:
    void openTransferManager(int tab);
    void dismissStorageOverquota(bool oq);
    // signal emitted when showing or dismissing the overquota message.
    // parameter messageShown is true when alert is enabled, false when dismissed
    void transferOverquotaMsgVisibilityChange(bool messageShown);
    // signal emitted when showing or dismissing the almost overquota message.
    // parameter messageShown is true when alert is enabled, false when dismissed
    void almostTransferOverquotaMsgVisibilityChange(bool messageShown);
    void userActivity();

private:
    Ui::InfoDialog *ui;
    QPushButton *overlay;
#ifdef __APPLE__
    QPushButton *arrow;
    QWidget *dummy; // Patch to let text input on line edits of GuestWidget
#endif

    FilterAlertWidget *filterMenu;

    MenuItemAction *cloudItem;
    MenuItemAction *sharesItem;
    MenuItemAction *rubbishItem;

    int activeDownloadState, activeUploadState;
    int remainingUploads, remainingDownloads;
    bool remainingUploadsTimerRunning = false;
    bool remainingDownloadsTimerRunning = false;
    int totalUploads, totalDownloads;
    long long leftUploadBytes, completedUploadBytes;
    long long leftDownloadBytes, completedDownloadBytes;
    long long currentUploadBytes, currentCompletedUploadBytes;
    long long currentDownloadBytes, currentCompletedDownloadBytes;
    bool circlesShowAllActiveTransfersProgress;
    unsigned long long uploadActiveTransferPriority, downloadActiveTransferPriority;
    int uploadActiveTransferTag, downloadActiveTransferTag;
    int uploadActiveTransferState, downloadActiveTransferState;
    void showSyncsMenu(QPushButton* b, mega::MegaSync::SyncType type);

    bool indexing; //scanning
    bool waiting;
    bool syncing; //if any sync is in syncing state
    bool transferring; // if there are ongoing regular transfers
    GuestWidget *gWidget;
    int state;
    bool overQuotaState;
    bool transferOverquotaAlertEnabled;
    bool transferAlmostOverquotaAlertEnabled;
    int storageState;
    QuotaState transferQuotaState;
    int actualAccountType;
    int loggedInMode = STATE_NONE;
    bool notificationsReady = false;
    bool isShown = false;
    long long unseenNotifications = 0;

    AccountDetailsDialog* accountDetailsDialog;
    QPointer<BackupsWizard> mBackupsWizard;
    QPointer<AddBackupDialog> mAddBackupDialog;

#ifdef Q_OS_LINUX
    bool doNotActAsPopup;
#endif

    QPropertyAnimation *animation;
    QGraphicsOpacityEffect *opacityEffect;

    bool shownBlockedError = false;
    QPropertyAnimation *minHeightAnimationBlockedError;
    QPropertyAnimation *maxHeightAnimationBlockedError;
    QParallelAnimationGroup animationGroupBlockedError;
    void hideBlockedError(bool animated = false);
    void showBlockedError();
    QHash<QPushButton*, SyncsMenu*> mSyncsMenus;

protected:
    void setBlockedStateLabel(QString state);
    void updateBlockedState();
    void updateState();
    void changeEvent(QEvent * event) override;
    bool eventFilter(QObject *obj, QEvent *e) override;
    void paintEvent( QPaintEvent * e) override;

protected:
    QDateTime lastPopupUpdate;
    QTimer downloadsFinishedTimer;
    QTimer uploadsFinishedTimer;
    QTimer transfersFinishedTimer;
    MegaApplication *app;
    std::shared_ptr<Preferences> preferences;
    SyncModel *model;
    Controller *controller;
    mega::MegaApi *megaApi;
    mega::MegaTransfer *activeDownload;
    mega::MegaTransfer *activeUpload;
    std::shared_ptr<SyncController> mSyncController;

 private:
    static double computeRatio(long long completed, long long remaining);

    QtPositioningBugFixer qtBugFixer;
};

#endif // INFODIALOG_H
