#ifndef WIDGET_H
#define WIDGET_H

#include <QPixmap>
#include <QWidget>

namespace Ui {
class Widget;
}

namespace Annotator {
namespace Plugins {
class SSD;
}
}

class Widget : public QWidget {
  Q_OBJECT

 public:
  explicit Widget(QWidget *parent = 0);
  ~Widget();
  void setSSD(Annotator::Plugins::SSD *ssd);

 private slots:

  void on_prototxtButton_clicked();

  void on_caffemodelButton_clicked();

  void on_confidenceSpinBox_editingFinished();

  void on_prototxtLineEdit_editingFinished();

  void on_caffemodelLineEdit_editingFinished();

  void on_labelmapButton_clicked();

  void on_labelmapLineEdit_editingFinished();

 private:
  Ui::Widget *ui;
  Annotator::Plugins::SSD *ssd;
  bool training = false;
};

#endif  // WIDGET_H
