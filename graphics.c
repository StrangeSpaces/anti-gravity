#include "graphics.h"
#include "gravity_macros.h"
#include "gravity_vmmacros.h"

SDL_Window* Window = NULL;
SDL_Surface* ScreenSurface = NULL;


gravity_class_t *image;
bool new_image (gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex) 
{
    gravity_instance_t *i = gravity_instance_new(vm, image);

    i->xdata = SDL_LoadBMP( "test.bmp" );

    RETURN_VALUE(VALUE_FROM_OBJECT(i), rindex);
}

bool draw (gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex) 
{
    gravity_instance_t *i = VALUE_AS_INSTANCE(args[1]);

    // bool draw
    SDL_BlitSurface(i->xdata, NULL, ScreenSurface, NULL);
                
    //Update the surface
    SDL_UpdateWindowSurface(Window);

    //Wait two seconds
    // SDL_Delay(2000);

    RETURN_VALUE(VALUE_FROM_NULL, rindex);
}

bool graphics_init(gravity_vm *vm)
{
    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        return false;
    }
    else
    {
        //Create window
        Window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 360, SDL_WINDOW_SHOWN );
        if( Window == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            return false;
        }
        else
        {
            //Get window surface
            ScreenSurface = SDL_GetWindowSurface( Window );
        }
    }

    gravity_class_t *graphics = gravity_class_new_pair (vm, "Graphics", NULL, 0, 0);

    gravity_class_bind(gravity_class_get_meta(graphics), "new_image", NEW_CLOSURE_VALUE(new_image));
    gravity_class_bind(gravity_class_get_meta(graphics), "draw", NEW_CLOSURE_VALUE(draw));

    // Register class inside VM
    gravity_vm_setvalue(vm, "Graphics", VALUE_FROM_OBJECT(graphics));

    image = gravity_class_new_pair (vm, "Image", NULL, 0, 0);

    return true;
}
