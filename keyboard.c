#include "keyboard.h"
#include "gravity_macros.h"
#include "gravity_vmmacros.h"

bool is_down (gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex) 
{
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    SDL_Scancode scancode = SDL_GetScancodeFromKey(SDL_GetKeyFromName(VALUE_AS_CSTRING(args[1])));
    
    RETURN_VALUE(VALUE_FROM_BOOL(state[scancode]), rindex);
}

bool keyboard_init(gravity_vm *vm)
{
    gravity_class_t *core = gravity_class_new_pair (vm, "Keyboard", NULL, 0, 0);

    gravity_class_bind(gravity_class_get_meta(core), "is_down", NEW_CLOSURE_VALUE(is_down));

    // Register class inside VM
    gravity_vm_setvalue(vm, "Keyboard", VALUE_FROM_OBJECT(core));

    return true;
}
