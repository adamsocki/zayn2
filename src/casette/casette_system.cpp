

#define INTRO 0
#define LIGHTING 1


#include "../managers/component_manager.cpp"
#include "../managers/entity_manager.cpp"
#include "../managers/event_manager.cpp"
#include "../managers/system_manager.cpp"
#include "../managers/coordinator.cpp"


#if INTRO
#include "intro_casette.cpp"

#elif LIGHTING
#include "lighting_casette.cpp"


#endif