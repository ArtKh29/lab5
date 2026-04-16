#ifndef CARDDIALOG_H
#define CARDDIALOG_H

#include <QDialog>
#include "parent.h"

class CardDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CardDialog(const Parent& p, bool isMag, QWidget* parent = nullptr);

private:
    const Parent& obj;
    bool isMag;
};

#endif // CARDDIALOG_H
