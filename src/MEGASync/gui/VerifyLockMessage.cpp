#include "VerifyLockMessage.h"
#ifdef __APPLE__
#include "macx/MacXFunctions.h"
#endif
#include "ui_VerifyLockMessage.h"

#include <QTimer>
#include <QDebug>
#include <QStyle>
#include "Utilities.h"
#include "MegaApplication.h"

using namespace mega;

VerifyLockMessage::VerifyLockMessage(int lockStatus, bool isMainDialogAvailable, QWidget *parent) :
    QDialog(parent), m_ui(new Ui::VerifyLockMessage),
    m_haveMainDialog(isMainDialogAvailable)
{
    m_ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    m_ui->lEmailSent->setVisible(false);

    m_lockStatus = MegaApi::ACCOUNT_NOT_BLOCKED;
    regenerateUI(lockStatus);

    megaApi = ((MegaApplication *)qApp)->getMegaApi();
    delegateListener = new QTMegaRequestListener(megaApi, this);

    QStyle *style = QApplication::style();
    QIcon tmpIcon = style->standardIcon(QStyle::SP_MessageBoxWarning, 0, this);
    m_ui->bWarning->setIcon(tmpIcon);

    connect(static_cast<MegaApplication *>(qApp), SIGNAL(unblocked()), this, SLOT(close()));

#ifdef __APPLE__
    QSize size = m_nativeWidget->size();
    m_popover = allocatePopOverWithView(m_nativeWidget->nativeView(), size);
    m_nativeWidget->show();
#endif
}

void VerifyLockMessage::mousePressEvent(QMouseEvent *event)
{
    if (m_lockStatus == MegaApi::ACCOUNT_BLOCKED_VERIFICATION_EMAIL &&
            m_ui->lWhySeenThis->rect().contains(m_ui->lWhySeenThis->mapFrom(this, event->pos())))
    {
#ifdef __APPLE__
        showPopOverRelativeToRect(winId(), m_popover, event->localPos());
#else

        QPoint pos = event->globalPos();

        mLockedPopOver->show();
        mLockedPopOver->ensurePolished();
        mLockedPopOver->move(pos - QPoint(mLockedPopOver->width()/2, mLockedPopOver->height() + 10));
        Utilities::adjustToScreenFunc(pos, mLockedPopOver.get());

        auto initialWidth = mLockedPopOver->width();
        auto initialHeight = mLockedPopOver->height();

        // size might be incorrect the first time it's shown. This works around that and repositions at the expected position afterwards
        QTimer::singleShot(1, this, [this, pos, initialWidth, initialHeight] () {
            mLockedPopOver->update();
            mLockedPopOver->ensurePolished();

            if (initialWidth != mLockedPopOver->width() || initialHeight != mLockedPopOver->height())
            {
                mLockedPopOver->move(pos - QPoint(mLockedPopOver->width()/2, mLockedPopOver->height()));
                Utilities::adjustToScreenFunc(pos, mLockedPopOver.get());
                mLockedPopOver->update();
            }
        });
#endif
    }
}

void VerifyLockMessage::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        m_ui->retranslateUi(this);
        regenerateUI(m_lockStatus, true);
    }

    QDialog::changeEvent(event);
}

void VerifyLockMessage::regenerateUI(int currentStatus, bool force)
{
    if (!force && m_lockStatus == currentStatus)
    {
        return;
    }

    m_lockStatus = currentStatus;

    switch (m_lockStatus)
    {
        case MegaApi::ACCOUNT_BLOCKED_VERIFICATION_EMAIL:
        {
            QString title = m_haveMainDialog ? tr("Verify your email") : tr("Locked account");
            setWindowTitle(title);
            m_ui->lVerifyEmailTitle->setText(title);
            m_ui->lVerifyEmailDesc->setText(tr("Your account has been temporarily suspended for your safety. Please verify your email and follow its steps to unlock your account."));
            m_ui->lWhySeenThis->setVisible(true);
            m_ui->lEmailSent->setText(tr(m_ui->lEmailSent->text().toUtf8().constData()));
            m_ui->lEmailSent->setVisible(true);
            m_ui->bResendEmail->setText(tr("Resend email"));

            break;
        }
        case MegaApi::ACCOUNT_BLOCKED_VERIFICATION_SMS:
        {
            QString title = m_haveMainDialog ? tr("Verify your account") : tr("Locked account");
            setWindowTitle(title);
            m_ui->lVerifyEmailTitle->setText(title);
            m_ui->lVerifyEmailDesc->setText(tr("Your account has been suspended temporarily due to potential abuse. Please verify your phone number to unlock your account."));
            m_ui->lWhySeenThis->setVisible(false);
            m_ui->lEmailSent->setVisible(false);
            m_ui->bResendEmail->setText(tr("Verify now"));
            break;
        }

    }
}

void VerifyLockMessage::onRequestFinish(MegaApi*, MegaRequest* request, MegaError* e)
{
    int error = e->getErrorCode();

    if (request->getType() == MegaRequest::TYPE_RESEND_VERIFICATION_EMAIL)
    {
        if (error == MegaError::API_OK)
        {
            m_ui->lEmailSent->setStyleSheet(QString::fromUtf8("#lEmailSent {color: #666666;}"));
            m_ui->lEmailSent->setText(tr("Email sent"));
        }
        else
        {
            m_ui->lEmailSent->setStyleSheet(QString::fromUtf8("#lEmailSent {color: #F0373A;}"));

            if(error == MegaError::API_ETEMPUNAVAIL)
            {
                m_ui->lEmailSent->setText(QString::fromUtf8("Email already sent"));
            }
            else
            {
                m_ui->lEmailSent->setText(QString::fromUtf8("%1").arg(QCoreApplication::translate("MegaError", e->getErrorString())));
            }
        }

        m_ui->lEmailSent->setVisible(true);

        Utilities::animateProperty(m_ui->lEmailSent, 400, "opacity", m_ui->lEmailSent->property("opacity"), 1.0);

        int animationTime = 500;
        QTimer::singleShot(10000-animationTime, this, [this, animationTime] () {
            Utilities::animateProperty(m_ui->lEmailSent, animationTime, "opacity", 1.0, 0.5);
            QTimer::singleShot(animationTime, this, [this] () {
                m_ui->bResendEmail->setEnabled(true);
            });
        });
    }
}

VerifyLockMessage::~VerifyLockMessage()
{
    delete delegateListener;
    delete m_ui;
#ifdef __APPLE__
    releaseIdObject(m_popover);
#endif
}

void VerifyLockMessage::on_bLogout_clicked()
{
    close();
    emit logout();
}

void VerifyLockMessage::on_bResendEmail_clicked()
{
    switch (m_lockStatus)
    {
        case MegaApi::ACCOUNT_BLOCKED_VERIFICATION_EMAIL:
        {
            m_ui->lEmailSent->setProperty("opacity", 0.0);

            m_ui->bResendEmail->setEnabled(false);
            megaApi->resendVerificationEmail(delegateListener);
            break;
        }
        case MegaApi::ACCOUNT_BLOCKED_VERIFICATION_SMS:
        {
            static_cast<MegaApplication *>(qApp)->goToMyCloud();
        }
    }
}
