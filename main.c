#include "core.h"
#include "timer.h"
#include "graphics.h"

static void report_error (error_type_t error_type, const char *message, error_desc_t error_desc, void *xdata) {
    #pragma unused(xdata)
    const char *type = "N/A";
    switch (error_type) {
        case GRAVITY_ERROR_NONE: type = "NONE"; break;
        case GRAVITY_ERROR_SYNTAX: type = "SYNTAX"; break;
        case GRAVITY_ERROR_SEMANTIC: type = "SEMANTIC"; break;
        case GRAVITY_ERROR_RUNTIME: type = "RUNTIME"; break;
        case GRAVITY_WARNING: type = "WARNING"; break;
        case GRAVITY_ERROR_IO: type = "I/O"; break;
    }

    if (error_type == GRAVITY_ERROR_RUNTIME) printf("RUNTIME ERROR: ");
    else printf("%s ERROR on %d (%d,%d): ", type, error_desc.fileid, error_desc.lineno, error_desc.colno);
    printf("%s\n", message);
}

static const char *load_file (const char *file, size_t *size, uint32_t *fileid, void *xdata) {
    #pragma unused(fileid, xdata)

    if (!file_exists(file)) return NULL;
    return file_read(file, size);
}

int main(int argc, char const *argv[])
{
    gravity_delegate_t delegate = {
        .error_callback = report_error,
        .loadfile_callback = load_file
    };
    gravity_vm *vm = gravity_vm_new(&delegate);
    gravity_compiler_t *compiler = gravity_compiler_create(&delegate);
    gravity_core_register(vm);

    size_t size = 0;
    const char *source_code = file_read("main.gravity", &size);

    core_init(vm);
    timer_init(vm);
    graphics_init(vm);
    gravity_closure_t *closure = gravity_compiler_run(compiler, source_code, size, 0, false);

    if (closure) {
        gravity_compiler_transfer(compiler, vm);
        gravity_vm_runmain(vm, closure);
    }

    if (compiler) gravity_compiler_free(compiler);
    if (vm) gravity_vm_free(vm);
    gravity_core_free();

    return 0;
}