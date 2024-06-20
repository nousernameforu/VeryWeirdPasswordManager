#ifndef AUTHENTICATIONPOPUP_H
#define AUTHENTICATIONPOPUP_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>

class AuthenticationDialog : public QDialog {
    Q_OBJECT

public:
    explicit AuthenticationDialog(QWidget *parent = nullptr);

    QString getUsername() const;
    QString getPassword() const;

private:
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QPushButton *loginButton;
    QPushButton *cancelButton;
};

#endif // AUTHENTICATIONPOPUP_H
