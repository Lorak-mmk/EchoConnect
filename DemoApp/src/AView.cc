#include "AView.h"

AView::AView(std::string name, std::string title, std::vector<ViewPtr> children)
    : name(std::move(name)), title(std::move(title)), parent(nullptr), children(std::move(children)) {
    for (auto child : this->children) {
        child->setParent(this);
    }
}

AView::~AView() {
    for (auto child : children) {
        delete child;
    }
}

void AView::addChild(ViewPtr child) {
    children.emplace_back(child);
    child->setParent(this);
}

void AView::setParent(ViewPtr newParent) {
    this->parent = newParent;
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
