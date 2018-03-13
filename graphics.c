#include "graphics.h"
#include "gravity_macros.h"
#include "gravity_vmmacros.h"

SDL_Window *Window = NULL;
SDL_Renderer *Renderer = NULL;

gravity_class_t *image;
bool new_image (gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex) 
{
    if (nargs != 2 || !VALUE_ISA_STRING(args[1]))
        RETURN_ERROR("First argument must be a String.");

    gravity_instance_t *i = gravity_instance_new(vm, image);

    i->xdata = IMG_LoadTexture(Renderer, VALUE_AS_CSTRING(args[1]));

    RETURN_VALUE(VALUE_FROM_OBJECT(i), rindex);
}

gravity_class_t *quad;
bool new_quad (gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex) 
{
    if (nargs < 5)
        RETURN_ERROR("Four arguments are needed to make a Quad.");

    gravity_instance_t *i = gravity_instance_new(vm, quad);

    SDL_Rect *rect = malloc(sizeof(SDL_Rect));
    rect->x = VALUE_AS_INT(args[1]);
    rect->y = VALUE_AS_INT(args[2]);
    rect->w = VALUE_AS_INT(args[3]);
    rect->h = VALUE_AS_INT(args[4]);
    i->xdata = rect;

    RETURN_VALUE(VALUE_FROM_OBJECT(i), rindex);
}

bool draw (gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex) 
{
    gravity_instance_t *i = VALUE_AS_INSTANCE(args[1]);
    
    int w,h;
    SDL_QueryTexture(i->xdata, NULL, NULL, &w, &h);
    SDL_Rect dest = {.x = 0, .y = 0, .w = w, .h = h};

    SDL_RenderCopy(Renderer, i->xdata, NULL, &dest);

    RETURN_VALUE(VALUE_FROM_NULL, rindex);
}

bool drawq (gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex) 
{
    int x = nargs >= 4 ? VALUE_AS_INT(args[3]) : 0;
    int y = nargs >= 5 ? VALUE_AS_INT(args[4]) : 0;
    int angle = nargs >= 6 ? VALUE_AS_INT(args[5]) : 0;
    int scale_x = nargs >= 7 ? VALUE_AS_INT(args[6]) : 1;
    int scale_y = nargs >= 8 ? VALUE_AS_INT(args[7]) : 1;

    SDL_RendererFlip flip = 0;
    if (scale_x < 0) {
        flip |= SDL_FLIP_HORIZONTAL;
    }
    if (scale_y < 0) {
        flip |= SDL_FLIP_VERTICAL;
    }

    SDL_Rect *src = VALUE_AS_INSTANCE(args[2])->xdata;
    SDL_Rect dest = {.x = x, .y = y, .w = src->w * llabs(scale_x), .h = src->h * llabs(scale_y)};

    SDL_RenderCopyEx(Renderer, VALUE_AS_INSTANCE(args[1])->xdata, src, &dest, angle, NULL, flip);

    RETURN_VALUE(VALUE_FROM_NULL, rindex);
}

bool clear (gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex) 
{
    SDL_RenderClear(Renderer);

    RETURN_VALUE(VALUE_FROM_NULL, rindex);
}

bool render (gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex) 
{
    SDL_RenderPresent(Renderer);

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
            Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
        }
    }

    gravity_class_t *graphics = gravity_class_new_pair (vm, "Graphics", NULL, 0, 0);

    gravity_class_bind(gravity_class_get_meta(graphics), "new_image", NEW_CLOSURE_VALUE(new_image));
    gravity_class_bind(gravity_class_get_meta(graphics), "new_quad", NEW_CLOSURE_VALUE(new_quad));
    gravity_class_bind(gravity_class_get_meta(graphics), "draw", NEW_CLOSURE_VALUE(draw));
    gravity_class_bind(gravity_class_get_meta(graphics), "drawq", NEW_CLOSURE_VALUE(drawq));
    gravity_class_bind(gravity_class_get_meta(graphics), "clear", NEW_CLOSURE_VALUE(clear));
    gravity_class_bind(gravity_class_get_meta(graphics), "render", NEW_CLOSURE_VALUE(render));

    // Register class inside VM
    gravity_vm_setvalue(vm, "Graphics", VALUE_FROM_OBJECT(graphics));

    image = gravity_class_new_pair (vm, "Image", NULL, 0, 0);
    quad = gravity_class_new_pair (vm, "Quad", NULL, 0, 0);

    return true;
}
