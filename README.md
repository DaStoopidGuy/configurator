# Configurator

## Usage

Configurator is a (stb-style) header-only library.

To include it in your program:

```c
#define CONFIG_IMPL
#define SV_IMPL
#define SB_IMPL
#include "configurator.h"
```
> _NOTE:_ the `configurator.h` uses StringBuilder and StringView internally,
> that's why we explicitly define _Implementation Macros_ for both of these as well
> 
> **IMPORTANT:** `string_view.h` and `string_builder.h` must be in the include
> path

It provides the following functions:

```c
Config config_open(const char* filename);
void config_close(Config *c);
const char *config_get(Config *c, const char* key); // very important to call at the end
```
# Example

demo `test.cfg` config file:

```
name=john wick  
age=19  
```

In your `main.c`:  

```c
#define CONFIG_IMPL
#define SV_IMPL
#define SB_IMPL
#include "configurator.h"

int main() {
    Config cfg = config_open("test.cfg");

    const char *name = config_get(&cfg, "name");
    const char *age  = config_get(&cfg, "age");

    printf("%s is %s years old\n", name, age);

    config_close(&cfg);
    return 0;
}
```

