#include "authenticationpopup.h"

AuthenticationDialog::AuthenticationDialog(QWidget *parent) : QDialog(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QFormLayout *formLayout = new QFormLayout();

    usernameEdit = new QLineEdit(this);
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);

    formLayout->addRow(new QLabel("Username:"), usernameEdit);
    formLayout->addRow(new QLabel("Password:"), passwordEdit);

    mainLayout->addLayout(formLayout);

    loginButton = new QPushButton("Login", this);
    cancelButton = new QPushButton("Cancel", this);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(loginButton);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(buttonLayout);

    connect(loginButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    setLayout(mainLayout);
    setWindowTitle("Authentication");
}

QString AuthenticationDialog::getUsername() const {
    return usernameEdit->text();
}

QString AuthenticationDialog::getPassword() const {
    return passwordEdit->text();
}
