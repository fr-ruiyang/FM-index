#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_textBrowser_textChanged();

    void Approximate_match(QString s1, QString s2);

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
