#include "core.h"
#include "gravity_macros.h"
#include "gravity_vmmacros.h"

bool get_event (gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex) 
{
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        switch (event.type)
        {
        case SDL_QUIT:
            RETURN_VALUE(VALUE_FROM_CSTRING(vm, "quit"), rindex);
            break;
        }

        RETURN_VALUE(VALUE_FROM_CSTRING(vm, "empty"), rindex);
    }
    
    RETURN_VALUE(VALUE_FROM_NULL, rindex);
}

bool core_init(gravity_vm *vm)
{
    gravity_class_t *core = gravity_class_new_pair (vm, "Core", NULL, 0, 0);

    gravity_class_bind(gravity_class_get_meta(core), "get_event", NEW_CLOSURE_VALUE(get_event));

    // Register class inside VM
    gravity_vm_setvalue(vm, "Core", VALUE_FROM_OBJECT(core));

    return true;
}
