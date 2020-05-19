#ifndef DEMOAPP_IVIEW_H
#define DEMOAPP_IVIEW_H

#include <memory>
#include <string>
#include <vector>

class AView;

using ViewPtr = std::shared_ptr<AView>;

class AView {
public:
    AView(std::string name, std::string title) : name(name), title(title) {}

    virtual ~AView() = default;

    void addChild(const ViewPtr &child);
    void setParent(const ViewPtr &parent);
    ViewPtr getChild(const std::string &childName);
    std::string getName();
    std::string getTitle();

    virtual ViewPtr execute() = 0;

protected:
    std::string name;
    std::string title;
    ViewPtr parent;
    std::vector<ViewPtr> children;
};

#endif  // DEMOAPP_IVIEW_H