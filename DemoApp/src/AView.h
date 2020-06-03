#ifndef DEMOAPP_IVIEW_H
#define DEMOAPP_IVIEW_H

#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <map>

class AView;

using ViewPtr = AView *;

class AView {
public:
    AView(std::string title, std::vector<ViewPtr> children);

    virtual ~AView();

    void addChild(ViewPtr child);
    void setParent(ViewPtr newParent);
    ViewPtr getChild(const std::string &childName);
    std::string getName();
    std::string getTitle();

    virtual ViewPtr execute() = 0;
    virtual void printHelp() = 0;
    virtual bool executeCLI(const std::string& name, const std::map<std::string, std::string>& args) = 0;

protected:
    std::string name;
    std::string title;
    ViewPtr parent;
    std::vector<ViewPtr> children;
};

#endif  // DEMOAPP_IVIEW_H