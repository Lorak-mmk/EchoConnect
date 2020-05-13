#ifndef DEMOAPP_IVIEW_H
#define DEMOAPP_IVIEW_H

#include <memory>
#include <string>
#include <vector>

class IView;

using ViewPtr = std::shared_ptr<IView>;

class IView {
public:
    IView(std::string name) : name(name) {}
    IView(const IView &other) = delete;
    IView(IView &&other) = default;
    IView &operator=(const IView &other) = delete;
    IView &operator=(IView &&other) = default;
    virtual ~IView() = default;

    void addChild(ViewPtr child);
    void setParent(ViewPtr parent);
    ViewPtr getChild(std::string childName);
    std::string getName();

    virtual ViewPtr execute() = 0;

protected:
    std::string name;
    ViewPtr parent;
    std::vector<ViewPtr> children;
};

#endif  // DEMOAPP_IVIEW_H