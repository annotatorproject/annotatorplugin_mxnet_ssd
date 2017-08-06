#include "widget.h"
#include <QtWidgets/QFileDialog>
#include "ssd.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
  ui->setupUi(this);
}

Widget::~Widget() { delete ui; }

void Widget::setSSD(Annotator::Plugins::SSD *ssd) { this->ssd = ssd; }

void Widget::on_prototxtButton_clicked() {
  QString fileName = QFileDialog::getOpenFileName(
      this, tr("Load Caffe prototxt File"), "", tr("model (*.prototxt)"));
  ui->prototxtLineEdit->setText(fileName);
  ssd->setPrototxt(fileName.toStdString());
}

void Widget::on_caffemodelButton_clicked() {
  QString fileName =
      QFileDialog::getOpenFileName(this, tr("Load Caffemodel File"), "",
                                   tr("caffemodel weights (*.caffemodel)"));
  ui->caffemodelLineEdit->setText(fileName);
  ssd->setCaffemodel(fileName.toStdString());
}

void Widget::on_confidenceSpinBox_editingFinished() {
  ssd->setConfidenceThreshold(ui->confidenceSpinBox->value());
}

void Widget::on_prototxtLineEdit_editingFinished() {
  ssd->setPrototxt(ui->prototxtLineEdit->text().toStdString());
}

void Widget::on_caffemodelLineEdit_editingFinished() {
  ssd->setCaffemodel(ui->caffemodelLineEdit->text().toStdString());
}

void Widget::on_labelmapButton_clicked() {
  QString fileName = QFileDialog::getOpenFileName(
      this, tr("Load Caffe label map File"), "", tr("model (*.prototxt)"));
  ui->labelmapLineEdit->setText(fileName);
  ssd->setLabelmap(fileName.toStdString());
}

void Widget::on_labelmapLineEdit_editingFinished() {
  ssd->setLabelmap(ui->labelmapLineEdit->text().toStdString());
}
