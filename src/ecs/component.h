#ifndef COMPONENT_H_INCLUDED
#define COMPONENT_H_INCLUDED

#include <vector>
#include <json.hpp>

using json = nlohmann::json;

//TODO Should a component hold its own name so we don't accidently pass in the wrong name to things!?
//The answer is yes, at creation we should ensure it stores its name - somehow
class Component {
public:
    /*
     * A component holds the data/state of an entity for use in specific
     * systems. A component is first exported via the ECSManager
     * and then created through the ECSManager.
     */
    Component();
    virtual ~Component();

    /*
     * create() is used during the creation process of the component
     * by the ECSManager.
     * setValues() takes in a json object and sets the initial data
     * state for the component. This is virtual and must be implemented
     * by each different component seperately!
     * In addition, setValues should implement checks to test that
     * the json object contains valid data. *Else an exception
     * should be thrown.* TODO Verify the behaviour we want
     * Do not use either of these outside of ECSManager!
     */
    template<typename T>
        static Component* create() {return new T; };
    virtual void setValues(json inValues);

private:
    /*
     * Used to export so that ECSManager can see a specific component
     * Usage inside components's cpp file:
     *      bool Component::exported = ECSManager::exportComponent<ComponentClass>("componentName");
     */
    static bool exported;
};

#endif // COMPONENT_H_INCLUDED