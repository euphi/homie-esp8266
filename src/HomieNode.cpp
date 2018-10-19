#include "HomieNode.hpp"
#include "Homie.hpp"

using namespace HomieInternals;

std::vector<HomieNode*> HomieNode::nodes;

PropertyInterface::PropertyInterface()
: _property(nullptr) {
}

void PropertyInterface::settable(const PropertyInputHandler& inputHandler) {
  _property->settable(inputHandler);
}

PropertyInterface& PropertyInterface::setProperty(Property* property) {
  _property = property;
  return *this;
}

HomieNode::HomieNode(const char* id, const char* name, const char* type, const NodeInputHandler& inputHandler)
: _id(id)
, _name(name)
, _type(type)
, _properties()
, _inputHandler(inputHandler) {
  if (strlen(id) + 1 > MAX_NODE_ID_LENGTH || strlen(type) + 1 > MAX_NODE_TYPE_LENGTH) {
    Helpers::abort(F("✖ HomieNode(): either the id or type string is too long"));
    return;  // never reached, here for clarity
  }
  Homie._checkBeforeSetup(F("HomieNode::HomieNode"));

  HomieNode::nodes.push_back(this);
}

HomieNode::~HomieNode() {
    Helpers::abort(F("✖✖ ~HomieNode(): Destruction of HomieNode object not possible\n  Hint: Don't create HomieNode objects as a local variable (e.g. in setup())"));
    return;  // never reached, here for clarity
}

// advertise function overloading
PropertyInterface& HomieNode::advertise(const char* id) {
  Property* propertyObject = new Property(id);

  _properties.push_back(propertyObject);

  return _propertyInterface.setProperty(propertyObject);
}

PropertyInterface& HomieNode::advertise(const char* id, const char* name) {
  Property* propertyObject = new Property(id, name);

  _properties.push_back(propertyObject);

  return _propertyInterface.setProperty(propertyObject);
}

PropertyInterface& HomieNode::advertise(const char* id, const char* name, const char* datatype) {
  Property* propertyObject = new Property(id, name, datatype);

  _properties.push_back(propertyObject);

  return _propertyInterface.setProperty(propertyObject);
}

PropertyInterface& HomieNode::advertise(const char* id, const char* name, const char* datatype, const char* unit) {
  Property* propertyObject = new Property(id, name, datatype, unit);

  _properties.push_back(propertyObject);

  return _propertyInterface.setProperty(propertyObject);
}

PropertyInterface& HomieNode::advertise(const char* id, const char* name, const char* datatype, const char* unit, const char* format) {
  Property* propertyObject = new Property(id, name, datatype, unit, format);

  _properties.push_back(propertyObject);

  return _propertyInterface.setProperty(propertyObject);
}


// advertiseRange function overloading
PropertyInterface& HomieNode::advertiseRange(const char* id, uint16_t lower, uint16_t upper) {
  Property* propertyObject = new Property(id, "", "", "", "", true, lower, upper);

  _properties.push_back(propertyObject);

  return _propertyInterface.setProperty(propertyObject);
}

PropertyInterface& HomieNode::advertiseRange(const char* id, const char* name, uint16_t lower, uint16_t upper) {
  Property* propertyObject = new Property(id, name, "", "", "", true, lower, upper);

  _properties.push_back(propertyObject);

  return _propertyInterface.setProperty(propertyObject);
}

PropertyInterface& HomieNode::advertiseRange(const char* id, const char* name, const char* datatype, uint16_t lower, uint16_t upper) {
  Property* propertyObject = new Property(id, name, datatype, "", "", true, lower, upper);

  _properties.push_back(propertyObject);

  return _propertyInterface.setProperty(propertyObject);
}

PropertyInterface& HomieNode::advertiseRange(const char* id, const char* name, const char* datatype, 
                                             const char* unit, uint16_t lower, uint16_t upper) {
  Property* propertyObject = new Property(id, name, datatype, unit, "", true, lower, upper);

  _properties.push_back(propertyObject);

  return _propertyInterface.setProperty(propertyObject);
}

PropertyInterface& HomieNode::advertiseRange(const char* id, const char* name, const char* datatype, 
                                             const char* unit, const char* format, uint16_t lower, uint16_t upper) {
  Property* propertyObject = new Property(id, name, datatype, unit, format, true, lower, upper);

  _properties.push_back(propertyObject);

  return _propertyInterface.setProperty(propertyObject);
}

SendingPromise& HomieNode::setProperty(const String& property) const {
  return Interface::get().getSendingPromise().setNode(*this).setProperty(property).setQos(1).setRetained(true).overwriteSetter(false).setRange({ .isRange = false, .index = 0 });
}

bool HomieNode::handleInput(const String& property, const HomieRange& range, const String& value) {
  return _inputHandler(property, range, value);
}

const std::vector<HomieInternals::Property*>& HomieNode::getProperties() const {
  return _properties;
}
