#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "gravity_compiler.h"
#include "gravity_macros.h"
#include "gravity_core.h"
#include "gravity_vm.h"

extern SDL_Window* Window;

bool graphics_init(gravity_vm *vm);
