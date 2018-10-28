#ifndef COMPONENT_H_INCLUDED
#define COMPONENT_H_INCLUDED

#include <vector>
#include <json.hpp>
#include "../json_utils.h"

using json = nlohmann::json;

class ComponentBase {
public:
	/*
	 * A component holds the data/state of an entity for use in specific
	 * systems. A component is first exported via the ECSManager
	 * and then created through the ECSManager.
	 */
	ComponentBase();
	virtual ~ComponentBase();

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
		static ComponentBase* create() {return new T; };
	virtual void setValues(json inValues) = 0;

	virtual const std::string getName() = 0;
};

template <typename T>
class Component : public ComponentBase
{
public:
	static const std::string name;
	const std::string getName() final { return name; }

	/*
	 * Used to export so that ECSManager can see a specific component
	 */
	static const bool exported;
};

#endif // COMPONENT_H_INCLUDED