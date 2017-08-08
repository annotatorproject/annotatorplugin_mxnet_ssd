
// source:
// https://github.com/weiliu89/caffe/blob/ssd/examples/ssd/ssd_detect.cpp
#include "ssd.h"
#include "widget.h"

#include <AnnotatorLib/Annotation.h>
#include <AnnotatorLib/Commands/NewAnnotation.h>
#include <AnnotatorLib/Commands/UpdateAnnotation.h>
#include <AnnotatorLib/Frame.h>
#include <AnnotatorLib/Object.h>
#include <AnnotatorLib/Session.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <ctype.h>
#include <iostream>
#include <memory>
#include <chrono>
#include <thread>

using namespace Annotator::Plugins;

Annotator::Plugins::SSD::SSD() { widget.setSSD(this); }

SSD::~SSD() {}

QString SSD::getName() { return "mxnet SSD"; }

QWidget *SSD::getWidget() { return &widget; }

bool SSD::setFrame(shared_ptr<Frame> frame, cv::Mat image) {
  this->lastFrame = this->frame;
  this->frame = frame;
  this->frameImg = image;
  return lastFrame != frame;
}

// first call
void SSD::setObject(shared_ptr<Object> object) {
  if (object != this->object) {
    this->object = object;
  }
}

shared_ptr<Object> SSD::getObject() const { return object; }

void SSD::setLastAnnotation(shared_ptr<Annotation> /*annotation*/) {}

std::vector<shared_ptr<Commands::Command>> SSD::getCommands() {
  std::vector<shared_ptr<Commands::Command>> commands;
  if (frame == nullptr || lastFrame == nullptr || lastFrame == frame ||
      frameImg.cols < 1)
    return commands;

  try {
    initDetector();
    std::vector<float> dets =
        detector->detect(this->frameImg);
        //    detector->detect(this->labelmap_file);

    for (std::size_t i = 0; i < dets.size(); i += 6) {
      if (dets[i] < 0) continue;  // not an object
      int id = static_cast<int>(dets[i]);
      float score = dets[i + 1];
      if (score < confidence_threshold) continue;
      int xmin = static_cast<int>(dets[i + 2] * width);
      int ymin = static_cast<int>(dets[i + 3] * height);
      int xmax = static_cast<int>(dets[i + 4] * width);
      int ymax = static_cast<int>(dets[i + 5] * height);

      if (id < this->labels.size()) {
          std::shared_ptr<Class> labelClass = getClass(id);
          if (labelClass) {
            int objectId = AnnotatorLib::Object::genId();
            int width = xmax - xmin;
            int height = ymax - ymin;
            shared_ptr<Commands::NewAnnotation> nA =
                std::make_shared<Commands::NewAnnotation>(
                    objectId, labelClass, project->getSession(), this->frame,
                    xmin, ymin, width, height, score, false);
            commands.push_back(nA);
          }
      }
    }
  } catch (std::exception &e) {
  }

  return commands;
}

void SSD::setPrototxt(std::string file) {
  this->prototxt_file = file;
  modelLoaded = false;
}

void SSD::setModel(std::string file) {
  this->model_file = file;
  modelLoaded = false;
}

void SSD::setLabelmap(std::string file) {
  this->labelmap_file = file;
  modelLoaded = false;
}

void SSD::setConfidenceThreshold(float threshold) {
  this->confidence_threshold = threshold;
}

void SSD::initDetector() {
  if (modelLoaded) return;
  if (detector) {
    delete detector;
  }
  const std::string mean_file = "";
  detector =
          new det::Detector(model_file, this->prototxt_file, epoch, width, height,
              mean_r, mean_g, mean_b, device_type, device_id);
  labels.clear();
  labels.push_back("background");
  labels.push_back("aeroplane");
  labels.push_back("bicycle");
  labels.push_back("bird");
  labels.push_back("boat");
  labels.push_back("bottle");
  labels.push_back("bus");
  labels.push_back("car");
  labels.push_back("cat");
  labels.push_back("chair");
  labels.push_back("cow");
  labels.push_back("diningtable");
  labels.push_back("dog");
  labels.push_back("horse");
  labels.push_back("motorbike");
  labels.push_back("person");
  labels.push_back("pottedplant");
  labels.push_back("sheep");
  labels.push_back("sofa");
  labels.push_back("train");
  labels.push_back("tvmonitor");
  modelLoaded = true;
}

std::shared_ptr<Class> SSD::getClass(int label) {
  try {
    std::string className = labels[label];
    return this->project->getSession()->getClass(className);
  } catch (...) {
    return nullptr;
  }
}

QPixmap SSD::getImgCrop(shared_ptr<AnnotatorLib::Annotation> annotation,
                        int size) const {
  if (annotation == nullptr) return QPixmap();

  cv::Mat cropped = getImg(annotation);

  cropped.convertTo(cropped, CV_8U);
  cv::cvtColor(cropped, cropped, CV_BGR2RGB);

  QImage img((const unsigned char *)(cropped.data), cropped.cols, cropped.rows,
             cropped.step, QImage::Format_RGB888);

  QPixmap pim = QPixmap::fromImage(img);
  pim = pim.scaledToHeight(size);
  return pim;
}

cv::Mat SSD::getImg(shared_ptr<Annotation> annotation) const {
  cv::Mat tmp = project->getImageSet()->getImage(
      annotation->getFrame()->getFrameNumber());

  float x = std::max(annotation->getX(), 0.f);
  float y = std::max(annotation->getY(), 0.f);
  float w = std::min(annotation->getWidth(), tmp.cols - x);
  float h = std::min(annotation->getHeight(), tmp.rows - y);

  cv::Rect rect(x, y, w, h);
  cv::Mat cropped;
  try {
    tmp(rect).copyTo(cropped);
  } catch (cv::Exception &e) {
    std::cout << e.what();
  }
  return cropped;
}
