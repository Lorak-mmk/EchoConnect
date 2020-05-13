#include "IView.h"

void IView::addChild(ViewPtr child) {
    children.emplace_back(child);
}

void IView::setParent(ViewPtr parent) {
    this->parent = parent;
}

ViewPtr IView::getChild(std::string childName) {
    for (ViewPtr childPtr : children) {
        if (childPtr->getName() == childName) {
            return childPtr;
        }
    }

    return nullptr;
}

std::string IView::getName() {
    return name;
}