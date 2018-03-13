#include "core.h"
#include "gravity_macros.h"
#include "gravity_vmmacros.h"

uint64_t now = 0;
uint64_t last = 0;

bool step (gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex) 
{
    last = now;
    now = SDL_GetPerformanceCounter();
    
    RETURN_VALUE(VALUE_FROM_FLOAT((now - last) / (double)(SDL_GetPerformanceFrequency())), rindex);
}

bool delta (gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex) 
{   
    RETURN_VALUE(VALUE_FROM_FLOAT((now - last) / (double)(SDL_GetPerformanceFrequency())), rindex);
}

bool timer_init(gravity_vm *vm)
{
    gravity_class_t *core = gravity_class_new_pair (vm, "Timer", NULL, 0, 0);

    gravity_class_bind(gravity_class_get_meta(core), "step", NEW_CLOSURE_VALUE(step));
    gravity_class_bind(gravity_class_get_meta(core), "delta", NEW_CLOSURE_VALUE(delta));

    // Register class inside VM
    gravity_vm_setvalue(vm, "Timer", VALUE_FROM_OBJECT(core));

    return true;
}
