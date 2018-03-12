#include <SDL2/SDL.h>

#include "gravity_compiler.h"
#include "gravity_macros.h"
#include "gravity_core.h"
#include "gravity_vm.h"

extern SDL_Window* Window;
    
//The surface contained by the window
extern SDL_Surface* ScreenSurface;

bool graphics_init(gravity_vm *vm);
