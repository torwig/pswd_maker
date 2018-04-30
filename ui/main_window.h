#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

#include "passwordmaker.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

public slots:
    void start();
    void onGeneratePasswordClicked();
    void onWritePasswordsClicked();
    void onSelectFileToolBoxPressed();

private:
    QString generatePassword();

    Ui::MainWindow* ui;
    PasswordMaker m_passwordMaker;
    bool m_fileSelectedByDialog = false;
};

#endif // MAIN_WINDOW_H
