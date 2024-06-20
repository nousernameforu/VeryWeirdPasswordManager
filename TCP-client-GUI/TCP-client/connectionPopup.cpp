#include "connectionPopup.h"

ConnectionPopup::ConnectionPopup(QWidget *parent) : QDialog(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QFormLayout *formLayout = new QFormLayout();

    ipEdit = new QLineEdit(this);
    portEdit = new QLineEdit(this);
    portEdit->setValidator(new QIntValidator(1, 65535, this));

    formLayout->addRow(new QLabel("Server IP:"), ipEdit);
    formLayout->addRow(new QLabel("Port:"), portEdit);

    mainLayout->addLayout(formLayout);

    connectButton = new QPushButton("Connect", this);
    cancelButton = new QPushButton("Cancel", this);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(connectButton);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(buttonLayout);

    connect(connectButton, &QPushButton::clicked, this, &ConnectionPopup::connectClicked);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    setLayout(mainLayout);
    setWindowTitle("Connection Settings");
}

void ConnectionPopup::connectClicked() {
    QString ip = ipEdit->text();
    int port = portEdit->text().toInt();
    emit connectToServer(ip, port);
    accept();
}
