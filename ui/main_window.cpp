#include "main_window.h"
#include "ui_main_window.h"
#include <pwm_common.h>

#include <QTimer>
#include <QMessageBox>
#include <QMap>
#include <QFile>
#include <QFileDialog>
#include <QScreen>


static QMap<QString, hashType> algorithms = {
    { "MD4", PWM_MD4 },
    { "MD5", PWM_MD5 },
    { "SHA1", PWM_SHA1 },
    { "SHA256", PWM_SHA256 },
    { "RIPEMD160", PWM_RIPEMD160 }
};

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->cypherComboBox->addItems( algorithms.keys() );
    connect( ui->generatePasswordPushButton, SIGNAL( pressed() ),
             this, SLOT( onGeneratePasswordClicked() ) );
    connect(ui->writePasswordsPushButton, SIGNAL( pressed() ),
            this, SLOT( onWritePasswordsClicked() ) );
    connect( ui->selectFileToolButton, SIGNAL( pressed() ),
             this, SLOT( onSelectFileToolBoxPressed() ) );
    connect( ui->filePathLineEdit, &QLineEdit::textEdited, [this](QString) {
        if ( m_fileSelectedByDialog ) {
            m_fileSelectedByDialog = false;
        }
    });
    // Actions
    connect( ui->actionExit,SIGNAL( triggered(bool) ), qApp, SLOT( quit() ) );
    connect( ui->actionAbout_Qt, SIGNAL( triggered(bool) ), qApp, SLOT( aboutQt() ) );
    QScreen* screen = QApplication::primaryScreen();
    QRect geom = screen->geometry();
    move( ( geom.width() - width() ) / 2,
          ( geom.height() - height() ) / 2 );
    QTimer::singleShot( 0, this, SLOT( start() ) );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::start()
{
    if ( !m_passwordMaker.initialized() ) {
        QMessageBox::critical( this, QString("Initialization error"),
                               QString("Unable to initialize hashing library"),
                               QMessageBox::Ok );
        QApplication::quit();
    }
}



void MainWindow::onGeneratePasswordClicked()
{
    QString generatedPassword = generatePassword();
    if ( !generatedPassword.isEmpty() ) {
        ui->yourPasswordLineEdit->setText( generatedPassword );
    }
}

void MainWindow::onWritePasswordsClicked()
{
    QString filePath = ui->filePathLineEdit->text();
    if ( filePath.isEmpty() ) {
        QMessageBox::warning( this, QString("Warning"),
                              QString("Please specify path to output file"),
                              QMessageBox::Ok );
        return;
    }

    int minPswdLen = ui->minLengthSpinBox->value();
    int maxPswdLen = ui->maxLengthSpinBox->value();
    if ( minPswdLen < 1 || maxPswdLen > 255
         || minPswdLen > maxPswdLen ) {
        QMessageBox::warning( this, QString("Warning"),
                              QString("Invalid bounds for password length range"),
                              QMessageBox::Ok );
        return;
    }

    QFile outputFile(filePath);
    if ( !m_fileSelectedByDialog && outputFile.exists() ) {
        auto rv = QMessageBox::question(this, QString("Warning"),
                                        QString("File %1 already exists. "
                                                "Do you want to overwrite it?").arg(outputFile.fileName()),
                               QMessageBox::Yes, QMessageBox::No );
        if ( rv == QMessageBox::No ) {
            return;
        }
    }

    if ( outputFile.open( QIODevice::WriteOnly | QIODevice::Truncate ) ) {
        QString generatedPassword = generatePassword();
        if ( !generatedPassword.isEmpty() ) {
            maxPswdLen = qMin( maxPswdLen, generatedPassword.length() );
            for ( int pswdLen = minPswdLen; pswdLen <= maxPswdLen; ++pswdLen ) {
                QString line = generatedPassword.mid( 0, pswdLen );
                outputFile.write( line.toUtf8() );
                outputFile.write( "\n" );
            }
        }
        outputFile.close();
    }
    else {
        QMessageBox::critical( this, QString("Error"),
                               QString("Unable to open file %1: %2")
                               .arg(outputFile.fileName()).arg(outputFile.errorString()),
                               QMessageBox::Ok );
    }
}

void MainWindow::onSelectFileToolBoxPressed()
{
    QString filename = QFileDialog::getSaveFileName( this, QString("Select file") );
    if ( !filename.isEmpty() ) {
        m_fileSelectedByDialog = true;
        ui->filePathLineEdit->setText( filename );
    }
}

QString MainWindow::generatePassword()
{
    QString url = ui->urlLineEdit->text();
    QString masterPassword = ui->masterPasswordLineEdit->text();
    QString cypher = ui->cypherComboBox->currentText();
    hashType algo = algorithms.value( cypher, hashType::PWM_SHA256);
    int passwordLength = ui->lengthSpinBox->value();

    if ( url.isEmpty() || masterPassword.isEmpty() || passwordLength < 1) {
        QMessageBox::warning( this, QString("Warning"),
                              QString("Invalid or empty values provided"), QMessageBox::Ok );
        return QString();
    }

    auto generatedPassword = m_passwordMaker.generatePassword( masterPassword.toStdString(),
                                                 algo, false, true, url.toStdString(),
                                                 passwordLength );
    return QString::fromStdString( generatedPassword );
}
