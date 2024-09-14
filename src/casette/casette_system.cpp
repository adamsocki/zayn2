

#define INTRO 0
#define LIGHTING_01 0
#define LIGHTING_02 1


#include "../managers/component_manager.cpp"
#include "../managers/entity_manager.cpp"
#include "../managers/event_manager.cpp"
#include "../managers/system_manager.cpp"
#include "../managers/coordinator.cpp"


#if INTRO
#include "intro_casette.cpp"

#elif LIGHTING_01
#include "lighting_casette_01.cpp"

#elif LIGHTING_02
#include "lighting_casette_02.cpp"



#endif