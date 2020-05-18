#include "AView.h"

void AView::addChild(const ViewPtr &child) {
    children.emplace_back(child);
}

void AView::setParent(const ViewPtr &parent) {
    this->parent = parent;
}

ViewPtr AView::getChild(const std::string &childName) {
    for (ViewPtr childPtr : children) {
        if (childPtr->getName() == childName) {
            return childPtr;
        }
    }

    return nullptr;
}

std::string AView::getName() {
    return name;
}

std::string AView::getTitle() {
    return title;
}