#ifndef SUBJECT_H
#define SUBJECT_H
#include <vector>

template <typename InfoType> class Observer;

template <typename InfoType> class Subject {
  std::vector<Observer<InfoType>*> observers;
 public:
  void attach(Observer<InfoType> *o);
  void notifyObservers();
  virtual InfoType getInfo() = 0;
};

template <typename InfoType>
void Subject<InfoType>::attach(Observer<InfoType> *o) {
  observers.emplace_back(o);
}

template <typename InfoType>
void Subject<InfoType>::notifyObservers() {
  for (auto &ob : observers) ob->notify(*this);
}

#endif
